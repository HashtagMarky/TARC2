#ifndef GUARD_IKIGAI_CONSTANTS_CHARACTERS_H
#define GUARD_IKIGAI_CONSTANTS_CHARACTERS_H

#define OPINION_TYPE_KINDNESS               0
#define OPINION_TYPE_STRENGTH               1
#define RELATIONSHIP_AFFINITY_MULTIPLIER    3
#define MAX_RELATIONSHIPS                   3
#define MAX_POSTS                           3
#define OPINION_POSITIVE                    5
#define OPINION_VERY_POSITIVE               10
#define OPINION_EXTREMELY_POSITIVE          20
#define OPINION_NEUTRAL_BUFFER              20

enum DialogueOption
{
    DIALOGUE_OPTION_TALK,
    DIALOGUE_OPTION_OPINION,
    DIALOGUE_OPTION_ROMANCE,
    DIALOGUE_OPTION_QUESTS,
    DIALOGUE_OPTION_BATTLE,
    DIALOGUE_OPTION_GIFT,
    DIALOGUE_OPTION_GOODBYE,
    DIALOGUE_OPTION_COUNT,
};

enum AttitudeId
{
    ATTITUDE_NEUTRAL,
    ATTITUDE_INSPIRED,
    ATTITUDE_HUMBLE,
    ATTITUDE_DOMINANT,
    ATTITUDE_CYNICAL,
    ATTITUDE_COUNT,
};

enum PoseId
{
    POSE_INSPIRED = ATTITUDE_INSPIRED - 1,
    POSE_HUMBLE = ATTITUDE_HUMBLE - 1,
    POSE_DOMINANT = ATTITUDE_DOMINANT - 1,
    POSE_CYNICAL = ATTITUDE_CYNICAL - 1,
    POSE_COUNT = ATTITUDE_COUNT - 1,
};

enum RelationshipType
{
    RELATIONSHIP_SINGLE,
    RELATIONSHIP_MONOGOMOUS,
    RELATIONSHIP_POLYAMOROUS,
};

enum CharacterId
{
    CHARACTER_DEFAULT,
    CHARACTER_RESIDENT_SAMUEL,
    CHARACTER_RESIDENT_LAUREN,
    CHARACTER_RESIDENT_CAPTAIN,
    CHARACTER_RESIDENT_THOMAS,
    CHARACTER_RESIDENT_KATIE,
    CHARACTER_RESIDENT_DIEGO,
    CHARACTER_RESIDENT_GEORGIA,
    CHARACTER_RESIDENT_HANNAH,
    CHARACTER_RESIDENT_LOUIS,
    CHARACTER_RESIDENT_COUNT,
    CHARACTER_WAYFARER_ASH,
    CHARACTER_WAYFARER_STEVEN,
    CHARACTER_WAYFARER_CYNTHIA,
    CHARACTER_WAYFARER_N,
    CHARACTER_COUNT_TOTAL,
};

#define CHARACTER_PLAYER    CHARACTER_DEFAULT
#define CHARACTER_FIRST     CHARACTER_DEFAULT + 1
#define CHARACTER_LAST      CHARACTER_COUNT_TOTAL - 1

enum RomanceOptions // Temporary (Moved From Poryscript but will formalise)
{
    SET_CLEAR_ROMANCE,
    ROMANCE_EXCLUSIVE,
    CHECK_ROMANCE,
};

enum OpinionBonusFunctionIds
{
    OB_FUNCTION_RELATIONSHIP,
    OB_FUNCTION_PARTNER_POKEMON,
    OB_FUNCTION_STARTER_POKEMON,
    OB_FUNCTION_COUNT,
};

#endif // GUARD_IKIGAI_CONSTANTS_CHARACTERS_H