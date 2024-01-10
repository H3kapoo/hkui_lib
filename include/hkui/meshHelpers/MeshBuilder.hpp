#pragma once

#include <cstdint>
#include <vector>

#include "../../hkui/vendor/GL/Glew.hpp"

#include "Types.hpp"

namespace meshHelpers
{

/**
 * @brief Singleton class for generating vertex buffer objects
 *
 * Simple singleton class for generating boundable vao objects.
 */
class MeshBuilder
{
public:
    static MeshBuilder& get();

    vaoId genQuadMesh();

private:
    MeshBuilder() = default;
    MeshBuilder(const MeshBuilder&) = delete;
    MeshBuilder& operator=(const MeshBuilder&) = delete;

    /* Quad mesh data */
    vaoId gQuadCachedId{ 0 };
    std::vector<float> gQuadVertices = {
        // POS 3F         TEX 2F
        1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f
    };

    std::vector<uint32_t> gQuadIndices = {
        0, 1, 3,
        1, 2, 3
    };

    std::vector<uint32_t> gQuadLayout = { 3, 2 };
    /* End */
};
}