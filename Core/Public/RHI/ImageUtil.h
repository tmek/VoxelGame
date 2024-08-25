#pragma once

#include <vector>

#include "HAL/Platform.h"


class ImageUtil
{

public:

    /** Fill image with noise */
    static void FillImageWithNoise(const uint32 ImageWidth, const uint32 ImageHeight, std::vector<uint32>& ImageDataOut) noexcept;
        
     
    /** Fill image with a checkerboard pattern */
    static void FillImageWithCheckerboardPattern(const uint32 ImageWidth, const uint32 ImageHeight, std::vector<uint32>& ImageDataOut) noexcept;
};
