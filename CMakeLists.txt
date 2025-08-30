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

#set(COMPILER_FLAGS -Wall -Wextra -Wpedantic -Werror)
set(COMPILER_FLAGS -Wall -Wextra -Wpedantic) # -Werror is too strict for now
if (COVERAGE OR USAN)
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

add_library(chess_engine_lib)
add_executable(chess_engine
        ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_sources(chess_engine_lib
        PUBLIC  ${HEADERS}
        PRIVATE ${SOURCES}
)

target_include_directories(chess_engine_lib
        PUBLIC  ${PROJECT_SOURCE_DIR}/include
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)
target_include_directories(chess_engine
        PUBLIC  ${PROJECT_SOURCE_DIR}/include
        PRIVATE ${PROJECT_SOURCE_DIR}/src
)

target_compile_options(chess_engine_lib
        PRIVATE ${COMPILER_FLAGS}
        PRIVATE $<$<BOOL:${COVERAGE}>:--coverage>
)
target_compile_options(chess_engine
        PRIVATE ${COMPILER_FLAGS}
        PRIVATE $<$<BOOL:${COVERAGE}>:--coverage>
)

target_link_options(chess_engine_lib
        PRIVATE ${LINKER_FLAGS}
)
target_link_options(chess_engine
        PRIVATE ${LINKER_FLAGS}
)

target_link_libraries(chess_engine
        PRIVATE chess_engine_lib
)

# Install
file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
install(
        FILES ${CMAKE_BINARY_DIR}/chess_engine
        DESTINATION ${PROJECT_SOURCE_DIR}/bin
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

#########
# Tests #
#########
enable_testing()
file(GLOB_RECURSE TESTS   CONFIGURE_DEPENDS tests/*.cpp)

include(FetchContent)
# Fetch latest Google Test
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        main
)
FetchContent_MakeAvailable(googletest)

# Add the tests
add_executable(chess_engine_tests ${TESTS} ${HEADERS})
target_include_directories(chess_engine_tests
        PUBLIC ${PROJECT_SOURCE_DIR}/include
        PRIVATE ${googletest_SOURCE_DIR}/googletest/include
)
target_link_libraries(chess_engine_tests chess_engine_lib gtest_main gmock gtest)
target_compile_options(chess_engine_tests
        PRIVATE ${COMPILER_FLAGS}
        PRIVATE $<$<BOOL:${COVERAGE}>:--coverage>
)
target_link_options(chess_engine_tests
        PRIVATE ${LINKER_FLAGS}
)
include(GoogleTest)
gtest_discover_tests(chess_engine_tests)
