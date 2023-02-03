//
// Created by alex on 2/2/23.
//

#include "Editor.h"
#include "Age/EntryPoint.h"

namespace AGEd {

} // AGEd

AGE::Application* AGE::CreateApplication() {
  return new AGEd::Editor{};
}
