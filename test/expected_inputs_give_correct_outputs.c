#include "test_asserts.h"
#include "test_structure.h"

#include <kinetic_notation.h>

// std
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *test_file = "name: \"kinetic_notation\"\n"
                        "c_version: 69 # defaults to 11\n"
                        "version: 4.2.0\n\n"
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

  if (kinetic_notation_parse(definition, test_file) != true) {
    printf("%s", kinetic_notation_get_error());
    return EXIT_FAILURE;
  }

  struct get_string_result name = kn_definition_get_string(definition, "name");
  ASSERT_EQ(name.result, SUCCESS);
  ASSERT_EQ(strcmp(name.string, "kinetic_notation"), 0);
  free(name.string);

  struct get_number_result c_version =
      kn_definition_get_number(definition, "c_version");
  ASSERT_EQ(c_version.result, SUCCESS);
  ASSERT_EQ(c_version.number, 69);

  struct get_version_result version =
      kn_definition_get_version(definition, "version");
  ASSERT_EQ(version.result, SUCCESS);
  ASSERT_EQ(version.version.major, 4);
  ASSERT_EQ(version.version.minor, 2);
  ASSERT_EQ(version.version.patch, 0);

  struct get_object_array_length_result length_result =
      kn_definition_get_object_array_length(definition, "dependencies");
  ASSERT_EQ(length_result.result, SUCCESS);
  ASSERT_EQ(length_result.length, 2);
  size_t length = length_result.length;

  for (size_t i = 0; i < length; ++i) {
    struct get_object_at_index_result result =
        kn_definition_get_object_from_array_at_index(definition, "dependencies",
                                                     i);
    ASSERT_EQ(result.result, SUCCESS);
    kn_definition *object = result.object;

    struct get_version_result version =
        kn_definition_get_version(object, "version");
    ASSERT_EQ(version.result, SUCCESS);
    ASSERT_EQ(version.version.major, 4);
    ASSERT_EQ(version.version.minor, 2);
    ASSERT_EQ(version.version.patch, 0);
  }

  kn_definition_destroy(definition);

  return 0;
}
