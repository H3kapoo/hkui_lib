#include "../../include/hkui/meshHelpers/RectMesh.hpp"

namespace meshHelpers
{
RectMesh::RectMesh(const std::string& vertCorP, const std::string& fragCorP, const bool immediate)
    : gMeshBuilder(MeshBuilder::get())
    , gShInstance(shaderHelpers::ShaderHelper::get())
{
    gVaoId = gMeshBuilder.genQuadMesh();

    /* Load shader */
    if (immediate)
    {
        gShaderIdPtr = gShInstance.loadFromImmediate("BASE_SHADER", vertCorP, fragCorP);
        if (*gShaderIdPtr == -1)
        {
            fprintf(stderr, "Failed to load RectMesh using: BASE_SHADER shader\n");
            *gShaderIdPtr = 0;
        }
    }
    else
    {
        gShaderIdPtr = gShInstance.loadFromPath(vertCorP, fragCorP);
        if (*gShaderIdPtr == -1)
        {
            fprintf(stderr, "Failed to load RectMesh using: %s and %s\n", vertCorP.c_str(), fragCorP.c_str());
            *gShaderIdPtr = 0;
        }
    }
}

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