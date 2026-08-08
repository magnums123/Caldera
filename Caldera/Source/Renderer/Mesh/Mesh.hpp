
#pragma once

#include <Defines.hpp>
#include <cstdint>

#include "Core/Containers/Vector.hpp"
#include "Renderer/Mesh/Vertex.hpp"
#include "Renderer/Structures/Buffer.hpp"

namespace CAL
{

class Mesh
{
   public:
    Mesh() {}
    Mesh(Vector<Vertex>& vertices, Vector<uint32_t>& indices) : vertices(vertices), indices(indices) {}

    Vector<Vertex> vertices;
    Vector<uint32_t> indices;
};

struct MeshAsset
{
    Ref<Buffer> vertexBuffer;
    Ref<Buffer> indexBuffer;
};

}  // namespace CAL
