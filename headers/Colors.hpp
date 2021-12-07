#ifndef Colors_HPP
#define Colors_HPP
#include "SDL.h"

// constants for collors in the game
namespace Color {
//  rgba(16,255,0)
constexpr SDL_Color Green { 16, 225, 0 };
// rgba(247,138,10)
constexpr SDL_Color Orange { 247, 138, 10 };
// rgba(240,0,0)
constexpr SDL_Color Red { 240, 0, 0 };
// rgba(2,45,248)
constexpr SDL_Color Blue { 2, 45, 248 };
// rgba(243,237,75) // REMOVE INU
constexpr SDL_Color Yellow { 243, 237, 75 };
// rgba(94,255,185)
constexpr SDL_Color Teal { 94, 255, 185 };
// rgba(167,0,255)
constexpr SDL_Color Violet { 167, 0, 255 };
}

#endif // !Colors_HPP
