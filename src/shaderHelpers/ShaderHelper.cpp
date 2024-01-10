#include "../../include/hkui/shaderHelpers/ShaderHelper.hpp"

namespace shaderHelpers
{

/**
 * @brief Get class instance.
 *
 * @return A reference to the class instance.
 */
ShaderHelper& ShaderHelper::get()
{
    static ShaderHelper instance;
    return instance;
}


/**
 * @brief Deallocate cached shader pointers at end of life.
 *
 */
ShaderHelper::~ShaderHelper()
{
    for (auto& [key, value] : gShaderPathToGenId)
    {
        /* Free shaderIdPtr memory */
        delete value;
    }
}


/**
 * @brief Load and retrieve a shader **Id** pointer handle from the specified vertex and fragment shader paths.
 *
 * This function checks if a shader with the specified vertex and fragment shader already
 * exists in the shader cache. If it does, it returns a pointer to its **Id**. Otherwise, it compiles
 * and links the shaders, stores the **Id** in the cache, and returns a pointer to the new **Id**.
 *
 * @note Returned dereferenced pointer value will be -1 in case any errors occured. It will never be **nullptr**.
 * @note Reason for pointer return instead of plain integer is to have easier shader reloading capabilities.
 *
 * @param vertPath The file path to the vertex shader source code.
 * @param fragPath The file path to the fragment shader source code.
 *
 * @return A pointer to the shader program **Id**.
 */
shaderIdPtr ShaderHelper::loadFromPath(const std::string& vertPath, const std::string& fragPath)
{
    const std::string combinedPath{ vertPath + fragPath };
    shaderIdPtr idPtr = gShaderPathToGenId[combinedPath];
    if (idPtr != nullptr) { return idPtr; }

    shaderIdPtr newId = new shaderId;
    *newId = linkShaders(
        compileShader(vertPath, GL_VERTEX_SHADER),
        compileShader(fragPath, GL_FRAGMENT_SHADER));

    gShaderPathToGenId[combinedPath] = newId;
    printf("Loaded shader {%d}: %s + %s\n", *newId, vertPath.c_str(), fragPath.c_str());

    return newId;
}


/**
 * @brief Reload a shader from the specified vertex and fragment shader paths.
 *
 * This function creates a new shader **Id** from the provided combined paths and overrides
 * the cached value at combined paths location with the newly created shader **Id** if successfull,
 * otherwise it keeps previous shader **Id** value.
 *
 * @param vertPath The file path to the vertex shader source code.
 * @param fragPath The file path to the fragment shader source code.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::reloadFromPath(const std::string& vertPath, const std::string& fragPath)
{
    shaderId newId = linkShaders(
        compileShader(vertPath, GL_VERTEX_SHADER),
        compileShader(fragPath, GL_FRAGMENT_SHADER));
    if (newId != -1)
    {
        const std::string combinedPath{ vertPath + fragPath };
        assert(gShaderPathToGenId[combinedPath] != nullptr && "Tried to reload shader that wasn't loaded before!");
        *gShaderPathToGenId[combinedPath] = newId;
        return;
    }
}


/**
 * @brief Load integer uniform to currently active shader **Id**.
 *
 * @param location Location of uniform to be set in the shader.
 * @param value    Value to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setInt(const char* location, int value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1i(loc, value);
}


/**
 * @brief Load integer vector uniform to currently active shader **Id**.
 *
 * @param location     Location of uniform to be set in the shader.
 * @param amount       Amount of values to be set.
 * @param flatValues   Array of values to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setIntVec(const char* location, uint32_t amount, int* flatValues)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1iv(loc, amount, flatValues);
}


/**
 * @brief Load float uniform to currently active shader **Id**.
 *
 * @param location Location of uniform to be set in the shader.
 * @param value    Value to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setVec1f(const char* location, float value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1f(loc, value);
}


/**
 * @brief Load 2D-float uniform to currently active shader **Id**.
 *
 * @param location Location of uniform to be set in the shader.
 * @param value    2D-Value to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setVec2f(const char* location, glm::vec2 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform2f(loc, value.x, value.y);
}


/**
 * @brief Load 3D-float uniform to currently active shader **Id**.
 *
 * @param location Location of uniform to be set in the shader.
 * @param value    3D-Value to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setVec3f(const char* location, glm::vec3 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform3f(loc, value.x, value.y, value.z);
}


/**
 * @brief Load 4D-float uniform to currently active shader **Id**.
 *
 * @param location Location of uniform to be set in the shader.
 * @param value    4D-Value to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setVec4f(const char* location, glm::vec4 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform4f(loc, value.r, value.g, value.b, value.a);
}


/**
 * @brief Load 4x4 Matrix uniform to currently active shader **Id**.
 *
 * @param location Location of uniform to be set in the shader.
 * @param value    Matrix value to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setMatrix4(const char* location, const glm::mat4 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}


/**
 * @brief Load 4x4 Matrix vector uniform to currently active shader **Id**.
 *
 * @param location   Location of uniform to be set in the shader.
 * @param amount     Amount of matrices to be set in the shader.
 * @param flatValues Vector of matrices values to be set.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setMatrix4Vec(const char* location, uint32_t amount, const float* flatValues)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniformMatrix4fv(loc, amount, GL_FALSE, flatValues);
}


/**
 * @brief Set currently active shader **Id**.
 *
 * @param id   Shader **Id** to be bound.
 *
 * @note All functions from this class will only affect the currenly active shader.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::setActiveShaderId(const shaderId id)
{
    gActiveShaderId = id;
    glUseProgram(gActiveShaderId);
}


/**
 * @brief Reset currently active shader **Id**.
 *
 * @return Doesn't return anything.
 */
void ShaderHelper::resetBoundShader() const { glUseProgram(0); }


/**
 * @brief Get currently active shader
 *
 * @return Returns currently active shader **Id**.
 */
shaderId ShaderHelper::getActiveShaderId() const { return gActiveShaderId; }


/**
 * @brief Link shader stages.
 *
 * @return -1 on Failure or >0 value on success.
 */
int ShaderHelper::linkShaders(int vertShaderId, int fragShaderId)
{
    if (!vertShaderId || !fragShaderId)
        return -1;

    gActiveShaderId = glCreateProgram();

    glAttachShader(gActiveShaderId, vertShaderId);
    glAttachShader(gActiveShaderId, fragShaderId);
    glLinkProgram(gActiveShaderId);

    int success;
    char infoLog[512];
    glGetProgramiv(gActiveShaderId, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(gActiveShaderId, 512, nullptr, infoLog);
        fprintf(stderr, "Could not link program because:\n\t%s\n", infoLog);
        exit(1);
        return -1;
    }

    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);

