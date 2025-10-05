cmake_minimum_required(VERSION 3.20)

project(HadesModLoader VERSION 1.2 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MODLOADER_DEDICATED_LAUNCHER "Build dedicated launcher" ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

add_subdirectory(libs/lua-5.2.2)

set(MOD_LOADER_INSTALL_LIST HadesModNativeExtension)

if(${MODLOADER_DEDICATED_LAUNCHER})
add_subdirectory(launcher)
list(APPEND MOD_LOADER_INSTALL_LIST HadesModdedLauncher)
endif()

add_subdirectory(libs/hades2-engine-interface)

add_subdirectory(core)

install(TARGETS ${MOD_LOADER_INSTALL_LIST}
        RUNTIME
        DESTINATION ${CMAKE_SOURCE_DIR}/bin
)
