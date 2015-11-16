
cmake_minimum_required(VERSION 3.1.0)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)

set(server_SOURCES ${server_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/AccountController.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UserManager.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ConversationController.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/Packet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ServerError.cpp)
set(server_HEADERS ${server_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/inc/AccountController.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/AController.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/UserManager.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/ConversationController.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/Enum.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/IServerMonitor.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/IServerSocket.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/ISocketSet.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/NetworkManager.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/Packet.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/ServerError.h)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	set(server_SOURCES ${server_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/WinServerMonitor.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinServerSocket.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinSocketSet.cpp)
	set(server_HEADERS ${server_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/inc/WinServerMonitor.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/WinServerSocket.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/WinSocketSet.h)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	set(server_SOURCES ${server_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixServerMonitor.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixServerSocket.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixSocketSet.cpp)
	set(server_HEADERS ${server_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixServerMonitor.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixServerSocket.h ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixSocketSet.h)
endif (CMAKE_SYSTEM_NAME STREQUAL Windows)

add_executable(server_rtype ${server_SOURCES} ${server_HEADERS})

target_compile_features(server_rtype PRIVATE cxx_range_for)

set_target_properties(server_rtype PROPERTIES OUTPUT_NAME "server_rtype")

if (MSVC)
    set_target_properties(server_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set_target_properties(server_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
    set_target_properties(server_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
endif (MSVC)
