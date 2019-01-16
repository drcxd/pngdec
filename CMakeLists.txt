cmake_minimum_required(VERSION 3.5)

project (libpngdec)

find_library (PNG_LIBRARY
             NAMES png
             PATHS /home/chang/install/libpng/lib/
             )

find_path (LIBPNG_INCLUDES png.h
          /home/chang/install/libpng/include/
          )

include_directories (${LIBPNG_INCLUDES})

set(GCC_FLAGS -g)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_FLAGS}")

add_executable (libpngdec libpngdec.c)

target_link_libraries (libpngdec ${PNG_LIBRARY})
