//gamemusic4.c
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "convert3.h"
#define GENERAL_MIDI_ON 1
#define PROGRAM_NUMBERS_ON 1
#define RHYTHMPATTERN 5
#define RHYTHMPATTERN1 6
#define RHYTHMPATTERN2 7
#define RHYTHMPATTERN3 8


#define F3 53
#define SUBMEDIANT 49
#define RASIED_SUBMEDIANT 50
#define SUBTONIC 51
#define TONIC 53
#define SUPERTONIC 55
#define MEDIANT 56
#define RASIED_MEDIANT 57
#define SUBDOMINANT 58
#define DOMINANT 48


#define MINOR 0
#define MAJOR 1

// kirk > picard 
int channel[8] = { 144, 145, 146, 147, 148, 149, 150, 151 };

int transposefactor = 0;
int tempo;
int volume;
int sound[3];
int sounds[5];
int mode;
int note;
int index_number;
int array0[20];
int random_arp2;
int random_arp1;
int chordsbassminor[5];
int chordsbassmajor[5];

int rhythm[22][8] = { {1, 1, 2, 1, 1, 2, 0, 0},	//0    may randomize order of these. Only 4th four will be main theme after randomization.  
{2, 2, 2, 1, 1, 0, 0, 0},	//1 
{3, 1, 2, 1, 1, 0, 0, 0},	//2 
{2, 2, 2, 2, 0, 0, 0, 0},	//3 
{3, 1, 2, 2, 0, 0, 0, 0},	//4
{2, 2, 3, 1, 0, 0, 0, 0},	//5
{3, 1, 3, 1, 0, 0, 0, 0},	//6 
{4, 2, 1, 1, 0, 0, 0, 0},	//7
{4, 2, 2, 0, 0, 0, 0, 0},	//8
{3, 1, 4, 0, 0, 0, 0, 0},	//9
{4, 3, 1, 0, 0, 0, 0, 0},	//10
{4, 2, 2, 0, 0, 0, 0, 0},	//11 
		  // now 2 beat measures
{1, 1, 1, 1, 0, 0, 0, 0},	//12
{1, 1, 2, 0, 0, 0, 0, 0},	//13  
{1, 1, 2, 0, 0, 0, 0, 0},	//14 
{2, 1, 1, 0, 0, 0, 0, 0},	//15
{3, 1, 0, 0, 0, 0, 0, 0},	//16
{2, 2, 0, 0, 0, 0, 0, 0},	//17 
		  // now 1 beat measures
{1, 1, 0, 0, 0, 0, 0, 0},	//18       
{2, 0, 0, 0, 0, 0, 0, 0},	//19 
		 // cadence for piece  - MUST END WITH THIS
{8, 0, 0, 0, 0, 0, 0, 0},	//20 / DOMINANT      
{8, 0, 0, 0, 0, 0, 0, 0}
};				//21 / TONIC 










int rhythm_events[22];
int melody[22][8];
int chords[22][3];
int chords_scale[22][5];
int arp1[22][2];
int arp2[22][4];

int choose_chords_pitch (void);
int choose_non_harmonic_pitch (void);
void set_up_rhythm_events (void);
void make_arp2 (void);
void make_arp1 (void);





void makeminorchords (int note);
void makeminorscale (int note);
void makemajorchords (int note);
void makemajorscale (int note);

void make_minor_progression (void);
void make_major_progression (void);
void quality_function_minor_key (void);
void quality_function_major_key (void);
void procedure1 (void);
void play_procedure1 (void);
void play_procedure2 (void);
void play_procedure3 (void);
void changing_order_numbers_of_index (void);
void length_first_intenstiy_arp (void);
void length_second_intensity_arp (void);


