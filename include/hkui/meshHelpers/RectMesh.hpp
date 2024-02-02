#pragma once

#include <cstdint>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../shaderHelpers/ShaderHelper.hpp"
#include "../shaderHelpers/UniformKeeper.hpp"
#include "../shaderHelpers/Types.hpp"
#include "MeshBuilder.hpp"
#include "Types.hpp"

namespace meshHelpers
{

/**
 * @brief Basic rectangular mesh class encapsulation
 *
 * Class representing a rectangular mesh who's shader can be set and transforms changed.
 * Contains subclass BoxModel representing translation/rotation/scale of the rectangular mesh.
 */
class RectMesh
{
public:
    struct BoxModel
    {
        glm::vec3 pos{ 0, 0, 1 };
        glm::vec3 scale{ 10, 10, 1 };
        glm::vec3 rot{ 0 };
    };

    RectMesh(const std::string& vertPath, const std::string& fragPath, const bool immediate);
    RectMesh(const RectMesh&) = delete; /* Can't copy due to reference */
    RectMesh& operator=(const RectMesh&) = delete; /* Can't copy due to reference */

    /**
     * @brief Get model matrix of object.
     *
     * Compute and return the model matrix of this object. Scale and Translation is relative to top left corner
     * and Rotation is relative to the center of the rectangle.
     *
     * @note Off by one pixel render defects might occur due to rotation being applied from the center instead
     *       or from the top left corner.
     *
     * @return Mat4x4 representing OpenGL compliant model matrix.
     */
    glm::mat4 getTransform();

    inline shaderHelpers::shaderId getShaderId() const;
    inline shaderHelpers::shaderIdPtr getShaderIdPtr() const;
    inline vaoId getVaoId() const;

private:
    MeshBuilder& gMeshBuilder;
    shaderHelpers::shaderIdPtr gShaderIdPtr{ nullptr };
    shaderHelpers::ShaderHelper& gShInstance; /* Justified use of ref& */

    vaoId gVaoId{ 0 };

public:
    shaderHelpers::UniformKeeper gUniKeeper;
    glm::vec4 gColor{ 0 };
    BoxModel gBox;
};

/**
 * @brief Get the object's shader **Id**.
 *
 * Get object's shader **Id** and, if not set, return 0 denoting no shader has been
 * computed for this object.
 *
 * @note Note that this is a pure value returned, not a reference type. If the shader of the
 *       object is reloaded, consumer needs to call getShaderId again to get the after-reload
 *       new **Id**.
 * @return Shader **Id** of the current object.
 */
inline shaderHelpers::shaderId  RectMesh::getShaderId() const
{
    return gShaderIdPtr == nullptr ? 0 : *gShaderIdPtr;
}


/**
 * @brief Get the object's shader **Id** pointer.
 *
 * Get object's shader **Id** pointer. It may be nullptr (very unlikely) if not initialized or -1
 * if shader loading failed.
 *
 * @note If shader of object tends to frequently reload, it's better to use this function to get the
 *       **Id** instead of **getShaderId**.
 * @return Shader **Id** pointer of the current object.
 */
inline shaderHelpers::shaderIdPtr  RectMesh::getShaderIdPtr() const
{
    return gShaderIdPtr;
}


/**
 * @brief Get the object's vao **Id** to be bound.
 *
 * @return Vao **Id** of object.
 */
inline vaoId RectMesh::getVaoId() const
{
    return gVaoId;
}

}