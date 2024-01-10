#pragma once

#include <cstdint>
#include <glm/glm.hpp>

namespace meshHelpers
{
/**
 * @brief Simple class retaining details about the style of the mesh
 *
 */
struct MeshStyle
{
    glm::vec4 gBorderColor{ 0.0f, 0.0f, 0.0f, 1.0f };
    glm::vec4 gBorderSize{ 0.0f }; /* Top Bot Left Right order */
    uint32_t gTextureId{ 0 };
};
}