int
main (void)
{
  int i;
  time_t t;
  open_midfiles ();
#if GENERAL_MIDI_ON
  general_midi ();
#endif
  srand ((unsigned) time (&t));
  transposefactor = 0;
  tempo = 48;
  random_arp2 = rand () % 6;
  random_arp1 = rand () % 6;
  play (48 * 2 * 4, 152 - 16, 100, 0);	/* Handshake */
  program_number[0] = 48;	/* strings */
  program_number[1] = 73;	/* flute */
  program_number[2] = 0;	/* piano */
  program_number[3] = 80;	/* square wave */
  program_number[4] = 6;	/* xylophone = 13 and 6 is harpsichord */
  program_number[5] = 47;	/* 14 = tubular bell; 47 = timpani */
  program_number[6] = 57;	/* Trombone */
  program_number[7] = 72;	/* Piccolo */
  program_setup ();
  set_up_rhythm_events ();
  changing_order_numbers_of_index ();
  index_number = array0[0];
/*printf("%d ",index_number);*/
  note = TONIC;
  quality_function_minor_key ();
  procedure1 ();
  index_number = 20;		// cadence and not randomized;
/*printf("%d ",index_number);*/
  note = DOMINANT;
  quality_function_minor_key ();
  procedure1 ();
  index_number = 21;		// cadence and not randomized;
/*printf("%d ",index_number);*/
  note = TONIC;
  quality_function_minor_key ();
  procedure1 ();
  for (i = 1; i < 20; ++i)
    {
      index_number = array0[i];
// printf("%d ",index_number); 
      make_minor_progression ();
      quality_function_minor_key ();
      procedure1 ();
    }
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure1 ();
    }
  {
    index_number = 21;
    play_procedure1 ();
  }
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure1 ();
    }
  {
    index_number = 21;
    play_procedure1 ();
  }
  for (i = 4; i < 8; ++i)
    {
      index_number = array0[i];
      play_procedure1 ();
    }
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure1 ();
    }
  {
    index_number = 20;
    play_procedure1 ();
  }
  {
    index_number = 21;
    play_procedure1 ();
  }
  program_number[1] = 69;
  program_setup ();
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure2 ();
    }
  {
    index_number = 21;
    play_procedure2 ();
  }
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure2 ();
    }
  {
    index_number = 21;
    play_procedure2 ();
  }
  for (i = 4; i < 8; ++i)
    {
      index_number = array0[i];
      play_procedure2 ();
    }
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure2 ();
    }
  {
    index_number = 20;
    play_procedure2 ();
  }
  {
    index_number = 21;
    play_procedure2 ();
  }
  program_number[1] = 56;
  program_setup ();
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure3 ();
    }
  {
    index_number = 21;
    play_procedure3 ();
  }
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure3 ();
    }
  {
    index_number = 21;
    play_procedure3 ();
  }
  for (i = 4; i < 8; ++i)
    {
      index_number = array0[i];
      play_procedure3 ();
    }
  for (i = 0; i < 4; ++i)
    {
      index_number = array0[i];
      play_procedure3 ();
    }
  {
    index_number = 20;
    play_procedure3 ();
  }
  {
    index_number = 21;
    play_procedure3 ();
  }
  close_midfiles ();
  return (1);
}

void
changing_order_numbers_of_index (void)
{
// THIS IS TO CHANGE THE INDEX SO EACH NUMBER IS ASSIGNED A UNIQUE NUMBER DIFFERENT, RANDOM ASSIGNMENT EACH TIME 
  int number;
  int counter = 0;
  int success;
  int i;
  number = rand () % 20;
  array0[counter] = number;
  ++counter;
  do
    {
      success = 1;
      number = rand () % 20;
      for (i = 0; i < counter; ++i)
	{
	  if (array0[i] == number)
	    {
	      success = 0;
	    }
	}
      if (success == 1)
	{
	  array0[counter] = number;
	  counter++;
	}
    }
  while (success == 0 || counter < 20);
//   for (i = 0; i < 20; ++i)
//     printf(" %d ",array0[i]);
}

void
set_up_rhythm_events (void)
{
  int i, j, count;
  for (i = 0; i < 22; ++i)
    {
      count = 0;
      for (j = 0; j < 8; ++j)
	{
	  if (rhythm[i][j] > 0)
	    count = count + 1;
	}
      rhythm_events[i] = count;
    }
}

