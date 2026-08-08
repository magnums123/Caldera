#pragma once

#include <Defines.hpp>
#include <map>

#include "Core/Containers/Vector.hpp"
#include "Core/Memory.hpp"
#include "Core/UUID.hpp"
#include "Renderer/Mesh/Mesh.hpp"
#include "Renderer/Renderer.hpp"
#include "Utility/ModelLoader.hpp"

namespace CAL
{

class AssetManager
{
   public:
    // FIX: Validate Mesh exists instead of returning only.
    Ref<MeshAsset> getMesh(MeshHandle handle)
    {
        if (meshMap.find(handle) != meshMap.end()) return meshMap.find(handle)->second;
        return nullptr;
    }

    MeshHandle loadMesh(std::filesystem::path path, Ref<Renderer>& renderer)
    {
        auto meshData = Utility::loadMesh(path);
        Ref<MeshAsset> asset = CreateRef<MeshAsset>(Memory::MemoryTag::APPLICATION);
        asset->vertexBuffer = renderer->createVertexBuffer(meshData.vertices.data(), meshData.vertices.size());
        asset->indexBuffer = renderer->createIndexBuffer(meshData.indices.data(), meshData.indices.size());

        MeshHandle handle{};
        meshMap[handle] = asset;
        return handle;
    }

    void unloadMesh(MeshHandle handle) { meshMap.erase(handle); }

   private:
    std::map<MeshHandle, Ref<MeshAsset>> meshMap;
};

}  // namespace CAL
