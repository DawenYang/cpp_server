file(GLOB_RECURSE cpp_server_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(cpp_server_shared SHARED ${cpp_server_sources} include/Macros.h include/util.h)
