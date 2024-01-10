#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../../hkui/shaderHelpers/ShaderHelper.hpp"
#include "../../hkui/meshHelpers/RectMesh.hpp"
#include "../../hkui/meshHelpers/MeshStyle.hpp"
#include "../treeHelpers/RectNodeABC.hpp"

namespace renderHelpers
{

/**
 * @brief Class for rendering basic meshes.
 *
 */
class RenderHelper
{
public:
    static RenderHelper& get();
    static constexpr float MAX_LAYERS{ -100 };

    void setProjectionMatrix(const glm::mat4& projMatrix);
    void renderRectNode(treeHelpers::RectNodeABC& node);
    void clearScreen();

private:
    RenderHelper();
    RenderHelper(const RenderHelper&) = delete;
    RenderHelper& operator=(const RenderHelper&) = delete;

    shaderHelpers::ShaderHelper& gShInstance; /* Justified use of ref& */

    glm::mat4 gProjectionMat;
};
}