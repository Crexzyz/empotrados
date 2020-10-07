#ifndef MUSIC_H
#define MUSIC_H

#include <maxmod.h>

#include <stdio.h>
#include <stdlib.h>

#include "soundbank.h"
#include "soundbank_bin.h"

#include <tonc.h>

void initial_song();
void initial_sound();
void sound_setting();
void effects();
void music_note_play(int note, int octave);

#endif