#include <iostream>
#include <string>
#include <vector>
#include "../BMP/bmp_decoder.h"
#include "../BMP/bmp_encoder.h"
#include "Image.h"
#include "filter_factory.h"

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " input.bmp output.bmp [-filter [args]]" << std::endl;
        return 1;
    }

    const std::string input_path = argv[1];
    const std::string output_path = argv[2];

    std::vector<std::unique_ptr<Filter>> filters;

    try {
        for (int i = 3; i < argc; ++i) {
            const std::string arg = argv[i];

            if (arg[0] == '-') {
                const FilterType type = StringToFilterType(arg);
                std::vector<std::string> filter_args;

                while (i + 1 < argc && argv[i + 1][0] != '-') {
                    filter_args.push_back(argv[++i]);
                }

                filters.push_back(FilterFactory(type, filter_args));
            }
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    try {
        Image image = DecodeBMP(input_path);

        for (const auto& filter : filters) {
            filter->Apply(image);
        }

        EncodeBMP(output_path, image);

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }
}