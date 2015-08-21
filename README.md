## (ღ˘⌣˘ღ)  **libugly**
_Ultrathin GL Yrapper_

Another object-oriented, multi-context capable wrapper for modern OpenGL (4.x) using modern C++ (14...17).

```Note: This project is still in its early, tumultuous days and is probably not yet suitable for human consumption.```

##### Background

This is essentially spun-out from other OpenGL projects I've been working on. I was getting tired of copying/redoing a lot of the same OpenGL code whenever I wanted to render something in a new project.

The intention is to be compatible with your OS API of choice, whether it's native OS code or a cross-platform library like SDL2 or GLFW. You just use your API to set up your window, then provide **ugly** with the GL context(s).

Likewise, this library should be compatible with whatever you would usually add on top of OpenGL for matrix calculations and other math (e.g. **glm**) or for loading 3D models and textures.


##### Design goals

- _thin:_ the library should not try to hide the way the OpenGL API actually works
- _correct_: the library should manage OpenGL state(s) and memory in a coherent, object-oriented way


_More to come._



