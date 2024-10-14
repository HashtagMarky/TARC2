#include "global.h"
#include "bike.h"
#include "decompress.h"
#include "event_data.h"
#include "event_scripts.h"
#include "field_player_avatar.h"
#include "field_weather.h"
#include "main.h"
#include "overworld.h"
#include "script.h"
#include "sprite.h"
#include "sound.h"
#include "toggleable_transport.h"
#include "constants/flags.h"
#include "constants/songs.h"

#define ICON_NONE               0
#define ICON_AUTO_RUN           1
#define ICON_FAST_SURF          2
#define ICON_FAST_BIKE          3
#define ICON_AUTO_BIKE          4
#define ICON_COUNT              5

#define ICON_SET                0
#define ICON_UNSET              1
#define ICON_STATES             2

// Adjusted x and y values are used.
// ICON_X and ICON_Y represent the location of the centre of the image, relative to the display height and width.
#define ICON_X                  DISPLAY_WIDTH - 48
#define ICON_Y                  56
#define ICON_X_ADJUSTED         ICON_X + 16
#define ICON_Y_ADJUSTED         ICON_Y - 16

#define TAG_TT_ICON             0x9000

static const u32 sToggleableTransportIconGfx_RunningShoes[]         = INCBIN_U32("graphics/toggleable_transport/running_shoes/running_shoes.4bpp.lz");
static const u16 sToggleableTransportIconPal_RunningShoesSet[]      = INCBIN_U16("graphics/toggleable_transport/running_shoes/running_shoes_set.gbapal");
static const u16 sToggleableTransportIconPal_RunningShoesUnset[]    = INCBIN_U16("graphics/toggleable_transport/running_shoes/running_shoes_unset.gbapal");
static const u32 sToggleableTransportIconGfx_FastLapras[]           = INCBIN_U32("graphics/toggleable_transport/lapras/fast_lapras.4bpp.lz");
static const u32 sToggleableTransportIconGfx_SlowLapras[]           = INCBIN_U32("graphics/toggleable_transport/lapras/slow_lapras.4bpp.lz");
static const u16 sToggleableTransportIconPal_Lapras[]               = INCBIN_U16("graphics/toggleable_transport/lapras/lapras.gbapal");
static const u32 sToggleableTransportIconGfx_MachBike[]             = INCBIN_U32("graphics/toggleable_transport/bikes/mach_bike.4bpp.lz");
static const u16 sToggleableTransportIconPal_MachBike[]             = INCBIN_U16("graphics/toggleable_transport/bikes/mach_bike.gbapal");
static const u32 sToggleableTransportIconGfx_AcroBike[]             = INCBIN_U32("graphics/toggleable_transport/bikes/acro_bike.4bpp.lz");
static const u16 sToggleableTransportIconPal_AcroBike[]             = INCBIN_U16("graphics/toggleable_transport/bikes/acro_bike.gbapal");
static const u32 sToggleableTransportIconGfx_Bicycle[]              = INCBIN_U32("graphics/toggleable_transport/bikes/bicycle.4bpp.lz");
static const u16 sToggleableTransportIconPal_BicycleSet[]           = INCBIN_U16("graphics/toggleable_transport/bikes/bicycle_set.gbapal");
static const u16 sToggleableTransportIconPal_BicycleUnset[]         = INCBIN_U16("graphics/toggleable_transport/bikes/bicycle_unset.gbapal");

extern const u8 EventScript_ToggleableTransport_Indicators_AutoRunSet[];
extern const u8 EventScript_ToggleableTransport_Indicators_AutoRunUnset[];
extern const u8 EventScript_ToggleableTransport_Indicators_FastSurfSet[];
extern const u8 EventScript_ToggleableTransport_Indicators_FastSurfUnset[];
extern const u8 EventScript_ToggleableTransport_Indicators_FastBikeSet[];
extern const u8 EventScript_ToggleableTransport_Indicators_FastBikeUnset[];
extern const u8 EventScript_ToggleableTransport_Indicators_AutoBikeSet[];
extern const u8 EventScript_ToggleableTransport_Indicators_AutoBikeUnset[];

static bool8 CanSetBikingState(void);
static bool8 ToggleAutoRun(void);
static bool8 ToggleFastSurf(void);
static bool8 ToggleFastBike(void);
static void HandleFastSurf_OnWater(u8 direction);
static void HandleFastSurf_Underwater(u8 direction);
static void CreateToggleableTransportIcon(u8 icon, u8 state);

