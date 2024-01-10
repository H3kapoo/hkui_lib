#include "../../include/hkui/meshHelpers/MeshBuilder.hpp"

#include <numeric>

namespace meshHelpers
{

MeshBuilder& MeshBuilder::get()
{
    static MeshBuilder instance;
    return instance;
}

/**
 * @brief Generate vao **Id** for a simple quad mesh.
 *
 * Generate vertex data for a quad that's using an index buffer. It has two attributes:
 * position and texture coordinates. If quad vao has been generated before, simply
 * return it's cached value.
 *
 * @return vaoID of generated quad mesh to be bound.
 */
vaoId MeshBuilder::genQuadMesh()
{
    if (gQuadCachedId) { return gQuadCachedId; }

    unsigned int vaoId, vboId, eboId;
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);
    glGenBuffers(1, &eboId);

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);

    /* Buffer data about the vertices to the GPU */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * gQuadVertices.size(), &gQuadVertices[0], GL_STATIC_DRAW);

    /* Buffer data about the indices to the GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * gQuadIndices.size(), &gQuadIndices[0], GL_STATIC_DRAW);

    uint32_t totalVertexLen = std::accumulate(gQuadLayout.begin(), gQuadLayout.end(), 0);
    int32_t currentOffset = 0;
    for (uint32_t i = 0; i < gQuadLayout.size(); i++)
    {
        glVertexAttribPointer(
            i,                                       // attribId
            gQuadLayout[i],                          // attrLenBytes
            GL_FLOAT,                                // attrType
            GL_FALSE,                                // bNorm
            totalVertexLen * sizeof(float),          // vertexLen
            reinterpret_cast<void*>(currentOffset)); // vertexAttrOffset
        glEnableVertexAttribArray(i);
        currentOffset += gQuadLayout[i] * sizeof(float);
    }

    gQuadCachedId = vaoId;
    return vaoId;
}

}