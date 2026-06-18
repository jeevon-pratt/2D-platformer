#pragma once

#ifdef _MSC_VER
#include <vcruntime.h>  // operator new annotations
#endif

#include <json/json.h>      // Jsoncpp functionality

#include <string>           // std::string



// ****************
// MEMORY FUNCTIONS
// ****************

 // Operator overload for observing the allocation of dynamic memory
 //
 // Note: Information is recorded using the debug logging system.
#ifdef _MSC_VER

_NODISCARD _Ret_notnull_ _Post_writable_byte_size_(size) _VCRT_ALLOCATOR
void* __cdecl operator new(size_t size) noexcept(false);

#else

void* operator new(size_t size) noexcept(false); 

#endif


 // Operator overload for observing the deallocation of dynamic memory
 //
 // Note: Information is recorded using the debug logging system.
void operator delete(void* memory, size_t size) noexcept;




// *****************
// JSON IO FUNCTIONS
// *****************

 // Function for loading data from JSON File
 //
 // Note: Empty data is returned and prints an error message is logged upon failure.
Json::Value LoadJson(const std::string& filepath);

 // Function for overwriting data from JSON File
 //
 // Note: An error message is logged upon failure.
void OverWriteJson(const std::string& filepath, Json::Value& root);
