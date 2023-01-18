file(GLOB_RECURSE cpp_server_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(cpp_server_shared SHARED ${cpp_server_sources} include/Macros.h include/util.h Poller.cpp Buffer.cpp include/Buffer.h Channel.cpp include/Channel.h EventLoop.cpp include/EventLoop.h Socket.cpp include/Socket.h ThreadPool.cpp include/ThreadPool.h Acceptor.cpp include/Acceptor.h Connection.cpp include/Connection.h)
