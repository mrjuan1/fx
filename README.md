FX
====
A small post-processing fx demo

Effects include object-based motion blur, fake chromatics, vignette and contrast adjustments.

The main colour buffer used in the shaders is blitted from a framebuffer with multisampled renderbuffers attached, as to keep GLES 3.0 compatibility. Samples for this and for the motion blur can be adjusted in app.c

![alt text](https://github.com/mrjuan1/fx/raw/master/logo.png "Screenshot")

Same license applies to all the files here as in the template repo (included below for reference).

Controls
========
Move mouse to rotate model and mouse wheel to zoom. Middle-mouse click resets the zoom-level.

Future
======
Would like to implement blooming, but might need to separate bright objects and render them on their own with a blurring shader of some sort, then overlay the result over the screen. Not sure how this'll effect the other fx, will need to do a little planning here...

Other than that, some fog effects and hazy lights (part of bloom?) will be implemented here too.

Only concern is performance with so many special effects overlaying each other. Will probably implement toggles for each effect later too.

Simple breakdown in todo.txt

Build
=====
See template repo for more details and build instructions. This demo also only runs on Linux for now. Might include Windows/Android support later.

License
=======
Copyright (c) 2016 youka

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
   claim that you wrote the original software. If you use this software
   in a product, an acknowledgement in the product documentation would be
   appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.

