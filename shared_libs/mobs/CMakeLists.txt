
cmake_minimum_required(VERSION 3.1.0)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/)
set(mobs_SOURCES ${mobs_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/mobs.cpp ${CMAKE_CURRENT_SOURCE_DIR}/../common_src/JSONParser.cpp)
set(mobs_HEADERS ${mobs_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Entity/ComponentManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Entity/Entity.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Entity/ManagerException.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Component/Component.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/Component/IComponent.hh ${CMAKE_CURRENT_SOURCE_DIR}/../common_inc/JSONParser.hh)

add_library(mobs SHARED ${mobs_SOURCES} ${mobs_HEADERS})

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/shared_libs/)
	set_target_properties(mobs PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/shared_libs/)
	set_target_properties(mobs PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/shared_libs/)
	set_target_properties(mobs PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/shared_libs/)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wall -Wextra")
endif()
