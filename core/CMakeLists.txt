add_library(HadesModNativeExtension SHARED)

add_definitions(-DHADES_MOD_LOADER -DNOMINMAX)

include_directories (
   src/.
   ../libs/lua-5.2.2/src
   ../libs/json
   ../libs/hades2-engine-interface/src
)

file(GLOB_RECURSE HADES_MOD_CORE_FILES src/*.cpp src/*.h)

target_sources(
    HadesModNativeExtension
    PUBLIC
        ${HADES_MOD_CORE_FILES} )

target_link_libraries(HadesModNativeExtension PRIVATE liblua Hades2EngineInterface)
target_precompile_headers(HadesModNativeExtension PRIVATE src/pch.h)
set_target_properties(HadesModNativeExtension PROPERTIES
    OUTPUT_NAME "HadesModNativeExtension"
    SUFFIX ".asi"
)