static EWRAM_DATA u8 sToggleableTransportIconSpriteId = 0;

struct ToggleableTransport_IconGfx
{
    const u32 *gfx;
    const u16 *pal;
};

static const struct OamData sToggleableTransportIcon_Oam = {
    .size = SPRITE_SIZE(32x32),
    .shape = SPRITE_SHAPE(32x32),
    .priority = 0,
};

static const struct SpriteTemplate sToggleableTransportIcon_SpriteTemplate = {
    .tileTag = TAG_TT_ICON,
    .paletteTag = TAG_TT_ICON,
    .oam = &sToggleableTransportIcon_Oam,
    .callback = SpriteCallbackDummy,
    .anims = gDummySpriteAnimTable,
    .affineAnims = gDummySpriteAffineAnimTable,
};

static const struct ToggleableTransport_IconGfx sToggleableTransport_Icons[ICON_COUNT][ICON_STATES] =
{
    [ICON_AUTO_RUN] =
    {
        [ICON_SET] =
        {
            .gfx = sToggleableTransportIconGfx_RunningShoes,
            .pal = sToggleableTransportIconPal_RunningShoesSet,
        },

        [ICON_UNSET] =
        {
            .gfx = sToggleableTransportIconGfx_RunningShoes,
            .pal = sToggleableTransportIconPal_RunningShoesUnset,
        },
    },
    [ICON_FAST_SURF] =
    {
        [ICON_SET] =
        {
            .gfx = sToggleableTransportIconGfx_FastLapras,
            .pal = sToggleableTransportIconPal_Lapras,
        },

        [ICON_UNSET] =
        {
            .gfx = sToggleableTransportIconGfx_SlowLapras,
            .pal = sToggleableTransportIconPal_Lapras,
        },
    },
    [ICON_FAST_BIKE] =
    {
        [ICON_SET] =
        {
            .gfx = sToggleableTransportIconGfx_MachBike,
            .pal = sToggleableTransportIconPal_MachBike,
        },

        [ICON_UNSET] =
        {
            .gfx = sToggleableTransportIconGfx_AcroBike,
            .pal = sToggleableTransportIconPal_AcroBike,
        },
    },
    [ICON_AUTO_BIKE] =
    {
        [ICON_SET] =
        {
            .gfx = sToggleableTransportIconGfx_Bicycle,
            .pal = sToggleableTransportIconPal_BicycleSet,
        },

        [ICON_UNSET] =
        {
            .gfx = sToggleableTransportIconGfx_Bicycle,
            .pal = sToggleableTransportIconPal_BicycleUnset,
        },
    },
};


static bool8 CanSetBikingState(void)
{
    if (Overworld_IsBikingAllowed() == TRUE && IsBikingDisallowedByPlayer() == 0 && FlagGet(FLAG_RECEIVED_BIKE))
        return TRUE;
    
    return FALSE;
}

bool8 ToggleTransport(void)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT) && !IsRunningDisallowed(gObjectEvents[gPlayerAvatar.objectEventId].currentMetatileBehavior))
        return ToggleAutoRun();
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING) || TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_UNDERWATER))
        return ToggleFastSurf();
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE) || TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE))
        return ToggleFastBike();

    return FALSE;
}

static bool8 ToggleAutoRun(void)
{
    if (!FlagGet(FLAG_SYS_B_DASH))
        return FALSE;

    if (gSaveBlock3Ptr->autoRun)
    {
        gSaveBlock3Ptr->autoRun = FALSE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_AutoRunUnset);
    }
    else
    {
        gSaveBlock3Ptr->autoRun = TRUE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_AutoRunSet);
    }
    return TRUE;
}

static bool8 ToggleFastSurf(void)
{
    if (gSaveBlock3Ptr->fastSurf)
    {
        gSaveBlock3Ptr->fastSurf = FALSE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_FastSurfUnset);
    }
    else
    {
        gSaveBlock3Ptr->fastSurf = TRUE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_FastSurfSet);
    }
    return TRUE;
}

