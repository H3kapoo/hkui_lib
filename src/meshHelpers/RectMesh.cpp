#include "../../include/hkui/meshHelpers/RectMesh.hpp"

namespace meshHelpers
{
RectMesh::RectMesh(const std::string& vertPath, const std::string& fragPath)
    : gMeshBuilder(MeshBuilder::get())
    , gShInstance(shaderHelpers::ShaderHelper::get())
{
    gVaoId = gMeshBuilder.genQuadMesh();

    /* Load shader */
    gShaderIdPtr = gShInstance.loadFromPath(vertPath, fragPath);
    if (*gShaderIdPtr == -1)
    {
        fprintf(stderr, "Failed to load RectMesh using: %s and %s\n", vertPath.c_str(), fragPath.c_str());
        *gShaderIdPtr = 0;
    }
}

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

glm::mat4 RectMesh::getTransform()
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, gBox.pos);
    modelMat = glm::translate(modelMat, glm::vec3(gBox.scale.x / 2, gBox.scale.y / 2, 0));
    modelMat = glm::rotate(modelMat, glm::radians(gBox.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMat = glm::translate(modelMat, glm::vec3(-gBox.scale.x / 2, -gBox.scale.y / 2, 0));
    modelMat = glm::scale(modelMat, gBox.scale);
    return modelMat;
}

}