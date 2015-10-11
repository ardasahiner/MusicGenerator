import scalable_music_generator as scalable

genre_names=['mozart', 'jazz', 'gamemusic4']

mozart_generator=scalable.Scalable_Midi_Generator(50, 250, genre_names[0])
print(mozart_generator.choose_midi(150))
