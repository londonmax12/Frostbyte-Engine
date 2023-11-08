#include "Scene/Layer.h"

void Frostbyte::Layer::Render()
{
	for (auto& child : m_Children) {
		child->Render();
	}
	OnRender();
}

void Frostbyte::Layer::Push(Layer* sceneNode)
{
	m_Children.push_back(sceneNode);
}

void Frostbyte::Layer::Pop(Layer* node)
{
	for (int i = 0; i < m_Children.size(); i++) {
		if (m_Children[i] == node) {
			m_Children.erase(m_Children.begin() + i);
		}
	}
}

void Frostbyte::Layer::DeleteChildren()
{
	for (auto& child : m_Children) {
		child->DeleteChildren();
		delete child;
	}
}
