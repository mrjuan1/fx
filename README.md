Template
========
A simple template for preparing GLES3-based presentations/experiments. Made for my own personal use and for use at my work place, sharing for educational purposes, etc.

Licensed under zlib license. Not accepting any contributions for now as this is just intended for personal use. Only sharing because sharing is caring :)

See todo.txt for future development notes.

Build
=====
This'll only build on Linux at the moment. Windows support will come later, though it isn't too hard to port to Windows for now.

Dependencies:
```
sudo apt-get install libsdl2-dev libgl1-mesa-dev libgles2-mesa-dev
```
Either that or just install libsdl2-dev, link against libGL and use GLESv3 headers.

After dependencies are installed...
```
cd template
make
```
This will automatically run a simple demo after building.

Files
=====
```
Makefile
```
It's obvious what this is for. Just some extra notes that the window title and size are hard-coded here as defines in the CFLAGS.

Stricter rules for C and some unnecessary optimizations are applied here too. These can be removed with just -std=c89 that could cause some conflicts, but mostly just defines in 3dm.h.

```
main
```
Sets up the SDL2 window and GL context and handles some input. Some context values are hard-coded here too, might work on a config system in the future.

```
gl
```
Provides some "GL shortcuts" for us lazy people who don't feel like typing out long functions or lines of code to achieve something simple. Such shortcuts include shader loading, texture loading, texture filtering and framebuffer-related tasks.

```
3dm
```
All the vectorize matrix and vector transform functions. Probably not the best out there, but I get good performance with these (especially on arm-based devices)

```
app
```
More or less where the user's custom code will come in. There are three functions here: init, loop, and done. It's kind of obvious what these are for.

```
basic and *.glsl
```
A simple shader-loader that prepares vs.glsl and fs.glsl for application use. Also includes lazy shortcut functions.

A note on texmode in fs.glsl: this is just a placeholder for more special effects. I tend to cram all shader experiments into one program for simple examples.

```
vbo
```
Simple (and lazy) vbo manipulating functions. Simple examples only have one vbo that all model data goes to.

```
quad
```
Sets up a triangle fan quad. Nice for quick fragment shader tests

