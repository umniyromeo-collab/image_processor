#pragma once
#include "../Main/Image.h"

class Filter {
public:
    virtual ~Filter() noexcept = default;
    virtual void Apply(Image& image) const = 0;
};