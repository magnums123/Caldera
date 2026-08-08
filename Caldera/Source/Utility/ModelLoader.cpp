#include "ModelLoader.hpp"

#include <cstdint>

#include "Core/Containers/Vector.hpp"
#include "Core/Logger.hpp"
#include "Renderer/Mesh/Vertex.hpp"

namespace CAL::Utility
{

Mesh loadMesh(std::filesystem::path path)
{
    Vector<Vertex> vertices{};
    Vector<uint32_t> indices{};

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    tinyobj::LoadObj(
        &attrib, &shapes, &materials, &warn, &err, path.string().c_str(), path.parent_path().string().c_str());

    if (err != "") LOG_ERROR("Error Loading Mesh: {}", err);
    if (warn != "") LOG_ERROR("Warning Loading Mesh: {}", warn);

    for (auto& index : shapes[0].mesh.indices)
    {
        Vertex v{ .Position = { attrib.vertices[index.vertex_index * 3], -attrib.vertices[index.vertex_index * 3 + 1],
                                attrib.vertices[index.vertex_index * 3 + 2] },
                  .Normal = { attrib.normals[index.normal_index * 3], -attrib.normals[index.normal_index * 3 + 1],
                              attrib.normals[index.normal_index * 3 + 2] },
                  .UV = { attrib.texcoords[index.texcoord_index * 2],
                          1.f - attrib.texcoords[index.texcoord_index * 2 + 1] } };
        vertices.pushBack(v);
        indices.pushBack(indices.size());
    }

    return { vertices, indices };
}

}  // namespace CAL::Utility
