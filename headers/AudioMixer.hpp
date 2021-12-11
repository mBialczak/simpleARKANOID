#ifndef AudioMixer_hpp
#define AudioMixer_hpp
#include "Enums.hpp"
#include "SDL_mixer.h"
#include "SDLexception.hpp"
#include <functional>
#include <memory>
#include <unordered_map> // REMOVE INU

// REVIEW: consider removing exceptions. Sound not crucial
// Try removing MixInitializer completely and check if works

// COMMENT
class AudioMixer
{
  public:
  // TODO: check if can be private
  // COMMENT
  // REMOVE INU
  class MixInitializer;
  class MixOpener;

  AudioMixer(const std::unordered_map<Sound, std::string>& soundsAndPaths);
  // TODO: remove copy operations?
  // REVIEW:
  ~AudioMixer() = default;

  // REVIEW:
  void PlaySound(Sound soundCode);

  private:
  // RAII - initializes  and cleans up SDL_Mixer automatically
  std::unique_ptr<MixInitializer> _mix_initializer;
  // RAII - initializes  and cleans up sound mixer
  std::unique_ptr<MixOpener> _mix_opener;
  // COMMENT and Modify
  std::unordered_map<Sound,
      std::unique_ptr<Mix_Chunk, std::function<void(Mix_Chunk*)>>>
      _sound_effects;
};

// helper inner class for AudioMixer
// It sole task is to initialize SDL_Mixer support
// and free menaged resources on destruction
class AudioMixer::MixInitializer
{
  public:
  // Default constructor. Initializes SDL_Mixer.
  // Throws SDLexception if initialization was unsuccessful
  MixInitializer();
  // Destructor. Cleans up SDL_Mixer
  ~MixInitializer();
};

// helper inner class for AudioMixer
// It sole task is to initialize SDL sound mixer
// and free menaged resources on destruction
class AudioMixer::MixOpener
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

#endif // !AudioMixer_hpp
