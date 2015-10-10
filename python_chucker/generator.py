class Generator:
    # take the style parameter into considerations
    def __init__(self, bpm=100, style=default):
        self.bpm = bpm
        self.style = style
    def new_bpm(self, bpm):
        self.bpm = bpm
    def write_section(self):
