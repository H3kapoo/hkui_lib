#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../../hkui/vendor/GL/Glew.hpp"

#include "Types.hpp"

namespace shaderHelpers
{

/* Enable or disable shader prints regarding uniforms not found */
#define UNIFORMS_DEBUG_PRINT 1 /* 0 - disabled; 1 - hard exit; 2 - soft continue */

/**
 * @brief Singleton providing shader load/reload and uniform access capabilities.
 *
 * Singleton class providing easy means to load/reload shaders and access to their uniforms.
 * Class also provides caching capabilities for shaders.
 */
class ShaderHelper
{
public:
    static ShaderHelper& get();

    /* Setters */
    shaderIdPtr loadFromPath(const std::string& vertPath, const std::string& fragPath);
    void reloadFromPath(const std::string& vertPath, const std::string& fragPath);
    void setActiveShaderId(const shaderId id);
    void resetBoundShader() const;

    /* Uniforms */
    void setInt(const char* location, int value);
    void setIntVec(const char* location, uint32_t amount, int* flatValues);
    void setVec1f(const char* location, float value);
    void setVec2f(const char* location, glm::vec2 value);
    void setVec3f(const char* location, glm::vec3 value);
    void setVec4f(const char* location, glm::vec4 value);
    void setMatrix4(const char* location, const glm::mat4 value);
    void setMatrix4Vec(const char* location, uint32_t amount, const float* flatValues);

    /* Getters */
    shaderId getActiveShaderId() const;

private:
    ShaderHelper() = default;
    ~ShaderHelper();

    /* Private copy and assignment ctor to prevent duplicates */
    ShaderHelper(const ShaderHelper&) = delete;
    ShaderHelper& operator=(const ShaderHelper&) = delete;

    void handleNotFound(const char* location);
    int linkShaders(int vertShaderId, int fragShaderId);
    int compileShader(const std::string& sourcePath, int32_t shaderType);

    /* In case a lot of objects use the same shader, it's pointless to allocate memory for each SAME shader.
       We can reference the same one, just changing uniforms as needed. */
    std::map<std::string, shaderIdPtr> gShaderPathToGenId;

    shaderId gActiveShaderId{ 0 };
};
}