import random
import os
import midi

class Scalable_Midi_Generator():

    midi_per_bpm=8
    tempo_interval=5
    remake_num=0

    def __init__(self, lower_bound=60, upper_bound=200, genre='jazz'):
        self.min_bpm=lower_bound
        self.max_bpm=upper_bound
        self.remake_num=0
        self.genre_name=genre #genre should be name of c file name
        self.create_all_files()

    def create_all_files(self):

        self.all_file_names=[] #clears any existing files for this generator
        self.create_scalable_files()

        current_bpm=self.min_bpm

        while current_bpm<=self.max_bpm:

            current_row=[]
            index=0

            for fname in self.scalable_file_names:
                current_row.append(self.create_file_from_scalable(fname, current_bpm, index))
                index+=1

            current_bpm+=self.tempo_interval

            self.all_file_names.append(current_row)

    #creates the files that will be scaled
    def create_scalable_files(self):
        self.scalable_file_names=[]

        #create midi_per_bpm number of files
        for i in range(0, self.midi_per_bpm):
            self.scalable_file_names.append(self.create_file_from_c(i))

    #returns file name
    def create_file_from_c(self, num):

        #command line argument to run c program
        command1_name='make '+self.genre_name
        os.system(command1_name)
        new_executable_name=self.genre_name+str(num)
        os.rename(self.genre_name, new_executable_name)
        command2_name='./'+new_executable_name
        os.system(command2_name)
        #command3_name='rm '+self.genre_name
        #os.system(command3_name)

        #rename midi file that c file created
        new_file_name='genre_'+self.genre_name+'_scalable_file_'+str(self.remake_num)+'_'+str(num)+'.mid'
        os.rename('music.mid', new_file_name)

        if num!=0:
            doc_1=open(new_file_name, 'r')
            doc_1_string=doc_1.read(50)

            prev_file_name='genre_'+self.genre_name+'_scalable_file_'+str(self.remake_num)+'_'+str(num-1)+'.mid'
            doc_2=open(prev_file_name, 'r')
            doc_2_string=doc_2.read(50)

            if doc_1_string==doc_2_string:
                self.create_file_from_c(num)

        return new_file_name

    #returns file name
    def create_file_from_scalable(self, scalable_fname, tempo, num):
        scalable_pattern=midi.read_midifile(scalable_fname)
        new_pattern=midi.Pattern(format=0, resolution=96)
        new_track=midi.Track()
        new_pattern.append(new_track)

        new_track.append(midi.SetTempoEvent(tick=0, data=self.generate_array(tempo)))

        for event in scalable_pattern[0]:
                new_track.append(event)

        new_file_name='genre_'+self.genre_name+'_tempo_'+str(tempo)+"_file_"+str(self.remake_num)+'_'+str(num)+'.mid'
        midi.write_midifile(new_file_name, new_pattern)
        return new_file_name

    #generates an array of three bytes
    #given a tempo in bpm
    def generate_array(self, tempo):
        microseconds_per_beat=(60*10**6)//tempo
        hex_of_us=hex(microseconds_per_beat)[2:]

        if hex_of_us[:-4]=='':
            first_byte=0
        else:
            first_byte=self.hex_to_int(hex_of_us[:-4])

        second_byte=self.hex_to_int(hex_of_us[-4:-2])
        third_byte=self.hex_to_int(hex_of_us[-2:])

        return [first_byte,second_byte,third_byte ]

    #returns int
    def hex_to_int(self, hexidecimal):
        return int(hexidecimal, 16)

    def choose_midi(self, tempo):

        assert tempo>=self.min_bpm
        assert tempo<=self.max_bpm

        available_files_length=len(self.scalable_file_names)

        if available_files_length==0:
            self.create_all_files()
            available_files_length=len(self.scalable_file_names)
            self.remake_num+=1

        column_index=random.randint(0, available_files_length-1)
        row_index=(tempo-self.min_bpm)//5

        self.scalable_file_names.pop(column_index)

        toReturn=self.all_file_names[row_index][column_index]

        #pop off of the column in every row (can't use that same base song)
        for row in self.all_file_names:
            row.pop(column_index)

        return toReturn
