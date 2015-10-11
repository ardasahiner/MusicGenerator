/* jazz.c */
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "convert3.h"
#define TONIC 0
#define RAISED_TONIC 1
#define SUPERTONIC 2
#define RAISED_SUPERTONIC 3
#define MEDIANT 4
#define SUBDOMINANT 5
#define RAISED_SUBDOMINANT 6
#define DOMINANT 7
#define RAISED_DOMINANT 8
#define SUBMEDIANT 9
#define RAISED_SUBMEDIANT 10
#define LEADING_TONE 11
#define MAJOR_MINOR 0
#define DIMINISHED_MINOR 1
#define DIMINISHED_DIMINISHED 2
#define MAJOR_MAJOR 3
#define MINOR_MINOR 4
#define MAJOR_TRIAD 5
#define MINOR_TRIAD 6
#define DIMINISHED_TRIAD 7
#define BEGINNING_PITCH  48
#define LOWER_MELODIC_RANGE 70
#define UPPER_MELODIC_RANGE 86
#define LOWER_ARPEGGIO_RANGE 55
#define UPPER_ARPEGGIO_RANGE 70
#define NEW_RANGE 15
#define ABOVE_RESOLUTION 0
#define BELOW_RESOLUTION 1
#define ABOVE_PREPARATION 2
#define BELOW_PREPARATION 3
#define ABOVE 0
#define BELOW 1
#define BEATS_PER_MEASURE  4
#define MEASURES 4
#define TOTAL_CHORDS  128	/*  128 = 4 measures times */
/*16 phrases times 2 for eighth notes */
#define GENERAL_MIDI_ON 1
#define PERCUSSION_SECTION_ON 0
#define PROGRAM_NUMBERS_ON 1
struct sounds
{
  int scale_degree;
  int quality;
  int sound[5];
};
struct sounds chords_a[MEASURES];
struct sounds chords_b[MEASURES];
struct sounds chords_c[MEASURES];
struct sounds chords_d[MEASURES];
int density1;			/* sets how dense the arpeggios line will be */
int density2;
int density3;
int transposefactor;
int tempo;
int layer[7][TOTAL_CHORDS * BEATS_PER_MEASURE];
int find_quality (int scale_degree);
int find_fifth_up (int current_note);
int find_halfstep_up (int current_note);
int find_diatonicstep_up (int current_note);
int find_diatonicstep_down (int current_note);
void makechords (int quality, int sound[5]);
int progression_logic (int current_note);
void make_progression_a_chords (int current_note);
void make_progression_b_chords (int current_note);
void make_progression_c_chords (int current_note);
void make_progression_d_chords (int current_note);
void initialize_layers (void);
int copy_progression_to_layers (struct sounds chords[], int layer_start);
void setup_arpeggio_patterns (int pattern_a[], int pattern_a_prime[],
			      int pattern_b[], int pattern_new_b[],
			      int pattern_c[], int pattern_c_prime[],
			      int pattern_d[], int pattern_d_prime[]);
void make_arpeggiated_bass (int pattern[],
			    struct sounds chords[], int arpeggio[]);
int copy_arpeggio_to_layers (int arpeggio[], int layer_start);
void add_non_harmonic_tones_arpeggio (struct sounds chords[], int arpeggio[]);
void setup1_arpeggio_patterns (int pattern1_a[], int pattern1_a_prime[],
			       int pattern1_b[], int pattern1_new_b[],
			       int pattern1_c[], int pattern1_c_prime[],
			       int pattern1_d[], int pattern1_d_prime[]);
void make_skeleton_melody (struct sounds chords[], int melody[]);
void copy_melody_to_another (int melody[], int melody1[]);
int find_nonharmonic_tone (int scale_degree, int quality, int harmonic_tone,
			   int position);
int find_nonharmonic_type (int preparation, int resolution);
void add_non_harmonic_tones (struct sounds chords[], int melody[]);
void add_sustaining_tones (int melody[]);
int copy_melody_to_layers (int melody[], int layer_start);
void play_layers (void);

