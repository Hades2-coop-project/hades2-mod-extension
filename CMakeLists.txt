cmake_minimum_required(VERSION 3.20)

project(Hades2ModLoader VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

set(HADES_PLUGIN_DIR ${CMAKE_SOURCE_DIR}/bin)
set(HADES_MOD_INSTALL_PATH ${CMAKE_SOURCE_DIR}/bin/TN_Core)

# Load environment variables from env.cmake
if (EXISTS ${CMAKE_SOURCE_DIR}/env.cmake)
    include(${CMAKE_SOURCE_DIR}/env.cmake)
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
        DESTINATION ${HADES_PLUGIN_DIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/core/resources/
        DESTINATION ${HADES_MOD_INSTALL_PATH}
)
