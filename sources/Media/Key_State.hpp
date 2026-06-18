#pragma once

#include <SDL3/SDL_keyboard.h>    // SDL_GetKeyboardState function
#include <SDL3/SDL_scancode.h>    // SDL_Scancode enum


/**
 *  Enumeration for representing key scan codes
 */
enum SCANCODE
{
    SCANCODE_A = SDL_SCANCODE_A,
    SCANCODE_D = SDL_SCANCODE_D,
    SCANCODE_F = SDL_SCANCODE_F,
    SCANCODE_W = SDL_SCANCODE_W
};


/**
 *  Global variable that holds the keyboard state
 */
inline const bool* g_keyState = SDL_GetKeyboardState(nullptr);
