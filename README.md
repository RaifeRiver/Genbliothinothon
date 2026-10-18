# Modular synthesizer
This is a cool modular audio synthesizer. It allows you to make modules and link them together to generate audio however you want from raw waves.

## GPU acceleration
It uses GPU for audio generation as all the other synths I found seem to just use CPU, allowing it to generate audio for exporting to a file many times faster than traditional CPU generation.

## Internal architecture
OpenGL is used for rendering and audio generation. Audio is played through the cross-platform portaudio library.

It is designed to be as cross platform as possible and has been tested on both Linux and mac OS, although Windows should work fine without issue too.
