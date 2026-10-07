#pragma once
#include "../Abstract_Filters/MatrixFilter.h"

class Sharpen final : public MatrixFilter {
public:
    void Apply(Image& image) const override {
        const std::vector<std::vector<double>> sharpen_matrix = {{0, -1, 0}, {-1, 5, -1}, {0, -1, 0}};
        ApplyMatrix(image, sharpen_matrix);
    }
};