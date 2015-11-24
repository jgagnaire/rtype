
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/ ${CMAKE_CURRENT_SOURCE_DIR}/inc/Audio)

file(GLOB_RECURSE client_SOURCES "src/Graphics/*.cpp")
file(GLOB_RECURSE client_SOURCES "src/Scene/*.cpp")
file(GLOB_RECURSE client_SOURCES "src/Audio/*.cpp")
file(GLOB_RECURSE client_SOURCES "src/*.cpp")
file(GLOB_RECURSE client_HEADERS "inc/Audio/*.hh")
file(GLOB_RECURSE client_HEADERS "inc/*.hh")
file(GLOB_RECURSE client_HEADERS "inc/Entity/*.hh")
file(GLOB_RECURSE client_HEADERS "inc/Compenent/*.hh")
file(GLOB_RECURSE client_HEADERS "inc/Graphics/*.hh")

add_executable(client_rtype ${client_SOURCES} ${client_HEADERS})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)

target_compile_features(client_rtype PRIVATE cxx_range_for)

set_target_properties(client_rtype PROPERTIES OUTPUT_NAME "client_rtype")

if (MSVC)
    set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
    set_target_properties(client_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
    set(CMAKE_EXE_LINKER_FLAGS, "${CMAKE_EXE_LINKER_FLAGS} -pthread")
endif (MSVC)

target_link_libraries(client_rtype ${CMAKE_THREAD_LIBS_INIT} ${SFML_LIBRARIES})
