#include "AudioMixer.hpp"
#include <iostream> // REMOVE AT
#include <string>

// for operator""s usage
using namespace std::string_literals;

// COMMENT
AudioMixer::AudioMixer(
    const std::unordered_map<Sound, std::string>& soundsAndPaths)
    : _mix_initializer(std::make_unique<MixInitializer>())
    , _mix_opener(std::make_unique<MixOpener>())
{ // COMMENT
  // REMOVE INU
  // auto _chunk_deleter = [](Mix_Chunk* chunk_ptr) { Mix_FreeChunk(chunk_ptr);
  // };

  for (auto& sound_path_pair : soundsAndPaths) {
    auto& sound = sound_path_pair.first;
    auto& path = sound_path_pair.second;

    auto&& chunk_ptr
        = std::unique_ptr<Mix_Chunk, std::function<void(Mix_Chunk*)>>(
            Mix_LoadWAV(path.c_str()), [](Mix_Chunk* chunk_ptr) {
              Mix_FreeChunk(chunk_ptr);
              // REMOVE AT
              std::cout << "Mix_Chunk custom deleter called!" << std::endl;
            });

    if (!chunk_ptr) {
      throw SDLexception(
          "Unable to load sound: "s + path + " SDL_Mixer error: "s,
          Mix_GetError(), __FILE__, __LINE__);
    }

    _sound_effects[sound] = std::move(chunk_ptr);

    // _sound_effects[sound]
    //     = std::unique_ptr<Mix_Chunk, std::function<void(Mix_Chunk*)>>(
    //         Mix_LoadWAV(path.c_str()), [](Mix_Chunk* chunk_ptr) {
    //           Mix_FreeChunk(chunk_ptr);
    //           // REMOVE AT
    //           std::cout << "Mix_Chunk custom deleter called!" << std::endl;
    //         });
  }
}

// REVIEW: and COMMENT
void AudioMixer::PlaySound(Sound soundCode)
{
  if (_sound_effects.contains(soundCode)) {
    auto chunk_ptr = _sound_effects[soundCode].get();

    if (chunk_ptr) {
      Mix_PlayChannel(-1, chunk_ptr, 0);
    }
  }
};

// ---------- HELPER INNER CLASSES -------------

// Default constructor. Initializes SDL_Mixer.
// Throws SDLexception if initialization was unsuccessful
AudioMixer::MixInitializer::MixInitializer()
{
  // Initialize SDL_Mixer and report error if it was unsuccessfull
  if (Mix_Init(MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG)
      == 0) {
    throw SDLexception("Mix_Init failed. SDL_Mixer error:  ", Mix_GetError(),
        __FILE__, __LINE__);
  }
}

// Destructor. Cleans up SDL_Mixer
AudioMixer::MixInitializer::~MixInitializer()
{
  Mix_Quit();

  // REMOVE AT
  std::cout << "MixInitializer custom deleter called!" << std::endl;
}

/* Constructor. Initializes SDL sound mixer support. Arguments:
  - output sampling frequency in samples per second (Hz),
  - output sample format,
  - number of sound channels in output
  - bytes used per output sample.
  Throws SDLexception if initialization was unsuccessful  */
AudioMixer::MixOpener::MixOpener(
    int frequency, Uint16 format, int channels, int chunksize)
{
  // Initialize SDL_Mixer and report error if it was unsuccessfull
  if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
    throw SDLexception(
        "SDL sound mixer could not be initialized. SDL_Mixer error: ",
        Mix_GetError(), __FILE__, __LINE__);
  }
}

// Destructor. Closes sound mixer
AudioMixer::MixOpener::~MixOpener()
{
  Mix_CloseAudio();
  // REMOVE AT
  std::cout << "MixOpener custom deleter called!" << std::endl;
}