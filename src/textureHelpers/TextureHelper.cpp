#include "../../include/hkui/textureHelpers/TextureHelper.hpp"

#include "../../include/hkui/vendor/GL/Glew.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../../include/hkui/vendor/stb/stb_image.h"

namespace textureHelpers
{
/**
* @brief Get class instance.
*
* @return A reference to the class instance.
*/
TextureHelper& TextureHelper::get()
{
    static TextureHelper instance;
    return instance;
}


/**
 * @brief Deallocate cached texture pointers at end of life.
 *
 */
TextureHelper::~TextureHelper()
{
    for (auto& [key, value] : gTexturePathToGenId)
    {
        /* Free textureIdPtr memory */
        delete value;
    }
}


/**
 * @brief Load and retrieve a texture **TextureData** pointer handle from the specified texture path.
 *
 * This function checks if a texture with the specified texture path already exists in the textures cache.
 * If it does, it returns a pointer to its **TextureData**. Otherwise, it loads the texture, stores the
 * loaded **TextureData** in the cache, and returns a pointer to the newly loaded **TextureData**.
 *
 * @note Returned dereferenced pointer value **gId** will be 0 in case any errors occured. It will never be **nullptr**.
 * @note Reason for pointer return instead of plain structure is to have easier texture reloading capabilities.
 *
 * @param filePath The file path to the texture.
 *
 * @return A pointer to the texture data structure.
 */
TextureDataPtr TextureHelper::loadTexture(const std::string& filePath)
{
    TextureDataPtr dataPtr = gTexturePathToGenId[filePath];
    if (dataPtr != nullptr) { return dataPtr; }

    TextureDataPtr newDataPtr = new TextureData;
    *newDataPtr = loadInternal(filePath);

    gTexturePathToGenId[filePath] = newDataPtr;

    return newDataPtr;
}

/**
 * @brief Reload a texture from the specified texture path.
 *
 * This function creates a new **TextureData** from the provided texture path and overrides
 * the cached value at **filePath** location with the newly created **TextureData** if successfull,
 * otherwise it keeps previous data.
 *
 * @param filePath The file path to the texture.
 *
 * @return Doesn't return anything.
 */
void TextureHelper::reloadFromPath(const std::string& filePath)
{
    TextureData reloadedData = loadInternal(filePath);
    if (reloadedData.gId != 0)
    {
        assert(gTexturePathToGenId[filePath] != nullptr && "Tried to reload texture that wasn't loaded before!");
        *gTexturePathToGenId[filePath] = reloadedData;
        return;
    }
}


/**
 * @brief Internal - helper for loading the texture data using stbi and gl.
 *
 */
TextureData TextureHelper::loadInternal(const std::string& filePath)
{
    TextureData newData;

    unsigned char* data = stbi_load(filePath.c_str(), &newData.gWidth, &newData.gWHeight, &newData.gNumChannels, 0);
    if (!data)
    {
        fprintf(stderr, "Failed to load texture from: %s .Check path correctness.\n", filePath.c_str());
        return newData;
    }

    glGenTextures(1, &newData.gId);
    glBindTexture(GL_TEXTURE_2D, newData.gId);

    // filters and repeat method
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newData.gWidth, newData.gWHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    printf("Loaded texture from: %s ", filePath.c_str());
    printf("Channels: %d Width: %d Height: %d\n", newData.gNumChannels, newData.gWidth, newData.gWHeight);

    return newData;
}

}