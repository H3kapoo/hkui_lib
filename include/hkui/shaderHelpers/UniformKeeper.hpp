#pragma once

#include <vector>
#include <utility>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace shaderHelpers
{
/**
 * @brief Class that keeps track of a node's uniforms.
 *
 * This class tracks what uniform a node has (and also watches them by means or pointers)
 * so that the user doesn't have to alter the rendering code itself (setting of uniforms to
 * the shader). This acts like a middle man and lets the user set/track whatever uniforms he
 * wants without altering the rendering code. It may be a trade-off between performance and
 * readability in the future.
 *
 * @note EXTRA CARE needs to be taken so that we don't push to watch the same variable
 *       in some kind of loop. It will KILL performance.
 */
class UniformKeeper
{
public:
    /**
     * @brief Watch the vec4 address of an named uniform.
     *
     * @param name  Uniform name.
     * @param value Pointer to the vec4 to be watched.
     */
    void watch(const std::string& name, glm::vec4* value)
    {
        gVec4Ptrs.push_back({ name, value });
    }


    /**
     * @brief Watch the vec3 address of an named uniform.
     *
     * @param name  Uniform name.
     * @param value Pointer to the vec3 to be watched.
     */
    void watch(const std::string& name, glm::vec3* value)
    {
        gVec3Ptrs.push_back({ name, value });
    }


    /**
     * @brief Watch the int address of an named uniform.
     *
     * @param name  Uniform name.
     * @param value Pointer to the int to be watched.
     */
    void watch(const std::string& name, uint32_t* value)
    {
        gIntPtrs.push_back({ name, value });
    }

    /*
        This default init is needed in cases where X_obj and Y_obj use the same
        A_shader but X_obj doesn't need to use A_shader.someUniform but Y_obj uses it.
        If X_obj doesnt set a default value, Y_obj's someUniform will be used as OpenGL
        is a big state machine and from it's perspective that uniform hasn't changed.
    */

    /**
     * @brief Default zero initialize the named uniform.
     *
     * Used in case we don't want any particular value for this uniform
     * but we need to initialize it with a default value.
     *
     * @param name Vec4 uniform name.
     */
    void defaultVec4(const std::string& name)
    {
        watch(name, &gDefaultVec4);
    }

    /**
     * @brief Default zero initialize the named uniform.
     *
     * Used in case we don't want any particular value for this uniform
     * but we need to initialize it with a default value.
     *
     * @param name Vec3 uniform name.
     */
    void defaultVec3(const std::string& name)
    {
        watch(name, &gDefaultVec3);
    }


    /**
     * @brief Default zero initialize the named uniform.
     *
     * Used in case we don't want any particular value for this uniform
     * but we need to initialize it with a default value.
     *
     * @param name Int uniform name.
     */
    void defaultInt(const std::string& name)
    {
        watch(name, &gDefaultVec4);
    }


    /* Stored data type can be a typePtr so that changes made to the memory
    address for that variable reflects instantly */

    /* Vec4 ref storage */
    std::vector<std::pair<std::string, glm::vec4*>> gVec4Ptrs;

    /* Vec3 ref storage */
    std::vector<std::pair<std::string, glm::vec3*>> gVec3Ptrs;

    /* Int ref storage */
    std::vector<std::pair<std::string, uint32_t*>> gIntPtrs;


private:
    glm::vec4 gDefaultVec4{ 0 };
    glm::vec3 gDefaultVec3{ 0 };
    uint32_t gDefaultInt{ 0 };
};
}