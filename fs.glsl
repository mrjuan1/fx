/* Copyright (c) 2016 Juan Wolfaardt

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
3. This notice may not be removed or altered from any source distribution. */

// Main fragment shader
#version 300 es

precision mediump int;
precision mediump float;

// Input colour
uniform vec4 col;

// Bool for enabling/disabling textures
uniform bool texmode;
uniform sampler2D texmap;

// Texture coords
in vec2 vtc;
// View (camera) position and last view position (used to calculate velocity map
// for motion blur)
in vec4 vpos, vlpos;

// Output to 2 framebuffers, one for colour (diffuse) and the other for velocity
layout(location = 0) out vec4 oc;
layout(location = 1) out vec4 ov;

void main(void) {
  // Set the output colour to the input colour
  oc = col;
  // If we're using a texture, multiply by the sampled texture colour
  if (texmode)
    oc *= texture(texmap, vtc);
  // Discard fully-transparent colours
  if (oc.a == 0.0f)
    discard;

  // Normalize the view positions
  vec2 pos = vpos.xy / vpos.w;
  vec2 lpos = vlpos.xy / vlpos.w;
  // Get the velocity between the view positions and render it to the velocity
  // framebuffer
  ov = vec4(pos - lpos, 0.0f, 1.0f);
}
