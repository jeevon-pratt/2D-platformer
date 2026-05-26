#include <new>                  // std::bad_alloc
#include <stdlib.h>             // malloc and free functions

#include "Utility/Log.hpp"      // GAME_2D_LOG_VERBOSE macro function



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
