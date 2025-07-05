cmake_minimum_required(VERSION 3.29)
project(ChessEngine)

set(CMAKE_CXX_STANDARD 23)

################
# Source files #
################
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_executable(ChessEngine ${PROJECT_SOURCE_DIR}/src/main.cpp ${SOURCES} ${HEADERS})

target_include_directories(ChessEngine
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)
