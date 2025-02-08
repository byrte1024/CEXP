# Very work in progress

## File overview

* **textureGrid.c** Handles the creation, manipulation, and drawing of 2D textures representing the game world.  Includes functions for setting/getting pixel colors, updating the texture, and crucially, the flood-fill algorithm for identifying connected regions ("islands").  Also provides functionality for creating sub-grids from islands.
* **ivec2.c** Defines a simple 2D integer vector structure (`IVec2`) and associated functions for basic vector operations (setting, adding, subtracting, multiplying, dividing, scaling).  Includes a `Vector2` cache for interoperability with Raylib's `Vector2` type. this was mainly made so i had a 2d vector that does not have floating point errors
* **gameObject.c** Implements a basic GameObject system.  GameObjects hold position, rotation, scale, and an array of function pointers for event handling (Start, Update, Render, etc.).  Provides a simple data storage mechanism for associating data with GameObjects.
* **main.c** The main entry point of the application.  Sets up the Raylib window, initializes the world grid, and handles the game loop.

## Building

This project uses raylib, and that is pretty much the only library needed.
For set up, simply download the raylib files and extract them into C:/raylib (or edit Makefile to specify your own path)

Make sure your environment is set up like this:

* src

* * the regular project files

* bin

* * Leave this empty, this is where the executable will build

* lib

* * in here place `librarylib.a`
