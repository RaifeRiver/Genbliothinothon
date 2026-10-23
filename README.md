# Modular synthesizer
This is a cool modular audio synthesizer. It allows you to make modules and link them together to generate audio however you want from raw waves.

## INSTRUCTIONS
-you can open the build first
-then you can add oscillators and csv players from the side panel
-after that you can simply click on an output from a module and then click where you want it to go
-dials are valid targets for input
-you can send the output of an oscillator to the main output by clicking its output then one of the black boxes in the output module
-to play a csv preview you can set the text bar to ../res/songs/song1_voice1.csv or ../res/songs/song1_voice2.csv for the 2 parts of the first bars of a song.
-you can mess with the dials, have fun!

## GPU acceleration
It uses GPU for audio generation as all the other synths I found seem to just use CPU, allowing it to generate audio for exporting to a file many times faster than traditional CPU generation.

## Internal architecture
OpenGL is used for rendering and audio generation. Audio is played through the cross-platform portaudio library.

It is designed to be as cross platform as possible and has been tested on both Linux and mac OS, although Windows should work fine without issue too.
