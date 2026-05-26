# **********************************
cmake_minimum_required(VERSION 3.10)

project(2D_Platformer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# **********************************



# ********************
# PROJECT SOURCE FILES
# ********************

set(COLLISION_SOURCES
    sources/Collision/Contact_Listener.cpp
    sources/Collision/Contact_Listener.hpp
)

set(CORE_SOURCES
    sources/Core/Game_2D.cpp
    sources/Core/Game_2D.hpp
    sources/Core/Main.cpp
)

set(ENTITY_SOURCES
    sources/Entity/Enemy.cpp
    sources/Entity/Enemy.hpp
    sources/Entity/Game_Object.cpp
    sources/Entity/Game_Object.hpp
    sources/Entity/Player.cpp
    sources/Entity/Player.hpp
)

set(GAME_STATE_SOURCES
    sources/Game_State/Game_Over_State.cpp
    sources/Game_State/Game_State.hpp
    sources/Game_State/Game_State_Manager.cpp
    sources/Game_State/Game_State_Manager.hpp
    sources/Game_State/Level_1_State.cpp
    sources/Game_State/Level_2_State.cpp
    sources/Game_State/Level_3_State.cpp
    sources/Game_State/Level_4_State.cpp
    sources/Game_State/Level_5_State.cpp
    sources/Game_State/Main_Menu_State.cpp
    sources/Game_State/Paused_State.cpp
    sources/Game_State/Settings_State.cpp
)

set(MEDIA_SOURCES
    sources/Media/Animation.cpp
    sources/Media/Animation.hpp
    sources/Media/Audio.hpp
    sources/Media/Audio_Manager.cpp
    sources/Media/Button.cpp
    sources/Media/Button.hpp
    sources/Media/Camera.cpp
    sources/Media/Camera.hpp
    sources/Media/Font_Manager.cpp
    sources/Media/Key_State.hpp
    sources/Media/Renderer.cpp
    sources/Media/Renderer.hpp
    sources/Media/Sprite.cpp
    sources/Media/Sprite.hpp
    sources/Media/Text.hpp
    sources/Media/Texture_Manager.cpp
    sources/Media/Texture_Manager.hpp
    sources/Media/User_Interface.cpp
    sources/Media/User_Interface.hpp
    sources/Media/Window.cpp
    sources/Media/Window.hpp
)

if(WIN32)
    list(APPEND MEDIA_SOURCES sources/Media/Icon.rc)
endif()


set(PLAYER_STATE_SOURCES
    sources/Player_State/Attack_State.cpp
    sources/Player_State/Dead_State.cpp
    sources/Player_State/Free_Fall_State.cpp
    sources/Player_State/Hit_Ground_State.cpp
    sources/Player_State/Idle_State.cpp
    sources/Player_State/Jump_State.cpp
    sources/Player_State/Player_State.hpp
    sources/Player_State/Player_State_Manager.cpp
    sources/Player_State/Player_State_Manager.hpp
    sources/Player_State/Walk_State.cpp
)

set(UTILITY_SOURCES
    sources/Utility/Assert.hpp
    sources/Utility/Log.cpp
    sources/Utility/Log.hpp
    sources/Utility/Math.cpp
    sources/Utility/Math.hpp
    sources/Utility/Memory.cpp
    sources/Utility/Memory.hpp
    sources/Utility/Perf_Monitor.cpp
    sources/Utility/Perf_Monitor.hpp
    sources/Utility/Save_System.cpp
    sources/Utility/Save_System.hpp
)



# *****************
# Adding Executable
# *****************

add_executable(${PROJECT_NAME}
    ${COLLISION_SOURCES}
    ${CORE_SOURCES}
    ${ENTITY_SOURCES}
    ${GAME_STATE_SOURCES}
    ${MEDIA_SOURCES}
    ${PLAYER_STATE_SOURCES}
    ${UTILITY_SOURCES}
)



# ******************
# PROJECT PROPERTIES
# ******************

if(MSVC)
    # Set executable working directory
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/$(Configuration)"
    )
    
    # Enable Multi-processor Compilation
    add_definitions(/MP)
endif()

# Define DEBUG Macro for the debug configuration
target_compile_definitions(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:DEBUG>)




# ***************************
# Setting Include Directories
# ***************************

target_include_directories(${PROJECT_NAME} PRIVATE
    # Local include directory
    ${CMAKE_SOURCE_DIR}/sources
    
    # Library include directories
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/include
    ${CMAKE_SOURCE_DIR}/vendor/Box2D/include
    ${CMAKE_SOURCE_DIR}/vendor/JsonCpp/include
)



# **************************
# Linking External Libraries
# **************************

target_link_libraries(${PROJECT_NAME} PRIVATE
    # SDL2 Libraries
    "${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2main.lib"
    "${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2.lib"
    "${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_image.lib"
    "${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_ttf.lib"
    "${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_mixer.lib"
    "${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2test.lib"
    
    # Box2D Libraries
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/debug/box2d.lib>
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/debug/glad.lib>
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/debug/glfw.lib>
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/debug/imgui.lib>
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/debug/sajson.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/release/box2d.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/release/glad.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/release/glfw.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/release/imgui.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/vendor/Box2D/lib/release/sajson.lib>
    
    # JsonCpp Libraries
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/vendor/JsonCpp/lib/debug/jsoncpp.lib>
    $<$<CONFIG:Debug>:${CMAKE_SOURCE_DIR}/vendor/JsonCpp/lib/debug/jsoncpp_static.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/vendor/JsonCpp/lib/release/jsoncpp.lib>
    $<$<CONFIG:Release>:${CMAKE_SOURCE_DIR}/vendor/JsonCpp/lib/release/jsoncpp_static.lib>
)



# ******************************
# Copying Dynamic Link Libraries
# ******************************

file(
    COPY 
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2.dll
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_image.dll
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_mixer.dll
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_ttf.dll
    ${CMAKE_SOURCE_DIR}/vendor/JsonCpp/build/bin/debug/jsoncpp.dll
    
    DESTINATION
    ${CMAKE_BINARY_DIR}/debug
)

file(
    COPY 
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2.dll
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_image.dll
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_mixer.dll
    ${CMAKE_SOURCE_DIR}/vendor/SDL2-w64/lib/x64/SDL2_ttf.dll
    ${CMAKE_SOURCE_DIR}/vendor/JsonCpp/build/bin/release/jsoncpp.dll
    
    DESTINATION
    ${CMAKE_BINARY_DIR}/release
)



# **********************
# Adding Virtual Folders
# **********************

source_group("Collision"    FILES ${COLLISION_SOURCES}   )
source_group("Core"         FILES ${CORE_SOURCES}        )
source_group("Entity"       FILES ${ENTITY_SOURCES}      )
source_group("Game_State"   FILES ${GAME_STATE_SOURCES}  )
source_group("Media"        FILES ${MEDIA_SOURCES}       )
source_group("Player_State" FILES ${PLAYER_STATE_SOURCES})
source_group("Utility"      FILES ${UTILITY_SOURCES}     )
