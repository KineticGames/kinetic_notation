## API Outline

1. Define how the structure should look.
2. Feed in a file.
3. get back the wanted structure.

```c
OUTLINE(outline) = {
    {.key = "name", .value_type = STRING},
    {.key = "c_version", .value_type = NUMBER},
    {.key = "version", .value_type = VERSION},
    {.key = "repository", .value_type = STRING},
    {.key = "owner", .value_type = STRING},
    {
        .key = "dependencies",
        .object_array_outline =
            OBJECT_ARRAY_OUTLINE{
                {.key = "name", .value_type = STRING},
                {.key = "version", .value_type = VERSION},
                {.key = "options", .value_type = VARIABLE_KEY_ARRAY},
                {.key = "repository", .value_type = STRING},
            },
    },
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

