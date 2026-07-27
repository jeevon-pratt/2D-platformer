# **********************************
cmake_minimum_required(VERSION 3.20)

project(2D_Platformer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# **********************************



# ********************
# PROJECT SOURCE FILES
# ********************

file(GLOB_RECURSE PROJECT_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.hpp"
)

if(WIN32)
    list(APPEND PROJECT_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/Media/Icon.rc"
    )
endif()



# ******************
# IMGUI SOURCE FILES
# ******************

file(GLOB_RECURSE IMGUI_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/*.h")



# *****************
# ADDING EXECUTABLE
# *****************

add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${IMGUI_SOURCES}
)



# ******************
# PROJECT PROPERTIES
# ******************

if(MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    )

    target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
    SDL_MAIN_HANDLED
)



# ***************************
# SETTING INCLUDE DIRECTORIES
# ***************************

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/sources"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/backends"
)



# **************************
# FINDING EXTERNAL LIBRARIES
# **************************

find_package(SDL3       CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(SDL3_mixer CONFIG REQUIRED)
find_package(SDL3_ttf   CONFIG REQUIRED)
find_package(box2d      CONFIG REQUIRED)
find_package(jsoncpp    CONFIG REQUIRED)



# **************************
# LINKING EXTERNAL LIBRARIES
# **************************

target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
    box2d::box2d
    JsonCpp::JsonCpp
)



# **********************
# ADDING VIRTUAL FOLDERS
# **********************

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/sources"
    PREFIX "Sources"
    FILES ${PROJECT_SOURCES}
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui"
    PREFIX "ImGui"
    FILES ${IMGUI_SOURCES}
)