#include <kinetic_notation.h>

#define ASSERT_EQ(lhs, rhs)                                                    \
  {                                                                            \
    if ((lhs) != (rhs)) {                                                      \
      return 1;                                                                \
    }                                                                          \
  }

#define ASSERT_NE(lhs, rhs)                                                    \
  {                                                                            \
    if ((lhs) == (rhs)) {                                                      \
      return 1;                                                                \
    }                                                                          \
  }

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
                  .objectOutline =
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

  kinetic_notation_structure_destroy(structure);

  return 0;
}
