#ifndef KINETIC_NOTATION_STRUCTURE_H
#define KINETIC_NOTATION_STRUCTURE_H

#include "key_value_pair.h"

struct KnStructure_t {
  KVPair *keys;
  uint32_t key_count;
};

KnResult create_structure_from_create_info(KnStructureCreateInfo createInfo,
                                           struct KnStructure_t *structure);

KVPair *structure_find_key(KnStructure structure, const char *key);

#endif // KINETIC_NOTATION_STRUCTURE_H
