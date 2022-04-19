cmake_minimum_required(VERSION 3.21)
project(biomapper2)

# Set the C++ Standard to C++20
set(CMAKE_CXX_STANDARD 20)
# Set the output directory to bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##########################################
# Message coloring convenience variables
# This is just for better looking output.
##########################################
if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColourReset "${Esc}[m")
    set(ColourBold  "${Esc}[1m")
    set(Red         "${Esc}[31m")
    set(Green       "${Esc}[32m")
    set(Yellow      "${Esc}[33m")
    set(Blue        "${Esc}[34m")
    set(Magenta     "${Esc}[35m")
    set(Cyan        "${Esc}[36m")
    set(White       "${Esc}[37m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
    set(BoldYellow  "${Esc}[1;33m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldMagenta "${Esc}[1;35m")
    set(BoldCyan    "${Esc}[1;36m")
    set(BoldWhite   "${Esc}[1;37m")
endif()

# Convenience function to color output
function(DisplayPackage pkg)
    message("[${Yellow}${pkg}${ColourReset}]")
endfunction()


# Set subdirectory cmake options
if(CMAKE_BUILD_TYPE MATCHES Debug)
    DisplayPackage("BioMapper")
    message(STATUS "Building Debug Version of BioMapper")
    # Should pass to the compiler so we can have this defined in the code
    # Allows for conditional code for debug variant
    add_definitions(-DDEBUG_MESSAGES)
else()
    DisplayPackage("BioMapper")
    message(STATUS "Building Release Version of BioMapper")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

file(GLOB SOURCE_FILES "src/*.c" "src/*.h" "src/*.cpp" "src/*.hpp")

# Add in subdirectories that files are included from to the project
target_include_directories(BioMapperTest PRIVATE src)
target_include_directories(BioMapperTest PRIVATE ${CMAKE_BINARY_DIR})

#add_library(biomapper2 SHARED src/BioMapper.cpp)

add_executable(BioMapperTest src/main.cpp ${SOURCE_FILES})

target_link_libraries(BioMapperTest PRIVATE biomapper2)


# Set G++ build info for this project (depending on debug vs release)
if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(BioMapperTest PRIVATE -g -pthread -Wno-deprecated-declarations)
else()
    target_compile_options(BioMapperTest PRIVATE -O3 -pthread -Wno-deprecated-declarations)
endif(CMAKE_BUILD_TYPE MATCHES Debug)
