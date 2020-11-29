cmake_minimum_required(VERSION 3.1)
project(SeamCarving)

add_subdirectory(src/ext)

# include directories
include_directories(${PROJECT_SOURCE_DIR}/src
                    ${PROJECT_SOURCE_DIR}/src/ext
                    ${PROJECT_SOURCE_DIR}/src/ext/stb
)

set(ext_stb_srcs
    src/ext/stb/stb_image_write.h
    src/ext/stb/stb_image.h
)

add_definitions(-DDATA_DIR=R\"\(${PROJECT_SOURCE_DIR}/data\)\")

set(common_srcs
    src/array3D.h
    src/exceptions.h
    src/floatimage.cpp
    src/floatimage.h
    src/utils.h
)

# Compile common library
add_library(common_lib STATIC
    ${common_srcs}
    ${ext_stb_srcs}
)
SOURCE_GROUP("src\\ext\\stb" FILES ${ext_stb_srcs})

# dependencies for SeamCarving_main.cpp
set(SeamCarving_srcs   src/SeamCarving_main.cpp src/filtering.cpp src/SeamCarving.cpp)   # SeamCarving
add_executable(SeamCarving ${SeamCarving_srcs})                                 # SeamCarving
target_link_libraries(SeamCarving common_lib)                                   # SeamCarving
SOURCE_GROUP("src" FILES ${SeamCarving_srcs})                                   # SeamCarving

# Create the output data directory
add_custom_command(TARGET SeamCarving POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory "${PROJECT_SOURCE_DIR}/data/output") # SeamCarving