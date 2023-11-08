#include "Scene/Layer.h"

void Frostbyte::Layer::Render()
{
	for (auto& child : m_Children) {
		child->Render();
	}
	OnUpdate();
}

void Frostbyte::Layer::PushChild(Layer* sceneNode)
{
	m_Children.push_back(sceneNode);
	sceneNode->OnInit();
}

void Frostbyte::Layer::PopChild(Layer* node)
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
