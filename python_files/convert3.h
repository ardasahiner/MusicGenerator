/* header file convert3.h that must be in all of these programs
to convert delta variables and write files. Uses shifiting bits.
This produces a format 0 midi file */

void open_midfiles (void);
void play (unsigned long int l, int c, int p, int v);
void convert_hexbyte_varbyte (unsigned char byte[4],
			      unsigned char varbyte[4]);
void convertlong_4hexs (unsigned long d, unsigned char byte[4]);
void close_midfiles (void);
void general_midi (void);
void program_setup (void);
int midioctave (int midinumber);
int midinote (int midinumber);
void printit (int midinumber);

int program_number[8];
long place = 0L;
FILE *fp2;
FILE *fp1;



void program_setup (void)
{
  int duration, j;
  int temp;
  for (j = 0; j < 8; ++j)
    {
      duration = 0;
      fwrite (&duration, 1, 1, fp1);
      ++place;
      temp = j + 192;
      fwrite (&temp, 1, 1, fp1);
      ++place;
      temp = program_number[j];
      fwrite (&temp, 1, 1, fp1);
      ++place;
    }
}

void general_midi (void)
{
  int duration;
  int temp;
  duration = 0;
  fwrite (&duration, 1, 1, fp1);
  ++place;
  temp = 240;
  fwrite (&temp, 1, 1, fp1);
  ++place;
  temp = 5;
  fwrite (&temp, 1, 1, fp1);
  ++place;
  temp = 126;
  fwrite (&temp, 1, 1, fp1);
  ++place;
  temp = 127;
  fwrite (&temp, 1, 1, fp1);
  ++place;
  temp = 9;
  fwrite (&temp, 1, 1, fp1);
  ++place;
  temp = 1;
  fwrite (&temp, 1, 1, fp1);
  ++place;
  temp = 247;
  fwrite (&temp, 1, 1, fp1);
  ++place;
}

void close_midfiles (void)
{
  int beginning[] = { 77, 84, 104, 100, 0, 0, 0, 6, 0, 0, 0, 1, 0, 96,
    77, 84, 114, 107, -1
  };
  int end[] = { 0, 255, 47, 0, -1 };
  int i;
  unsigned char byte[4];
  i = 0;
  do
    {
      fwrite (&end[i], 1, 1, fp1);
      ++place;
      ++i;
    }
  while (end[i] != -1);
  fclose (fp1);
  fp2 = fopen ("music.mid", "wb");
  i = 0;
  do
    {
      fwrite (&beginning[i], 1, 1, fp2);
      ++i;
    }
  while (beginning[i] != -1);
  convertlong_4hexs (place, byte);
  for (i = 0; i < 4; ++i)
    fwrite (&byte[i], 1, 1, fp2);
  fp1 = fopen ("tempfile", "rb");
  do
    {
      fread (&i, 1, 1, fp1);
      if (!feof (fp1))
	fwrite (&i, 1, 1, fp2);
    }
  while (!feof (fp1));
  fclose (fp1);
  fclose (fp2);
}

void convertlong_4hexs (unsigned long d, unsigned char byte[4])
/* breaks long number up into four bytes. */
{
  unsigned long temp;
  temp = d;
  temp = temp >> 8 * 3;
  byte[0] = (unsigned char) temp;
  temp = d;
  temp = temp << 8;
  temp = temp >> 8 * 3;
  byte[1] = (unsigned char) temp;
  temp = d;
  temp = temp << 8 * 2;
  temp = temp >> 8 * 3;
  byte[2] = (unsigned char) temp;
  temp = d;
  temp = temp << 8 * 3;
  temp = temp >> 8 * 3;
  byte[3] = (unsigned char) temp;
}

