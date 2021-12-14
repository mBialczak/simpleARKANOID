Hello,

regarding the problem with the OGG.

I have removed one of the helper classess for AudioMixer, that is MixInitializer.

I believe, finally it is not needed and only MixOpener will suffice.

If another SDLexception shows, especially:

"SDL sound mixer could not be initialized. SDL_Mixer error: "

it may mean that hte SDL_mixer libraries have not been installed.

I found one of the hints in the internet to install it on Ubuntu:

#install sdl mixer - if you want sound
sudo apt install libmikmod-dev libfishsound1-dev libsmpeg-dev liboggz2-dev libflac-dev libfluidsynth-dev libsdl2-mixer-dev libsdl2-mixer-2.0-0 -y;

from the following [page](https://gist.github.com/VitoScalettaMafia/cd99ca6fda54f87fe3c93f27abfdfc70).

This should do the trick. I'am running on Linux Mint 20.2 and it worked for me before first submission.
