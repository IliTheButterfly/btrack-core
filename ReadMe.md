# BTRACK-CORE

This repo contains the core components of the btrack system. This library will allow easy 3D tracking using multiple cameras/webcams.

## Build from source

In order to build from source, you may need to adjust these steps depending on your system. This project is designed for Windows, however, it should be possible to build it on other platforms (this is why CMake is used and not MSBuild).

The following steps apply to windows, but there may be some common ground for other OSs.

### Requirements

List of requirements:
- A CUDA capable GPU
- Windows 10 or above (as mentioned before, this applies to a windows setup)
- Visual Studio 22 or above
- Visual Studio c++ build tools
- CUDA 12.5
- cuDNN 9.2.1
- CMake
- A prebuilt OpenCV 4.10.0 version with CUDA support (or build it from source yourself)

### Installing CUDA and cuDNN

Start by downloading and installing CUDA 12.5 from NVIDIAs website. Then download and install cuDNN 9.2.1. Finally, go to cuDNN root folder and copy the `bin`, `lib` and `include` folders then paste them in `CUDA\v12.5` folder.

### Prebuilt OpenCV versions

You can find prebuilt OpenCV versions [here](https://github.com/IliTheButterfly/opencv-builds/releases).

OpenCV with CUDA support was built using [these steps](https://medium.com/@jinscott/build-opencv-on-windows-with-cuda-f880270eadb0).

