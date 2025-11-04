# ============================================================
# Toolchain file for cross-compiling to Windows (x86_64)
# ============================================================

# Tell CMake we are targeting Windows
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 10)

# Choose the compiler executables
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)

# Optional: location where MinGW-w64 libraries and includes live
# (On most Linux distros these are already in /usr/x86_64-w64-mingw32/)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)

# Tell CMake how to search for headers and libraries
# Use only the target system’s paths, not the host’s
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
