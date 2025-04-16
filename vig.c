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

#include "vig.h"

byte _vig_init = 0;

unint _vig_p = 0;
struct {
  int size, vigmode;
  int colmap;
} _vig_pv;

byte init_vig(void) {
  if (!_vig_init && init_basic()) {
    _vig_p = glCreateProgram();
    if (load_pro(_vig_p, "sqvs.glsl", "vig.glsl")) {
      use_vig();

      _vig_pv.size = glGetUniformLocation(_vig_p, "size");
      _vig_pv.vigmode = glGetUniformLocation(_vig_p, "vigmode");

      _vig_pv.colmap = glGetUniformLocation(_vig_p, "colmap");

      send_vig_size(0.0f, 0.0f);
      vigmode(0);
      glUniform1i(_vig_pv.colmap, 0);

      _vig_init = 1;
    } else {
      glDeleteProgram(_vig_p);
      info("Program \"vig\" not created\n");
    }
  }

  return _vig_init;
}

void done_vig(void) {
  if (_vig_init) {
    glDeleteProgram(_vig_p);
    _vig_init = 0;
  }
}

void use_vig(void) { glUseProgram(_vig_p); }

void send_vig_size(float x, float y) { glUniform2f(_vig_pv.size, x, y); }

void vigmode(int mode) { glUniform1i(_vig_pv.vigmode, mode); }