    return gActiveShaderId;
}


/**
 * @brief Compile shader source.
 *
 * @return -1 on Failure or >0 value on success.
 */
int ShaderHelper::compileShader(const std::string& sourcePath, int32_t shaderType)
{
    std::string type = shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAG";
    std::ifstream shaderFile(sourcePath);
    if (!shaderFile)
    {
        fprintf(stderr, "Could not open %s shader file at %s\n", type.c_str(), sourcePath.c_str());
        exit(1);
        return -1;
    }

    std::stringstream stream;
    stream << shaderFile.rdbuf();
    std::string content = stream.str();
    const char* src = content.c_str();
    shaderFile.close();

    uint32_t shaderPart = glCreateShader(shaderType);
    glShaderSource(shaderPart, 1, &src, NULL);
    glCompileShader(shaderPart);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderPart, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderPart, 512, NULL, infoLog);
        fprintf(stderr, "Compile failed for shader %s because:\n\t%s\n", type.c_str(), infoLog);
        exit(1);
        return -1;
    }

    return shaderPart;
}

/**
 * @brief Simply print uniform not found
 *
 * @return Nothing.
 */
#if UNIFORMS_DEBUG_PRINT == 1
void ShaderHelper::handleNotFound(const char* location)
{
    fprintf(stderr, "Uniform '%s' has not been found in bound shader: {%d}\n", location, gActiveShaderId);
    exit(1);
}
#elif UNIFORMS_DEBUG_PRINT == 2
void ShaderHelper::handleNotFound(const char* location)
{
    fprintf(stderr, "Uniform '%s' has not been found in bound shader: {%d}\n", location, gActiveShaderId);
}
#else
void ShaderHelper::handleNotFound(const char* location) {}
#endif
}