FX
====
A small post-processing fx demo

Effects include object-based motion blur, fake chromatics, vignette and contrast adjustments.

The main colour buffer used in the shaders is blitted from a framebuffer with multisampled renderbuffers attached, as to keep GLES 3.0 compatibility. Samples for this and for the motion blur can be adjusted in app.c

Same license applies to all the files here as in the template repo.

Future
======
Would like to implement blooming, but might need to separate bright objects and render them on their own with a blurring shader of some sort, then overlay the result over the screen. Not sure how this'll effect the other fx, will need to do a little planning here...

Other than that, some fog effects and hazy lights (part of bloom?) will be implemented here too.

Only concern is performance with so many special effects overlaying each other. Will probably implement toggles for each effect later too.

Build
=====
See template repo for more details and build instructions

