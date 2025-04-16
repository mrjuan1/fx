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

#include "app.h"

mat4 proj, view, pview;
float dir = 180.0f, tilt = 90.0f, zoom = 10.0f;
float ndir = 180.0f, ntilt = 90.0f, nzoom = 10.0f;

vbo_data area_mod;
unint area_tex = 0;

int nsw = 0, nsh = 0;
unint texs[4], rbs[3];
unint fbos[4];

void init(void) {
  if (init_mblur() && init_vig() && init_quad()) {
    int size = 0;
    void *data = NULL;

    /*glClearColor(1.0f,1.0f,1.0f,1.0f);*/
    glClearColor(0.04f, 0.0f, 0.04f, 1.0f);

    use_mblur();
    send_mblur_samples(16.0f); /* adjust motion blur samples here */

    use_vig();
    send_vig_size((float)sw(), (float)sh());
    vigmode(0);

    use_basic();
    persp(proj, 75.0f, asp(), 0.1f, 24.0f);

    if (fload("data/area-model.bin", &size, &data)) {
      add_to_vbo(&area_mod, data, size);
      free(data);

      update_vbo();

      gen_texs(1, &area_tex);
      // if(load_tex_compressed(area_tex,"data/area-ao-texture.bin",tf_mipmap))
      if (load_tex_compressed(area_tex, "data/area-texture.bin", tf_mipmap)) {
        const int downsample = 1; /* adjust fb down-sizing here */
        texmode(1);

        nsw = sw() / downsample;
        nsh = sh() / downsample;

        gen_texs(4, texs);
        gen_rbs(3, rbs);
        gen_fbs(4, fbos);

        use_fb(fbos[0]);
        add_fb_rb(0, rbs[0], nsw, nsh, GL_RGB8,
                  4); /* adjust renderbuffer samples here */
        add_fb_rb(0, rbs[1], nsw, nsh, GL_DEPTH_COMPONENT24,
                  4); /* adjust renderbuffer samples here */

        use_fb(fbos[1]);
        add_fb_tex(0, texs[0], nsw, nsh, GL_RGB8, tf_linear);
        add_fb_rb(0, rbs[2], nsw, nsh, GL_DEPTH_COMPONENT24, 0);

        use_fb(fbos[2]);
        add_fb_tex(0, texs[1], nsw, nsh, GL_RGB8, tf_linear);
        add_fb_tex(1, texs[2], nsw, nsh, GL_RGBA32F, tf_linear);
        add_fb_rb(0, rbs[2], nsw, nsh, GL_DEPTH_COMPONENT24, 0);

        use_fb(fbos[3]);
        add_fb_tex(0, texs[3], nsw, nsh, GL_RGB8, tf_linear);

        use_fb(0);
        use_rb(0);
        use_tex(0);
      } else
        quit();
    } else
      quit();
  } else
    quit();
}

void loop(void) {
  const float spd1 = 30.0f * dt();
  const float spd2 = 15.0f * dt();

  float r = 0.0f;
  float dc = 0.0f, ds = 0.0f;
  float tc = 0.0f, ts = 0.0f;

  /*viewport(0,0,nsw,nsh);*/
  use_basic();
  send_lpview(pview);

  if (mm() || mw() != 0.0f) {
    if (mm()) {
      dir += mx() * spd1;
      if (dir < -0.0f) {
        dir += 360.0f;
        ndir += 360.0f;
      } else if (dir > 360.0f) {
        dir -= 360.0f;
        ndir -= 360.0f;
      }

      tilt -= my() * spd1;
      if (tilt < 0.001f)
        tilt = 0.001f;
      else if (tilt > 100.0f)
        tilt = 100.0f;
    }

    if (mw() != 0.0f) {
      zoom -= mw();
      if (zoom < 7.0f)
        zoom = 7.0f;
      else if (zoom > 12.0f)
        zoom = 12.0f;
    }
  }
  if (mp(3))
    zoom = 10.0f;

  ndir -= (ndir - dir) * spd2;
  r = d2r(ndir);
  dc = cosf(r);
  ds = sinf(r);

  ntilt -= (ntilt - tilt) * spd2;
  r = d2r(ntilt);
  tc = cosf(r);
  ts = sinf(r);

  nzoom -= (nzoom - zoom) * spd2;

  look(view, v3(ds * ts * nzoom, dc * ts * nzoom, 3.0f + (tc * nzoom)),
       v3(0.0f, 0.0f, 3.0f));
  mult(pview, proj, view);

  send_pview(pview);

  /* ----- */
  use_fb(fbos[0]);
  clear();
  /* ----- */
  use_tex(area_tex);
  /* ----- */
  draw_vbo(&area_mod);
  use_fb(0);

  blit_fb(fbos[0], fbos[1], nsw, nsh);
  /* ----- */

  use_fb(fbos[2]);
  set_drawbufs(2);
  clear();
  draw_vbo(&area_mod);
  use_fb(0);

  use_mblur();
  use_tex(texs[0]); /* ----- */
  active_tex(GL_TEXTURE1, texs[2]);
  default_tex();

  use_fb(fbos[3]);
  set_drawbufs(1);
  clear();
  quad();
  use_fb(0);

  active_tex(GL_TEXTURE1, 0);
  default_tex();

  /*viewport(0,0,sw,sh);*/
  use_vig();
  use_tex(texs[3]);

  clear();
  quad();

  if (kp(SDL_SCANCODE_DELETE) || kp(SDL_SCANCODE_ESCAPE))
    quit();
}

void done(void) {
  del_fbs(4, fbos);
  del_rbs(3, rbs);
  del_texs(4, texs);

  del_texs(1, &area_tex);

  done_quad();
  done_vbo();

  done_vig();
  done_mblur();
  done_basic();
}
