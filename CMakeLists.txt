cmake_minimum_required(VERSION 3.22)
project(hkui)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../debug)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    # Linux-specific code or configuration here
    message(STATUS "Configuring for Linux")
    
    set(CMAKE_CXX_STANDARD 23)
    # set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -ggdb")

    # Deps include path
    set(FT_LIBRARY_INCLUDE_DIRECTORY "/home/hekapoo/Downloads/freeType2/freetype-2.13.1/include/")
    set(GLEW_LIBRARY_INCLUDE_DIRECTORY "/home/hekapoo/Downloads/glew-2.1.0/include")
    set(GLFW_LIBRARY_INCLUDE_DIRECTORY "/home/hekapoo/Downloads/glfw-3.3.8/include")
    
    include_directories (${FT_LIBRARY_INCLUDE_DIRECTORY})
    include_directories (${GLEW_LIBRARY_INCLUDE_DIRECTORY})
    include_directories (${GLFW_LIBRARY_INCLUDE_DIRECTORY})

    add_library(${PROJECT_NAME} STATIC)

    # Include of build lib
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    add_subdirectory(src/shaderHelpers)
    add_subdirectory(src/meshHelpers)
    add_subdirectory(src/renderHelpers)
    add_subdirectory(src/inputHelpers)
    add_subdirectory(src/treeHelpers)
    add_subdirectory(src/textureHelpers)
    add_subdirectory(src/utils)
    add_subdirectory(src/textHelpers)

    set(FT_LIBRARY_PATH_DIRECTORY "/home/hekapoo/Downloads/freeType2/freetype-2.13.1/objs/.libs")
    set(GLEW_LIBRARY_PATH_DIRECTORY "/home/hekapoo/Downloads/glew-2.1.0/lib")
    set(GLFW_LIBRARY_PATH_DIRECTORY "/home/hekapoo/Downloads/glfw-3.3.8/src")

    target_link_directories(${PROJECT_NAME} PRIVATE
        ${GLEW_LIBRARY_PATH_DIRECTORY}        # Path to the directory containing GLEW library file
        ${GLFW_LIBRARY_PATH_DIRECTORY}        # Path to the directory containing GLFW library file
        ${FT_LIBRARY_PATH_DIRECTORY}          # Path to the directory containing FreeType library file
        ${LZMA_LIBRARY_PATH_DIRECTORY}        # Path to the directory containing LZMA library file
    )

    target_link_libraries(${PROJECT_NAME} glfw3 GLEW GL X11 pthread Xrandr dl freetype)

elseif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    # Windows-specific code or configuration here

    set(CMAKE_CXX_STANDARD 20)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

    # Deps include path
    set(FT_LIBRARY_INCLUDE_DIRECTORY "C:/Users/boje/Desktop/helperz/cpp/deps/glew_source/glew-2.1.0/include")
    set(GLEW_LIBRARY_INCLUDE_DIRECTORY "C:/Users/boje/Desktop/helperz/cpp/deps/lzma/include")
    set(GLFW_LIBRARY_INCLUDE_DIRECTORY "C:/Users/boje/Desktop/helperz/cpp/deps/freetype-2.13.2/include")

    include_directories (${FT_LIBRARY_INCLUDE_DIRECTORY})
    include_directories (${GLEW_LIBRARY_INCLUDE_DIRECTORY})
    include_directories (${GLFW_LIBRARY_INCLUDE_DIRECTORY})
    
    add_library(${PROJECT_NAME} STATIC)

    # Include of build lib
    target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )

    add_subdirectory(src/shaderHelpers)
    add_subdirectory(src/meshHelpers)
    add_subdirectory(src/renderHelpers)
    add_subdirectory(src/inputHelpers)
    add_subdirectory(src/treeHelpers)
    add_subdirectory(src/textureHelpers)
    add_subdirectory(src/utils)
    add_subdirectory(src/textHelpers)
    add_subdirectory(src/vendor)

    # Deps lib path
    set(GLEW_LIBRARY_PATH_DIRECTORY "C:/Users/boje/Desktop/helperz/cpp/deps/glew_source/glew-2.1.0/build/cmake/build/lib")
    set(GLFW_LIBRARY_PATH_DIRECTORY "C:/Users/boje/Desktop/helperz/cpp/deps/glfw-3.3.8.bin.WIN64/lib-mingw-w64")
    set(FT_LIBRARY_PATH_DIRECTORY "C:/Users/boje/Desktop/helperz/cpp/deps/freetype-2.13.2/build")

    target_link_directories(${PROJECT_NAME} PRIVATE
        ${GLEW_LIBRARY_PATH_DIRECTORY}        # Path to the directory containing GLEW library file
        ${GLFW_LIBRARY_PATH_DIRECTORY}        # Path to the directory containing GLFW library file
        ${FT_LIBRARY_PATH_DIRECTORY}          # Path to the directory containing FreeType library file
    )

    # There is a problem with statically linking stdlibc++ and libgcc on mingw with c++23
    target_link_options(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc -static C:/MinGW/x86_64-w64-mingw32/lib/libwinpthread.a)

    target_link_libraries(${PROJECT_NAME} glew32 opengl32 gdi32 user32 kernel32 glfw3 freetype elzip lzma)

# If the operating system is not recognized
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()