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

    /**
     * @brief Sets the projection matrix that will be used on next draw call
     *
     * @param projMatrix Projection Matrix to be used.
     */
    void setProjectionMatrix(const glm::mat4& projMatrix);

    /**
     * @brief Render node of type **meshHelpers::RectNodeABC** .
     *
     * Quickly render **meshHelpers::RectNodeABC** type. This enables it's
     * stored shader to be the active one while also uploads uniforms such as
     * projection/model matrix and scale of the object to the shader.
     *
     * @param node - Rectangle node to be rendered.
     */
    void renderRectNode(treeHelpers::RectNodeABC& node);

    /**
     * @brief Clear currently bound window depth and color bit.
     *
     */
    void clearScreen();

private:
    RenderHelper();
    RenderHelper(const RenderHelper&) = delete;
    RenderHelper& operator=(const RenderHelper&) = delete;

    shaderHelpers::ShaderHelper& gShInstance; /* Justified use of ref& */

    glm::mat4 gProjectionMat;
};
}