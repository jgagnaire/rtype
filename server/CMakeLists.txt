
cmake_minimum_required(VERSION 3.1.0)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc/)

set(server_SOURCES ${server_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/AccountController.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UserManager.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ServerError.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/GameController.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/GameManager.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/GameplayController.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/JSONParser.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/JSONSerializer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ShootSystem.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/MobSystem.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/BonusSystem.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ASystem.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/BossSystem.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/ADLLoader.cpp)
set(server_HEADERS ${server_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/inc/AccountController.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/AController.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/UserManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ConversationController.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Enum.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/IServerMonitor.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/IServerSocket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ISocketSet.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/NetworkManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Packet.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ServerError.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ACommunicator.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/IThreadPool.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ThreadPool.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/IThread.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ThreadFactory.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/GameController.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/GameManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/GameplayController.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Component.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/IComponent.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/Entity.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ComponentManager.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ManagerException.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/JSONParser.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ShootSystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ASystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/JSONSerializer.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/MobSystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/BonusSystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/BossSystem.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/ADLLoader.hh)

if (CMAKE_SYSTEM_NAME STREQUAL Windows)
	set(server_SOURCES ${server_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/WinServerMonitor.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinServerSocket.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinSocketSet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinMutex.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinTCPSocketSet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinUDPSocketSet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/WinDLLoader.cpp)
	set(server_HEADERS ${server_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/inc/WinServerMonitor.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/WinServerSocket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/WinSocketSet.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/WinDLLoader.hh)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	set(server_SOURCES ${server_SOURCES} ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixServerMonitor.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixServerSocket.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixSocketSet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixMutex.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixTCPSocketSet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixUDPSocketSet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixDLLoader.cpp)
	set(server_HEADERS ${server_HEADERS} ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixServerMonitor.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixServerSocket.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixSocketSet.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixMutex.hh ${CMAKE_CURRENT_SOURCE_DIR}/inc/UnixDLLoader.hh)
endif (CMAKE_SYSTEM_NAME STREQUAL Windows)

add_executable(server_rtype ${server_SOURCES} ${server_HEADERS})

target_compile_features(server_rtype PRIVATE cxx_range_for)

set_target_properties(server_rtype PROPERTIES OUTPUT_NAME "server_rtype")

if (MSVC)
	set_target_properties(server_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set_target_properties(server_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR})
    set_target_properties(server_rtype PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR})
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -Wall -Wextra")
endif (MSVC)

target_link_libraries(server_rtype ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})
