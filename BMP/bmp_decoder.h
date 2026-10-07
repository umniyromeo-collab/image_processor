#pragma once

#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include "../Main/Image.h"
#include "bmp_decode_structs.h"
#include "../Exceptions/custom_exc_lib.h"

inline Image DecodeBMP(const std::string& path) {
    static constexpr uint16_t PixelBitSize = 24;
    static constexpr int PaddingBase = 31;
    static constexpr int PaddingDivisor = 32;
    static constexpr int BytesPerWord = 4;
    static constexpr int PixelSizeof = 3;
    static constexpr uint16_t BmpSignature = 0x4D42;
    static constexpr double MaxColorVal = 255.0;

    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) {
        throw InvalidBMPFile("Could not open file: " + path);
    }

    BMPFileHeader fh;
    BMPInfoHeader ih;

    f.read(reinterpret_cast<char*>(&fh), sizeof(fh));
    f.read(reinterpret_cast<char*>(&ih), sizeof(ih));

    if (fh.bfType != BmpSignature) {
        throw InvalidBMPFile("File is not a valid BMP");
    }

    if (ih.biBitCount != PixelBitSize) {
        throw BadBMPParse("BMP format should be 24-bit");
    }

    const int width = ih.biWidth;
    const int height = ih.biHeight;

    if (width <= 0 || height <= 0) {
        throw BadBMPParse("Invalid image dimensions in header");
    }

    const int row_size_in_file = ((PixelBitSize * width + PaddingBase) / PaddingDivisor) * BytesPerWord;

    Image result(width, height);
    std::vector<uint8_t> row_buffer(row_size_in_file);

    f.seekg(fh.bfOffBits, std::ios::beg);

    for (int y = height - 1; y >= 0; --y) {
        f.read(reinterpret_cast<char*>(row_buffer.data()), row_size_in_file);

        for (int x = 0; x < width; ++x) {
            const int pos = x * PixelSizeof;

            const double b = static_cast<double>(row_buffer[pos + 0]) / MaxColorVal;
            const double g = static_cast<double>(row_buffer[pos + 1]) / MaxColorVal;
            const double r = static_cast<double>(row_buffer[pos + 2]) / MaxColorVal;

            result.At(x, y) = {r, g, b};
        }
    }

    return result;
}