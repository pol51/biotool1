# biotool1

"biotool1" a Cellular Biology tool writen in C++x17, using Qt6 and OpenGL.

## Install
You can build biotool1 with CMake:

* `cmake -S . -B build`
* `cmake --build build`

Then you sould find a "biotool1" executable in this folder.

## Windows Installer (NSIS)
The CMake setup bundles Qt6 dependencies using `windeployqt` and packages an NSIS installer via CPack.

1. Configure and build:
   - `cmake -S . -B build`
   - `cmake --build build --config Release`
2. Create the installer (from the build directory):
   - `cpack -G NSIS`

The resulting installer includes Qt6 plugins and DLLs needed to run the app.
