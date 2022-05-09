#pragma once

#include "CImg.h"

#include <string>

class Filter;

class ImageProcessor
{
public:
    using Image = cimg_library::CImg<>;

    ImageProcessor(const std::string& fileName);
    ImageProcessor(const Image& image);

    void apply(const Filter& filter);
    void apply(const Filter& filter, Image& image);

    Image getImage() const;
    void saveImage(const std::string& fileName);

private:
    Image _image;
};
