#ifndef GUARD_IKIGAI_CHARACTERS_H
#define GUARD_IKIGAI_CHARACTERS_H

#define TAG_CHARACTER_DIALOGUE_ICON 4001

/*
#define CHAR_OBJ(name)
#define CHAR_MUGSHOT(name)
#define CHAR_PARTNET(name)
*/

struct Poses {
    const u8 *movement;
};

struct Relationship {
    u8 characterId;
    const u8 *title;
    s8 affinity;
};

struct InventoryItem
{
    u16 itemId;             // Unique identifier for the item
    u8 itemQuantity;           // Quantity of the item
    u8 itemChance;         // Whether the item is a key item
};

struct IkigaiCharacterInfo
{
    // Character Identification
    u8 charcterId;
    u8 name[PLAYER_NAME_LENGTH + 1];
    u8 pronouns;
    const u8 *highlights;
    bool8 isMainCharacter;
    u8 voice;
    struct Poses poses[POSE_COUNT];

    // Basic Details
    u16 partnerPokemon;
    u8 favouriteColour;
    u8 favouriteType;
    u8 personality;
    u8 birthDate;
    u8 birthMonth;

    // Appearance & Graphics
    u16 overworldGraphicsId;
    u16 mugshotId;

    // Mood & Behaviour
    u8 defaultEmotion;
    u8 moodPattern;     // Description of mood patterns (based on location, time, etc.)
    u16 morningMapId;  // Map they prefer to go to in the morning
    u16 afternoonMapId;// Map they prefer to go to in the afternoon
    u16 eveningMapId;  // Map they prefer to go to in the evening
    u16 homeMapSec;
    u16 homeMapId;

    // Friendship & Social Interactions
    s8 baseOpinionKindness;
    s8 baseOpinionStrength;
    u8 opinionDecayChance;    // Percentage chance of an opinion decaying
    u16 flagMet;
    u16 flagRomantic;
    // char** likes;          // Array of items, locations, or weather preferences
    // char** dislikes;       // Array of items, locations, or weather dislikes
    // char** uniqueDialogue; // Unique dialogue lines for the character

    // Relationships
    struct Relationship relationships[MAX_RELATIONSHIPS];  // Array of relationships with other characters

    // Quests & Events
    // int* questIds;         // Array of quest IDs that involve this character

    // Inventory
    struct InventoryItem* items; // Array of items the character carries

    // Battle Information
    u8 trainerPartyCount;
    u16 trainerParty;
};

struct DialogueOptions
{
    const u8 *nameTitle;
    const u8 *nameLower;
    const u8 *nameTitleColour;
    const u8 *nameLowerColour;
    s8 kindnessEffect;
    s8 strengthEffect;
    const u32 *iconImage;
    const u16 *iconPal;
};

struct OpinionBonusFunctions
{
    const u8 *stringKindness;
    const u8 *stringStrength;
    s32 (*function)(enum CharacterId character, bool32 opinionType);
};

extern const struct IkigaiCharacterInfo gIkigaiCharactersInfo[];
extern const struct DialogueOptions gDialogueOptions[];
extern const struct DialogueOptions gDialogueAttitudes[];
extern const struct OpinionBonusFunctions gOpinionBonusFunction[];