void
quality_function_major_key (void)
{
  if (note == TONIC)
    mode = MAJOR;
  else if (note == SUPERTONIC)
    mode = MINOR;
  else if (note == RASIED_MEDIANT)
    mode = MINOR;
  else if (note == SUBDOMINANT)
    mode = MAJOR;
  else if (note == DOMINANT)
    mode = MAJOR;
  else if (note == RASIED_SUBMEDIANT)
    mode = MINOR;
}

void
quality_function_minor_key (void)
{
  if (note == TONIC)
    mode = MINOR;
  else if (note == MEDIANT)
    mode = MAJOR;
  else if (note == SUBDOMINANT)
    mode = MINOR;
  else if (note == DOMINANT)
    mode = MINOR;
  else if (note == SUBMEDIANT)
    mode = MAJOR;
  else if (note == SUBTONIC)
    mode = MAJOR;
}

void
make_minor_progression (void)
{
  int i;
  int new_note;
  int new_mode;
  int random_factor;
  random_factor = rand () % 100;
  if (note == TONIC)
    {
      if (random_factor < 20)
	new_note = SUBTONIC;
      else if (random_factor < 40)
	new_note = MEDIANT;
      else if (random_factor < 60)
	new_note = SUBMEDIANT;
      else if (random_factor < 80)
	new_note = SUBDOMINANT;
      else if (random_factor < 100)
	new_note = DOMINANT;
    }
  else if (note == MEDIANT)
    {
      if (random_factor < 40)
	new_note = SUBDOMINANT;
      else if (random_factor < 80)
	new_note = SUBTONIC;
      else if (random_factor < 100)
	new_note = TONIC;
    }
  else if (note == SUBDOMINANT)
    {
      if (random_factor < 40)
	new_note = MEDIANT;
      else if (random_factor < 80)
	new_note = SUBTONIC;
      else if (random_factor < 100)
	new_note = TONIC;
    }
  else if (note == DOMINANT)
    {
      if (random_factor < 40)
	new_note = SUBTONIC;
      else if (random_factor < 80)
	new_note = SUBMEDIANT;
      else if (random_factor < 100)
	new_note = TONIC;
    }
  else if (note == SUBTONIC)
    {
      if (random_factor < 45)
	new_note = SUBDOMINANT;
      else if (random_factor < 90)
	new_note = DOMINANT;
      else if (random_factor < 100)
	new_note = TONIC;
    }
  else if (note == SUBMEDIANT)
    {
      if (random_factor < 45)
	new_note = DOMINANT;
      else if (random_factor < 90)
	new_note = SUBTONIC;
      else if (random_factor < 100)
	new_note = TONIC;
    }
  note = new_note;
}

void
make_major_progression (void)
{
  int i;
  int new_note;
  int new_mode;
  int random_factor;
  random_factor = rand () % 100;
  if (note == TONIC)
    {
      if (random_factor < 20)
	new_note = SUPERTONIC;
      else if (random_factor < 40)
	new_note = RASIED_MEDIANT;
      else if (random_factor < 60)
	new_note = RASIED_SUBMEDIANT;
      else if (random_factor < 80)
	new_note = SUBDOMINANT;
      else if (random_factor < 100)
	new_note = DOMINANT;
    }
  else if (note == RASIED_MEDIANT)
    {
      if (random_factor < 33)
	new_note = SUBDOMINANT;
      else if (random_factor < 66)
	new_note = TONIC;
      else if (random_factor < 100)
	new_note = SUPERTONIC;
    }
  else if (note == SUBDOMINANT)
    {
      if (random_factor < 33)
	new_note = RASIED_MEDIANT;
      else if (random_factor < 66)
	new_note = TONIC;
      else if (random_factor < 100)
	new_note = SUPERTONIC;
    }
  else if (note == DOMINANT)
    {
      if (random_factor < 50)
	new_note = TONIC;
      else if (random_factor < 100)
	new_note = RASIED_SUBMEDIANT;
    }
  else if (note == SUPERTONIC)
    {
      if (random_factor < 33)
	new_note = TONIC;
      else if (random_factor < 66)
	new_note = RASIED_MEDIANT;
      else if (random_factor < 100)
	new_note = SUBDOMINANT;
    }
  else if (note == RASIED_SUBMEDIANT)
    {
      if (random_factor < 50)
	new_note = DOMINANT;
      else if (random_factor < 100)
	new_note = TONIC;
    }
  note = new_note;
}

