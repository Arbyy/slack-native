cmake_minimum_required(VERSION 3.0.0)
project(slack-native VERSION 0.0.0)

file(GLOB_RECURSE sources "${CMAKE_SOURCE_DIR}/src/*.c"
                          "${CMAKE_SOURCE_DIR}/src/*.h")

add_executable(slack-native "${sources}")

target_include_directories(slack-native PUBLIC 
                                        "${CMAKE_SOURCE_DIR}/lib/include")

# We currently just using 32-bit builds for windows
if(WIN32)
    set(LIB_PATH "${CMAKE_SOURCE_DIR}/lib/x86")

    find_library(SDL2_lib NAMES SDL2 PATHS "${LIB_PATH}")
    find_library(SDL2main_lib NAMES SDL2main PATHS "${LIB_PATH}")
    find_library(SDL2_image_lib NAMES SDL2_image PATHS "${LIB_PATH}")
    # Directly specifying the libcurl version here, this is only because I had a version
    # of libcurl installed on my system that caused issues because it was static
    set(cURL_lib "${LIB_PATH}/libcurl.dll.a")
    find_library(SDL2_ttf_lib NAMES SDL2_ttf PATHS "${LIB_PATH}")
    find_library(avcodec_lib NAMES libavcodec PATHS "${LIB_PATH}")
    find_library(avformat_lib NAMES libavformat PATHS "${LIB_PATH}")
    find_library(websockets_lib NAMES libwebsockets PATHS "${LIB_PATH}")
ENDIF()

if(UNIX)
    find_library(SDL2_lib NAMES SDL2)
    find_library(SDL2main_lib NAMES SDL2main)
    find_library(SDL2_image_lib NAMES SDL2_image)
    find_library(cURL_lib NAMES curl)
    find_library(SDL2_ttf_lib NAMES SDL2_ttf)
    find_library(avcodec_lib NAMES avcodec)
    find_library(avformat_lib NAMES avformat)
    find_library(websockets_lib NAMES websockets)
    # Don't know how to do this on windows :( we need it for math.h
    find_library(math_lib NAMES m)
ENDIF()

target_link_libraries(slack-native ${SDL2_lib}
                                   ${SDL2main_lib}
                                   ${SDL2_image_lib}
                                   ${SDL2_ttf_lib}
                                   ${cURL_lib}
                                   ${avcodec_lib}
                                   ${avformat_lib}
                                   ${websockets_lib}
                                   ${math_lib})