int main (void)
{
  int layer_start;
  int melody_a[MEASURES * BEATS_PER_MEASURE];
  int melody_a_prime[MEASURES * BEATS_PER_MEASURE];
  int melody_b[MEASURES * BEATS_PER_MEASURE];
  int melody_new_b[MEASURES * BEATS_PER_MEASURE];
  int melody_c[MEASURES * BEATS_PER_MEASURE];
  int melody_c_prime[MEASURES * BEATS_PER_MEASURE];
  int melody_d[MEASURES * BEATS_PER_MEASURE];
  int melody_d_prime[MEASURES * BEATS_PER_MEASURE];
  int first_key;
  int second_key;
  int arpeggio_a[MEASURES * BEATS_PER_MEASURE * 2];
  int arpeggio_a_prime[MEASURES * BEATS_PER_MEASURE * 2];
  int arpeggio_b[MEASURES * BEATS_PER_MEASURE * 2];
  int arpeggio_new_b[MEASURES * BEATS_PER_MEASURE * 2];
  int arpeggio_c[MEASURES * BEATS_PER_MEASURE * 2];
  int arpeggio_c_prime[MEASURES * BEATS_PER_MEASURE * 2];
  int arpeggio_d[MEASURES * BEATS_PER_MEASURE * 2];
  int arpeggio_d_prime[MEASURES * BEATS_PER_MEASURE * 2];
  int pattern_a[BEATS_PER_MEASURE * 2];
  int pattern_a_prime[BEATS_PER_MEASURE * 2];
  int pattern_b[BEATS_PER_MEASURE * 2];
  int pattern_new_b[BEATS_PER_MEASURE * 2];
  int pattern_c[BEATS_PER_MEASURE * 2];
  int pattern_c_prime[BEATS_PER_MEASURE * 2];
  int pattern_d[BEATS_PER_MEASURE * 2];
  int pattern_d_prime[BEATS_PER_MEASURE * 2];
  time_t t;
  open_midfiles ();
#if GENERAL_MIDI_ON
  general_midi ();
#endif
  srand ((unsigned) time (&t));
  transposefactor = 0;
  tempo = 150;
  first_key = transposefactor;
  second_key = first_key + 7;
  density1 = rand () % (20);
  density2 = rand () % (15);
  density3 = rand () % (13);
  initialize_layers ();
  transposefactor = first_key;
  make_progression_a_chords (DOMINANT);
  make_progression_b_chords (TONIC);
  transposefactor = second_key;
  make_progression_c_chords (DOMINANT);
  make_progression_d_chords (TONIC);
  make_skeleton_melody (chords_a, melody_a);
  copy_melody_to_another (melody_a, melody_a_prime);
  make_skeleton_melody (chords_b, melody_b);
  make_skeleton_melody (chords_b, melody_new_b);
  make_skeleton_melody (chords_c, melody_c);
  copy_melody_to_another (melody_c, melody_c_prime);
  make_skeleton_melody (chords_d, melody_d);
  make_skeleton_melody (chords_d, melody_d_prime);
  transposefactor = first_key;
  add_non_harmonic_tones (chords_a, melody_a);
  add_non_harmonic_tones (chords_a, melody_a);
  add_sustaining_tones (melody_a);
  add_non_harmonic_tones (chords_a, melody_a_prime);
  add_non_harmonic_tones (chords_a, melody_a_prime);
  add_sustaining_tones (melody_a_prime);
  add_non_harmonic_tones (chords_b, melody_b);
  add_non_harmonic_tones (chords_b, melody_b);
  add_sustaining_tones (melody_b);
  add_non_harmonic_tones (chords_b, melody_new_b);
  add_non_harmonic_tones (chords_b, melody_new_b);
  add_sustaining_tones (melody_new_b);
  transposefactor = second_key;
  add_non_harmonic_tones (chords_c, melody_c);
  add_non_harmonic_tones (chords_c, melody_c);
  add_sustaining_tones (melody_c);
  add_non_harmonic_tones (chords_c, melody_c_prime);
  add_non_harmonic_tones (chords_c, melody_c_prime);
  add_sustaining_tones (melody_c_prime);
  add_non_harmonic_tones (chords_d, melody_d);
  add_non_harmonic_tones (chords_d, melody_d);
  add_sustaining_tones (melody_d);
  add_non_harmonic_tones (chords_d, melody_d_prime);
  add_non_harmonic_tones (chords_d, melody_d_prime);
  add_sustaining_tones (melody_d_prime);
  setup_arpeggio_patterns (pattern_a, pattern_a_prime, pattern_b,
			   pattern_new_b, pattern_c, pattern_c_prime,
			   pattern_d, pattern_d_prime);
  transposefactor = first_key;
  make_arpeggiated_bass (pattern_a, chords_a, arpeggio_a);
  make_arpeggiated_bass (pattern_a_prime, chords_a, arpeggio_a_prime);
  add_non_harmonic_tones_arpeggio (chords_a, arpeggio_a_prime);
  add_non_harmonic_tones_arpeggio (chords_a, arpeggio_a_prime);
  make_arpeggiated_bass (pattern_b, chords_b, arpeggio_b);
  make_arpeggiated_bass (pattern_new_b, chords_b, arpeggio_new_b);
  add_non_harmonic_tones_arpeggio (chords_b, arpeggio_new_b);
  add_non_harmonic_tones_arpeggio (chords_b, arpeggio_new_b);
  transposefactor = second_key;
  make_arpeggiated_bass (pattern_c, chords_c, arpeggio_c);
  make_arpeggiated_bass (pattern_c_prime, chords_c, arpeggio_c_prime);
  add_non_harmonic_tones_arpeggio (chords_c, arpeggio_c_prime);
  add_non_harmonic_tones_arpeggio (chords_c, arpeggio_c_prime);
  make_arpeggiated_bass (pattern_d, chords_d, arpeggio_d);
  make_arpeggiated_bass (pattern_d_prime, chords_d, arpeggio_d_prime);
  add_non_harmonic_tones_arpeggio (chords_d, arpeggio_d_prime);
  add_non_harmonic_tones_arpeggio (chords_d, arpeggio_d_prime);
  layer_start = 0;
  layer_start = copy_arpeggio_to_layers (arpeggio_a, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_b, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_a_prime, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_new_b, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_a, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_b, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_a_prime, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_new_b, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_c, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_d, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_c_prime, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_d_prime, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_a, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_b, layer_start);
  layer_start = copy_arpeggio_to_layers (arpeggio_a_prime, layer_start);
/*layer_start = */ copy_arpeggio_to_layers (arpeggio_new_b, layer_start);
  layer_start = 0;
  layer_start = copy_progression_to_layers (chords_a, layer_start);
  layer_start = copy_progression_to_layers (chords_b, layer_start);
  layer_start = copy_progression_to_layers (chords_a, layer_start);
  layer_start = copy_progression_to_layers (chords_b, layer_start);
  layer_start = copy_progression_to_layers (chords_a, layer_start);
  layer_start = copy_progression_to_layers (chords_b, layer_start);
  layer_start = copy_progression_to_layers (chords_a, layer_start);
  layer_start = copy_progression_to_layers (chords_b, layer_start);
  layer_start = copy_progression_to_layers (chords_c, layer_start);
  layer_start = copy_progression_to_layers (chords_d, layer_start);
  layer_start = copy_progression_to_layers (chords_c, layer_start);
  layer_start = copy_progression_to_layers (chords_d, layer_start);
  layer_start = copy_progression_to_layers (chords_a, layer_start);
  layer_start = copy_progression_to_layers (chords_b, layer_start);
  layer_start = copy_progression_to_layers (chords_a, layer_start);
/*layer_start = */ copy_progression_to_layers (chords_b, layer_start);
  layer_start = 0;
  layer_start = copy_melody_to_layers (melody_a, layer_start);
  layer_start = copy_melody_to_layers (melody_b, layer_start);
  layer_start = copy_melody_to_layers (melody_a_prime, layer_start);
  layer_start = copy_melody_to_layers (melody_new_b, layer_start);
  layer_start = copy_melody_to_layers (melody_a, layer_start);
  layer_start = copy_melody_to_layers (melody_b, layer_start);
  layer_start = copy_melody_to_layers (melody_a_prime, layer_start);
  layer_start = copy_melody_to_layers (melody_new_b, layer_start);
  layer_start = copy_melody_to_layers (melody_c, layer_start);
  layer_start = copy_melody_to_layers (melody_d, layer_start);
  layer_start = copy_melody_to_layers (melody_c_prime, layer_start);
  layer_start = copy_melody_to_layers (melody_d_prime, layer_start);
  layer_start = copy_melody_to_layers (melody_a, layer_start);
  layer_start = copy_melody_to_layers (melody_b, layer_start);
  layer_start = copy_melody_to_layers (melody_a_prime, layer_start);
/*layer_start = */ copy_melody_to_layers (melody_new_b, layer_start);
  play (48 * 2 * 4, 152 - 16, 100, 0);	/* Handshake */
  play_layers ();
  close_midfiles ();
  return (1);
}

