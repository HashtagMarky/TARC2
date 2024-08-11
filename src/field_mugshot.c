#include "global.h"
#include "decompress.h"
#include "sprite.h"
#include "script.h"
#include "event_data.h"
#include "field_weather.h"
#include "constants/field_mugshots.h"
#include "data/field_mugshots.h"

#include "global.h"
#include "list_menu.h"
#include "main.h"
#include "menu.h"
#include "script.h"
#include "sound.h"
#include "strings.h"
#include "task.h"
#include "constants/songs.h"

#include "constants/event_objects.h"

static EWRAM_DATA u8 sFieldMugshotSpriteId = 0;

#define TAG_MUGSHOT 0x9000

// don't remove the `+ 32` for 
// otherwise your sprite will not be placed in the place you desire
#define MUGSHOT_DEFINED_X   168 + 32
#define MUGSHOT_DEFINED_Y   51  + 32
#define MUGSHOT_NPC_X       200
#define MUGSHOT_NPC_Y       74
#define MUGSHOT_PMD_X       192
#define MUGSHOT_PMD_Y       72

static const struct WindowTemplate sMugshotWindowNPC =
{
    .bg = 0,
    .tilemapLeft = 21,
    .tilemapTop = 5,
    .width = 8,
    .height = 8,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct WindowTemplate sMugshotWindowPMD =
{
    .bg = 0,
    .tilemapLeft = 23,
    .tilemapTop = 8,
    .width = 5,
    .height = 5,
    .paletteNum = 15,
    .baseBlock = 1,
};

static const struct OamData sFieldMugshot_Oam = {
    .size = SPRITE_SIZE(64x64),
    .shape = SPRITE_SHAPE(64x64),
    .priority = 0,
};

static const struct SpriteTemplate sFieldMugshot_SpriteTemplate = {
    .tileTag = TAG_MUGSHOT,
    .paletteTag = TAG_MUGSHOT,
    .oam = &sFieldMugshot_Oam,
    .callback = SpriteCallbackDummy,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
};

void RemoveFieldMugshot(void)
{
    u8 windowId;
    
    if (IndexOfSpritePaletteTag(TAG_MUGSHOT) != 0xFF)
    {
        ResetPreservedPalettesInWeather();
        windowId = AddWindow(&sMugshotWindowNPC);
        ClearStdWindowAndFrame(windowId, TRUE);
        windowId = AddWindow(&sMugshotWindowPMD);
        ClearStdWindowAndFrame(windowId, TRUE);
        RemoveWindow(windowId);
        DestroySprite(&gSprites[sFieldMugshotSpriteId]);
        FreeSpritePaletteByTag(TAG_MUGSHOT);
        FreeSpriteTilesByTag(TAG_MUGSHOT);
        sFieldMugshotSpriteId = SPRITE_NONE;
    }
}

void CreateFieldMugshot(u8 mugshotType, u16 mugshotId, u16 mugshotEmotion)
{
    u8 windowId = 0;
    s16 x = MUGSHOT_DEFINED_X;
    s16 y = MUGSHOT_DEFINED_Y;

    struct CompressedSpriteSheet sheet = { .size=0x1000, .tag=TAG_MUGSHOT };
    struct SpritePalette pal = { .tag = sheet.tag };

    RemoveFieldMugshot();
    if (mugshotId >= NELEMS(sFieldMugshots))
    {
        return;
    }

    if ((sFieldMugshots[mugshotId][mugshotEmotion].gfx == NULL || sFieldMugshots[mugshotId][mugshotEmotion].pal == NULL))
    {
        return;
    }
    else
    {
        sheet.data = sFieldMugshots[mugshotId][mugshotEmotion].gfx;
        pal.data = sFieldMugshots[mugshotId][mugshotEmotion].pal;
    }
    
    if (mugshotType != MUGSHOT_DEFINED)
    {
        if (mugshotType == MUGSHOT_NPC)
        {
            windowId = AddWindow(&sMugshotWindowNPC);
            x = MUGSHOT_NPC_X;
            y = MUGSHOT_NPC_Y;
        }
        else if (mugshotType == MUGSHOT_PMD)
        {
            windowId = AddWindow(&sMugshotWindowPMD);
            x = MUGSHOT_PMD_X;
            y = MUGSHOT_PMD_Y;
        }
        
        DrawStdWindowFrame(windowId, FALSE);
        CopyWindowToVram(windowId, 3);
    }

    LoadCompressedSpriteSheet(&sheet);
    LoadSpritePalette(&pal);

    sFieldMugshotSpriteId = CreateSprite(&sFieldMugshot_SpriteTemplate, x, y, 0);
    if (sFieldMugshotSpriteId == SPRITE_NONE)
    {
        return;
    }
    PreservePaletteInWeather(gSprites[sFieldMugshotSpriteId].oam.paletteNum + 0x10);
}
