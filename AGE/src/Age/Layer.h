//
// Created by alex on 29.08.22.
//

#ifndef AGE_LAYER_H
#define AGE_LAYER_H

#include "Age/Core.h"
#include "Age/Events/Event.h"

namespace AGE {
	class DLL_PUBLIC Layer {
	public:
		explicit Layer(const age_string_t &debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& e) {}

		inline const age_string_t& Name() const { return debugName_; }

	protected:
		age_string_t debugName_;
	};

} // AGE

#endif //AGE_LAYER_H
