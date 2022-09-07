//
// Created by alex on 07.09.22.
//

#ifndef AGE_IMGUIBUILD_H
#define AGE_IMGUIBUILD_H

#include <imgui.h>

#ifdef AGE_RENDER_PLATFORM_OPENGL
  #define IMGUI_IMPL_OPENGL_LOADER_GLAD
  #include <backends/imgui_impl_glfw.h>
  #include <backends/imgui_impl_opengl3.h>
#endif

#endif//AGE_IMGUIBUILD_H
