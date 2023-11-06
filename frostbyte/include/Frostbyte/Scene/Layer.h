#pragma once

#include <vector>

namespace Frostbyte {
	class Layer {
	public:
		void Render();
		void PushChild(Layer* node);
		void PopChild(Layer* node);
		void DeleteChildren();

	private:
		virtual void OnInit() {};
		virtual void OnUpdate() {};

		std::vector<Layer*> m_Children;
	};
}