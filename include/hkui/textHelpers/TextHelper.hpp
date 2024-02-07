#pragma once

#include <map>
#include <stdio.h>
#include <string>

#include "Types.hpp"

namespace textHelpers
{
/**
 * @brief Singleton class responsible for loading fonts.
 *
 * Class that loads fonts into memory using vendor/stb_image and caches
 * data for later use so that two calls to load the same font will result in
 * cache being accessed instead.
 *
 * @note Currently this is not hot reloadable like shaders/textures.
 */
class TextHelper
{
public:
    /**
     * @brief Get class instance.
     *
     * @return A reference to the class instance.
     */
    static TextHelper& get();

    /**
     * @brief Load font and get a pointer to stored cache location of loaded font.
     *
     * Function loads font into memory, if not already in cache, and returns pointer
     * inside the map where this data is located.
     *
     * @note No SDF support as of now. There are some questionable bugs.
     *
     * @param fontPath       Path to font to be loaded.
     * @param fontSize       Size of the font.
     * @return LoadedFontPtr Pointer to loaded font data.
     */
    LoadedFontPtr loadFont(const std::string& fontPath, const int32_t fontSize);

private:
    TextHelper() = default;
    TextHelper(const TextHelper&) = delete;
    TextHelper& operator=(const TextHelper&) = delete;

    // TODO: Make it hot reloadable and unique
    std::map<std::string, LoadedFont> gFontPathToGenFont;
};
} // namespace textHelpers