int find_quality (int scale_degree)
{
  int quality;
  switch (scale_degree)
    {
    case TONIC:
      quality = MAJOR_MAJOR;
      break;
    case RAISED_TONIC:
      quality = rand () % (3);
      break;
    case SUPERTONIC:
      quality = MINOR_MINOR;
      break;
    case RAISED_SUPERTONIC:
      quality = rand () % (3);
      break;
    case MEDIANT:
      quality = MINOR_MINOR;
      break;
    case SUBDOMINANT:
      quality = MAJOR_MAJOR;
      break;
    case RAISED_SUBDOMINANT:
      quality = rand () % (3);
      break;
    case DOMINANT:
      quality = MAJOR_MINOR;
      break;
    case RAISED_DOMINANT:
      quality = rand () % (3);
      break;
    case SUBMEDIANT:
      quality = MINOR_MINOR;
      break;
    case RAISED_SUBMEDIANT:
      quality = rand () % (3);
      break;
    case LEADING_TONE:
      quality = DIMINISHED_MINOR;
      break;
    }
  return (quality);
}

int find_fifth_up (int current_note)
{
  int fifth_up;
  switch (current_note)
    {
    case TONIC:
      fifth_up = DOMINANT;
      break;
    case SUPERTONIC:
      fifth_up = SUBMEDIANT;
      break;
    case MEDIANT:
      fifth_up = LEADING_TONE;
      break;
    case SUBDOMINANT:
      fifth_up = TONIC;
      break;
    case DOMINANT:
      fifth_up = SUPERTONIC;
      break;
    case SUBMEDIANT:
      fifth_up = MEDIANT;
      break;
    case LEADING_TONE:
      fifth_up = SUBDOMINANT;
      break;
    }
  return (fifth_up);
}

int find_halfstep_up (int current_note)
{
  int halfstep_up;
  halfstep_up = current_note + 1;
  if (halfstep_up == 12)
    halfstep_up = 0;
  return (halfstep_up);
}

int find_diatonicstep_up (int current_note)
{
  int diatonicstep_up;
  switch (current_note)
    {
    case TONIC:
      diatonicstep_up = SUPERTONIC;
      break;
    case SUPERTONIC:
      diatonicstep_up = MEDIANT;
      break;
    case MEDIANT:
      diatonicstep_up = SUBDOMINANT;
      break;
    case SUBDOMINANT:
      diatonicstep_up = DOMINANT;
      break;
    case DOMINANT:
      diatonicstep_up = SUBMEDIANT;
      break;
    case SUBMEDIANT:
      diatonicstep_up = LEADING_TONE;
      break;
    case LEADING_TONE:
      diatonicstep_up = TONIC;
      break;
    }
  return (diatonicstep_up);
}

int find_diatonicstep_down (int current_note)
{
  int diatonicstep_down;
  switch (current_note)
    {
    case TONIC:
      diatonicstep_down = LEADING_TONE;
      break;
    case SUPERTONIC:
      diatonicstep_down = TONIC;
      break;
    case MEDIANT:
      diatonicstep_down = SUPERTONIC;
      break;
    case SUBDOMINANT:
      diatonicstep_down = MEDIANT;
      break;
    case DOMINANT:
      diatonicstep_down = SUBDOMINANT;
      break;
    case SUBMEDIANT:
      diatonicstep_down = DOMINANT;
      break;
    case LEADING_TONE:
      diatonicstep_down = SUBMEDIANT;
      break;
    }
  return (diatonicstep_down);
}

void makechords (int quality, int sound[5])
{
  switch (quality)
    {
    case MAJOR_MAJOR:
      sound[1] = sound[0] + 4;
      sound[2] = sound[0] + 7;
      sound[3] = sound[0] + 9;
      sound[4] = sound[0] + 14;
      break;
    case MAJOR_MINOR:
      sound[1] = sound[0] + 10;
      sound[2] = sound[0] + 14;
      sound[3] = sound[0] + 16;
      sound[4] = sound[0] + 21;
      break;
    case MINOR_MINOR:
      sound[1] = sound[0] + 3;
      sound[2] = sound[0] + 7;
      sound[3] = sound[0] + 10;
      sound[4] = sound[0] + 14;
      break;
    case DIMINISHED_MINOR:
      sound[1] = sound[0] + 3;
      sound[2] = sound[0] + 6;
      sound[3] = sound[0] + 10;
      sound[4] = sound[0] + 12;
      break;
    case DIMINISHED_DIMINISHED:
      sound[1] = sound[0] + 3;
      sound[2] = sound[0] + 6;
      sound[3] = sound[0] + 9;
      sound[4] = sound[0] + 12;
      break;
    case MAJOR_TRIAD:
      sound[1] = sound[0] + 4;
      sound[2] = sound[0] + 7;
      sound[3] = sound[0];
      break;
    case MINOR_TRIAD:
      sound[1] = sound[0] + 3;
      sound[2] = sound[0] + 7;
      sound[3] = sound[0];
      break;
    case DIMINISHED_TRIAD:
      sound[1] = sound[0] + 3;
      sound[2] = sound[0] + 6;
      sound[3] = sound[0];
      break;
    }
}

