class MidiGenerator():

    cNames=[''] #the names of the c executable files TODO: change this
    num_midi_per_bpm=3 #number of midi files that will be created for each bpm

    #to generate midi files for bpms in set [lower_bound, upper_bound]
    def __init__(self, lower_bound, upper_bound):
        self.min_bpm=lower_bound
        self.max_bpm=upper_bound

    #creates all the files we need for execution
    #updates 2-dimensional array of midi file names
    def create_all_files(self):

        current_bpm=self.min_bpm
        self.midi_names=[] #initializes empty 2x2 array

        while current_bpm<=self.max_bpm:
            self.midinames.append(create_files_for_tempo(current_bpm))

    #returns an array of midi file names
    #the file names correspond to newly created midi files for given tempo
    def create_files_for_tempo(self, tempo):

        toReturn=[]

        for c_file_name in cNames:
            for i in range(0, num_midi_per_bpm):
                toReturn.append(create_file_from_c(c_file_name))

        return toReturn

    #creates a single midi file from the given executable c file
    #returns the name of the file created
    def create_file_from_c(self, executable_name):


    #goes into the midi file and manually changes the tempo in text
    def edit_midi(self, tempo):


    #returns name of randomly chosen midi file given the tempo
    #if none remaining, generates new ones
    def choose_midi(self, tempo):
        array_row=tempo-self.min_bpm

        if len(self.midi_names[array_row])==0:
            self.midi_names[array_row]=create_files_for_tempo
