#pragma once
#include "../Abstract_Filters/Filter.h"

class Grayscale final : public Filter {
public:
    void Apply(Image& image) const override {
        for (size_t y = 0; y < image.Height(); ++y) {
            for (size_t x = 0; x < image.Width(); ++x) {
                Pixel& p = image.At(x, y);
                const double val = 0.299 * p.r + 0.587 * p.g + 0.114 * p.b;
                p.r = p.g = p.b = val;
            }
        }
    }
    friend class EdgeDetection;
};