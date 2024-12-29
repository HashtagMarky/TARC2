#ifndef GUARD_IKIGAI_CHARACTERS_H
#define GUARD_IKIGAI_CHARACTERS_H

#define TAG_CHARACTER_DIALOGUE_ICON 4001

/*
#define CHAR_OBJ(name)
#define CHAR_MUGSHOT(name)
#define CHAR_PARTNET(name)
*/

struct Relationship {
    u8 characterId;
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
    bool8 isMainCharacter;
    u8 voice;

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

extern const struct IkigaiCharacterInfo gIkigaiCharactersInfo[];

u32 IkigaiCharacter_GetPlayerAttitude(void);
void IkigaiCharacter_SetDefaultOpinion(u32 character);
void IkigaiCharacter_SetAllCharacterDefaultOpinion(void);
s32 IkigaiCharacter_GetKindness(u32 character);
s32 IkigaiCharacter_GetStrength(u32 character);
s32 IkigaiCharacter_GetAverageKindness(void);
s32 IkigaiCharacter_GetAverageStrength(void);
s32 IkigaiCharacter_GetSetConversedFlag(u32 character, bool32 setFlag);
void IkigaiCharacter_ClearConversedFlags(void);
void IkigaiCharacter_SetRomanticFlag(u32 character);
void IkigaiCharacter_ToggleRomanticFlag(u32 character);
void IkigaiCharacter_ClearRomanticFlag(u32 character);
bool32 IkigaiCharacter_GetRomanticFlag(u32 character);
void IkigaiCharacter_SetRomanticFlag_Exclusive(u32 character);
void IkigaiCharacter_ClearRomanticFlag_Amicable(u32 character);
void IkigaiCharacter_ClearRomanticFlag_Hostile(u32 character);
u32 IkigaiCharacter_CheckRelationships(void);
bool32 IkigaiCharacter_IsPlayerSingleOrMonogamous(void);
bool32 IkigaiCharacter_ReturnOpinionDecay(u32 character);
void IkigaiCharacter_OpinionDecay(u32 character);
void IkigaiCharacter_AllOpinionDecay_NonConverse(void);
void IkigaiCharacter_HandleDialogue_Attitudes(void);
void DEBUG_IkigaiCharacter_CharacterOpinions(void);
u32 ReturnIkigaiCharacter_ObjectEventGraphicsId(u16 graphicsId);
u32 ReturnIkigaiCharacter_SelectedObject(void);
u32 ReturnIkigaiCharacter_RomanceFlag_Exclusive(void);
u8 CreateDialogueOptionIconSprite(u32 attitudeIndex);
u8 CreateAttitudeIconSprite(u32 attitudeIndex);

#endif // GUARD_IKIGAI_CHARACTERS_H