int progression_logic (int current_note)
{
  int next_note;
  int choice;
  if (current_note == LEADING_TONE)
    {
      choice = rand () % (2);
      if (choice == 0)
	next_note = find_fifth_up (current_note);
      else if (choice == 1)
	next_note = find_diatonicstep_down (current_note);
    }
  else if (current_note == TONIC)
    {
      choice = rand () % (3);
      if (choice == 0)
	next_note = find_diatonicstep_up (current_note);
      else if (choice == 1)
	next_note = find_halfstep_up (current_note);
      else if (choice == 2)
	next_note = find_fifth_up (current_note);
    }
  else if (current_note == MEDIANT)
    {
      choice = rand () % (4);
      if (choice == 0)
	next_note = find_fifth_up (current_note);
      else if (choice == 1)
	next_note = find_diatonicstep_down (current_note);
      else if (choice == 2)
	next_note = find_diatonicstep_up (current_note);
      else if (choice == 3)
	next_note = TONIC;
    }
  else if (current_note == SUPERTONIC ||
	   current_note == SUBDOMINANT ||
	   current_note == DOMINANT || current_note == SUBMEDIANT)
    {
      choice = rand () % (8);
      if (choice == 0 || choice == 5 || choice == 6)
	next_note = find_fifth_up (current_note);
      else if (choice == 1)
	next_note = find_halfstep_up (current_note);
      else if (choice == 2 || choice == 7)
	next_note = find_diatonicstep_up (current_note);
      else if (choice == 3)
	next_note = find_diatonicstep_down (current_note);
      else if (choice == 4)
	next_note = TONIC;
    }
  else if (current_note == RAISED_TONIC ||
	   current_note == RAISED_SUPERTONIC ||
	   current_note == RAISED_SUBDOMINANT ||
	   current_note == RAISED_DOMINANT ||
	   current_note == RAISED_SUBMEDIANT)
    {
      choice = rand () % (5);
      if (choice < 4)
	next_note = find_halfstep_up (current_note);
      else if (choice == 4)
	next_note = TONIC;
    }
  return (next_note);
}

void make_progression_a_chords (int current_note)
/* this progression always starts on the tonic chord */
{
  int quality;
  int sound[5];
  int i = 0;
  int j;
  struct sounds temp[MEASURES];
/* makes a progression backwards from the current note
then recopies it so the progression works to the current note
which is actually the last note */
  quality = find_quality (current_note);
  sound[0] = current_note + BEGINNING_PITCH + transposefactor;
  makechords (quality, sound);
  temp[i].scale_degree = current_note;
  temp[i].quality = quality;
  for (j = 0; j < 5; ++j)
    temp[i].sound[j] = sound[j];
  for (i = 1; i < MEASURES; ++i)
    {
      current_note = progression_logic (current_note);
      quality = find_quality (current_note);
      sound[0] = current_note + BEGINNING_PITCH + transposefactor;
      makechords (quality, sound);
      temp[i].scale_degree = current_note;
      temp[i].quality = quality;
      for (j = 0; j < 5; ++j)
	temp[i].sound[j] = sound[j];
    }
  for (i = 0; i < MEASURES; ++i)
    {
      chords_a[MEASURES - 1 - i].scale_degree = temp[i].scale_degree;
      chords_a[MEASURES - 1 - i].quality = temp[i].quality;
      for (j = 0; j < 5; ++j)
	chords_a[MEASURES - 1 - i].sound[j] = temp[i].sound[j];
    }
/* now make first measure of phrase the TONIC chord */
  chords_a[0].scale_degree = TONIC;
  chords_a[0].quality = MAJOR_MAJOR;
  sound[0] = BEGINNING_PITCH + transposefactor;
  makechords (MAJOR_MAJOR, sound);
  for (j = 0; j < 5; ++j)
    chords_a[0].sound[j] = sound[j];
}

void make_progression_b_chords (int current_note)
/* this progression always starts on the tonic chord */
{
  int quality;
  int sound[5];
  int i = 0;
  int j;
  struct sounds temp[MEASURES];
/* makes a progression backwards from the current note
then recopies it so the progression works to the current note
which is actually the last note */
  quality = find_quality (current_note);
  sound[0] = current_note + BEGINNING_PITCH + transposefactor;
  makechords (quality, sound);
  temp[i].scale_degree = current_note;
  temp[i].quality = quality;
  for (j = 0; j < 5; ++j)
    temp[i].sound[j] = sound[j];
  for (i = 1; i < MEASURES; ++i)
    {
      current_note = progression_logic (current_note);
      quality = find_quality (current_note);
      sound[0] = current_note + BEGINNING_PITCH + transposefactor;
      makechords (quality, sound);
      temp[i].scale_degree = current_note;
      temp[i].quality = quality;
      for (j = 0; j < 5; ++j)
	temp[i].sound[j] = sound[j];
    }
  for (i = 0; i < MEASURES; ++i)
    {
      chords_b[MEASURES - 1 - i].scale_degree = temp[i].scale_degree;
      chords_b[MEASURES - 1 - i].quality = temp[i].quality;
      for (j = 0; j < 5; ++j)
	chords_b[MEASURES - 1 - i].sound[j] = temp[i].sound[j];
    }
}

