//
// Created by a on 24.08.22.
//

#include "Age.h"

class Sandbox : public AGE::Application {
	public:
	Sandbox() {}

	~Sandbox() {}

	void Run() {
		while(true);
	}
};

AGE::Application *AGE::CreateApplication() {
	return new Sandbox{};
}