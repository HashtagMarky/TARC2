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

        .highlights = COMPOUND_STRING(
            "The new Gym Leader of Vyraton\n"
            "and a true spark in the community,\n"
            "igniting passion and growth."
        ),
    },

    [CHARACTER_RESIDENT_SAMUEL] =
    {
        .name = _("Samuel"),
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
        .defaultEmotion = MUGSHOT_EMOTE_HAPPY,

        .baseOpinionKindness = 20,
        .baseOpinionStrength = 25,
        .opinionDecayChance = 0,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_LAUREN,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
            
            {
                .characterId = CHARACTER_RESIDENT_THOMAS,
                .title = COMPOUND_STRING("Best-Friend"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        },
    },

    [CHARACTER_RESIDENT_LAUREN] =
    {
        .name = _("Lauren"),
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
        .defaultEmotion = MUGSHOT_EMOTE_WORRIED,

        .baseOpinionKindness = 15,
        .baseOpinionStrength = 20,
        .opinionDecayChance = 10,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_SAMUEL,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
            
            {
                .characterId = CHARACTER_RESIDENT_KATIE,
                .title = COMPOUND_STRING("Best-Friend"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
        },
    },

    [CHARACTER_RESIDENT_THOMAS] =
    {
        .name = _("Thomas"),
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
        .defaultEmotion = MUGSHOT_EMOTE_JOYOUS,

        .baseOpinionKindness = 20,
        .baseOpinionStrength = 25,
        .opinionDecayChance = 5,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_HANNAH,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
            
            {
                .characterId = CHARACTER_RESIDENT_SAMUEL,
                .title = COMPOUND_STRING("Best-Friend"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        },
    },

    [CHARACTER_RESIDENT_HANNAH] =
    {
        .name = _("Hannah"),
        .highlights = COMPOUND_STRING(
            "An up and coming influencer who\n"
            "works with her cousin, showing her\n"
            "love and proficiency in performing."
        ),

        .partnerPokemon = SPECIES_AMOONGUSS,
        .favouriteType = TYPE_POISON,
        //.favouriteColour = ,
        .personality = ATTITUDE_HUMBLE,

        .overworldGraphicsId = OBJ_EVENT_GFX_HANNAH,
        .mugshotId = MUGSHOT_HANNAH,
        .defaultEmotion = MUGSHOT_EMOTE_INSPIRED,

        //.baseOpinionKindness = ,
        //.baseOpinionStrength = ,
        //.opinionDecayChance = ,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_THOMAS,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },

            {
                .characterId = CHARACTER_RESIDENT_GEORGIA,
                .title = COMPOUND_STRING("Cousin"),
                .affinity = OPINION_VERY_POSITIVE,
            },

            {
                .characterId = CHARACTER_RESIDENT_CAPTAIN,
                .title = COMPOUND_STRING("Grandfather"),
                .affinity = OPINION_POSITIVE,
            },
        },
    },

    [CHARACTER_RESIDENT_KATIE] =
    {
        .name = _("Katie"),
        .highlights = COMPOUND_STRING(
            "A kind-spirited teacher who hasn't\n"
            "ever said no to helping a friend,\n"
            "or even a foe, in need."
        ),

        .partnerPokemon = SPECIES_RIBOMBEE,
        .favouriteType = TYPE_BUG,
        //.favouriteColour = ,
        .personality = ATTITUDE_INSPIRED,

        .overworldGraphicsId = OBJ_EVENT_GFX_KATIE,
        .mugshotId = MUGSHOT_KATIE,
        .defaultEmotion = MUGSHOT_EMOTE_HAPPY,

        //.baseOpinionKindness = ,
        //.baseOpinionStrength = ,
        //.opinionDecayChance = ,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_LOUIS,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
            
            {
                .characterId = CHARACTER_RESIDENT_LAUREN,
                .title = COMPOUND_STRING("Best-Friend"),
                .affinity = OPINION_EXTREMELY_POSITIVE,
            },
        },
    },

    [CHARACTER_RESIDENT_LOUIS] =
    {
        .name = _("Louis"),
        .highlights = COMPOUND_STRING(
            "The gruff head of a construction\n"
            "company. Whilst not very talkative,\n"
            "he can show extreme compassion."
        ),

        .partnerPokemon = SPECIES_GURDURR,
        .favouriteType = TYPE_FIGHTING,
        //.favouriteColour = ,
        .personality = ATTITUDE_CYNICAL,

        .overworldGraphicsId = OBJ_EVENT_GFX_LOUIS,
        .mugshotId = MUGSHOT_LOUIS,
        .defaultEmotion = MUGSHOT_EMOTE_DETERMINED,

        //.baseOpinionKindness = ,
        //.baseOpinionStrength = ,
        //.opinionDecayChance = ,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_KATIE,
                .title = COMPOUND_STRING("Spouse"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        },
    },

    [CHARACTER_RESIDENT_GEORGIA] =
    {
        .name = _("Georgia"),
        .highlights = COMPOUND_STRING(
            "A trailblazing performer who\n"
            "thrives in the spotlight, and is\n"
            "passionate about singing."
        ),

        .partnerPokemon = SPECIES_WHIMSICOTT,
        .favouriteType = TYPE_FAIRY,
        //.favouriteColour = ,
        .personality = ATTITUDE_HUMBLE,

        .overworldGraphicsId = OBJ_EVENT_GFX_GEORGIA,
        .mugshotId = MUGSHOT_GEORGIA,
        .defaultEmotion = MUGSHOT_EMOTE_SHOUTING,

        //.baseOpinionKindness = ,
        //.baseOpinionStrength = ,
        //.opinionDecayChance = ,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_HANNAH,
                .title = COMPOUND_STRING("Cousin"),
                .affinity = OPINION_VERY_POSITIVE,
            },

            {
                .characterId = CHARACTER_RESIDENT_CAPTAIN,
                .title = COMPOUND_STRING("Grandfather"),
                .affinity = OPINION_POSITIVE,
            },
        },
    },

    [CHARACTER_RESIDENT_CAPTAIN] =
    {
        .name = _("Skip"),
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
        .defaultEmotion = MUGSHOT_EMOTE_STUNNED,

        .baseOpinionKindness = 0,
        .baseOpinionStrength = 0,
        .opinionDecayChance = 20,
        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_HANNAH,
                .title = COMPOUND_STRING("Granddaughter"),
                .affinity = OPINION_VERY_POSITIVE,
            },

            {
                .characterId = CHARACTER_RESIDENT_GEORGIA,
                .title = COMPOUND_STRING("Granddaughter"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        }
    },

    [CHARACTER_RESIDENT_DIEGO] =
    {
        .name = _("Diego"),
        .highlights = COMPOUND_STRING(
            "A dedicated conservationist, known\n"
            "for his efforts in protecting and\n"
            "repopulating endangered Pokémon."
        ),

        .partnerPokemon = SPECIES_LAPRAS,
        .favouriteType = TYPE_WATER,
        .favouriteColour = BODY_COLOR_BLUE,
        .personality = ATTITUDE_HUMBLE,

        .overworldGraphicsId = OBJ_EVENT_GFX_DIEGO,
        .mugshotId = MUGSHOT_DIEGO,
        .defaultEmotion = MUGSHOT_EMOTE_INSPIRED,

        .baseOpinionKindness = -10,
        .baseOpinionStrength = 0,
        .opinionDecayChance = 5,
    },

    [CHARACTER_WAYFARER_ASH] =
    {
        .name = _("Ash"),
        .highlights = COMPOUND_STRING(
            "A young but prolific battler, with\n"
            "an ability to maximise every bit\n"
            "of potential within his Pokémon."
        ),

        .partnerPokemon = SPECIES_PIKACHU,
        .favouriteType = TYPE_STEEL,
        .favouriteColour = BODY_COLOR_YELLOW,

        .personality = ATTITUDE_INSPIRED,

        .overworldGraphicsId = OBJ_EVENT_GFX_ASH_KETCHUM,
        .mugshotId = MUGSHOT_ASH,
        .defaultEmotion = MUGSHOT_EMOTE_DETERMINED,
    },

    [CHARACTER_WAYFARER_STEVEN] =
    {
        .name = _("Steven"),
        .highlights = COMPOUND_STRING(
            "A once Pokémon Champion who had\n"
            "a career change, in archaeolgy he\n"
            "focuses on his love of rare stones."
        ),

        .partnerPokemon = SPECIES_METAGROSS,
        .favouriteType = TYPE_STEEL,
        .favouriteColour = BODY_COLOR_PURPLE,

        .personality = ATTITUDE_HUMBLE,

        .overworldGraphicsId = OBJ_EVENT_GFX_STEVEN_GEN4,
        .mugshotId = MUGSHOT_STEVEN,
        .defaultEmotion = MUGSHOT_EMOTE_SPECIAL,

        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_LAUREN,
                .title = COMPOUND_STRING("Colleague"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        },
    },

    [CHARACTER_WAYFARER_CYNTHIA] =
    {
        .name = _("Cynthia"),
        .highlights = COMPOUND_STRING(
            "A renowned Pokémon Champion known\n"
            "for her mastery of strategy and\n"
            "deep knowledge of Pokémon history."
        ),

        .partnerPokemon = SPECIES_GARCHOMP,
        .favouriteType = TYPE_GROUND,
        .favouriteColour = BODY_COLOR_BLACK,

        .personality = ATTITUDE_DOMINANT,

        .overworldGraphicsId = OBJ_EVENT_GFX_CYNTHIA,
        .mugshotId = MUGSHOT_CYNTHIA,
        .defaultEmotion = MUGSHOT_EMOTE_SPECIAL,

        .relationships =
        {
            {
                .characterId = CHARACTER_RESIDENT_SAMUEL,
                .title = COMPOUND_STRING("Colleague"),
                .affinity = OPINION_VERY_POSITIVE,
            },
        },
    },

    [CHARACTER_WAYFARER_N] =
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
        .defaultEmotion = MUGSHOT_EMOTE_SPECIAL,
    },
};

