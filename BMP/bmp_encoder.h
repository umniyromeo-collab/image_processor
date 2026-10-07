#pragma once

#include <fstream>
#include <vector>
#include <cstdint>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <iostream>
#include "../Main/Image.h"
#include "bmp_decode_structs.h"

inline void EncodeBMP(const std::string& path, const Image& image) {
    static constexpr uint16_t PixelBitSize = 24;
    static constexpr int PaddingBase = 31;
    static constexpr int PaddingDivisor = 32;
    static constexpr int BytesPerWord = 4;
    static constexpr int PixelSizeof = 3;
    static constexpr uint16_t BmpSignature = 0x4D42;
    static constexpr double MaxColorVal = 255.0;

    std::ofstream f(path, std::ios::out | std::ios::binary | std::ios::trunc);
    if (!f.is_open()) {
        std::cerr << "System error: " << strerror(errno) << std::endl;
        throw std::runtime_error("Cannot open file for writing: " + path);
    }

    const int width = static_cast<int>(image.Width());
    const int height = static_cast<int>(image.Height());

    const int row_size_in_file = ((PixelBitSize * width + PaddingBase) / PaddingDivisor) * BytesPerWord;

    BMPFileHeader fh{};
    BMPInfoHeader ih{};

    fh.bfType = BmpSignature;
    fh.bfSize = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + row_size_in_file * height;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;
    fh.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);

    ih.biSize = sizeof(BMPInfoHeader);
    ih.biWidth = width;
    ih.biHeight = height;
    ih.biPlanes = 1;
    ih.biBitCount = PixelBitSize;
    ih.biCompression = 0;
    ih.biSizeImage = 0;
    ih.biXPelsPerMeter = 0;
    ih.biYPelsPerMeter = 0;
    ih.biClrUsed = 0;
    ih.biClrImportant = 0;

    f.write(reinterpret_cast<const char*>(&fh), sizeof(fh));
    f.write(reinterpret_cast<const char*>(&ih), sizeof(ih));

    std::vector<uint8_t> row_buffer(row_size_in_file, 0);

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            const auto [r, g, b] = image.GeneralAt(x, y);

            const int pos = x * PixelSizeof;
            row_buffer[pos + 0] = static_cast<uint8_t>(std::round(std::clamp(b, 0.0, 1.0) * MaxColorVal));
            row_buffer[pos + 1] = static_cast<uint8_t>(std::round(std::clamp(g, 0.0, 1.0) * MaxColorVal));
            row_buffer[pos + 2] = static_cast<uint8_t>(std::round(std::clamp(r, 0.0, 1.0) * MaxColorVal));
        }
        f.write(reinterpret_cast<const char*>(row_buffer.data()), row_size_in_file);
    }
}