#include "test_asserts.h"
#include "test_structure.h"

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

int expected_inputs_give_correct_outputs() {
  kn_definition *definition = kn_definition_new();
  kn_definition_add_string(definition, "name");
  kn_definition_add_number(definition, "c_version");
  kn_definition_add_version(definition, "version");
  kn_definition *dependencies = kn_definition_new();
  kn_definition_add_string(dependencies, "name");
  kn_definition_add_version(dependencies, "version");
  kn_definition_add_object_array(definition, "dependencies", dependencies);

  kn_definition_destroy(dependencies);
  kn_definition_destroy(definition);

  // if (kinetic_notation_structure_parse(test_file, structure) != SUCCESS) {
  //   fprintf(stderr, "%s\n", kinetic_notation_get_error());
  //   return 1;
  // }

  // KnValue name_value;
  // ASSERT_EQ(kinetic_notation_structure_get_key(structure, "name",
  // &name_value),
  //           SUCCESS);
  // KnValue c_version_value;
  // ASSERT_EQ(kinetic_notation_structure_get_key(structure, "c_version",
  //                                              &c_version_value),
  //           SUCCESS);
  // KnValue version_value;
  // ASSERT_EQ(
  //     kinetic_notation_structure_get_key(structure, "version",
  //     &version_value), SUCCESS);

  // ASSERT_EQ(strcmp(name_value.string, "kinetic_notation"), 0);
  // ASSERT_EQ(c_version_value.number, 69);
  // ASSERT_EQ(version_value.version.major, 4);
  // ASSERT_EQ(version_value.version.minor, 2);
  // ASSERT_EQ(version_value.version.patch, 0);

  // uint32_t object_count;
  // ASSERT_EQ(kinetic_notation_structure_get_object_array(
  //               structure, "dependencies", &object_count, NULL),
  //           SUCCESS);
  // ASSERT_EQ(object_count, 2);

  // KnStructure objects[object_count];
  // ASSERT_EQ(kinetic_notation_structure_get_object_array(
  //               structure, "dependencies", &object_count, objects),
  //           SUCCESS);

  // KnValue dependency_name_value_0;
  // ASSERT_EQ(kinetic_notation_structure_get_key(objects[0], "name",
  //                                              &dependency_name_value_0),
  //           SUCCESS);

  // KnValue dependency_name_value_1;
  // ASSERT_EQ(kinetic_notation_structure_get_key(objects[1], "name",
  //                                              &dependency_name_value_1),
  //           SUCCESS);

  // KnValue dependency_version_value;
  // ASSERT_EQ(kinetic_notation_structure_get_key(objects[1], "version",
  //                                              &dependency_version_value),
  //           SUCCESS);

  // ASSERT_EQ(strcmp(dependency_name_value_1.string, "dep_2"), 0);
  // ASSERT_EQ(dependency_version_value.version.major, 4);
  // ASSERT_EQ(dependency_version_value.version.minor, 2);
  // ASSERT_EQ(dependency_version_value.version.patch, 0);

  // kinetic_notation_structure_destroy(structure);

  return 0;
}
