//
// Created by alex on 04.10.22.
//

#include <glm/gtc/matrix_transform.hpp>

#include "Sandbox.h"
#include "Sandbox2DLayer.h"

AGE::Application* AGE::CreateApplication() {
  return new Sandbox;
}

Sandbox::Sandbox() : AGE::Application() {
  PushLayer(new Sandbox2DLayer);
//  PushOverlay(new Sandbox2DUI);
}

Sandbox::~Sandbox() {}

// Sandbox Layer