void convert_hexbyte_varbyte (unsigned char byte[4], unsigned char varbyte[4])
/* first it shifts all the bits past the 7th (8th) bit
of each byte */
{
  unsigned char oldbuffer;
  unsigned char newbuffer;
  unsigned char temp1;
  oldbuffer = (unsigned char) (byte[3] >> 7);
  temp1 = byte[3];
  temp1 = (unsigned char) (temp1 << 1);
  temp1 = (unsigned char) (temp1 >> 1);
  varbyte[3] = temp1;

  newbuffer = (unsigned char) (byte[2] >> 6);
  temp1 = byte[2];
  temp1 = (unsigned char) (temp1 << 2);
  temp1 = (unsigned char) (temp1 >> 1);
  temp1 = temp1 | oldbuffer;
  oldbuffer = newbuffer;
  varbyte[2] = temp1;

  newbuffer = (unsigned char) (byte[1] >> 5);
  temp1 = byte[1];
  temp1 = (unsigned char) (temp1 << 3);
  temp1 = (unsigned char) (temp1 >> 1);
  temp1 = temp1 | oldbuffer;
  oldbuffer = newbuffer;
  varbyte[1] = temp1;

/* newbuffer = (unsigned char) byte[0] >> 4; */
  temp1 = byte[0];
  temp1 = (unsigned char) (temp1 << 4);
  temp1 = (unsigned char) (temp1 >> 1);
  temp1 = temp1 | oldbuffer;
/* oldbuffer = newbuffer; */
  varbyte[0] = temp1;

/* now set the 7th bit */
  if (varbyte[0] > 0)
    varbyte[0] = (unsigned char) (varbyte[0] | 0x80);
  if (varbyte[0] > 0 || varbyte[1] > 0)
    varbyte[1] = (unsigned char) (varbyte[1] | 0x80);
  if (varbyte[0] > 0 || varbyte[1] > 0 || varbyte[2] > 0)
    varbyte[2] = (unsigned char) (varbyte[2] | 0x80);
}

void play (unsigned long int l, int c, int p, int v)
{
  unsigned long number;
  unsigned char varbyte[4];
  unsigned char byte[4];
  number = l;
  convertlong_4hexs (number, byte);
  convert_hexbyte_varbyte (byte, varbyte);
  if (varbyte[0] == 0 && varbyte[1] == 0 && varbyte[2] == 0)
    {
      fwrite (&varbyte[3], 1, 1, fp1);
      ++place;
    }
  else if (varbyte[0] == 0 && varbyte[1] == 0)
    {
      fwrite (&varbyte[2], 1, 1, fp1);
      ++place;
      fwrite (&varbyte[3], 1, 1, fp1);
      ++place;
    }
  else if (varbyte[0] == 0)
    {
      fwrite (&varbyte[1], 1, 1, fp1);
      ++place;
      fwrite (&varbyte[2], 1, 1, fp1);
      ++place;
      fwrite (&varbyte[3], 1, 1, fp1);
      ++place;
    }
  else
    {
      fwrite (&varbyte[0], 1, 1, fp1);
      ++place;
      fwrite (&varbyte[1], 1, 1, fp1);
      ++place;
      fwrite (&varbyte[2], 1, 1, fp1);
      ++place;
      fwrite (&varbyte[3], 1, 1, fp1);
      ++place;
    }
  fwrite (&c, 1, 1, fp1);
  ++place;
  fwrite (&p, 1, 1, fp1);
  ++place;
  fwrite (&v, 1, 1, fp1);
  ++place;
}



void open_midfiles (void)
{
  fp1 = fopen ("tempfile", "wb");
}

int midinote (int midinumber)
{
  int note;
  note = midinumber % 12;
  return (note);
}

int midioctave (int midinumber)
{
  int octave;
  octave = midinumber / 12;
  return (octave);
}

void printit (int midinumber)
{
  int midinote = midinumber % 12;
  int octave = (midinumber / 12) - 1;
  printf ("%d", octave);
  switch (midinote)
    {
    case 0:
      printf ("C  ");
      break;
    case 1:
      printf ("Db ");
      break;
    case 2:
      printf ("D  ");
      break;
    case 3:
      printf ("Eb ");
      break;
    case 4:
      printf ("E  ");
      break;
    case 5:
      printf ("F  ");
      break;
    case 6:
      printf ("Gb ");
      break;
    case 7:
      printf ("G  ");
      break;
    case 8:
      printf ("Ab ");
      break;
    case 9:
      printf ("A  ");
      break;
    case 10:
      printf ("Bb ");
      break;
    case 11:
      printf ("B  ");
      break;
    }
}
