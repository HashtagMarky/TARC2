#ifndef GUARD_CONSTANTS_FIELD_MUGSHOTS_H
#define GUARD_CONSTANTS_FIELD_MUGSHOTS_H

#include "constants/event_objects.h"
#include "constants/species.h"

// Mugshot Types
#define MUGSHOT_DEFINED     0
#define MUGSHOT_NPC         1
#define MUGSHOT_PMD         2
#define MUGSHOT_TYPES       3

// Mugshot IDs
#define MUGSHOT_BRENDAN                     OBJ_EVENT_GFX_BRENDAN_NORMAL
#define MUGSHOT_MAY                         OBJ_EVENT_GFX_MAY_NORMAL
#define MUGSHOT_SUBSTITUTE_DOLL             OBJ_EVENT_GFX_SPECIES(NONE)
#define MUGSHOT_SUBSTITUTE_DOLL_SHINY       OBJ_EVENT_GFX_SPECIES_SHINY(NONE)

// Emotes
#define EMOTE_NORMAL        0
#define EMOTE_ANGRY         1
#define EMOTE_CRYING        2
#define EMOTE_DETERMINED    3
#define EMOTE_DIZZY         4
#define EMOTE_HAPPY         5
#define EMOTE_INSPIRED      6
#define EMOTE_JOYOUS        7
#define EMOTE_PAIN          8
#define EMOTE_SAD           9
#define EMOTE_SHOUTING      10
#define EMOTE_SIGH          11
#define EMOTE_STUNNED       12
#define EMOTE_SURPRISED     13
#define EMOTE_TEARY         14
#define EMOTE_WORRIED       15
#define EMOTE_SPECIAL       16
#define EMOTE_COUNT         17

#endif // GUARD_CONSTANTS_FIELD_MUGSHOTS_H
