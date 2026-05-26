#pragma once

#include <cassert>      // assert function


// ****************
// ASSERT FUNCTIONS
// ****************

/**
 *  Debug macro functions for assertions that are disabled for distribution
 *
 *  Note: The message parameter should be a string literal.
 */
#ifdef DEBUG

    #define GAME_2D_ASSERT(cnd)          assert(cnd)
    #define GAME_2D_ASSERT_MSG(cnd, msg) assert(cnd && msg)

#else

    #define GAME_2D_ASSERT_FALLTHROUGH do {} while (false)

    #define GAME_2D_ASSERT(cnd)          GAME_2D_ASSERT_FALLTHROUGH
    #define GAME_2D_ASSERT_MSG(cnd, msg) GAME_2D_ASSERT_FALLTHROUGH

#endif
