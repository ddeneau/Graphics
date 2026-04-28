#include "Node.h"

void Node::AddChild(std::shared_ptr<Node> child) {
    m_children.push_back(child);
}

void Node::Update(const DirectX::XMMATRIX& parentTransform) {
    m_globalTransform = m_localTransform * parentTransform;

    for (auto& child : m_children) {
        child->Update(m_globalTransform);
    }
}