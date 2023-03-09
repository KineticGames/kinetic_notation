#include "kinetic_notation/error.h"
#include "kinetic_notation/structure.h"
#include <kinetic_notation.h>
#include <stdio.h>
#include <string.h>

#define ASSERT_EQ(lhs, rhs)                                                    \
  {                                                                            \
    if ((lhs) != (rhs)) {                                                      \
      fprintf(stderr, "[file: %s][line: %d]\n\t%s != %s\n", __FILE__,          \
              __LINE__, #lhs, #rhs);                                           \
      return 1;                                                                \
    }                                                                          \
  }

#define ASSERT_NE(lhs, rhs)                                                    \
  {                                                                            \
    if ((lhs) == (rhs)) {                                                      \
      fprintf(stderr, "[file: %s][line: %d]\n\t%s == %s\n", __FILE__,          \
              __LINE__, #lhs, #rhs);                                           \
      return 1;                                                                \
    }                                                                          \
  }

const char *test_file = "name: \"kinetic_notation\"\n"
                        "c_version: 69 # defaults to 11\n"
                        "version: 4.2.0\n"
                        "dependencies: [\n"
                        "	{\n"
                        "		name: \"kinetic_dependency\"\n"
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

  ASSERT_EQ(strcmp(name_value.string, "kinetic_notation"), 0);
  ASSERT_EQ(c_version_value.number, 69);

  kinetic_notation_structure_destroy(structure);

  return 0;
}
