//
// Created by a on 24.08.22.
//

#include "Age/Age.h"

#include <chrono>

class Sandbox : public AGE::Application {
	public:
	Sandbox() {}

	~Sandbox() {}

	void Run() {
		auto DoWork = []() -> void {
			using namespace std::chrono_literals;
			for(int i{1}; i <= 10; i++) {
				CLIENT_LOG_INFO("Working... {}", i);
				std::this_thread::sleep_for(1s);
			}
		};

		// Make it threaded just for fun. Simulating work here. Just to check if everything okay.
		std::thread thread{DoWork};
		thread.join();
		CLIENT_LOG_DEBUG("App ended execution");
	}
};

// Define app factory for entry point to have something to start.
AGE::Application *AGE::CreateApplication() {
	return new Sandbox{};
}