extern const struct CharacterInfo gCharacterInfo[];

enum // Up to 256 Character IDs
{
    CHARACTER_JACK,
    CHARACTER_JILL,
    CHARACTER_COUNT,
};

struct CharacterInfo
{
    u8 charcterId;
    u8 name[PLAYER_NAME_LENGTH + 1];
    u8 gender:2;
    u8 personality:3;
    u8 padding:3;
    u16 graphicsId;
};