#pragma once
#include "../Abstract_Filters/MatrixFilter.h"
#include "Grayscale.h"

class EdgeDetection final : public MatrixFilter {
public:
    explicit EdgeDetection(const double threshold) : threshold_(threshold) {
    }

    void Apply(Image& image) const override {
        Grayscale{}.Apply(image);

        const std::vector<std::vector<double>> edge_matrix = {{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}};
        ApplyMatrix(image, edge_matrix);

        const size_t width = image.Width();
        const size_t height = image.Height();

        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                Pixel& p = image.At(x, y);
                p = (p.r > threshold_) ? Pixel{1.0, 1.0, 1.0} : Pixel{0.0, 0.0, 0.0};
            }
        }
    }

private:
    double threshold_;
};