void
makeminorchords (int note)
{
  sound[0] = note;
  sound[1] = sound[0] + 3;
  sound[2] = sound[0] + 7;
}

void
makemajorchords (int note)
{
  sound[0] = note;
  sound[1] = sound[0] + 4;
  sound[2] = sound[0] + 7;
}

void
makeminorscale (int note)
{
  sounds[0] = note;
  sounds[1] = sounds[0] + 2;
  sounds[2] = sounds[0] + 3;
  sounds[3] = sounds[0] + 5;
  sounds[4] = sounds[0] + 7;
}

void
makemajorscale (int note)
{
  sounds[0] = note;
  sounds[1] = sounds[0] + 2;
  sounds[2] = sounds[0] + 4;
  sounds[3] = sounds[0] + 5;
  sounds[4] = sounds[0] + 7;
}

void
length_first_intensity_arp (void)
{
  play (0, 146, (arp1[index_number][0]) + 1, 127);	// the + 1 is the transpose factor
//printf(" %d ",arp1[index_number][0]);
  play (40, 152 - 16, 100, 0);
  play (0, 146 - 16, (arp1[index_number][0]) + 1, 0);
  play (0, 146, (arp1[index_number][1]) + 1, 127);
//printf(" %d ",arp1[index_number][1]);
  play (40, 152 - 16, 100, 0);
  play (0, 146 - 16, (arp1[index_number][1]) + 1, 0);
}

void
length_second_intensity_arp (void)
{
  if (rand () % 4 == 0)
    play (0, 153, 49, 127);	// percussion is 153 and number after is the instrument
  if (rand () % 4 == 0)
    play (0, 153, 75, 127);
  play (0, 146, (arp2[index_number][0]) + 2, 100);	// the + 2 is the transpose factor. 146 is piano
  play (0, 148, (arp2[index_number][0]) + 2 + 12, 127);	// 148 is harpsichord
  play (25, 152 - 16, 100, 0);
  if (rand () % 4 == 0)
    play (0, 153, 64, 127);
  play (0, 146 - 16, (arp2[index_number][0]) + 2, 0);
  play (0, 148 - 16, (arp2[index_number][0]) + 2 + 12, 0);
  play (0, 146, (arp2[index_number][1]) + 2, 100);
  play (0, 148, (arp2[index_number][1]) + 2 + 12, 127);
  play (25, 152 - 16, 100, 0);
  if (rand () % 4 == 0)
    play (0, 153, 60, 127);
  play (0, 146 - 16, (arp2[index_number][1]) + 2, 0);
  play (0, 148 - 16, (arp2[index_number][1]) + 2 + 12, 0);
  play (0, 146, (arp2[index_number][2]) + 2, 100);
  play (0, 148, (arp2[index_number][2]) + 2 + 12, 127);
  play (25, 152 - 16, 100, 0);
  if (rand () % 3 == 0)
    play (0, 153, 38, 127);
  if (rand () % 4 == 0)
    play (0, 153, 70, 127);
  play (0, 146 - 16, (arp2[index_number][2]) + 2, 0);
  play (0, 148 - 16, (arp2[index_number][2]) + 2 + 12, 0);
  play (0, 146, (arp2[index_number][3]) + 2, 100);
  play (0, 148, (arp2[index_number][3]) + 2 + 12, 127);
  play (25, 152 - 16, 100, 0);
  play (0, 146 - 16, (arp2[index_number][3]) + 2, 0);
  play (0, 148 - 16, (arp2[index_number][3]) + 2 + 12, 0);
  play (0, 153 - 16, 49, 0);	// turn off all percussion
  play (0, 153 - 16, 64, 0);
  play (0, 153 - 16, 60, 0);
  play (0, 153 - 16, 75, 0);
  play (0, 153 - 16, 38, 0);
  play (0, 153 - 16, 70, 0);
}

