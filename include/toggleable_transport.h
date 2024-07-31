#ifndef GUARD_TOGGLEABLE_TRANSPORT_H
#define GUARD_TOGGLEABLE_TRANSPORT_H

bool8 ToggleTransport(void);
bool8 ToggleAutoBike(void);
bool8 CanSetAutoBike(void);
void HandleRunning(u8 direction);
void HandleFastSurf(u8 direction);
void GetOnOffToggleableBike(void);
void HandleBikeTransition(u8 transitionFlags);
u8 AutoBike_ReturnPlayerAvatarStateId(void);
u16 AutoBike_ReturnPlayerAvatarTransitionFlags(void);
void RemoveToggleableTransportIcon(void);
void CreateToggleableTransportIcon_AutoRunSet(void);
void CreateToggleableTransportIcon_AutoRunUnset(void);
void CreateToggleableTransportIcon_FastSurfSet(void);
void CreateToggleableTransportIcon_FastSurfUnset(void);
void CreateToggleableTransportIcon_FastBikeSet(void);
void CreateToggleableTransportIcon_FastBikeUnset(void);
void CreateToggleableTransportIcon_AutoBikeSet(void);
void CreateToggleableTransportIcon_AutoBikeUnset(void);
u8 ToggleableTransport_OptionsMenuCreateSelection(void);
void ToggleableTransport_OptionsMenuSaveSelection(u8 selection);

#endif // GUARD_TOGGLEABLE_TRANSPORT_H