# BuildPad
A build storage plugin for ArcDPS

---

This isn't the primary repo I'm working on, I will only be commiting here when releasing new versions.

## Build
At least VS2017 is required to compile.

All the necessary libraries should be included, but if you need to recompile them - I cannot help you, good luck figuring that out on your own. Try vcpkg, I dunno.

Build in **Debug** configuration to run and debug it as a stand-alone application.

Build in **DLL** configuration to create a .dll file.

## Folder Structure
* **arcdps_buildpad/** - project folder
  * **buildpad/** - contains actual BuildPad code
    * **resources/** - contains resources converted from binary files to byte arrays
      * **sources/** - contains the originals of all the resource files and a batch file to mass convert them
  * **Main.cpp** - contains DLL code and ArcDPS bindings
  * **main_imgui.cpp** - contains code for stand-alone debugging
