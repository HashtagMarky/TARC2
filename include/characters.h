extern const struct CharacterInfo gCharacterInfo[];

enum // Up to 256 Character IDs
{
    CHARACTER_JACK,
    CHARACTER_JILL,
    CHARACTER_COUNT,
};

struct CharacterInfo
{
    // Character Identification
    u8 charcterId;
    u8 name[PLAYER_NAME_LENGTH + 1];
    u8 pronouns;
    bool8 isMainCharacter;

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