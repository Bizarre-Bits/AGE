//
// Created by alex on 29.08.22.
//

#ifndef AGE_LAYERSTACK_H
#define AGE_LAYERSTACK_H

#include <vector>

#include "Age/Core.h"
#include "Age/Layer.h"

namespace AGE {

	class LayerStack {
	public:
		LayerStack();
		~LayerStack();

		void PushLayer(Layer *layer);
		void PushOverlay(Layer *layer);
		void PopLayer(Layer *layer);
		void PopOverlay(Layer *layer);

		inline std::vector<Layer *>::iterator begin() { return layers_.begin(); }
		inline std::vector<Layer *>::iterator end() { return layers_.end(); }

	private:
		std::vector<Layer *> layers_;
		std::vector<Layer *>::iterator layerInsert_;
	};

} // AGE

#endif //AGE_LAYERSTACK_H
