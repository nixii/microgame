
#ifndef MICROENGINE_ENTITY_H
#define MICROENGINE_ENTITY_H

#define ME_MAX_ENTITIES 4096

typedef int me_entity;
#define ME_NIL_ENTITY -1

#ifndef MICROENGINE_NOSTRIP
    #define MAX_ENTITIES ME_MAX_ENTITIES
    #define entity me_entity
    #define NIL_ENTITY ME_NIL_ENTITY
#endif

#endif // MICROENGINE_ENTITY_H