void make_progression_c_chords (int current_note)
/* this progression always starts on the tonic chord */
{
  int quality;
  int sound[5];
  int i = 0;
  int j;
  struct sounds temp[MEASURES];
/* makes a progression backwards from the current note
then recopies it so the progression works to the current note
which is actually the last note */
  quality = find_quality (current_note);
  sound[0] = current_note + BEGINNING_PITCH + transposefactor;
  makechords (quality, sound);
  temp[i].scale_degree = current_note;
  temp[i].quality = quality;
  for (j = 0; j < 5; ++j)
    temp[i].sound[j] = sound[j];
  for (i = 1; i < MEASURES; ++i)
    {
      current_note = progression_logic (current_note);
      quality = find_quality (current_note);
      sound[0] = current_note + BEGINNING_PITCH + transposefactor;
      makechords (quality, sound);
      temp[i].scale_degree = current_note;
      temp[i].quality = quality;
      for (j = 0; j < 5; ++j)
	temp[i].sound[j] = sound[j];
    }
  for (i = 0; i < MEASURES; ++i)
    {
      chords_c[MEASURES - 1 - i].scale_degree = temp[i].scale_degree;
      chords_c[MEASURES - 1 - i].quality = temp[i].quality;
      for (j = 0; j < 5; ++j)
	chords_c[MEASURES - 1 - i].sound[j] = temp[i].sound[j];
    }
/* now make first measure of phrase the TONIC chord */
  chords_c[0].scale_degree = TONIC;
  chords_c[0].quality = MAJOR_MAJOR;
  sound[0] = BEGINNING_PITCH + transposefactor;
  makechords (MAJOR_MAJOR, sound);
  for (j = 0; j < 5; ++j)
    chords_c[0].sound[j] = sound[j];
}

void make_progression_d_chords (int current_note)
/* this progression always starts on the tonic chord */
{
  int quality;
  int sound[5];
  int i = 0;
  int j;
  struct sounds temp[MEASURES];
/* makes a progression backwards from the current note
then recopies it so the progression works to the current note
which is actually the last note */
  quality = find_quality (current_note);
  sound[0] = current_note + BEGINNING_PITCH + transposefactor;
  makechords (quality, sound);
  temp[i].scale_degree = current_note;
  temp[i].quality = quality;
  for (j = 0; j < 5; ++j)
    temp[i].sound[j] = sound[j];
  for (i = 1; i < MEASURES; ++i)
    {
      current_note = progression_logic (current_note);
      quality = find_quality (current_note);
      sound[0] = current_note + BEGINNING_PITCH + transposefactor;
      makechords (quality, sound);
      temp[i].scale_degree = current_note;
      temp[i].quality = quality;
      for (j = 0; j < 5; ++j)
	temp[i].sound[j] = sound[j];
    }
  for (i = 0; i < MEASURES; ++i)
    {
      chords_d[MEASURES - 1 - i].scale_degree = temp[i].scale_degree;
      chords_d[MEASURES - 1 - i].quality = temp[i].quality;
      for (j = 0; j < 5; ++j)
	chords_d[MEASURES - 1 - i].sound[j] = temp[i].sound[j];
    }
}

void initialize_layers (void)
{
  int i;
  int j;
  for (i = 0; i < TOTAL_CHORDS * BEATS_PER_MEASURE; ++i)
    for (j = 0; j < 7; ++j)
      layer[j][i] = -1;
}

int copy_progression_to_layers (struct sounds chords[], int layer_start)
/* makes each chord at beginning of the measure and
tied the number of beats times two so that melody
and arpeggiated bass may be added to layers */
{
  int i, j, k, m;
/* copy harmonic progression (1 to 5) to layers */
  k = layer_start;
  for (i = 0; i < MEASURES; ++i)
    {
      for (j = 1; j < 6; ++j)
	layer[j][k] = chords[i].sound[j - 1];
      ++k;
      for (m = 0; m < BEATS_PER_MEASURE * 2 - 1; ++m)
	{
	  for (j = 1; j < 6; ++j)
	    layer[j][k] = chords[i].sound[j - 1] + 128;
	  ++k;
	}
    }
  return (k);
}

void setup_arpeggio_patterns (int pattern_a[], int pattern_a_prime[],
			 int pattern_b[], int pattern_new_b[],
			 int pattern_c[], int pattern_c_prime[],
			 int pattern_d[], int pattern_d_prime[])
{
  int i;
  for (i = 0; i < BEATS_PER_MEASURE * 2; ++i)
    {
      pattern_a[i] = rand () % (5 + density1) - density1;
      pattern_a_prime[i] = rand () % (5 + density2) - density2;
      pattern_b[i] = rand () % (5 + density1) - density1;
      pattern_new_b[i] = rand () % (5 + density2) - density2;
      pattern_c[i] = rand () % (5 + density1) - density1;
      pattern_c_prime[i] = rand () % (5 + density2) - density2;
      pattern_d[i] = rand () % (5 + density1) - density1;
      pattern_d_prime[i] = rand () % (5 + density2) - density2;
    }
}

