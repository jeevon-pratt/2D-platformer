#include <json/json.h>          // Jsoncpp functionality

#include <fstream>              // std::ifstream, std::ofstream
#include <new>                  // std::bad_alloc
#include <stdlib.h>             // malloc and free functions
#include <string_view>          // std::string_view

#include "Utility/Log.hpp"      // GAME_2D_LOG_ERROR, GAME_2D_LOG_VERBOSE macro



// ****************
// MEMORY FUNCTIONS
// ****************

// Static variable for tracking total heap memory allocated
static size_t s_totalMemory = 0;



#ifdef _MSC_VER

    _NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
    void* __cdecl operator new(size_t size) noexcept(false)
    {
        void* allocation = malloc(size);

        if (!allocation)
            throw std::bad_alloc();


        s_totalMemory += size;

        GAME_2D_LOG_VERBOSE("Allocating %u bytes of memory", (unsigned int)size);
        GAME_2D_LOG_VERBOSE("Total Heap Memory Allocated: %u bytes\n\n", (unsigned int)s_totalMemory);

        return allocation;
    }

#else

    void* __cdecl operator new(size_t size) noexcept(false)
    {
        void* allocation = malloc(size);

        if (!allocation)
            throw std::bad_alloc();


        s_totalMemory += size;

        GAME_2D_LOG_VERBOSE("Allocating %u bytes of memory", (unsigned int)size);
        GAME_2D_LOG_VERBOSE("Total Heap Memory Allocated: %u bytes\n\n", (unsigned int)s_totalMemory);

        return allocation;
    }

#endif



void operator delete(void* memory, size_t size)
{
    free(memory);

    s_totalMemory -= size;

    GAME_2D_LOG_VERBOSE( "Deallocating %lu bytes of memory", (long unsigned int)size );
    GAME_2D_LOG_VERBOSE( "Total Heap Memory Allocated: %lu bytes\n\n", (long unsigned int)s_totalMemory );
}



Json::Value LoadJson(std::string_view filepath)
{
    static std::ifstream s_file;
    Json::CharReaderBuilder builder;
    Json::Value root;
    Json::String errs;

    builder["collectComments"] = false;

    s_file.open( filepath.data() );


    if ( !Json::parseFromStream(builder, s_file, &root, &errs) )
    {
        GAME_2D_LOG_ERROR("Couldn't open %s\n%s\n\n", filepath.data(), errs.c_str());
        s_file.clear();
    }

    s_file.close();

    return root;
}




void OverWriteJson(std::string_view filepath, Json::Value& root)
{
    static std::ofstream s_fileOut;

    // To clear the save file before writing new data
    s_fileOut.open( filepath.data(), std::ios::trunc );


     if ( !s_fileOut.is_open() )
    {
        GAME_2D_LOG_ERROR("Could not open %s.\n\n", filepath.data());
        return;
    }


    Json::StreamWriterBuilder builder;
    s_fileOut << Json::writeString(builder, root);
}

