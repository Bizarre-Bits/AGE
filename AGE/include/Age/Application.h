//
// Created by a on 24.08.22.
//

#ifndef AGE_APPLICATION_H
#define AGE_APPLICATION_H

#include "Core.h"

namespace AGE {
	class DLL_PUBLIC Application {
		public:
		Application();
		virtual ~Application();

		virtual void Run();
	};

	// To be defined in CLIENT
	Application* CreateApplication();
} // Age

#endif //AGE_APPLICATION_H
