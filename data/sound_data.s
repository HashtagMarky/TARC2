	.section .rodata

	.include "asm/macros/m4a.inc"
	.include "asm/macros/music_voice.inc"
	.include "include/config/pokemon.h"
	.include "include/config/music_expansion.h"
	.include "sound/voice_groups.inc"
	.include "sound/keysplit_tables.inc"
	.include "sound/programmable_wave_data.inc"
	.include "sound/music_player_table.inc"
	.include "sound/song_table.inc"
	.include "sound/direct_sound_data.inc"
	
.if MUSIC_EXPANSION_PMDR_MUSIC_ENABLED == TRUE
	.include "sound/pmd_voice_groups.inc"
	.include "sound/pmd_gb_waves.inc"
	.include "sound/pmd_waves.inc"
.endif @ MUSIC_EXPANSION_PMDR_MUSIC_ENABLED

	.align 2
