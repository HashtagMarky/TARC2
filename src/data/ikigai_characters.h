#include "global.h"
#include "constants/event_objects.h"
#include "constants/field_mugshots.h"
#include "constants/pokemon.h"
#include "constants/species.h"

// Ikigai Character Data
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
        .highlights = COMPOUND_STRING(
            "Mayor of Vyraton and an avid fan\n"
            "of Pokémon battles. The sponsor\n"
            "of the Vyraton Gym."
        ),

        .favouriteColour = BODY_COLOR_BLUE,
        .favouriteType = TYPE_FIRE,
        .personality = ATTITUDE_INSPIRED,

        .overworldGraphicsId = OBJ_EVENT_GFX_SAMUEL,
        .mugshotId = MUGSHOT_SAMUEL,
        .defaultEmotion = EMOTE_DETERMINED,

        .baseOpinionKindness = 20,
        .baseOpinionStrength = 25,
        .opinionDecayChance = 0,
        .relationships =
        {
            {
                .characterId = MAIN_CHARACTER_LAUREN,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
            
            {
                .characterId = MAIN_CHARACTER_THOMAS,
                .title = COMPOUND_STRING("Best-Friend"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        },
    },

    [MAIN_CHARACTER_LAUREN] =
    {
        .name = _("LAUREN"),
        .highlights = COMPOUND_STRING(
            "A devoted collector of rare stones\n"
            "and owner of Selenic Spirit, where\n"
            "they are showcased and marketed."
        ),

        .partnerPokemon = SPECIES_ESPURR,
        .favouriteType = TYPE_PSYCHIC,
        .favouriteColour = BODY_COLOR_YELLOW,
        .personality = ATTITUDE_CYNICAL,

        .overworldGraphicsId = OBJ_EVENT_GFX_LAUREN,
        .mugshotId = MUGSHOT_LAUREN,
        .defaultEmotion = EMOTE_WORRIED,

        .baseOpinionKindness = 15,
        .baseOpinionStrength = 20,
        .opinionDecayChance = 10,
        .relationships =
        {
            {
                .characterId = MAIN_CHARACTER_SAMUEL,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
        },
    },

    [MAIN_CHARACTER_THOMAS] =
    {
        .name = _("THOMAS"),
        .highlights = COMPOUND_STRING(
            "A Pokémon Professor with expertise\n"
            "in Pokémon breeding, and leads the\n"
            "research at Vyraton Laboratory."
        ),

        .partnerPokemon = SPECIES_ROCKRUFF,
        .favouriteType = TYPE_ROCK,
        .favouriteColour = BODY_COLOR_RED,
        .personality = ATTITUDE_DOMINANT,

        .overworldGraphicsId = OBJ_EVENT_GFX_THOMAS,
        .mugshotId = MUGSHOT_THOMAS,
        .defaultEmotion = EMOTE_JOYOUS,

        .baseOpinionKindness = 20,
        .baseOpinionStrength = 25,
        .opinionDecayChance = 5,
        .relationships =
        {
            {
                .characterId = MAIN_CHARACTER_SAMUEL,
                .title = COMPOUND_STRING("Best-Friend"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        },
    },

    [MAIN_CHARACTER_CAPTAIN] =
    {
        .name = _("SKIP"),
        .highlights = COMPOUND_STRING(
            "The still revered, but now retired\n"
            "captain of the S.S. Pathfinder, who\n"
            "now enjoys a less busy life."
        ),

        .partnerPokemon = SPECIES_RELICANTH,
        .favouriteType = TYPE_WATER,
        .favouriteColour = BODY_COLOR_GREEN,
        .personality = ATTITUDE_CYNICAL,

        .overworldGraphicsId = OBJ_EVENT_GFX_CAPTAIN,
        .mugshotId = MUGSHOT_CAPTAIN,
        .defaultEmotion = EMOTE_STUNNED,

        .baseOpinionKindness = 0,
        .baseOpinionStrength = 0,
        .opinionDecayChance = 20,
    },

    [SPECIAL_CHARACTER_N] =
    {
        .name = _("N"),
        .highlights = COMPOUND_STRING(
            "An advocate of Pokémon freedom\n"
            "and questioning the ethics of\n"
            "human-Pokémon bonds."
        ),

        .partnerPokemon = SPECIES_ZOROARK,
        .favouriteType = TYPE_DARK,
        .favouriteColour = BODY_COLOR_GRAY,

        .personality = ATTITUDE_HUMBLE,
        .poses =
        {
            [POSE_INSPIRED] =
            {
                .movement = IkigaiCharacter_Movement_N_Pose,
            },
            [POSE_HUMBLE] =
            {
                .movement = IkigaiCharacter_Movement_N_Shrug,
            },
            [POSE_DOMINANT] =
            {
                .movement = IkigaiCharacter_Movement_N_Shrug,
            },
            [POSE_CYNICAL] =
            {
                .movement = IkigaiCharacter_Movement_N_Headshake,
            },
        },

        .overworldGraphicsId = OBJ_EVENT_GFX_N,
        .mugshotId = MUGSHOT_N,
        .defaultEmotion = EMOTE_SPECIAL,
    }
};

// Dialogue Option Icons
static const u32 sCharacteristicDialogueIcon_Talk[] = INCBIN_U32("graphics/dialogue_icons/talk.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Talk[] = INCBIN_U16("graphics/dialogue_icons/talk.gbapal");
static const u32 sCharacteristicDialogueIcon_Opinion[] = INCBIN_U32("graphics/dialogue_icons/opinion.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Opinion[] = INCBIN_U16("graphics/dialogue_icons/opinion.gbapal");
static const u32 sCharacteristicDialogueIcon_Romance[] = INCBIN_U32("graphics/dialogue_icons/romance.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Romance[] = INCBIN_U16("graphics/dialogue_icons/romance.gbapal");
static const u32 sCharacteristicDialogueIcon_Quest[] = INCBIN_U32("graphics/dialogue_icons/quest.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Quest[] = INCBIN_U16("graphics/dialogue_icons/quest.gbapal");
static const u32 sCharacteristicDialogueIcon_Battle[] = INCBIN_U32("graphics/dialogue_icons/battle.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Battle[] = INCBIN_U16("graphics/dialogue_icons/battle.gbapal");
static const u32 sCharacteristicDialogueIcon_Gift[] = INCBIN_U32("graphics/dialogue_icons/gift.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Gift[] = INCBIN_U16("graphics/dialogue_icons/gift.gbapal");
static const u32 sCharacteristicDialogueIcon_Goodbye[] = INCBIN_U32("graphics/dialogue_icons/goodbye.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Goodbye[] = INCBIN_U16("graphics/dialogue_icons/goodbye.gbapal");

// Dialogue Attitude Icons
static const u32 sCharacteristicDialogueIcon_Neutral[] = INCBIN_U32("graphics/dialogue_icons/neutral.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Neutral[] = INCBIN_U16("graphics/dialogue_icons/neutral.gbapal");
static const u32 sCharacteristicDialogueIcon_Inspired[] = INCBIN_U32("graphics/dialogue_icons/inspired.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Inspired[] = INCBIN_U16("graphics/dialogue_icons/inspired.gbapal");
static const u32 sCharacteristicDialogueIcon_Humble[] = INCBIN_U32("graphics/dialogue_icons/humble.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Humble[] = INCBIN_U16("graphics/dialogue_icons/humble.gbapal");
static const u32 sCharacteristicDialogueIcon_Dominant[] = INCBIN_U32("graphics/dialogue_icons/dominant.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Dominant[] = INCBIN_U16("graphics/dialogue_icons/dominant.gbapal");
static const u32 sCharacteristicDialogueIcon_Cynical[] = INCBIN_U32("graphics/dialogue_icons/cynical.4bpp.lz");
static const u16 sCharacteristicDialoguePal_Cynical[] = INCBIN_U16("graphics/dialogue_icons/cynical.gbapal");

// Dialogue Options Data
const struct DialogueOptions gDialogueOptions[DIALOGUE_OPTION_COUNT] =
{
    [DIALOGUE_OPTION_TALK] =
    {
        .name = COMPOUND_STRING("Talk"),
        .iconImage = sCharacteristicDialogueIcon_Talk,
        .iconPal = sCharacteristicDialoguePal_Talk
    },
    [DIALOGUE_OPTION_OPINION] =
    {
        .name = COMPOUND_STRING("Opinion"),
        .iconImage = sCharacteristicDialogueIcon_Opinion,
        .iconPal = sCharacteristicDialoguePal_Opinion
    },
    [DIALOGUE_OPTION_ROMANCE] =
    {
        .name = COMPOUND_STRING("Romance"),
        .iconImage = sCharacteristicDialogueIcon_Romance,
        .iconPal = sCharacteristicDialoguePal_Romance
    },
    [DIALOGUE_OPTION_QUESTS] =
    {
        .name = COMPOUND_STRING("Quests"),
        .iconImage = sCharacteristicDialogueIcon_Quest,
        .iconPal = sCharacteristicDialoguePal_Quest
    },
    [DIALOGUE_OPTION_BATTLE] =
    {
        .name = COMPOUND_STRING("Battle"),
        .iconImage = sCharacteristicDialogueIcon_Battle,
        .iconPal = sCharacteristicDialoguePal_Battle
    },
    [DIALOGUE_OPTION_GIFT] =
    {
        .name = COMPOUND_STRING("Gift"),
        .iconImage = sCharacteristicDialogueIcon_Gift,
        .iconPal = sCharacteristicDialoguePal_Gift
    },
    [DIALOGUE_OPTION_GOODBYE] =
    {
        .name = COMPOUND_STRING("Goodbye"),
        .iconImage = sCharacteristicDialogueIcon_Goodbye,
        .iconPal = sCharacteristicDialoguePal_Goodbye
    },
};

// Dialogue Attitudes Data
const struct DialogueOptions gDialogueAttitudes[ATTITUDE_COUNT] =
{
    [ATTITUDE_NEUTRAL] =
    {
        .name = COMPOUND_STRING("Neutral"),
        .kindnessEffect = 0,
        .strengthEffect = 0,
        .iconImage = sCharacteristicDialogueIcon_Neutral,
        .iconPal = sCharacteristicDialoguePal_Neutral
    },
    [ATTITUDE_INSPIRED] =
    {
        .name = COMPOUND_STRING("Inspired"),
        .kindnessEffect = 1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicDialogueIcon_Inspired,
        .iconPal = sCharacteristicDialoguePal_Inspired
    },
    [ATTITUDE_HUMBLE] =
    {
        .name = COMPOUND_STRING("Humble"),
        .kindnessEffect = 1,
        .strengthEffect = -1,
        .iconImage = sCharacteristicDialogueIcon_Humble,
        .iconPal = sCharacteristicDialoguePal_Humble
    },
    [ATTITUDE_DOMINANT] =
    {
        .name = COMPOUND_STRING("Dominant"),
        .kindnessEffect = -1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicDialogueIcon_Dominant,
        .iconPal = sCharacteristicDialoguePal_Dominant
    },
    [ATTITUDE_CYNICAL] =
    {
        .name = COMPOUND_STRING("Cynical"),
        .kindnessEffect = -1,
        .strengthEffect = -1,
        .iconImage = sCharacteristicDialogueIcon_Cynical,
        .iconPal = sCharacteristicDialoguePal_Cynical
    },
};

// Opinion Bonus Functions
const struct OpinionBonusFunctions gOpinionBonusFunction[OB_FUNCTION_COUNT] =
{
    [OB_FUNCTION_RELATIONSHIP] =
    {
        .stringKindness = COMPOUND_STRING("I like who your\nfriends are."),
        .stringStrength = COMPOUND_STRING("My friends say\nyou're strong."),
        .function = IkigaiCharacterOpinionBonus_Relationship,
    },
    [OB_FUNCTION_PARTNER_POKEMON] =
    {
        .stringKindness = COMPOUND_STRING("I like the same\n{PKMN} as you."),
        .stringStrength = COMPOUND_STRING("I think your\ngym is strong."),
        .function = IkigaiCharacterOpinionBonus_PartnerPokemon,
    },
    [OB_FUNCTION_STARTER_POKEMON] =
    {
        .stringKindness = COMPOUND_STRING("I like your\npartner {PKMN}."),
        .stringStrength = COMPOUND_STRING("Your partner\n{PKMN} is strong."),
        .function = IkigaiCharacterOpinionBonus_StarterPokemon,
    },
};