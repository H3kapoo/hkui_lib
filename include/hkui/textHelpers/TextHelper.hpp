#pragma once

#include <string>
#include <map>
#include <stdio.h>

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
    static TextHelper& get();

    LoadedFontPtr loadFont(const std::string& fontPath, const int32_t fontSize);

private:
    TextHelper() = default;
    TextHelper(const TextHelper&) = delete;
    TextHelper& operator=(const TextHelper&) = delete;

    //TODO: Make it hot reloadable and unique
    std::map<std::string, LoadedFont> gFontPathToGenFont;
};
}