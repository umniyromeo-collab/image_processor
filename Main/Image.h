#pragma once
#include <vector>
#include <algorithm>

struct Pixel {
    double r = 0.0;
    double g = 0.0;
    double b = 0.0;

    Pixel operator+(const Pixel& other) const noexcept {
        return {r + other.r, g + other.g, b + other.b};
    }

    Pixel operator-(const Pixel& other) const noexcept {
        return {r - other.r, g - other.g, b - other.b};
    }

    Pixel operator*(const double scale) const noexcept {
        return {r * scale, g * scale, b * scale};
    }

    void Clamp() noexcept {
        r = std::clamp(r, 0.0, 1.0);
        g = std::clamp(g, 0.0, 1.0);
        b = std::clamp(b, 0.0, 1.0);
    }
};

class Image {
public:
    explicit Image(const size_t width, const size_t height) : width_(width), height_(height), pixels_(width * height) {
    }

    Image(Image&& other) noexcept : width_(other.width_), height_(other.height_), pixels_(std::move(other.pixels_)) {
        other.width_ = 0;
        other.height_ = 0;
    }

    Image& operator=(Image&& other) noexcept {
        if (this != &other) {
            width_ = other.width_;
            height_ = other.height_;
            pixels_ = std::move(other.pixels_);
            other.width_ = 0;
            other.height_ = 0;
        }
        return *this;
    }

    Image(const Image& other) = default;
    Image& operator=(const Image& other) = default;

    size_t Width() const noexcept {
        return width_;
    }
    void SetWidth(const size_t width) noexcept {
        width_ = width;
    }

    size_t Height() const noexcept {
        return height_;
    }
    void SetHeight(const size_t height) noexcept {
        height_ = height;
    }

    const Pixel& At(const size_t x, const size_t y) const noexcept {
        return pixels_[y * width_ + x];
    }

    Pixel& At(const size_t x, const size_t y) noexcept {
        return pixels_[y * width_ + x];
    }

    Pixel GeneralAt(const int x, const int y) const noexcept {
        const int res_x = std::clamp(x, 0, static_cast<int>(width_) - 1);
        const int res_y = std::clamp(y, 0, static_cast<int>(height_) - 1);
        return At(static_cast<size_t>(res_x), static_cast<size_t>(res_y));
    }

    void SetData(std::vector<Pixel> new_data) noexcept {
        pixels_ = std::move(new_data);
    }

    Pixel* GetData() noexcept {
        return pixels_.data();
    }

private:
    size_t width_ = 0;
    size_t height_ = 0;
    std::vector<Pixel> pixels_;
};