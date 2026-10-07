#pragma once
#include "Filter.h"
#include <vector>

class MatrixFilter : public Filter {
protected:
    void ApplyMatrix(Image& image, const std::vector<std::vector<double>>& matrix) const {
        const Image original = image;
        const int width = static_cast<int>(image.Width());
        const int height = static_cast<int>(image.Height());
        const int m_size = static_cast<int>(matrix.size());
        const int module = m_size / 2;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {

                Pixel res_pixel = {0, 0, 0};

                for (int i = 0; i < m_size; ++i) {
                    for (int j = 0; j < m_size; ++j) {
                        res_pixel = res_pixel + original.GeneralAt(x + j - module, y + i - module) * matrix[i][j];
                    }
                }

                res_pixel.Clamp();
                image.At(x, y) = res_pixel;
            }
        }
    }
};