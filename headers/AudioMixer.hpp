#ifndef AudioMixer_hpp
#define AudioMixer_hpp
#include "Enums.hpp"
#include "SDL_mixer.h"
#include "SDLexception.hpp"
#include <functional>
#include <memory>
#include <unordered_map>

// helper class for AudioMixer. It's sole task is to initialize SDL sound mixer
// and free managed resources on destruction using RAII
class MixOpener
{
  public:
  /* Constructor. Initializes SDL sound mixer support. Arguments:
    - output sampling frequency in samples per second (Hz),
    - output sample format,
    - number of sound channels in output
    - bytes used per output sample.
    Throws SDLexception if initialization was unsuccessful  */
  MixOpener(int frequency = 44100, Uint16 format = MIX_DEFAULT_FORMAT,
      int channels = 2, int chunksize = 2048);

  // Destructor. Closes sound mixer
  ~MixOpener();
};

// Provides sound effects output based on read audio files
class AudioMixer
{
  public:
  // Constructor. Initializes support for replaying audio sound effects.
  // Takes vector of sound enum codes and coresponding strings to path of sounds
  // to be replayed. Will throw SDLexception if initialization of sound mixer
  // support or loading of the sound  effect will fail
  AudioMixer(const std::unordered_map<Sound, std::string>& soundsAndPaths);
  // usage of RAII resource management helper classes  allows
  // for default destructor
  ~AudioMixer() = default;
  // the class is not meant to be copied, so copy operations are disabled
  AudioMixer(const AudioMixer&) = delete;
  AudioMixer& operator=(const AudioMixer&) = delete;

  // replays the sound effect corresponding to to enum sound code passed
  void PlaySound(Sound soundCode);

  private:
  MixOpener _mix_opener;
  // container used for combining enum sound codes with pointers to Mix_Chunks
  std::unordered_map<Sound,
      std::unique_ptr<Mix_Chunk, std::function<void(Mix_Chunk*)>>>
      _sound_effects;
};

#endif // !AudioMixer_hpp
