#include "../../include/hkui/shaderHelpers/ShaderHelper.hpp"

namespace shaderHelpers
{

ShaderHelper& ShaderHelper::get()
{
    static ShaderHelper instance;
    return instance;
}

ShaderHelper::~ShaderHelper()
{
    for (auto& [key, value] : gShaderPathToGenId)
    {
        /* Free shaderIdPtr memory */
        delete value;
    }
}

shaderIdPtr ShaderHelper::loadFromImmediate(const std::string& key, const std::string& vertCode,
    const std::string& fragCode)
{
    shaderIdPtr idPtr = gShaderPathToGenId[key];
    if (idPtr != nullptr) { return idPtr; }

    shaderIdPtr newId = new shaderId;
    *newId = linkShaders(
        compileShaderData(vertCode, GL_VERTEX_SHADER),
        compileShaderData(fragCode, GL_FRAGMENT_SHADER));

    gShaderPathToGenId[key] = newId;
    printf("Loaded shader Id {%d} from key {%s}\n", *newId, key.c_str());

    return newId;
}

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

void ShaderHelper::setInt(const char* location, int value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1i(loc, value);
}

void ShaderHelper::setIntVec(const char* location, uint32_t amount, int* flatValues)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1iv(loc, amount, flatValues);
}

void ShaderHelper::setVec1f(const char* location, float value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1f(loc, value);
}

void ShaderHelper::setVec2f(const char* location, glm::vec2 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform2f(loc, value.x, value.y);
}

void ShaderHelper::setVec3f(const char* location, glm::vec3 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform3f(loc, value.x, value.y, value.z);
}

void ShaderHelper::setVec4f(const char* location, glm::vec4 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniform4f(loc, value.r, value.g, value.b, value.a);
}

void ShaderHelper::setMatrix4(const char* location, const glm::mat4 value)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

void ShaderHelper::setMatrix4Vec(const char* location, uint32_t amount, const float* flatValues)
{
    int loc = glGetUniformLocation(gActiveShaderId, location);
    if (loc == -1) return handleNotFound(location);
    glUniformMatrix4fv(loc, amount, GL_FALSE, flatValues);
}

void ShaderHelper::setActiveShaderId(const shaderId id)
{
    gActiveShaderId = id;
    glUseProgram(gActiveShaderId);
}

void ShaderHelper::resetBoundShader() const { glUseProgram(0); }

shaderId ShaderHelper::getActiveShaderId() const { return gActiveShaderId; }

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
    shaderFile.close();

    return compileShaderData(content, shaderType);
}

int ShaderHelper::compileShaderData(const std::string& data, int32_t shaderType)
{
    const char* src = data.c_str();
    uint32_t shaderPart = glCreateShader(shaderType);
    glShaderSource(shaderPart, 1, &src, NULL);
    glCompileShader(shaderPart);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderPart, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderPart, 512, NULL, infoLog);
        std::string type = shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAG";
        fprintf(stderr, "Compile failed for shader %s because:\n\t%s\n", type.c_str(), infoLog);
        exit(1);
        return -1;
    }

    return shaderPart;
}

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