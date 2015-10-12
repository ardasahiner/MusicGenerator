# MusicGenerator - "MusicGen"
An on-the-fly music generator that takes in a variable user-defined tempo and outputs electronic and instrumental music. Created for CalHacks 2.0.

Input system through touch capacity of mobile devices, music output through web app interface.

Built in C, Python, Django, Swift, JavaScript.


The details:

Based on Dr. John R. Francis' work in algorithmic computer music, we created c files that would build midi files of randomized songs in the genres of jazz, classical music, and video game music.

Using python, we were able to create music generators for a given genre. These will maintain a set of midi files and when prompted return a random one at a given tempo. No two music files returned are the same.

We created an iOS app that registers the taps of the user, calculates the average taps per minute, and sends this data to a web server.

Our web app allows a user to choose a genre and play midi files from that genre at the tempo sent to the server by the iOS app.

**Note:** Be sure to install [Python Midi](https://github.com/vishnubob/python-midi) for reading and writing midi files from python

Team Members: Phillip Kuznetsov; Arda Sahiner; Kunal Gosar; Jaemin Kim.
