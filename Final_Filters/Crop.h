#pragma once
#include <algorithm>
#include <vector>
#include <cstring>
#include "../Abstract_Filters/Filter.h"
#include "../Main/Image.h"

class Crop final : public Filter {
public:
    explicit Crop(const size_t width, const size_t height) : width_(width), height_(height) {
    }

    void Apply(Image& image) const override {
        const size_t old_width = image.Width();
        const size_t old_height = image.Height();
        const size_t new_width = std::min(width_, old_width);
        const size_t new_height = std::min(height_, old_height);

        std::vector<Pixel> cropped_pixels;
        cropped_pixels.reserve(new_width * new_height);

        for (size_t y = 0; y < new_height; ++y) {
            for (size_t x = 0; x < new_width; ++x) {
                cropped_pixels.push_back(image.At(x, y));
            }
        }

        image.SetData(std::move(cropped_pixels));
        image.SetWidth(new_width);
        image.SetHeight(new_height);
    }

private:
    size_t width_;
    size_t height_;
};