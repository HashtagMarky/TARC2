#include "global.h"
#include "constants/event_objects.h"
#include "constants/field_mugshots.h"
#include "constants/pokemon.h"
#include "constants/species.h"

const struct IkigaiCharacterInfo gIkigaiCharactersInfo[] =
{
    [CHARACTER_DEFAULT] =
    {
        .name = _("??????"),

        .overworldGraphicsId = NUM_OBJ_EVENT_GFX,
    },

    [MAIN_CHARACTER_SAMUEL] =
    {
        .name = _("SAMUEL"),
        .partnerPokemon = SPECIES_INFERNAPE,
        .favouriteColour = BODY_COLOR_BLUE,
        .favouriteType = TYPE_FIRE,
        .personality = ATTITUDE_INSPIRED,

        .overworldGraphicsId = OBJ_EVENT_GFX_SAMUEL,
        .mugshotId = MUGSHOT_SAMUEL,
    },

    [MAIN_CHARACTER_LAUREN] =
    {
        .name = _("LAUREN"),
        .partnerPokemon = SPECIES_LIEPARD,
        .favouriteColour = BODY_COLOR_YELLOW,
        .personality = ATTITUDE_CYNICAL,

        .overworldGraphicsId = OBJ_EVENT_GFX_LAUREN,
        .mugshotId = MUGSHOT_LAUREN,
    },

    [MAIN_CHARACTER_THOMAS] =
    {
        .name = _("THOMAS"),

        .overworldGraphicsId = OBJ_EVENT_GFX_THOMAS,
    },

    [MAIN_CHARACTER_CAPTAIN] =
    {
        .name = _("SKIP"),

        .overworldGraphicsId = OBJ_EVENT_GFX_CAPTAIN,
    },
};