void make_arpeggiated_bass (int pattern[], struct sounds chords[], int arpeggio[])
{
  int note, k, i, m, j, found;
  int temp[5][MEASURES * BEATS_PER_MEASURE * 2];
  int pattern_counter = 0;
/* -1 or lower is silence; 0 is chords.sound[0] etc */
  k = 0;
  for (i = 0; i < MEASURES; ++i)
    for (m = 0; m < BEATS_PER_MEASURE * 2; ++m)
      {
	for (j = 0; j < 5; ++j)
	  temp[j][k] = chords[i].sound[j];
	++k;
      }
  for (i = 0; i < MEASURES * BEATS_PER_MEASURE * 2; ++i)
    arpeggio[i] = -1;
/* This sets the beginning of the pattern to a chord note so that
when the nonharmonic tones function is used, there will exist a
previous note that is a chord tone */
  do
    {
      note = rand () % (UPPER_ARPEGGIO_RANGE - LOWER_ARPEGGIO_RANGE)
	+ LOWER_ARPEGGIO_RANGE;
      if (note % 12 == (temp[rand () % (5)][0]) % 12)
	found = 1;
      else
	found = 0;
    }
  while (!found);
  arpeggio[0] = note - 12;	/* THIS IS THE BIG FUCKING CHANGE  (-12) */
  ++pattern_counter;
  for (i = 1; i < MEASURES * BEATS_PER_MEASURE * 2; ++i)
    {
      note = -1;
      if (pattern[pattern_counter] > -1)
	do
	  {
	    note = rand () % (UPPER_ARPEGGIO_RANGE - LOWER_ARPEGGIO_RANGE)
	      + LOWER_ARPEGGIO_RANGE;
	    if (note % 12 == (temp[pattern[pattern_counter]][i]) % 12)
	      found = 1;
	    else
	      found = 0;
	  }
	while (!found);
      arpeggio[i] = note;
      ++pattern_counter;
      if (pattern_counter == BEATS_PER_MEASURE * 2)
	pattern_counter = 0;
    }
}

int copy_arpeggio_to_layers (int arpeggio[], int layer_start)
{
  int i;
  int k = layer_start;
  for (i = 0; i < MEASURES * BEATS_PER_MEASURE * 2; ++i)
    {
      layer[6][k] = arpeggio[i];
      ++k;
    }
  return (k);
}

void add_non_harmonic_tones_arpeggio (struct sounds chords[], int arpeggio[])
{
  int temp[2][MEASURES * BEATS_PER_MEASURE * 2];
  int i, k, m;
  int previous;
  int quality;
  int scale_degree;
  int nonharmonic_type;
  int nonharmonic_tone;
  k = 0;
  for (i = 0; i < MEASURES; ++i)
    for (m = 0; m < BEATS_PER_MEASURE * 2; ++m)
      {
	temp[0][k] = chords[i].scale_degree;
	temp[1][k] = chords[i].quality;
	++k;
      }
/*  scale_degree = temp[0][0]; */
/* quality = temp[1][0]; */
  previous = arpeggio[0];
  for (i = 1; i < MEASURES * BEATS_PER_MEASURE * 2; ++i)
    {
      if (arpeggio[i] >= 0 && arpeggio[i] <= 127 && arpeggio[i - 1] == -1)
	{
	  quality = temp[1][i - 1];
	  scale_degree = temp[0][i - 1];
	  nonharmonic_type = find_nonharmonic_type (previous, arpeggio[i]);
	  if (nonharmonic_type == ABOVE_PREPARATION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, previous, ABOVE);
	  else if (nonharmonic_type == BELOW_PREPARATION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, previous, BELOW);
	  else if (nonharmonic_type == ABOVE_RESOLUTION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, arpeggio[i],
				     ABOVE);
	  else if (nonharmonic_type == BELOW_RESOLUTION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, arpeggio[i],
				     BELOW);
	  arpeggio[i - 1] = nonharmonic_tone;
	}
      if (arpeggio[i] >= 0 && arpeggio[i] <= 127)
	previous = arpeggio[i];
    }
}

void make_skeleton_melody (struct sounds chords[], int melody[])
/* makes a skeleton melody no more that the number of
beats per measure, but probably much less. The
first of every measure is a chord tone */
{
  int note;
  int i, j, k, m;
  int found;
  int previous;
  int temp[5][MEASURES * BEATS_PER_MEASURE];
  int new_range = rand () % (15) - 7;
  k = 0;
  for (i = 0; i < MEASURES; ++i)
    for (m = 0; m < BEATS_PER_MEASURE; ++m)
      {
	for (j = 0; j < 5; ++j)
	  temp[j][k] = chords[i].sound[j];
	++k;
      }
  for (i = 0; i < MEASURES * BEATS_PER_MEASURE; ++i)
    melody[i] = -1;
  do
    {
      note = rand () % (UPPER_MELODIC_RANGE - LOWER_MELODIC_RANGE)
	+ LOWER_MELODIC_RANGE + new_range;
      if (note % 12 == (temp[0][0]) % 12 ||
	  note % 12 == (temp[1][0]) % 12 ||
	  note % 12 == (temp[2][0]) % 12 ||
	  note % 12 == (temp[3][0]) % 12 || note % 12 == (temp[4][0]) % 12)
	found = 1;
      else
	found = 0;
    }
  while (!found);
  previous = note;
  melody[0] = note;
  for (i = 1; i < MEASURES * BEATS_PER_MEASURE; ++i)
    {
      if (i % BEATS_PER_MEASURE == 0 || rand () % (3) == 1)
	{
	  do
	    {
	      note = rand () % (UPPER_MELODIC_RANGE - LOWER_MELODIC_RANGE)
		+ LOWER_MELODIC_RANGE + new_range;
	      if (note % 12 == (temp[0][i]) % 12 ||
		  note % 12 == (temp[1][i]) % 12 ||
		  note % 12 == (temp[2][i]) % 12 ||
		  note % 12 == (temp[3][i]) % 12 ||
		  note % 12 == (temp[4][i]) % 12)
		found = 1;
	      else
		found = 0;
	      if (abs (previous - note) > 7)
		found = 0;
	    }
	  while (!found);
	  previous = note;
	  melody[i] = note;
	}
    }
  /* this makes the last measure one sustained melody note */
  for (i = ((MEASURES - 1) * BEATS_PER_MEASURE) + 1;
       i < MEASURES * BEATS_PER_MEASURE; ++i)
    melody[i] = melody[(MEASURES - 1) * BEATS_PER_MEASURE] + 128;
}

