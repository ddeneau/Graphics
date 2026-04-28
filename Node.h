#pragma once
#include <vector>
#include <memory>
#include "Scene.h" // To get the SceneData struct

class Node {
    public:
        void AddChild(std::shared_ptr<Node> child);
        void Update(const DirectX::XMMATRIX& parentTransform);

    private:
        DirectX::XMMATRIX m_localTransform; // Remember, m for member.
        DirectX::XMMATRIX m_globalTransform;
        std::vector<std::shared_ptr<Node>> m_children;
    };