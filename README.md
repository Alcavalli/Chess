# Chess

Chess game in C++ (core engine) + HTML/CSS/JS (frontend), compiled to WebAssembly.


## Architecture

- `core/` — C++ chess engine (rules, AI, move generation)
- `web/` — HTML/CSS/JS frontend
- `wasm/` — Emscripten build output (generated, not versioned)
- `assets/` — pieces graphics and sounds


## Build (native, for development)

```bash
mkdir build
cd build
cmake .. -G Ninja
cmake --build .
```


## Dependencies

- CMake 3.16+
- GCC via MSYS2 (native build)
- Emscripten (WASM build)
- Node.js (local dev server)