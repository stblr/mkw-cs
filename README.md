# mkw-cs

This project is abandoned in favor of [stblr/mkw-sp](https://github.com/stblr/mkw-sp).

Client-server netcode for Mario Kart Wii, currently in early stages of development.

## Overview

The project is in 4 parts:

- A new protocol suited for client-server and compatibility with custom content.

- A client patchset that implements its part of the protocol, lag compensation, as well as a fully
  custom multiplayer UI.

- A server patchset that also implements its part of the protocol and uses a tailored main loop with
  a CLI-only option.

- A dispatcher to handle multiple rooms from a single endpoint.

Both the client and the server patchsets are compatible with Wii, vWii and Dolphin.

## Roadmap

- [ ] Pre-alpha
  - [ ] Protocol
    - [ ] Design it
  - [ ] Client
    - [ ] Basic connection UI without any option
    - [ ] Socket code
    - [ ] Protocol reader/writer
    - [ ] Connect everything together
  - [ ] Server
    - [ ] Main loop
    - [ ] Socket code
    - [ ] Protocol reader/writer
    - [ ] Connect everything together

# Building

1. Download and build the CodeWarrior-compatible LLVM.

   ```bash
   mkdir tools

   cd tools

   git clone git@github.com:DotKuribo/llvm-project.git

   cd llvm-project

   mkdir build

   cd build

   cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DLLVM_TARGETS_TO_BUILD=PowerPC -DLLVM_ENABLE_PROJECTS=clang ../llvm

   cmake --build .
   ```

2. Build the loader.

   ```bash
   cd ../../loader
   make
   ```
