#pragma once

#include <cstdint>

namespace textureHelpers
{
struct TextureData
{
    uint32_t gId{ 0 };
    int32_t gWidth{ -1 };
    int32_t gWHeight{ -1 };
    int32_t gNumChannels{ -1 };
};

typedef TextureData* TextureDataPtr;
typedef uint32_t textureId;
typedef textureId* textureIdPtr;
}