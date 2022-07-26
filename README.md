# libfaber
A portable, no-dependency, lightweight library providing small utilities and abstractions that leverage some C++20 features.

## Table of Contents
- [Releases](#releases)
- [Features](#features)
- [Building](#building)
  1. [Docker](#docker)
  2. [Standalone](#standalone)
      - [Requirements](#requirements)
      - [Linux](#linux)
- [Usage](#usage)
  - [Linking With a CMake Project](#linking-with-a-cmake-project)

## <span id="releases">Releases [[^](#table-of-contents)]</span>

No prebuild binaries or packages are available as of yet, you'll have to build the library and link against it yourself. See [Building](#building) and [Usage](#usage) sections for more details.

## <span id="features">Features [[^](#table-of-contents)]</span>

Too lazy to list them off right now, check source code. Only a few filesystem facilities atm.

Feature suggestions welcome.

## <span id="building">Building [[^](#table-of-contents)]</span>

Using the Docker image is the recommended method for setting up the build environment. Standalone, platform-specific instructions are also provided.

### Docker
Coming Soon™

### Standalone
#### Requirements
- [Git](https://git-scm.com/downloads)
- [CMake](https://cmake.org/download/) 3.21+
- Compiler with [C++20 support](https://en.cppreference.com/w/cpp/compiler_support#C.2B.2B20_features) (GCC 12+ or LLVM Clang 14+ recommended)

#### Linux
(You) should be able to build this ^:)

## <span id="usage">Usage [[^](#table-of-contents)]</span>

### Linking With a CMake Project
nothing here yet

# To-do
- [ ] Actually write README.md
- [ ] Dockerize
