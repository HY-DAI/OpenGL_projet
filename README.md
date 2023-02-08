# OpenGL_projet

The project base uses a series of [tutorials](https://igm.univ-mlv.fr/~biri/Enseignement/IMAC2/opengl-TPs/opengl.php?section=teaching&teaching=opengl&teaching_section=tds) given to make the project. 

## Description

The main objective of this project is to create a graphic application whose theme is the rollercoaster.

You play as a bunny on a rollcoaster in a modern art museum made up of paintings and sculptures.
The initial goal of the bunny was to conquer the world by being a master of culture and the arts, 
but the lack of time resigned him to being only a visitor to the museum.
As a curious bunny, or as a camera, you can wander around the museum in various ways.

The 3D models were made on Blender or are openSources.

## Installations

You have git cloned the project and want to play with it?

### Prerequisites

- cpp compiler
- install [CMake](https://cmake.org/download/).

### Installations on WSL / Ubuntu

Be on your ubuntu machine, or activate WSL on a terminal that you will use later.

- Create a build directory in the GLImac-Template directory,
- Go to the build directory from a terminal,
- Run the following commands:
```bash
  cmake ../
  make
  ./ProjectBase/ProjectBase_main
```

The first command should detect which cpp compiler you have installed and generate an appropriate Makefile.

(!) If a GLSL version error occurs, such as:

> error: GLSL 3.30 is not supported. Supported versions are: 1.10, 1.20, 1.30, 1.40, 1.00 ES, and 3.00 ES

Enter the following command use the required version:
```bash
export MESA_GL_VERSION_OVERRIDE=3.3
```

Rerun the project that should fully work.

## Usage

- Being on the rollcoaster while being Bunny-chan: type B
- Move freely in Freefly mode (Q, Z, S, D + Mouse): type H
- Day/Night mode: type J/N
- Accelerate/Decelerate in the rollcoaster: type V/G

The available speeds are limited so as not to spoil the user experience.