void copy_melody_to_another (int melody[], int melody1[])
{
  int i;
  for (i = 0; i < MEASURES * BEATS_PER_MEASURE; ++i)
    melody1[i] = melody[i];
}

int find_nonharmonic_tone (int scale_degree, int quality, int harmonic_tone,
		       int position)
{
  int dorian[] = { 2, 1, 2, 2, 2, 1, 2 };
  int ionian[] = { 2, 2, 2, 1, 2, 2, 1 };	/* {2,2,1,2,2,2,1}; */
  int mixolydian[] = { 2, 2, 1, 2, 2, 1, 2 };
  int locrian[] = { 1, 2, 2, 1, 2, 2, 2 };
  int octatonic[] = { 2, 1, 2, 1, 2, 1, 2, 1 };
  int mode_marker = 0;
  int previous;
  int non_harmonic_tone;
  int note = scale_degree + transposefactor;
  if (quality == MAJOR_MAJOR)
    {
      do
	{
	  previous = note;
	  note = note + ionian[mode_marker];
	  ++mode_marker;
	  if (mode_marker == 7)
	    mode_marker = 0;
	}
      while (note < harmonic_tone);
      if (position == BELOW)
	non_harmonic_tone = previous;
      else if (position == ABOVE && note == harmonic_tone)
	non_harmonic_tone = note + ionian[mode_marker];
      else
	non_harmonic_tone = note;
    }
  if (quality == MAJOR_MINOR)
    {
      do
	{
	  previous = note;
	  note = note + mixolydian[mode_marker];
	  ++mode_marker;
	  if (mode_marker == 7)
	    mode_marker = 0;
	}
      while (note < harmonic_tone);
      if (position == BELOW)
	non_harmonic_tone = previous;
      else if (position == ABOVE && note == harmonic_tone)
	non_harmonic_tone = note + mixolydian[mode_marker];
      else
	non_harmonic_tone = note;
    }
  if (quality == MINOR_MINOR)
    {
      do
	{
	  previous = note;
	  note = note + dorian[mode_marker];
	  ++mode_marker;
	  if (mode_marker == 7)
	    mode_marker = 0;
	}
      while (note < harmonic_tone);
      if (position == BELOW)
	non_harmonic_tone = previous;
      else if (position == ABOVE && note == harmonic_tone)
	non_harmonic_tone = note + dorian[mode_marker];
      else
	non_harmonic_tone = note;
    }
  if (quality == DIMINISHED_MINOR)
    {
      do
	{
	  previous = note;
	  note = note + locrian[mode_marker];
	  ++mode_marker;
	  if (mode_marker == 7)
	    mode_marker = 0;
	}
      while (note < harmonic_tone);
      if (position == BELOW)
	non_harmonic_tone = previous;
      else if (position == ABOVE && note == harmonic_tone)
	non_harmonic_tone = note + locrian[mode_marker];
      else
	non_harmonic_tone = note;
    }
  if (quality == DIMINISHED_DIMINISHED)
    {
      do
	{
	  previous = note;
	  note = note + octatonic[mode_marker];
	  ++mode_marker;
	  if (mode_marker == 8)
	    mode_marker = 0;
	}
      while (note < harmonic_tone);
      if (position == BELOW)
	non_harmonic_tone = previous;
      else if (position == ABOVE && note == harmonic_tone)
	non_harmonic_tone = note + octatonic[mode_marker];
      else
	non_harmonic_tone = note;
    }
  return (non_harmonic_tone);
}

int find_nonharmonic_type (int preparation, int resolution)
{
  int nonharmonic_type;
  int number;
  if (preparation - resolution > 4)
    {
      if (rand () % (2))
	nonharmonic_type = BELOW_RESOLUTION;
      else
	nonharmonic_type = ABOVE_PREPARATION;
    }
  else if (resolution - preparation > 4)
    {
      if (rand () % (2))
	nonharmonic_type = ABOVE_RESOLUTION;
      else
	nonharmonic_type = BELOW_PREPARATION;
    }
  else if (preparation - resolution > 0)
    {
      number = rand () % (5);
      if (number == 0)
	nonharmonic_type = BELOW_RESOLUTION;
      else if (number == 1)
	nonharmonic_type = ABOVE_PREPARATION;
      else
	nonharmonic_type = BELOW_PREPARATION;
    }
  else if (resolution - preparation > 0)
    {
      number = rand () % (5);
      if (number == 0)
	nonharmonic_type = BELOW_PREPARATION;
      else if (number == 1)
	nonharmonic_type = ABOVE_RESOLUTION;
      else
	nonharmonic_type = BELOW_RESOLUTION;
    }
  else if (preparation == resolution)
    {
      if (rand () % (2))
	nonharmonic_type = BELOW_RESOLUTION;
      else
	nonharmonic_type = ABOVE_RESOLUTION;
    }
  return (nonharmonic_type);
}

