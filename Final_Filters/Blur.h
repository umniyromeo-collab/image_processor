#pragma once
#include "../Abstract_Filters/Filter.h"
#include <vector>
#include <cmath>
#include <algorithm>

class GaussianBlur final : public Filter {
public:
    explicit GaussianBlur(const double sigma) : sigma_(sigma) {
    }

    void Apply(Image& image) const override {
        const int width = static_cast<int>(image.Width());
        const int height = static_cast<int>(image.Height());
        const int radius = static_cast<int>(std::ceil(sigma_ * 3.0));

        std::vector<double> kernel(2 * radius + 1);
        double sum = 0.0;

        static constexpr double Two = 2.0;
        for (int i = -radius; i <= radius; ++i) {
            kernel[i + radius] = std::exp(-(i * i) / (Two * sigma_ * sigma_));
            sum += kernel[i + radius];
        }

        for (double& weight : kernel) {
            weight /= sum;
        }

        Image horizontal_blured = image;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel p = {0, 0, 0};
                for (int i = -radius; i <= radius; ++i) {
                    p = p + image.GeneralAt(x + i, y) * kernel[i + radius];
                }
                horizontal_blured.At(x, y) = p;
            }
        }

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                Pixel p = {0, 0, 0};
                for (int i = -radius; i <= radius; ++i) {
                    p = p + horizontal_blured.GeneralAt(x, y + i) * kernel[i + radius];
                }
                p.Clamp();
                image.At(x, y) = p;
            }
        }
    }

private:
    double sigma_;
};