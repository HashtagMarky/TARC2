#ifndef GUARD_IKIGAI_CHARACTERS_H
#define GUARD_IKIGAI_CHARACTERS_H

#define TAG_CHARACTER_DIALOGUE_ICON 4001
/*
#define CHAR_OBJ(name)
#define CHAR_MUGSHOT(name)
#define CHAR_PARTNET(name)
*/
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
    u8 baseFriendship;
    u8 friendshipGrowthRate;
    u8 friendshipDecayRate;
    // char** likes;          // Array of items, locations, or weather preferences
    // char** dislikes;       // Array of items, locations, or weather dislikes
    // char** uniqueDialogue; // Unique dialogue lines for the character

    // Relationships
    struct Relationship* relationships;  // Array of relationships with other characters

    // Quests & Events
    // int* questIds;         // Array of quest IDs that involve this character

    // Inventory
    struct InventoryItem* items; // Array of items the character carries

    // Battle Information
    u8 trainerPartyCount;
    u16 trainerParty;
};

struct Relationship
{
    u8 characterId;             // ID of the related character
    s8 affinity;                // Affinity level, representing the strength of the relationship
    bool8 isDynamic;            // Whether the relationship changes over time or with events
};

struct InventoryItem
{
    u16 itemId;             // Unique identifier for the item
    u8 itemQuantity;           // Quantity of the item
    u8 itemChance;         // Whether the item is a key item
};

extern const struct IkigaiCharacterInfo gIkigaiCharactersInfo[];

u8 ReturnIkigaiCharacter_ObjectEventGraphicsId(u16 graphicsId);
s8 IkigaiCharacter_GetSetConversedFlag(u8 character, bool8 setFlag);
void IkigaiCharacter_ClearConversedFlags(void);
void IkigaiCharacter_HandleDialogue(void);
s8 IkigaiCharacter_AverageKindness(void);
s8 IkigaiCharacter_AverageStrength(void);
u8 IkigaiCharacter_GetPlayerAttitude(void);
u8 CreateDialogueIconSprite(u8 characteristicIndex);

#endif // GUARD_IKIGAI_CHARACTERS_H