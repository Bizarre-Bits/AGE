//
// Created by a on 24.08.22.
//

#include "Age/Age.h"

class Sandbox : public AGE::Application {
public:
  Sandbox() {
  }

  ~Sandbox() {}
};

// Define app factory for entry point to have something to start.
AGE::Application* AGE::CreateApplication() {
  return new Sandbox{};
}