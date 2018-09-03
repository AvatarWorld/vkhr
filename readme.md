Real-Time Hair Renderer
=======================

List of Features
----------------

Compiling and Running
---------------------

1. Place yourself in the root directory of this project.
2. Execute `git submodule update --init --recursive`.
3. Acquire the latest version of the `premake5` build system.
4. Thereafter, execute `premake5 gmake` if building on Make.
    * If you're targeting e.g Visual Studio: `premake5 vs2017`
    * skip the steps below and open the solution in `build`.
5. Finally, issue the command `make -j8 -C build` and wait.
6. When complete, both `bin` and `lib` have been built.
7. Run the Vulkan hair renderer by using `bin/vkhr`.
8. **Shortcuts:** `make run` will both build and run.

Usage and Documents
-------------------

* `bin/vkhr [hair-scene-file]`

System Requirements
-------------------

Must support Vulkan 1.1 or higher.

* **AMD:** tested this on a RX 570,
* **NVIDIA:** also tested a MX 150,
* **Intel:** also tested on a  HD 620.

Dependencies
------------

* Premake 5 (build)
* A Vulkan 1.1 SDK
* GLFW 3.2

Structure
---------

* `bin`: contains the built software and any other accompanying tools.
* `build`: stores intermediate object files and generated GNU Make files.
    * `obj`: has all of the generated object files given under compilation.
    * `Makefile`: automatically generated by executing `premake5 gmake`.
    * `*.make`: program specific make config for augmenting `Makefile`.
* `docs`: any generated documentation for this project is over here.
* `foreign`: external headers and source for libraries and modules.
* `include`: only internal headers from this project should go here.
    * `vkhr`: internal headers for the hair Vulkan renderer project.
* `lib`: any generated libraries from the project reside here.
* `license.md`: please look through this very carefully.
* `premake5.lua`: configuration file for the build system.
* `readme.md`: this file contains information on the project.
* `share`: any extra data that needs to be bundled should go here.
    * `binary`: the blobs that will be used, like the hair style file.
    * `images`: any image on disk that should be used as textures.
    * `models`: the meshes/models/materials to be used in the project.
    * `shader`: all of the uncompiled shader that are to be used here.
    * `scenes`: any sort of scene files (e.g. in json) should go here.
* `src`: all source code for the project should be located below here.
    * `vkhr`: source code for the Vulkan hair renderer project itself.
    * `main.cc`: the primary entry point when generating the binaries.
* `utils`: any sort of helper scripts or similar should be over here.

Reporting Bugs
--------------

There are definitely no known bugs in this software at this time.

Acknowledgements
----------------
