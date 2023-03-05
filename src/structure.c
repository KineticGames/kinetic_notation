#include "kinetic_notation.h"

// std
#include <printf.h>
#include <stdio.h>

struct KnStructure {};

KnResult knCreateStructure(const KnStructureCreateInfo *createInfo,
                           KnStructure *structure) {
  *structure = malloc(sizeof(struct KnStructure));
  return SUCCESS;
}

void knDestroyStructure(KnStructure structure) { free(structure); }
