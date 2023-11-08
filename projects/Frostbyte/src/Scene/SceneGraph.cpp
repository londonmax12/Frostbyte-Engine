#include "Scene/SceneGraph.h"

bool Frostbyte::SceneGraph::Init()
{
    if (m_RootNode)
        return false;

    m_RootNode = new Layer();
    return true;
}

void Frostbyte::SceneGraph::Shutdown()
{
    if (m_RootNode) {
        m_RootNode->DeleteChildren();
        delete m_RootNode;
    }
}

void Frostbyte::SceneGraph::Render()
{
    if (m_RootNode)
        m_RootNode->Render();
}
Frostbyte::Layer* Frostbyte::SceneGraph::GetRoot()
{
    return m_RootNode;
}

