# TheMazeGame

This is a Randomised Maze Game made for my NEA project. For more information, please read the [report](./docs/Main.pdf).

## Dependencies

* [GLFW](https://github.com/glfw/glfw) - inside dependencies/glfw
* [FreeType](https://gitlab.freedesktop.org/freetype/freetype) - inside dependencies/freetype
* [GLEW](https://github.com/nigels-com/glew) - should already have built binaries for windows and linux inside correct folders (as repository did not have CMake implemented)
* [ImGUI](https://github.com/DeDerpster/imgui/tree/docking) - inside src/vendor/imgui **NOTE**: Should be my fork on the docking branch
* [GLM](https://github.com/g-truc/glm) - inside src/vendor/glm These files should be already setup.
* [stb_image](https://github.com/nothings/stb) - inside src/vendor/stb_image These files are already setup. This is just a subsection of the actual repository

## Known issues

* Issues with AMD graphics cards while swapping buffers
