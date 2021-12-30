#include "AudioMixer.hpp"
#include <string>

// for operator""s usage
using namespace std::string_literals;

// Constructor. Initializes support for replaying audio sound effects.
// Takes vector of sound enum codes and coresponding strings to path of sounds
// to be replayed. Will throw SDLexception if initialization of sound mixer
// support or loading of the sound  effect will fail
AudioMixer::AudioMixer(
    const std::unordered_map<Sound, std::string>& soundsAndPaths)
{
  // strip every pair of enum code (assigned to sound) and corresponding string
  // with path to the sound to be replayed
  for (auto& sound_path_pair : soundsAndPaths) {
    auto& sound = sound_path_pair.first;
    auto& path = sound_path_pair.second;
    // create unique_ptr to SDL Mix_Chunk with custom deleter assuring proper
    // releasing of resource on destruction
    auto chunk_ptr
        = std::unique_ptr<Mix_Chunk, std::function<void(Mix_Chunk*)>>(
            Mix_LoadWAV(path.c_str()),
            [](Mix_Chunk* chunk_ptr) { Mix_FreeChunk(chunk_ptr); });

    // check if the a sound was loaded to the Mix_Chunk pointer and report error
    // if not
    if (!chunk_ptr) {
      throw SDLexception(
          "Unable to load sound: "s + path + " SDL_Mixer error: "s,
          Mix_GetError(), __FILE__, __LINE__);
    }

    // store the pointer to created sound into map,
    // which combines sound enum codes with a ready to use sounds
    sound_effects_[sound] = std::move(chunk_ptr);
  }
}

// replays the sound effect corresponding to to enum sound code passed
void AudioMixer::PlaySound(Sound soundCode)
{
  // try to find the entry in sound_effects_ corresponding to the sound code
  auto search_iter = sound_effects_.find((soundCode));

  // check if the searched pair was found
  if (search_iter != sound_effects_.end()) {
    // retrieve the pointer to Mix_Chunk
    auto chunk_ptr = search_iter->second.get();
    // replay the sound on the first free unreserved channel (-1)
    // single time (0)
    Mix_PlayChannel(-1, chunk_ptr, 0);
  }
};

// ---------- class MixOpener  -------------

/* Constructor. Initializes SDL sound mixer support. Arguments:
  - output sampling frequency in samples per second (Hz),
  - output sample format,
  - number of sound channels in output
  - bytes used per output sample.
  Throws SDLexception if initialization was unsuccessful  */
MixOpener::MixOpener(int frequency, Uint16 format, int channels, int chunksize)
{
  // Initialize SDL_Mixer and report error if it was unsuccessfull
  if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) {
    throw SDLexception(
        "SDL sound mixer could not be initialized. SDL_Mixer error: ",
        Mix_GetError(), __FILE__, __LINE__);
  }
}

// Destructor. Closes sound mixer
MixOpener::~MixOpener() { Mix_CloseAudio(); }