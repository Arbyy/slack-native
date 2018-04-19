# slack-native

## Development Environment Setup Guide

### Linux (Ubuntu for now)
Run: 

```sh
sudo apt install cmake3 libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libwebsockets-dev libavcodec-dev libavformat-dev libcurl4-openssl-dev
git clone https://github.com/Arbyy/slack-native.git
cd slack-native
mkdir build
cd build
cmake ..
make
```

And thats it, it should just run.

### Mac OS
Download SDL2, SDL2_image and SDL2_ttf Mac OS development binaries and stick them in either `/Library/Frameworks` or `~/Library/Frameworks`

`brew install libwebsockets ffmpeg`

Run the CMake GUI tool, and after clicking configure, point `websockets_lib`, `avcodec_lib` and `avformat_lib` to their respective `.dylib` files in the brew cellar.

Open a terminal in the `build/` folder and run `make`.
