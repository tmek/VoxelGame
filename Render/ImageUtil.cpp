#include "ImageUtil.h"

void ImageUtil::CreateNoisePattern(UINT width, UINT height, std::vector<UINT>& data)
{
    data.resize(width * height);

    int min=128, max=255;
    
    for (UINT y = 0; y < height; ++y)
    {
        for (UINT x = 0; x < width; ++x)
        {
            // Generate random grayscale color
            // between min and max
            BYTE color = min + rand() % (max - min);
            
            BYTE a = 255;

            UINT PixelColor = (a << 24) | (color << 16) | (color << 8) | color;
            data[y * width + x] = PixelColor;
        }
    }
}

void ImageUtil::CreateCheckerboardPattern(UINT width, UINT height, std::vector<UINT>& data)
{
    data.resize(width * height);

    static bool isWhite = false;
    for (UINT y = 0; y < height; ++y)
    {
        isWhite = !isWhite;
        for (UINT x = 0; x < width; ++x)
        {
            isWhite = !isWhite;

            BYTE r,g,b;
            
            if (isWhite)
            {
                r = 64;
                g = 192;
                b = 64;
            }
            else
            {
                r = 64;
                g = 64;
                b = 192;
            }

            BYTE a = 255;
            
            UINT PixelColor = (a << 24) | (b << 16) | (g << 8) | r;
            data[y * width + x] = PixelColor;
        }
    }
}

bool LoadBMP(const std::string& filename, BMPHeader& header, BMPInfoHeader& infoHeader, std::vector<BYTE>& imageData)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
        return false;

    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (header.bfType != 0x4D42) // Check if it's a BMP file
        return false;

    imageData.resize(infoHeader.biSizeImage);
    file.seekg(header.bfOffBits, std::ios::beg);
    file.read(reinterpret_cast<char*>(imageData.data()), imageData.size());

    return true;
}
