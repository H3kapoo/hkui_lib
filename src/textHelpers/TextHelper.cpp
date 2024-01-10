#include "../../include/hkui/textHelpers/TextHelper.hpp"

#include <ft2build.h>
#include FT_FREETYPE_H  

#include "../../include/hkui/vendor/GL/Glew.hpp"

namespace textHelpers
{

/**
 * @brief Get class instance.
 *
 * @return A reference to the class instance.
 */
TextHelper& TextHelper::get()
{
    static TextHelper instance;
    return instance;
}


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
LoadedFontPtr TextHelper::loadFont(const std::string& fontPath, const int32_t fontSize)
{
    /*
        Note: there are still problems with loading SDF fonts and sending them to the GPU.
        Some glyphs do reach the gpu, some dont. If we increase the fontSize, more glpyhs can
        reach the gpu and it's very strange and unintuitive. Will tackle this problem in the future.
        For now basic bitmap fonts are enough.
        Alternatively: Use premade SDF font atlas.
    */

    FT_Library ftLib;
    if (FT_Init_FreeType(&ftLib))
    {
        fprintf(stderr, "FreeType lib failed to load!\n");
        return nullptr;
    }

    FT_Face ftFace;
    if (FT_New_Face(ftLib, fontPath.c_str(), 0, &ftFace))
    {
        fprintf(stderr, "Failed to load font: %s\n", fontPath.c_str());
        return nullptr;
    }

    std::string fontKey = fontPath + std::to_string(fontSize);
    //TODO: To avoid one copy of LoadedFont, pointers can be used. For now its ok
    LoadedFont font;

    const int32_t charLimit = 128;
    FT_Set_Pixel_Sizes(ftFace, fontSize, fontSize);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenTextures(1, &font.id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, font.id);

    // generate 128 lvls texture
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_R8, fontSize, fontSize,
        charLimit, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    // wrapping & mag settings
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // FT_GlyphSlot slot = ftFace->glyph; //SDF
    // buffer char data to GPU and retain info about char on CPU side
    for (int32_t i = 0; i < charLimit; i++)
    {
        if (FT_Load_Char(ftFace, i, FT_LOAD_RENDER))
        {
            fprintf(stderr, "Error loading char code: %d\n", i);
            continue;
        }

        // FT_Render_Glyph(slot, FT_RENDER_MODE_SDF); // SDF

        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i,
            ftFace->glyph->bitmap.width,
            ftFace->glyph->bitmap.rows,
            1, GL_RED, GL_UNSIGNED_BYTE, ftFace->glyph->bitmap.buffer);

        ASCIIChar ch = {
            .charCode = uint32_t(i),
            .hAdvance = ftFace->glyph->advance.x,
            .size = glm::ivec2(ftFace->glyph->bitmap_left + ftFace->glyph->bitmap.width, ftFace->glyph->bitmap_top + ftFace->glyph->bitmap.rows),
            .bearing = glm::ivec2(ftFace->glyph->bitmap_left, ftFace->glyph->bitmap_top)
        };

        font.data[i] = ch;
    }

    gFontPathToGenFont[fontKey] = font;

    printf("Loaded font texture {%d} from %s\n", font.id, fontPath.c_str());

    // unbind texture and free FTLIB resources
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    FT_Done_Face(ftFace);
    FT_Done_FreeType(ftLib);

    return &gFontPathToGenFont[fontKey];
}

}