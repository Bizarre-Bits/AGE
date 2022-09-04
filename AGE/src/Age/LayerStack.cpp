//
// Created by alex on 29.08.22.
//
#include "agepch.h"

#include "LayerStack.h"

namespace AGE {
	LayerStack::LayerStack() {
		layerInsert_ = layers_.begin();
	}
	LayerStack::~LayerStack() {
		for(Layer* l : layers_) {
			delete l;
		}
	}
	void LayerStack::PushLayer(Layer *layer) {
		layerInsert_ = layers_.emplace(layerInsert_, layer);
	}
	void LayerStack::PushOverlay(Layer *layer) {
		layers_.emplace_back(layer);
	}
	void LayerStack::PopLayer(Layer *layer) {
		auto it = std::find(layers_.begin(), layers_.end(), layer);
		if(it != layers_.end()) {
			layers_.erase(it);
			layerInsert_--;
		}
	}
	void LayerStack::PopOverlay(Layer *layer) {
		auto it = std::find(layers_.begin(), layers_.end(), layer);
		if(it != layers_.end())
			layers_.erase(it);
	}
} // AGE