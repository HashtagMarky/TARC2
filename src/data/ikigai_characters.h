#include "global.h"
#include "constants/event_objects.h"
#include "constants/field_mugshots.h"
#include "constants/species.h"

const struct IkigaiCharacterInfo gIkigaiCharactersInfo[] =
{
    [CHARACTER_DEFAULT] =
    {
        .name = _("??????"),

        .overworldGraphicsId = NUM_OBJ_EVENT_GFX,
    },

    [CHARACTER_SAMUEL] =
    {
        .name = _("SAMUEL"),

        .partnerPokemon = SPECIES_INFERNAPE,

        .overworldGraphicsId = OBJ_EVENT_GFX_SAMUEL,
        .mugshotId = MUGSHOT_SAMUEL,
    },

    [CHARACTER_LAUREN] =
    {
        .name = _("LAUREN"),

        .partnerPokemon = SPECIES_LIEPARD,

        .overworldGraphicsId = OBJ_EVENT_GFX_LAUREN,
        .mugshotId = MUGSHOT_LAUREN,
    },

    [CHARACTER_THOMAS] =
    {
        .name = _("THOMAS"),

        .overworldGraphicsId = OBJ_EVENT_GFX_THOMAS,
    },

    [CHARACTER_CAPTAIN] =
    {
        .name = _("SKIP"),

        .overworldGraphicsId = OBJ_EVENT_GFX_CAPTAIN,
    },
};