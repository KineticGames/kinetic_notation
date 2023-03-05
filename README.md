## API Outline

1. Define how the structure should look.
2. Feed in a file.
3. get back the wanted structure.

```c
  KnStructureCreateInfo create_info = {
      .keys =
          (KnKeyCreateInfo[]){
              {.key = "name", .type = STRING},
              {.key = "c_version", .type = NUMBER},
              {.key = "version", .type = VERSION},
              {
                  .key = "dependencies",
                  .type = OBJECT_ARRAY,
                  .objectArrayOutline =
                      (KnStructureCreateInfo){
                          .keys =
                              (KnKeyCreateInfo[]){
                                  {.key = "name", .type = STRING},
                                  {.key = "options",
                                   .type = VARIABLE_KEY_ARRAY},
                                  {.key = "version", .type = VERSION},
                              },
                          .keyCount = 3,
                      },
              },
          },
      .keyCount = 4,
  };

Structure *build_definition = parse_file("/path/to/file.kn", outline);

KineticValue name = structure_get_key(build_definition, "name");
if (name.filled)
  printf("The name is: %s\n", name.string);

int dependency_count = 0;
structure_get_object_array(build_definition, "dependencies", &dependency_count,
                           NULL);
if (dependency_count == 0) {
  printf("No dependencies were found.");
} else {
  Structure *dependency_definitions[dependency_count];
  structure_get_object_array(build_definition, "dependencies",
                             &dependency_count, dependency_definitions);
  for (int i = 0; i < dependency_count; ++i) {
    KineticValue dependency_name =
        structure_get_key(dependency_definitions[i], "name");
    if (name.filled)
      printf("The name of dependency #%d is: %s\n", i, name.string);
  }
}
```

