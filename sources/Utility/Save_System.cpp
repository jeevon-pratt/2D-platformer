#include <json/json.h>                  // jsoncpp functionality

#include <fstream>                      // std::fstream
#include <memory>                       // std::unique_ptr
#include <string>                       // std::string

#include "Utility/Save_System.hpp"      // SaveFileData struct
#include "Utility/Memory.hpp"           // LoadJson, OverWriteJson function



// *********************
// SAVE SYSTEM FUNCTIONS
// *********************

static std::fstream s_saveFile;   // Oject that reads and write to the save file
static std::string  s_filepath;   // The file path of the save file
static bool         s_saveExists; // Determines if a save currently exists



bool OpenSaveFile(const std::string& filepath)
{
    s_filepath = filepath;

    // If the save file does not exist,this operation will fail.
    s_saveFile.open(s_filepath);


    if ( !s_saveFile.is_open() )
    {
        s_saveFile.clear();
        s_saveFile.open(s_filepath, std::ios::in | std::ios::out | std::ios::trunc);

        s_saveExists = false;
    }

    else
    {
        s_saveFile.seekg(0, std::ios::end);

        s_saveExists = (s_saveFile.tellg() != 0);

        s_saveFile.seekg(0, std::ios::beg);
    }


    return s_saveFile.is_open();
}



bool SaveExists()
{
    return s_saveExists;
}



void WriteToSaveFile(const SaveFileData& data)
{
    Json::Value root;
    root["level"]  = static_cast<uint8_t>(data.level);
    root["x"]      = data.playerPos.x;
    root["y"]      = data.playerPos.y;
    root["health"] = data.playerHealth;

    OverWriteJson(s_filepath, root);
}



SaveFileData LoadFromSaveFile()
{
    if (!s_saveExists)
        return SaveFileData{};

    Json::Value root = LoadJson(s_filepath);


    SaveFileData data;
    data.level        = static_cast<GameStateID>( root["level"].asUInt() );
    data.playerPos.x  = root["x"].asFloat();
    data.playerPos.y  = root["y"].asFloat();
    data.playerHealth = root["health"].asFloat();

    return data;
}



void CloseSaveFile()
{
    s_saveFile.close();
}
