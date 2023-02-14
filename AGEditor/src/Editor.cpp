//
// Created by alex on 2/5/23.
//

#include "Editor.h"

namespace AGE {
  Editor::Editor(const ApplicationSpecs& specs): Application(specs) {
    PushLayer(new EditorLayer);
  }
} // AGE