static bool8 ToggleFastBike(void)
{
    if (gSaveBlock3Ptr->fastBike)
    {
        gPlayerAvatar.flags -= PLAYER_AVATAR_FLAG_MACH_BIKE;
        gPlayerAvatar.flags += PLAYER_AVATAR_FLAG_ACRO_BIKE;
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_ACRO_BIKE);
        gSaveBlock3Ptr->fastBike = FALSE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_FastBikeUnset);
    }
    else
    {
        gPlayerAvatar.flags -= PLAYER_AVATAR_FLAG_ACRO_BIKE;
        gPlayerAvatar.flags += PLAYER_AVATAR_FLAG_MACH_BIKE;
        SetPlayerAvatarTransitionFlags(PLAYER_AVATAR_FLAG_MACH_BIKE);
        gSaveBlock3Ptr->fastBike = TRUE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_FastBikeSet);
    }
    return TRUE;
}

bool8 ToggleAutoBike(void)
{
    if (!TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE | PLAYER_AVATAR_FLAG_ON_FOOT) || !CanSetBikingState())
    {
        return FALSE;
    }

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_ON_FOOT))
    {
        gSaveBlock3Ptr->autoBike = TRUE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_AutoBikeSet);
    }
    else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE))
    {
        if (gSaveBlock3Ptr->autoBike)
        {
            gSaveBlock3Ptr->autoBike = FALSE;
            ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_AutoBikeUnset);
        }
        else
        {
            gSaveBlock3Ptr->autoBike = TRUE;
            ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_AutoBikeSet);
        }
    }

    return TRUE;
}

bool8 CanSetAutoBike(void)
{
    if (gSaveBlock3Ptr->autoBike && CanSetBikingState())
        return TRUE;

    return FALSE;
}

void HandleRunning(u8 direction)
{
    if (gSaveBlock3Ptr->autoRun == TRUE)
    {
        if (ObjectMovingOnRockStairs(&gObjectEvents[gPlayerAvatar.objectEventId], direction))
            PlayerRunSlow(direction);
        else
            PlayerRun(direction);

        gPlayerAvatar.flags |= PLAYER_AVATAR_FLAG_DASH;
    }
}

void HandleFastSurf(u8 direction)
{
    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
        HandleFastSurf_OnWater(direction);

    if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_UNDERWATER))
        HandleFastSurf_Underwater(direction);
}

static void HandleFastSurf_OnWater(u8 direction)
{
    if (gSaveBlock3Ptr->fastSurf)
        PlayerWalkFaster(direction);
    else
        PlayerWalkFast(direction);
}

static void HandleFastSurf_Underwater(u8 direction)
{
    if (gSaveBlock3Ptr->fastSurf)
        PlayerWalkFast(direction);
    else
        PlayerWalkNormal(direction);
}

void GetOnOffToggleableBike(void)
{
    if (gSaveBlock3Ptr->fastBike)
        GetOnOffBike(PLAYER_AVATAR_FLAG_MACH_BIKE);
    else
        GetOnOffBike(PLAYER_AVATAR_FLAG_ACRO_BIKE);
}

void HandleBikeTransition(u8 transitionFlags)
{
    if (transitionFlags == PLAYER_AVATAR_FLAG_MACH_BIKE)
    {
        gSaveBlock3Ptr->fastBike = TRUE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_FastBikeSet);
    }
    else if (transitionFlags == PLAYER_AVATAR_FLAG_ACRO_BIKE)
    {
        gSaveBlock3Ptr->fastBike = FALSE;
        ScriptContext_SetupScript(EventScript_ToggleableTransport_Indicators_FastBikeUnset);
    }
    
//  These lines can be used if wanted to check if the bike has been used once instead of received.
//  If this is the case, change CanSetBikingState() to use the created flag, FLAG_USED_BIKE.
//  if (!FlagSet(FLAG_USED_BIKE))
//      FlagSet(FLAG_USED_BIKE);
}

u8 AutoBike_ReturnPlayerAvatarStateId(void)
{
    if ((CanSetAutoBike()) || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE)))
    {
        if (gSaveBlock3Ptr->fastBike)
            return PLAYER_AVATAR_STATE_MACH_BIKE;
        else
            return PLAYER_AVATAR_STATE_ACRO_BIKE;
    }
    else
    {
        return PLAYER_AVATAR_STATE_NORMAL;
    }
}