void add_non_harmonic_tones (struct sounds chords[], int melody[])
{
  int temp[2][MEASURES * BEATS_PER_MEASURE];
  int i, k, m;
  int previous;
  int quality;
  int scale_degree;
  int nonharmonic_type;
  int nonharmonic_tone;
  k = 0;
  for (i = 0; i < MEASURES; ++i)
    for (m = 0; m < BEATS_PER_MEASURE; ++m)
      {
	temp[0][k] = chords[i].scale_degree;
	temp[1][k] = chords[i].quality;
	++k;
      }
  previous = melody[0];
  /*       scale_degree = temp[0][0]; */
  /*       quality = temp[1][0];    */
  for (i = 1; i < MEASURES * BEATS_PER_MEASURE; ++i)
    {
      if (melody[i] >= 0 && melody[i] <= 127 &&
	  melody[i - 1] == -1 && rand () % (2))
	{
	  quality = temp[1][i - 1];
	  scale_degree = temp[0][i - 1];
	  nonharmonic_type = find_nonharmonic_type (previous, melody[i]);
	  if (nonharmonic_type == ABOVE_PREPARATION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, previous, ABOVE);
	  else if (nonharmonic_type == BELOW_PREPARATION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, previous, BELOW);
	  else if (nonharmonic_type == ABOVE_RESOLUTION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, melody[i], ABOVE);
	  else if (nonharmonic_type == BELOW_RESOLUTION)
	    nonharmonic_tone =
	      find_nonharmonic_tone (scale_degree, quality, melody[i], BELOW);
	  melody[i - 1] = nonharmonic_tone;
	}
      if (melody[i] >= 0 && melody[i] <= 127)
	previous = melody[i];
    }
}

void add_sustaining_tones (int melody[])
{
  int i;
  for (i = 1; i < MEASURES * BEATS_PER_MEASURE; ++i)
    {
      if (melody[i] == -1)
	{
	  if (melody[i - 1] > 127)
	    melody[i] = melody[i - 1];
	  else
	    melody[i] = melody[i - 1] + 128;
	}
    }
}

int copy_melody_to_layers (int melody[], int layer_start)
/* copies each  note  to three more
so that a harmonic bass of four notes to one
melody note may be possible */
{
  int i;
  int k = layer_start;
  for (i = 0; i < MEASURES * BEATS_PER_MEASURE; ++i)
    {
      layer[0][k] = melody[i];
      ++k;
      if (melody[i] == -1)
	layer[0][k] = -1;
      else if (melody[i] >= 0 && melody[i] <= 127)
	layer[0][k] = melody[i] + 128;
      else if (melody[i] >= 128)
	layer[0][k] = melody[i];
      ++k;
    }
  return (k);
}

void play_layers (void)
{
/* -1 represents a silence. 128 + note means that note is tied */
  int previous_note[10] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };
  int current_note[10];
  int i, j;
  int volume = 127;
  int coda_slowdown = 0;
  int channel[8] = { 144, 146, 146, 146, 146, 146, 145, 147 };
/* need as many channel arrays as layers */
  program_number[0] = 68;
  program_number[1] = 7;
  program_number[2] = 88;
  program_number[3] = 0;
  program_number[4] = 0;
  program_number[5] = 0;
  program_number[6] = 0;
  program_number[7] = 0;
  program_setup ();
  for (i = 0; i < TOTAL_CHORDS * BEATS_PER_MEASURE; ++i)
    {
      if (i == BEATS_PER_MEASURE * 2 * MEASURES * 2)
	{
	  program_number[0] = 71;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      if (i == BEATS_PER_MEASURE * 2 * MEASURES * 4)
	{
	  program_number[0] = 68;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      if (i == BEATS_PER_MEASURE * 2 * MEASURES * 6)
	{
	  program_number[0] = 71;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      else if (i == BEATS_PER_MEASURE * 2 * MEASURES * 8)
	{
	  program_number[0] = 74;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      else if (i == BEATS_PER_MEASURE * 2 * MEASURES * 10)
	{
	  program_number[0] = 71;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      else if (i == BEATS_PER_MEASURE * 2 * MEASURES * 11)
	{
	  program_number[0] = 74;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      else if (i == BEATS_PER_MEASURE * 2 * MEASURES * 12)
	{
	  program_number[0] = 68;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      else if (i == BEATS_PER_MEASURE * 2 * MEASURES * 14)
	{
	  program_number[0] = 71;
#if PROGRAM_NUMBERS_ON
	  program_setup ();
#endif
	}
      else if (i == BEATS_PER_MEASURE * 2 * MEASURES * 16 -
	       BEATS_PER_MEASURE * 2 * 2)
	coda_slowdown = tempo;
      for (j = 0; j < 7; ++j)
	{
	  current_note[j] = layer[j][i];
	  if (previous_note[j] >= 0 &&
	      previous_note[j] <= 127 && current_note[j] == -1)
	    play (0, channel[j] - 16, previous_note[j], 0);
	  else if (previous_note[j] >= 0 && current_note[j] >= 0 &&
		   previous_note[j] <= 127 && current_note[j] <= 127)
	    play (0, channel[j] - 16, previous_note[j], 0);
	  else if (previous_note[j] >= 128 && current_note[j] == -1)
	    play (0, channel[j] - 16, previous_note[j] - 128, 0);
	  else if (previous_note[j] >= 128 && current_note[j] >= 0
		   && current_note[j] <= 127)
	    play (0, channel[j] - 16, previous_note[j] - 128, 0);
	  previous_note[j] = layer[j][i];
	}			/* end of j loop */
      for (j = 0; j < 7; ++j)
	{
	  volume = 127;
	  current_note[j] = layer[j][i];
	  if (previous_note[j] == -1 && current_note[j] >= 0 &&
	      current_note[j] <= 127)
	    play (0, channel[j], current_note[j], volume);
	  else if (previous_note[j] >= 0 && current_note[j] >= 0 &&
		   previous_note[j] <= 127 && current_note[j] <= 127)
	    play (0, channel[j], current_note[j], volume);
	  else if (previous_note[j] >= 128 && current_note[j] >= 0
		   && current_note[j] <= 127)
	    play (0, channel[j], current_note[j], volume);
	  previous_note[j] = layer[j][i];
	}			/* end of second loop */
      play (tempo + coda_slowdown, 152 - 16, 100, 0);
    }				/* end of i loop */
  for (j = 0; j < 7; ++j)
    {
      if (current_note[j] != -1)
	{
	  if (current_note[j] > 127)
	    current_note[j] = current_note[j] - 128;
	  play (0, channel[j] - 16, current_note[j], 0);
	}
    }
}
