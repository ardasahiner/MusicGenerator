import scalable_music_generator as scalable
import os
genre_names=['mozart', 'jazz', 'gamemusic4']

jazz_generator=scalable.Scalable_Midi_Generator(50, 250, genre_names[1])
print(jazz_generator.choose_midi(154))