u16 AutoBike_ReturnPlayerAvatarTransitionFlags(void)
{
    if ((CanSetAutoBike()) || (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_MACH_BIKE | PLAYER_AVATAR_FLAG_ACRO_BIKE)))
    {
        if (gSaveBlock3Ptr->fastBike)
            return PLAYER_AVATAR_FLAG_MACH_BIKE;
        else
            return PLAYER_AVATAR_FLAG_ACRO_BIKE;
    }
    else
    {
        return PLAYER_AVATAR_FLAG_ON_FOOT;
    }
}

static void CreateToggleableTransportIcon(u8 icon, u8 state)
{
    struct CompressedSpriteSheet sheet = { .size=0x1000, .tag=TAG_TT_ICON };
    struct SpritePalette pal = { .tag = sheet.tag };

    RemoveToggleableTransportIcon();
    if (icon >= NELEMS(sToggleableTransport_Icons))
    {
        return;
    }

    sheet.data = (sToggleableTransport_Icons[icon][state].gfx != NULL ? sToggleableTransport_Icons[icon][state].gfx : sToggleableTransportIconGfx_RunningShoes);
    pal.data = (sToggleableTransport_Icons[icon][state].pal != NULL ? sToggleableTransport_Icons[icon][state].pal : sToggleableTransportIconPal_RunningShoesSet);

    LoadCompressedSpriteSheet(&sheet);
    LoadSpritePalette(&pal);

    sToggleableTransportIconSpriteId = CreateSprite(&sToggleableTransportIcon_SpriteTemplate, ICON_X_ADJUSTED, ICON_Y_ADJUSTED, 0);
    if (sToggleableTransportIconSpriteId == SPRITE_NONE)
    {
        return;
    }
    PreservePaletteInWeather(gSprites[sToggleableTransportIconSpriteId].oam.paletteNum + 0x10);
}

void RemoveToggleableTransportIcon(void)
{
    if (IndexOfSpritePaletteTag(TAG_TT_ICON) != 0xFF)
    {
        ResetPreservedPalettesInWeather();
        DestroySprite(&gSprites[sToggleableTransportIconSpriteId]);
        FreeSpritePaletteByTag(TAG_TT_ICON);
        FreeSpriteTilesByTag(TAG_TT_ICON);
        sToggleableTransportIconSpriteId = SPRITE_NONE;
    }
}

void CreateToggleableTransportIcon_AutoRunSet(void)
{
    CreateToggleableTransportIcon(ICON_AUTO_RUN, ICON_SET);
}

void CreateToggleableTransportIcon_AutoRunUnset(void)
{
    CreateToggleableTransportIcon(ICON_AUTO_RUN, ICON_UNSET);
}

void CreateToggleableTransportIcon_FastSurfSet(void)
{
    CreateToggleableTransportIcon(ICON_FAST_SURF, ICON_SET);
}

void CreateToggleableTransportIcon_FastSurfUnset(void)
{
    CreateToggleableTransportIcon(ICON_FAST_SURF, ICON_UNSET);
}

void CreateToggleableTransportIcon_FastBikeSet(void)
{
    CreateToggleableTransportIcon(ICON_FAST_BIKE, ICON_SET);
}

void CreateToggleableTransportIcon_FastBikeUnset(void)
{
    CreateToggleableTransportIcon(ICON_FAST_BIKE, ICON_UNSET);
}

void CreateToggleableTransportIcon_AutoBikeSet(void)
{
    CreateToggleableTransportIcon(ICON_AUTO_BIKE, ICON_SET);
}

void CreateToggleableTransportIcon_AutoBikeUnset(void)
{
    CreateToggleableTransportIcon(ICON_AUTO_BIKE, ICON_UNSET);
}

u8 ToggleableTransport_OptionsMenuCreateSelection(void)
{
    if (gSaveBlock3Ptr->autoBike == FALSE && gSaveBlock3Ptr->autoRun == FALSE)
        return 0;
    else if (gSaveBlock3Ptr->autoBike == FALSE && gSaveBlock3Ptr->autoRun == TRUE)
        return 1;
    else
        return 2;
}

void ToggleableTransport_OptionsMenuSaveSelection(u8 selection)
{
    switch (selection)
    {
    case 2:
        gSaveBlock3Ptr->autoBike = TRUE;
        break;
    case 1:
        gSaveBlock3Ptr->autoRun = TRUE;
        gSaveBlock3Ptr->autoBike = FALSE; 
        break;
    case 0:
    default:
        gSaveBlock3Ptr->autoRun = FALSE;
        gSaveBlock3Ptr->autoBike = FALSE;
        break;
    }
}