int
choose_chords_pitch (void)
{
  int x;
  int y;
  x = rand () % 3;
  if (x == 0)
    y = sounds[0];
  else if (x == 1)
    y = sounds[2];
  else if (x == 2)
    y = sounds[4];
  return (y);
}

int
choose_non_harmonic_pitch (void)
{
  int x;
  int y;
  x = rand () % 2;
  if (x == 0)
    y = sounds[1];
  else if (x == 1)
    y = sounds[3];
  return (y);
}

void
procedure1 (void)
{
  int i;
  int counter;
  if (mode == MINOR)
    {
      makeminorscale (note + 12);
      makeminorchords (note);
    }
  else if (mode == MAJOR)
    {
      makemajorscale (note + 12);
      makemajorchords (note);
    }
  chords[index_number][0] = sound[0];
  chords[index_number][1] = sound[1];
  chords[index_number][2] = sound[2];
  chords_scale[index_number][0] = sounds[0];
  chords_scale[index_number][1] = sounds[1];
  chords_scale[index_number][2] = sounds[2];
  chords_scale[index_number][3] = sounds[3];
  chords_scale[index_number][4] = sounds[4];
  make_arp2 ();
  make_arp1 ();
  counter = 0;
  for (i = 0; i < rhythm_events[index_number]; ++i)
    {
      if (counter % 2 == 0)
	{
	  melody[index_number][i] = choose_chords_pitch ();
	}
      else
	{
	  melody[index_number][i] = choose_non_harmonic_pitch ();
	}
      counter = counter + rhythm[index_number][i];
    }
}

void
make_arp2 (void)
{
  if (random_arp2 == 0)
    {
      arp2[index_number][0] = sounds[0] - 12;
      arp2[index_number][1] = sounds[1] - 12;
      arp2[index_number][2] = sounds[2] - 12;
      arp2[index_number][3] = sounds[1] - 12;
    }
  else if (random_arp2 == 1)
    {
      arp2[index_number][0] = sounds[1] - 12;
      arp2[index_number][1] = sounds[4] - 12;
      arp2[index_number][2] = sounds[3] - 12;
      arp2[index_number][3] = sounds[2] - 12;
    }
  else if (random_arp2 == 2)
    {
      arp2[index_number][0] = sounds[0] - 12;
      arp2[index_number][1] = sounds[2] - 12;
      arp2[index_number][2] = sounds[3] - 24;
      arp2[index_number][3] = sounds[4] - 12;
    }
  else if (random_arp2 == 3)
    {
      arp2[index_number][0] = sounds[0] - 12;
      arp2[index_number][1] = sounds[2] - 12;
      arp2[index_number][2] = sounds[3] - 12;
      arp2[index_number][3] = sounds[2] - 12;
    }
  else if (random_arp2 == 4)
    {
      arp2[index_number][0] = sounds[0] - 12;
      arp2[index_number][1] = sounds[4] - 12;
      arp2[index_number][2] = sounds[2] - 12;
      arp2[index_number][3] = sounds[1] - 12;
    }
  else if (random_arp2 == 5)
    {
      arp2[index_number][0] = sounds[0] - 12;
      arp2[index_number][1] = sounds[2] - 12;
      arp2[index_number][2] = sounds[1] - 12;
      arp2[index_number][3] = sounds[2] - 12;
    }
}

void
make_arp1 (void)
{
  if (random_arp1 == 0)
    {
      arp1[index_number][0] = sounds[0] - 24;
      arp1[index_number][1] = sounds[2] - 24;
    }
  else if (random_arp1 == 1)
    {
      arp1[index_number][0] = sounds[0] - 24;
      arp1[index_number][1] = sounds[1] - 24;
    }
  else if (random_arp1 == 2)
    {
      arp1[index_number][0] = sounds[0] - 24;
      arp1[index_number][1] = sounds[0] - 12;
    }
  else if (random_arp1 == 3)
    {
      arp1[index_number][0] = sounds[0] - 24;
      arp1[index_number][1] = sounds[4] - 24;
    }
  else if (random_arp1 == 4)
    {
      arp1[index_number][0] = sounds[4] - 24;
      arp1[index_number][1] = sounds[0] - 24;
    }
  else if (random_arp1 == 5)
    {
      arp1[index_number][0] = sounds[2] - 24;
      arp1[index_number][1] = sounds[0] - 24;
    }
}

