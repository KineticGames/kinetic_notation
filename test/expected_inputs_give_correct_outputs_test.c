#include "test_asserts.h"

#include <kinetic_notation.h>

// std
#include <stdint.h>
#include <stdio.h>
#include <string.h>

const char *test_file = "name: \"kinetic_notation\"\n"
                        "c_version: 69 # defaults to 11\n"
                        "version: 4.2.0\n"
                        "dependencies: [\n"
                        "	{\n"
                        "		name: \"kinetic_dependency\"\n"
                        "		version: 4.2.0\n"
                        "	}\n"
                        "	{\n"
                        "		name: \"dep_2\"\n"
                        "		version: 4.2.0\n"
                        "	}\n"
                        "]\n";

int main() {
  KnStructureCreateInfo create_info = {
      .keys =
          (KnKeyCreateInfo[]){
              {.key = "name", .type = STRING},
              {.key = "c_version", .type = NUMBER},
              {.key = "version", .type = VERSION},
              {
                  .key = "dependencies",
                  .type = OBJECT_ARRAY,
                  .with.object_outline =
                      (KnStructureCreateInfo){
                          .keys =
                              (KnKeyCreateInfo[]){
                                  {.key = "name", .type = STRING},
                                  {.key = "version", .type = VERSION},
                              },
                          .keyCount = 2,
                      },
              },
          },
      .keyCount = 4,
  };

  KnStructure structure = NULL;
  KnResult result = kinetic_notation_structure_create(&create_info, &structure);

  ASSERT_EQ(result, SUCCESS);
  ASSERT_NE(structure, NULL);

  if (kinetic_notation_structure_parse(test_file, structure) != SUCCESS) {
    fprintf(stderr, "%s\n", kinetic_notation_get_error());
    return 1;
  }

  KnValue name_value;
  ASSERT_EQ(kinetic_notation_structure_get_key(structure, "name", &name_value),
            SUCCESS);
  KnValue c_version_value;
  ASSERT_EQ(kinetic_notation_structure_get_key(structure, "c_version",
                                               &c_version_value),
            SUCCESS);
  KnValue version_value;
  ASSERT_EQ(
      kinetic_notation_structure_get_key(structure, "version", &version_value),
      SUCCESS);

  ASSERT_EQ(strcmp(name_value.string, "kinetic_notation"), 0);
  ASSERT_EQ(c_version_value.number, 69);
  ASSERT_EQ(version_value.version.major, 4);
  ASSERT_EQ(version_value.version.minor, 2);
  ASSERT_EQ(version_value.version.patch, 0);

  uint32_t object_count;
  ASSERT_EQ(kinetic_notation_structure_get_object_array(
                structure, "dependencies", &object_count, NULL),
            SUCCESS);
  ASSERT_EQ(object_count, 2);

  KnStructure objects[object_count];
  ASSERT_EQ(kinetic_notation_structure_get_object_array(
                structure, "dependencies", &object_count, objects),
            SUCCESS);

  KnValue dependency_name_value_0;
  ASSERT_EQ(kinetic_notation_structure_get_key(objects[0], "name",
                                               &dependency_name_value_0),
            SUCCESS);

  KnValue dependency_name_value_1;
  ASSERT_EQ(kinetic_notation_structure_get_key(objects[1], "name",
                                               &dependency_name_value_1),
            SUCCESS);

  KnValue dependency_version_value;
  ASSERT_EQ(kinetic_notation_structure_get_key(objects[1], "version",
                                               &dependency_version_value),
            SUCCESS);

  ASSERT_EQ(strcmp(dependency_name_value_1.string, "dep_2"), 0);
  ASSERT_EQ(dependency_version_value.version.major, 4);
  ASSERT_EQ(dependency_version_value.version.minor, 2);
  ASSERT_EQ(dependency_version_value.version.patch, 0);

  kinetic_notation_structure_destroy(structure);

  return 0;
}
