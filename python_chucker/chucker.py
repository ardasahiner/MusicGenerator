class Chucker:
    '''
    # creates the chucker object with a default filename
    chuck_it = Chucker()
    chuck_lst = ['SinOsc s => dac;', ....]
    # write chuck_lst to file
    chuck_it.write_chuck(chuck_lst)
    # execute chuck file of Chucker
    chuck_it.execute_chuck()
    '''
    def __init__(self, name='foo.ck'):
        self.set_name(name)
    def set_name(self, name):
        # add extension if it's not there
        if '.ck' not in name:
            self.name = name+'.ck'
        # don't need to if it is there
        else:
            self.name = name
    def write_chuck(self, chucks):
        assert type(chucks) == list, "Chucks argument should be a list, not a {0}".format(type(chucks))
        thefile = open(self.name, 'w')
        for chuck_line in chucks:
            thefile.write("%s\n" % chuck_line)
        thefile.close()
    def execute_chuck(self):
        import subprocess
        command = 'chuck ' + self.name
        chuck_cmd = subprocess.Popen(command.split(), stdout=subprocess.PIPE)
        output = process.communicate()[0]
