//
// Created by a on 24.08.22.
//

#include "Age/Age.h"

class ExampleLayer : public AGE::Layer {
public:
	ExampleLayer() : AGE::Layer("ExampleLayer") {}

	virtual void OnUpdate() override {
		AGE_INFO("ExampleLayer::Update");
	}

	virtual void OnEvent(AGE::Event &e) override {
		AGE_TRACE(e);
	}
};

class Sandbox : public AGE::Application {
	public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {}
};

// Define app factory for entry point to have something to start.
AGE::Application *AGE::CreateApplication() {
	return new Sandbox{};
}