// Dialogue Option Icons
static const u32 sCharacteristicDialogueIcon_Talk[] = INCBIN_U32("graphics/dialogue_icons/talk.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Talk[] = INCBIN_U16("graphics/dialogue_icons/talk.gbapal");
static const u32 sCharacteristicDialogueIcon_Opinion[] = INCBIN_U32("graphics/dialogue_icons/opinion.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Opinion[] = INCBIN_U16("graphics/dialogue_icons/opinion.gbapal");
static const u32 sCharacteristicDialogueIcon_Romance[] = INCBIN_U32("graphics/dialogue_icons/romance.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Romance[] = INCBIN_U16("graphics/dialogue_icons/romance.gbapal");
static const u32 sCharacteristicDialogueIcon_Quest[] = INCBIN_U32("graphics/dialogue_icons/quest.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Quest[] = INCBIN_U16("graphics/dialogue_icons/quest.gbapal");
static const u32 sCharacteristicDialogueIcon_Battle[] = INCBIN_U32("graphics/dialogue_icons/battle.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Battle[] = INCBIN_U16("graphics/dialogue_icons/battle.gbapal");
static const u32 sCharacteristicDialogueIcon_Gift[] = INCBIN_U32("graphics/dialogue_icons/gift.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Gift[] = INCBIN_U16("graphics/dialogue_icons/gift.gbapal");
static const u32 sCharacteristicDialogueIcon_Goodbye[] = INCBIN_U32("graphics/dialogue_icons/goodbye.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Goodbye[] = INCBIN_U16("graphics/dialogue_icons/goodbye.gbapal");

// Dialogue Attitude Icons
static const u32 sCharacteristicDialogueIcon_Neutral[] = INCBIN_U32("graphics/dialogue_icons/neutral.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Neutral[] = INCBIN_U16("graphics/dialogue_icons/neutral.gbapal");
static const u32 sCharacteristicDialogueIcon_Inspired[] = INCBIN_U32("graphics/dialogue_icons/inspired.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Inspired[] = INCBIN_U16("graphics/dialogue_icons/inspired.gbapal");
static const u32 sCharacteristicDialogueIcon_Humble[] = INCBIN_U32("graphics/dialogue_icons/humble.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Humble[] = INCBIN_U16("graphics/dialogue_icons/humble.gbapal");
static const u32 sCharacteristicDialogueIcon_Dominant[] = INCBIN_U32("graphics/dialogue_icons/dominant.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Dominant[] = INCBIN_U16("graphics/dialogue_icons/dominant.gbapal");
static const u32 sCharacteristicDialogueIcon_Cynical[] = INCBIN_U32("graphics/dialogue_icons/cynical.4bpp.smol");
static const u16 sCharacteristicDialoguePal_Cynical[] = INCBIN_U16("graphics/dialogue_icons/cynical.gbapal");

// Dialogue Options Data
const struct DialogueOptions gDialogueOptions[DIALOGUE_OPTION_COUNT] =
{
    [DIALOGUE_OPTION_TALK] =
    {
        .nameTitle = COMPOUND_STRING("Talk"),
        .iconImage = sCharacteristicDialogueIcon_Talk,
        .iconPal = sCharacteristicDialoguePal_Talk
    },
    [DIALOGUE_OPTION_OPINION] =
    {
        .nameTitle = COMPOUND_STRING("Opinion"),
        .iconImage = sCharacteristicDialogueIcon_Opinion,
        .iconPal = sCharacteristicDialoguePal_Opinion
    },
    [DIALOGUE_OPTION_ROMANCE] =
    {
        .nameTitle = COMPOUND_STRING("Romance"),
        .iconImage = sCharacteristicDialogueIcon_Romance,
        .iconPal = sCharacteristicDialoguePal_Romance
    },
    [DIALOGUE_OPTION_QUESTS] =
    {
        .nameTitle = COMPOUND_STRING("Quests"),
        .iconImage = sCharacteristicDialogueIcon_Quest,
        .iconPal = sCharacteristicDialoguePal_Quest
    },
    [DIALOGUE_OPTION_BATTLE] =
    {
        .nameTitle = COMPOUND_STRING("Battle"),
        .iconImage = sCharacteristicDialogueIcon_Battle,
        .iconPal = sCharacteristicDialoguePal_Battle
    },
    [DIALOGUE_OPTION_GIFT] =
    {
        .nameTitle = COMPOUND_STRING("Gift"),
        .iconImage = sCharacteristicDialogueIcon_Gift,
        .iconPal = sCharacteristicDialoguePal_Gift
    },
    [DIALOGUE_OPTION_GOODBYE] =
    {
        .nameTitle = COMPOUND_STRING("Goodbye"),
        .iconImage = sCharacteristicDialogueIcon_Goodbye,
        .iconPal = sCharacteristicDialoguePal_Goodbye
    },
};

// Dialogue Attitudes Data
const struct DialogueOptions gDialogueAttitudes[ATTITUDE_COUNT] =
{
    [ATTITUDE_NEUTRAL] =
    {
        .nameTitle = COMPOUND_STRING("Neutral"),
        .nameLower = COMPOUND_STRING("neutral"),
        .nameTitleColour = COMPOUND_STRING("{COLOR DARK_GRAY}Neutral{COLOR DARK_GRAY}"),
        .nameLowerColour = COMPOUND_STRING("{COLOR DARK_GRAY}neutral{COLOR DARK_GRAY}"),
        .kindnessEffect = 0,
        .strengthEffect = 0,
        .iconImage = sCharacteristicDialogueIcon_Neutral,
        .iconPal = sCharacteristicDialoguePal_Neutral
    },
    [ATTITUDE_INSPIRED] =
    {
        .nameTitle = COMPOUND_STRING("Inspired"),
        .nameLower = COMPOUND_STRING("inspired"),
        .nameTitleColour = COMPOUND_STRING("{COLOR BLUE}Inspired{COLOR DARK_GRAY}"),
        .nameLowerColour = COMPOUND_STRING("{COLOR BLUE}inspired{COLOR DARK_GRAY}"),
        .kindnessEffect = 1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicDialogueIcon_Inspired,
        .iconPal = sCharacteristicDialoguePal_Inspired
    },
    [ATTITUDE_HUMBLE] =
    {
        .nameTitle = COMPOUND_STRING("Humble"),
        .nameLower = COMPOUND_STRING("humble"),
        .nameTitleColour = COMPOUND_STRING("{COLOR GREEN}Humble{COLOR DARK_GRAY}"),
        .nameLowerColour = COMPOUND_STRING("{COLOR GREEN}humble{COLOR DARK_GRAY}"),
        .kindnessEffect = 1,
        .strengthEffect = -1,
        .iconImage = sCharacteristicDialogueIcon_Humble,
        .iconPal = sCharacteristicDialoguePal_Humble
    },
    [ATTITUDE_DOMINANT] =
    {
        .nameTitle = COMPOUND_STRING("Dominant"),
        .nameLower = COMPOUND_STRING("dominant"),
        .nameTitleColour = COMPOUND_STRING("{COLOR RED}Dominant{COLOR DARK_GRAY}"),
        .nameLowerColour = COMPOUND_STRING("{COLOR RED}dominant{COLOR DARK_GRAY}"),
        .kindnessEffect = -1,
        .strengthEffect = 1,
        .iconImage = sCharacteristicDialogueIcon_Dominant,
        .iconPal = sCharacteristicDialoguePal_Dominant
    },
    [ATTITUDE_CYNICAL] =
    {
        .nameTitle = COMPOUND_STRING("Cynical"),
        .nameLower = COMPOUND_STRING("cynical"),
        .nameTitleColour = COMPOUND_STRING("{COLOR LIGHT_BLUE}Cynical{COLOR DARK_GRAY}"),
        .nameLowerColour = COMPOUND_STRING("{COLOR LIGHT_BLUE}cynical{COLOR DARK_GRAY}"),
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