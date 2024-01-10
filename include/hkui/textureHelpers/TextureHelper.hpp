#pragma once

#include <string>
#include <map>

#include "Types.hpp"

namespace textureHelpers
{

/**
 * @brief Simple class that deals with texture objects.
 *
 * Class that deals with texture objects such as loading them and provinding
 * a gateway to their object id.
 *
 */
class TextureHelper
{
public:
    static TextureHelper& get();

    TextureDataPtr loadTexture(const std::string& filePath);
    void reloadFromPath(const std::string& filePath);

private:
    TextureHelper() = default;
    ~TextureHelper();
    TextureHelper(const TextureHelper&) = delete;
    TextureHelper& operator=(const TextureHelper&) = delete;

    TextureData loadInternal(const std::string& filePath);

    /* In case a lot of objects use the same texture, it's pointless to allocate memory for each SAME texture.
       We can reference the same one, just changing uniforms as needed. */
    std::map<std::string, TextureDataPtr> gTexturePathToGenId;

};
}