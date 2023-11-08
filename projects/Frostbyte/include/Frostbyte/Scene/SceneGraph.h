#pragma once

#include "Scene/Layer.h"

namespace Frostbyte {
	class SceneGraph {
	public:
		bool Init();
		void Shutdown();
		void Render();
		Layer* GetRoot();

	private:
		Layer* m_RootNode = nullptr;
	};
}