void
play_procedure1 (void)
{
  int i;
  for (i = 0; i < 3; ++i)
    play (0, 144, chords[index_number][i], 100);
  for (i = 0; i < rhythm_events[index_number]; ++i)
    {
      play (0, 145, melody[index_number][i], 127);
      play (120 * rhythm[index_number][i], 152 - 16, 100, 0);
// printf("rhythm[index_rhythm][i] = %d ",rhythm[index_number][i]);  
      play (0, 145 - 16, melody[index_number][i], 0);
    }
  for (i = 0; i < 3; ++i)
    play (0, 144 - 16, chords[index_number][i], 0);
}

void
play_procedure2 (void)
{
  int i;
  int j;
  for (i = 0; i < 3; ++i)
    play (0, 144, (chords[index_number][i]) + 1, 100);	// the + 1 is in the transpose factor 
  for (i = 0; i < rhythm_events[index_number]; ++i)
    {
      play (0, 145, (melody[index_number][i]) + 1, 127);	// the + 1 is the transpose factor 
      for (j = 0; j < rhythm[index_number][i]; ++j)
	length_first_intensity_arp ();	// the + 1 is in the function length_first_intensity_arp 
// printf("index_rhythm = %d",rhythm[index_number][i]);
      play (0, 145 - 16, (melody[index_number][i]) + 1, 0);	// the + 1 is the transpose factor   
    }
  for (i = 0; i < 3; ++i)
    play (0, 144 - 16, (chords[index_number][i]) + 1, 0);	// the + 1 is the transpose factor
}

void
play_procedure3 (void)
{
  int i;
  int j;
  for (i = 0; i < 3; ++i)
    play (0, 144, (chords[index_number][i]) + 2, 90);	// the + 2 is the tranpose factor
  play (0, 147, (chords[index_number][0]) + 2 - 12, 127);	// square wave
  play (0, 150, (chords[index_number][0]) + 2 - 12, 127);	// trombone 
  for (i = 0; i < rhythm_events[index_number]; ++i)
    {
      if (i == 0 || i == 2 || i == 3 || i == 4)
	play (0, 149, (chords[index_number][i]) + 2 - 12, 127);	// adds timpani on important beats
      play (0, 145, (melody[index_number][i]) + 2, 127);	// the + 2 is the tranpose factor
      play (0, 150, (melody[index_number][i]) + 2 + 12, 127);	// the + 2 is the tranpose factor
      play (0, 151, (melody[index_number][i]) + 2 + 12, 127);	// the + 2 is the tranpose factor
      for (j = 0; j < rhythm[index_number][i]; ++j)
	length_second_intensity_arp ();	// the + 2 is in the function length_first_intensity_arp
      play (0, 145 - 16, (melody[index_number][i]) + 2, 0);	// the + 2 is the tranpose factor
      play (0, 150 - 16, (melody[index_number][i]) + 2 + 12, 0);	// the + 2 is the tranpose factor
      play (0, 151 - 16, (melody[index_number][i]) + 2 + 12, 0);	// the + 2 is the tranpose factor
      if (i == 0 || i == 2 || i == 3 || i == 4)
	play (0, 149 - 16, (chords[index_number][i]) + 2 - 12, 0);	// timpani off
    }
  for (i = 0; i < 3; ++i)
    play (0, 144 - 16, (chords[index_number][i]) + 2, 0);	// the + 2 is the tranpose factor
  play (0, 147 - 16, (chords[index_number][0]) + 2 - 12, 0);	// square wave
  play (0, 150 - 16, (chords[index_number][0]) + 2 - 12, 0);	// trombone
}
