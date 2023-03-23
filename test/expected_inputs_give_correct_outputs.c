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

typedef struct {
  char *name;
  kn_version version;
} dependency;

typedef struct {
  char *name;
  uint64_t c_version;
  kn_version version;
  size_t dependency_count;
  dependency *dependencies;
} test_structure;

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

  test_structure test_structure = {};

  ASSERT_EQ(kn_definition_get_string(definition, "name", &test_structure.name),
            SUCCESS);

  ASSERT_EQ(kn_definition_get_number(definition, "c_version",
                                     &test_structure.c_version),
            SUCCESS);

  ASSERT_EQ(
      kn_definition_get_version(definition, "version", &test_structure.version),
      SUCCESS);

  ASSERT_EQ(kn_definition_get_object_array_length(
                definition, "dependencies", &test_structure.dependency_count),
            SUCCESS);
  ASSERT_EQ(test_structure.dependency_count, 2);

  test_structure.dependencies =
      calloc(test_structure.dependency_count, sizeof(dependency));
  for (size_t i = 0; i < test_structure.dependency_count; ++i) {
    kn_definition *dependency;
    ASSERT_EQ(kn_definition_get_object_from_array_at_index(
                  definition, "dependencies", i, &dependency),
              SUCCESS);

    ASSERT_EQ(
        kn_definition_get_version(dependency, "version",
                                  &test_structure.dependencies[i].version),
        SUCCESS);
    ASSERT_EQ(test_structure.dependencies[i].version.major, 4);
    ASSERT_EQ(test_structure.dependencies[i].version.minor, 2);
    ASSERT_EQ(test_structure.dependencies[i].version.patch, 0);
  }

  ASSERT_EQ(strcmp(test_structure.name, "kinetic_notation"), 0);
  ASSERT_EQ(test_structure.c_version, 69);
  ASSERT_EQ(test_structure.version.major, 4);
  ASSERT_EQ(test_structure.version.minor, 2);
  ASSERT_EQ(test_structure.version.patch, 0);
  free(test_structure.name);

  kn_definition_destroy(definition);

  return 0;
}
