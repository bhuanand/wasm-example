# Getting started with WebAssembly & Emscripten

Demo project in the course https://www.udemy.com/course/webassembly-emscripten/learn

Install the emsdk to compile the .c file and build the wasm

1. For sering the html and wasm - we are using `lite-server` npm package which starts a http server and also provides auto reload functionality when files changes.

    > Run `npx lite-server` command to start the server

2. To generate the player.js and .wasm files - use following command
    > Run `emcc animation-player.c -s WASM=1 -s EXPORTED_FUNCTIONS=["_main", "_get_circles"] -o player.js`

