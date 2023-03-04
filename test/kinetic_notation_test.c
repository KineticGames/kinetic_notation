#include <kinetic_notation.h>

int main() {
  Structure *structure = structure_create();

  structure_add_key(structure, "name", STRING);
  structure_add_key(structure, "c_version", NUMBER);
  structure_add_key(structure, "version", VERSION);
  // object_array_reference *dependencies = object_array_create("dependencies");
  // object_array_add_key(dependencies, "name", STRING);
  // object_array_add_key(dependencies, "version", VERSION);
  // object_array_add_key(dependencies, "repository", STRING);
  // variable_key_array_reference *options_array =
  //     object_array_add_variable_key_array(dependencies, "options");

  return 0;
}
