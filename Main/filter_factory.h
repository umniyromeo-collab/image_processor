#pragma once

#include <map>
#include <string>
#include <stdexcept>
#include <memory>
#include "../Abstract_Filters/all_filters_lib.h"
#include "../Exceptions/custom_exc_lib.h"

enum class FilterType { CROP, GRAYSCALE, NEGATIVE, SHARPEN, EDGE, BLUR };

inline FilterType StringToFilterType(const std::string& name) {
    static const std::map<std::string, FilterType> STRING_TO_TYPE = {
        {"-crop", FilterType::CROP},     {"-gs", FilterType::GRAYSCALE}, {"-neg", FilterType::NEGATIVE},
        {"-sharp", FilterType::SHARPEN}, {"-edge", FilterType::EDGE},    {"-blur", FilterType::BLUR}};

    const auto it = STRING_TO_TYPE.find(name);
    if (it == STRING_TO_TYPE.end()) {
        throw InvalidFiltersStyle(name + "is not a valid filter type");
    }
    return it->second;
}

template <typename T>
T SafeStrCast(const std::string& str);

inline std::unique_ptr<Filter> FilterFactory(const FilterType type, const std::vector<std::string>& args) {
    switch (type) {
        case FilterType::CROP: {
            if (args.size() != 2) {
                throw InvalidFilterArgs("Filter Crop requires 2 arguments: width and height");
            }
            const size_t w = SafeStrCast<uint64_t>(args[0]);
            const size_t h = SafeStrCast<uint64_t>(args[1]);
            return std::make_unique<Crop>(w, h);
        }
        case FilterType::GRAYSCALE: {
            if (!args.empty()) {
                throw InvalidFilterArgs("Filter Grayscale requires no arguments");
            }
            return std::make_unique<Grayscale>();
        }
        case FilterType::NEGATIVE: {
            if (!args.empty()) {
                throw InvalidFilterArgs("Filter Negative requires no arguments");
            }
            return std::make_unique<Negative>();
        }
        case FilterType::SHARPEN: {
            if (!args.empty()) {
                throw InvalidFilterArgs("Filter Sharpening requires no arguments");
            }
            return std::make_unique<Sharpen>();
        }
        case FilterType::EDGE: {
            if (args.size() != 1) {
                throw InvalidFilterArgs("Filter Edge Detection requires 1 argument: threshold");
            }
            const long double threshold = SafeStrCast<long double>(args[0]);
            return std::make_unique<EdgeDetection>(threshold);
        }
        case FilterType::BLUR: {
            if (args.size() != 1) {
                throw InvalidFilterArgs("Filter Gaussian Blur requires 1 argument: sigma");
            }
            const double sigma = SafeStrCast<double>(args[0]);
            return std::make_unique<GaussianBlur>(sigma);
        }
        default: {
            throw InvalidFiltersStyle("Unknown filter type");
        }
    }
}

template <typename T>
T SafeStrCast(const std::string& str) {
    size_t parsed_length = 0;
    T value;
    try {
        if constexpr (std::is_same_v<T, uint64_t>) {
            value = std::stoull(str, &parsed_length);
        } else if constexpr (std::is_same_v<T, double>) {
            value = std::stod(str, &parsed_length);
        } else if constexpr (std::is_same_v<T, long double>) {
            value = std::stold(str, &parsed_length);
        }
    } catch (const std::invalid_argument&) {
        throw InvalidFilterArgs("Cannot parse " + str + " as a number");
    }

    if (parsed_length < str.length()) {
        throw InvalidFilterArgs("Cannot parse " + str + " as a number");
    }

    return value;
}
