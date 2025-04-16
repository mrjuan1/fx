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

// The vignette shader
#version 300 es

precision mediump int;
precision mediump float;

// Input framebuffer size
uniform vec2 size;

// Input colour (diffuse) framebuffer texture
uniform sampler2D colmap;

// Texture coords from vertex shader
in vec2 vtc;

// Output colour texture
layout(location = 0) out vec4 oc;

void main(void) {
  // Set the output colour to the input colour
  oc = texture(colmap, vtc);

  // Apply the chromatic aberration effect (one I have come to dislike over the
  // years)
  const float offset = 1.005f;
  float r =
      texture(colmap,
              vec2((((vtc.x * 2.0f) - 1.0f) * offset * 0.5f) + 0.5f, vtc.y))
          .r;
  if (oc.r != r)
    oc.r = r;
  float b =
      texture(colmap,
              vec2(vtc.x, (((vtc.y * 2.0f) - 1.0f) * offset * 0.5f) + 0.5f))
          .b;
  if (oc.b != b)
    oc.b = b;

  // Apply some darkening
  // oc = pow(oc, vec4(1.5f)) * 1.5f;

  // Calculate and apply the vignette effect
  float vig = smoothstep(0.85f, 0.85f - (0.85f / 2.0f), length(vtc - 0.5f));
  oc.rgb *= vig;
}