enum AttitudeId IkigaiCharacter_GetPlayerAttitude(void);
enum AttitudeId IkigaiCharacter_GetPlayerAttitude_Character(enum CharacterId charcter);
void IkigaiCharacter_SetDefaultOpinion(enum CharacterId charcter);
void IkigaiCharacter_SetAllCharacterDefaultOpinion(void);
s32 IkigaiCharacter_GetKindness(enum CharacterId charcter);
s32 IkigaiCharacter_GetStrength(enum CharacterId charcter);
s32 IkigaiCharacter_GetKindness_Wayfarer(enum CharacterId charcter);
s32 IkigaiCharacter_GetStrength_Wayfarer(enum CharacterId charcter);
s32 IkigaiCharacter_GetOpinionBonus(enum CharacterId charcter, u32 opinionType);
s32 IkigaiCharacter_GetAverageKindness(void);
s32 IkigaiCharacter_GetAverageStrength(void);
s32 IkigaiCharacter_GetSetConversedFlag(enum CharacterId charcter, bool32 setFlag);
void IkigaiCharacter_ClearConversedFlags(void);
void IkigaiCharacter_SetRomanticFlag(enum CharacterId charcter);
void IkigaiCharacter_ToggleRomanticFlag(enum CharacterId charcter);
void IkigaiCharacter_ClearRomanticFlag(enum CharacterId charcter);
bool32 IkigaiCharacter_GetRomanticFlag(enum CharacterId charcter);
void IkigaiCharacter_SetRomanticFlag_Exclusive(enum CharacterId charcter);
void IkigaiCharacter_ClearRomanticFlag_Amicable(enum CharacterId charcter);
void IkigaiCharacter_ClearRomanticFlag_Hostile(enum CharacterId charcter);
enum CharacterId IkigaiCharacter_CheckRelationships(void);
bool32 IkigaiCharacter_IsPlayerSingleOrMonogamous(void);
void IkigaiCharacter_SetMetFlag(enum CharacterId charcter);
bool32 IkigaiCharacter_GetMetFlag(enum CharacterId charcter);
void IkigaiCharacter_SetAllMetFlags(void);
bool32 IkigaiCharacter_ReturnOpinionDecay(enum CharacterId charcter);
void IkigaiCharacter_OpinionDecay(enum CharacterId charcter);
void IkigaiCharacter_AllOpinionDecay_NonConverse(void);
void IkigaiCharacter_HandleDialogue_Attitudes(void);
void DEBUG_IkigaiCharacter_CharacterOpinions(void);
void SetDefaultPlayerNickname(void);
bool32 IkigaiCharacter_NicknameInsteadOfName(enum CharacterId charcter);
void IkigaiCharacter_MovementEmote(u8 localId, const u8 *movement);
enum PoseId IkigaiCharacter_GetPoseFromAttitude(enum AttitudeId attitude);
const u8 *IkigaiCharacter_GetMovementFromAttitude(enum CharacterId charcter, enum AttitudeId attitude);
void IkigaiCharacter_DefaultEmote(void);
void IkigaiCharacter_ResponseEmote(void);
enum CharacterId ReturnIkigaiCharacter_ObjectEventGraphicsId(u16 graphicsId);
enum CharacterId ReturnIkigaiCharacter_SelectedObject(void);
enum MugshotIDs ReturnIkigaiMugshotID_ObjectEventGraphicsId(u16 graphicsId);
enum MugshotIDs ReturnIkigaiMugshotID_SelectedObject(void);
enum CharacterId ReturnIkigaiCharacter_MugshotId(enum MugshotIDs mugshotId);
enum CharacterId ReturnIkigaiCharacter_RomanceFlag_Exclusive(void);
s32 IkigaiCharacterOpinionBonus_Relationship(enum CharacterId charcter, bool32 opinionType);
s32 IkigaiCharacterOpinionBonus_PartnerPokemon(enum CharacterId charcter, bool32 opinionType);
s32 IkigaiCharacterOpinionBonus_StarterPokemon(enum CharacterId charcter, bool32 opinionType);
u8 IkigaiCharacter_ReturnMessageBoxPersonalityPalette(enum CharacterId charcter);
u8 CreateDialogueOptionIconSprite(enum DialogueOption dialogue);
u8 CreateAttitudeIconSprite(enum AttitudeId attitude);

extern const u8 IkigaiCharacter_Movement_N_Shrug[];
extern const u8 IkigaiCharacter_Movement_N_Headshake[];
extern const u8 IkigaiCharacter_Movement_N_Pose[];

#endif // GUARD_IKIGAI_CHARACTERS_H