#pragma once

#include <glm/glm.hpp>
#include <cstdint>

namespace textHelpers
{
#define MAX_ASCII 128

struct ASCIIChar
{
    uint32_t charCode;
    int64_t hAdvance;
    glm::ivec2 size;
    glm::ivec2 bearing;
};

typedef uint32_t textureArrayId;

struct LoadedFont
{
    ASCIIChar data[MAX_ASCII];
    textureArrayId id;
};

typedef LoadedFont* LoadedFontPtr;

struct TextLine
{
    uint32_t startIdx{ 0 };
    uint32_t endIdx{ 0 };
    float length{ 0.0f };
    float height{ 0.0f };
};

}