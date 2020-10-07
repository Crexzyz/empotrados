#include "music.h"

// Play a little ditty
void initial_song()
{
	const u8 lens[6]= { 1,1,4, 1,1,4 };
	const u8 notes[6]= { 0x02, 0x05, 0x12,  0x02, 0x05, 0x12 };
	int ii;
	for(ii=0; ii<6; ii++)
	{
		music_note_play(notes[ii]&15, notes[ii]>>4);
		VBlankIntrDelay(8*lens[ii]);
	}
}

void initial_sound()
{
	// turn sound on
	REG_SNDSTAT= SSTAT_ENABLE;
	// snd1 on left/right ; both full volume
	REG_SNDDMGCNT = SDMG_BUILD_LR(SDMG_SQR1, 7);
	// DMG ratio to 100%
	REG_SNDDSCNT= SDS_DMG100;

	// no sweep
	REG_SND1SWEEP= SSW_OFF;
	// envelope: vol=12, decay, max step time (7) ; 50% duty
	REG_SND1CNT= SSQR_ENV_BUILD(12, 0, 7) | SSQR_DUTY1_2;
	REG_SND1FREQ= 0;

	initial_song();
}

void sound_setting()
{
    irqInit();

	// requires the vblank interrupt to reset sound DMA.
	// Link the VBlank interrupt to mmVBlank, and enable it. 
	irqSet( IRQ_VBLANK, mmVBlank );
	irqEnable(IRQ_VBLANK);

	 

	// initialise with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 8 );

	// Start playing module
	mmStart( MOD_FLATOUTLIES, MM_PLAY_LOOP );

    // sound effect handle (for cancelling it later)
	// mm_sfxhand amb = 0;
}

// Play a note
void music_note_play(int note, int octave)
{
	REG_SND1FREQ = SFREQ_RESET | SND_RATE(note, octave);
}

// void effects()
// {
//     mm_sound_effect ambulance = {
// 		{ SFX_AMBULANCE } ,			// id
// 		(int)(1.0f * (1<<10)),	// rate
// 		0,		// handle
// 		255,	// volume
// 		0,		// panning
// 	};

// 	mm_sound_effect boom = {
// 		{ SFX_BOOM } ,			// id
// 		(int)(1.0f * (1<<10)),	// rate
// 		0,		// handle
// 		255,	// volume
// 		255,	// panning
// 	};
// }