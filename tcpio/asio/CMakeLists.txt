cmake_minimum_required(VERSION 3.0)

file(STRINGS "version" version)
list(GET version 0 sw_version)
list(GET version 1 hw_version)
project(brown-sugar LANGUAGES C CXX VERSION ${sw_version})
message(STATUS "Project version: ${PROJECT_VERSION}")
message(STATUS "major: ${PROJECT_VERSION_MAJOR}")
message(STATUS "minor: ${PROJECT_VERSION_MINOR}")
message(STATUS "patch: ${PROJECT_VERSION_PATCH}")
message(STATUS "Hardware version: ${hw_version}")


find_package(Git)
set(GIT_HASH "")
if(GIT_FOUND)
  execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:%h 
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_HASH
        ERROR_QUIET)
  message(STATUS "git hash : ${GIT_HASH}")
endif()


string(TIMESTAMP COMPILE_TIME %Y%m%d_%H%M%S)
set(STATION_ID "BS_${hw_version}_${sw_version}_${COMPILE_TIME}_${GIT_HASH}")
message(STATUS "station id: ${STATION_ID}")

configure_file (
    "${PROJECT_SOURCE_DIR}/cmake/version.h.in"
    "${PROJECT_SOURCE_DIR}/app/infrastructure/common/version.h"
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(import_deps)

find_package(OpenCV REQUIRED)
find_package(Communication REQUIRED)
find_package(PackSDK REQUIRED)
find_package(Protobuf REQUIRED)
find_package(TurboJPEG REQUIRED)
find_package(Eigen3 REQUIRED)

INSTALL_DEPENDENCY("communication")
INSTALL_DEPENDENCY("pack-sdk")
# INSTALL_DEPENDENCY("opencv")
# INSTALL_DEPENDENCY("libzmq")
INSTALL_JSON()
INSTALL_CALI_FILE()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_RPATH "$ORIGIN:$ORIGIN/../lib")


OPTION(FT_TEST "complie control to remove hobot files" OFF)

# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++14 -Wno-unused-function -Wno-unused-variable ")
add_definitions("-Wall -g")

add_subdirectory(app)
add_subdirectory(libs)

SET(SRC_LIST stand_alone/main.cpp)

include_directories(
    app/application/include
    app/infrastructure/common
    app/infrastructure/common/ipc
    app/infrastructure/tcp/include
    app/infrastructure/udp/include
    app/infrastructure/ipcm/include
    app/domain/data_fusion/include
    app/domain/proxy/include
    app/domain/communication-user
)

ADD_EXECUTABLE(brown-sugar  ${SRC_LIST})


target_link_libraries(brown-sugar -lpthread adasbusiness hobot-adas-sdk hobot-communication glog)

if (FT_TEST)

else ()
    install(TARGETS ${PROJECT_NAME})
endif()


