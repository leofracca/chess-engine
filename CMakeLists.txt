cmake_minimum_required(VERSION 3.29)
project(ChessEngine
    DESCRIPTION "Chess Engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)

#########
# Flags #
#########
# Sanitizers
option(USAN     OFF)
# Coverage
option(COVERAGE OFF)

set(COMPILER_FLAGS -Wall -Wextra -Wpedantic -Werror)
if (NOT COVERAGE)
    set(COMPILER_FLAGS ${COMPILER_FLAGS} -O3)
else ()
    set(COMPILER_FLAGS ${COMPILER_FLAGS}
            -O0
            -g
            $<$<BOOL:${USAN}>:-fsanitize=undefined -fno-omit-frame-pointer>
    )
endif()
# message(WARNING "Compiler flags are set to: ${COMPILER_FLAGS}")

set(LINKER_FLAGS
        $<$<BOOL:${USAN}>:-fsanitize=undefined -fno-omit-frame-pointer>
        $<$<BOOL:${COVERAGE}>:--coverage>
)

################
# Source files #
################
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)
list(REMOVE_ITEM SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS include/*.h)

add_executable(chess_engine ${PROJECT_SOURCE_DIR}/src/main.cpp ${SOURCES} ${HEADERS})

target_include_directories(chess_engine
        PUBLIC ${PROJECT_SOURCE_DIR}/include
)

# Install
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
install(
        FILES ${CMAKE_BINARY_DIR}/chess_engine
        DESTINATION ${PROJECT_SOURCE_DIR}/bin
)

#################
# Code Coverage #
#################
if (COVERAGE)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/coverage)
    add_custom_target(coverage
            COMMAND gcovr --root ${PROJECT_SOURCE_DIR}
            --html
            --html-details
            --output coverage/coverage.html
            COMMENT "Generating code coverage"
    )
endif()

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
