#pragma once
#include "../Abstract_Filters/Filter.h"

class Negative final : public Filter {
public:
    void Apply(Image& image) const override {
        for (size_t y = 0; y < image.Height(); ++y) {
            for (size_t x = 0; x < image.Width(); ++x) {
                Pixel& p = image.At(x, y);
                p = Pixel{1, 1, 1} - p;
            }
        }
    }
};