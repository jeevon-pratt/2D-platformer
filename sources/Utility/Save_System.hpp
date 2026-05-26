#pragma once

#include <box2d/b2_math.h>              // b2Vec2 class

#include <string_view>                  // std::string_view

#include "Game_State/Game_State.hpp"    // GAME_STATE_TYPE enum


// ******************
// STRUCT DEFINITIONS
// ******************

/**
 * Structure representing save file data
 */
struct SaveFileData
{
    GAME_STATE_TYPE level;          // The current level of the game
    b2Vec2          playerPos;      // The current position of the player
    float           playerHealth;   // The current health of the player
};



// *********************
// SAVE SYSTEM FUNCTIONS
// *********************

// Opens and sets the save file
//
// Note: This function returns true if the save file is opened successfully
//       and false otherwise
bool OpenSaveFile(std::string_view filepath);


// Returns a boolean that indicates whether a save currently exists
//
// Note: This function will also return false if there is an empty save file.
bool SaveExists();


// Writes the current game data to the save file
//
// Note: This function will have no effect if the save file is not open.
void WriteToSaveFile(const SaveFileData& data);


// Loads the game data from the save file
//
// Note: 1) This function will return garbage data if a save does not exist
//          or if the save file could not be accessed.
//
//       2) You should check if a save exists with the 'SaveExists' function
//          before using the data returned by this function.
SaveFileData LoadFromSaveFile();

// Closes the file
//
// Note: This function has no effect if the save file is not open
void CloseSaveFile();
