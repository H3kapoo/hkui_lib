## HkUI

Simple UI library written in C++ with glew/GLFW backends.

### Building

For now only the **static library** can be built, namely: **libhkui.a**
MSVC support does not currently exist and Windows version has been created with MinGW support, so make sure you have that tool-chain installed first.

#### For Linux:
```bash
    chmod +x ./build.sh
    ./build.sh
```
```bash
    Build dependencies: GLFW glew freetype
```
---
#### For Windows:
```cmd
    cmake .
    make
```
```cmd
    Build dependencies: GLFW glew freetype
```

If you have **gitbash** installed, the build.sh from Linux can be ran as well.
Before building, make sure you **fill all** the necessary locations for the include/lib dependencies:

```bash
    # Deps include path
    set(FT_LIBRARY_INCLUDE_DIRECTORY "path to include of ft")
    set(GLEW_LIBRARY_INCLUDE_DIRECTORY "path to include of glew")
    set(GLFW_LIBRARY_INCLUDE_DIRECTORY "path to include of glfw")

    ...

    set(FT_LIBRARY_PATH_DIRECTORY "path to lib of ft")
    set(GLEW_LIBRARY_PATH_DIRECTORY "path to lib of glew")
    set(GLFW_LIBRARY_PATH_DIRECTORY "path to lib of glfw")    
```

To link, simply add ```-lhkui``` to your project link libraries.