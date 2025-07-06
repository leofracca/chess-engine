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

#################
# Documentation #
#################
find_package(Doxygen)
# Check if Doxygen is installed
if(DOXYGEN_FOUND)
    # Copy the Doxyfile from the source directory to the binary directory
    configure_file(${PROJECT_SOURCE_DIR}/Doxyfile ${PROJECT_BINARY_DIR}/Doxyfile @ONLY)

    # Create the output directory
    file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/docs)

    # Add a custom target to generate the documentation
    add_custom_target(doc
            COMMAND ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMENT "Generating documentation with Doxygen"
            VERBATIM
    )
else()
    message(WARNING "Doxygen need to be installed to generate the doxygen documentation.")
endif()
