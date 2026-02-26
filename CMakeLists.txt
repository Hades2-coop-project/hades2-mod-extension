cmake_minimum_required(VERSION 3.20)

project(Hades2ModLoader VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MODLOADER_DEDICATED_LAUNCHER "Build dedicated launcher" OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_definitions(-DHADES2_MOD_LOADER -DNOMINMAX)

add_definitions(-DEASTL_EASTDC_VSNPRINTF=0)
add_definitions(-DEASTL_DEBUG=0)
add_definitions(-DEASTL_CUSTOM_FLOAT_CONSTANTS_REQUIRED)

add_subdirectory(libs/lua-5.2.2)
add_subdirectory(libs/hades2-engine-interface)
add_subdirectory(core)

install(TARGETS HadesModNativeExtension
        RUNTIME
        DESTINATION ${CMAKE_SOURCE_DIR}/bin
)

if(${MODLOADER_DEDICATED_LAUNCHER})
add_subdirectory(launcher)

install(TARGETS HadesModdedLauncher
        RUNTIME
        DESTINATION ${CMAKE_SOURCE_DIR}/bin
)
endif()
