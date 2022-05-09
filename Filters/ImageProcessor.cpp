#include "ImageProcessor.h"
#include "Filter.h"

using namespace cimg_library;

namespace
{
void apply(const Filter& filter, const ImageProcessor::Image& in, ImageProcessor::Image& out)
{
    cimg_for2XY(in, x, y)
    {
        float colorOut[3];

        const float colorIn[3] = {in(x, y, 0, 0), in(x, y, 0, 1), in(x, y, 0, 2)};

        filter(colorIn, colorOut);

        out(x, y, 0, 0) = colorOut[0];
        out(x, y, 0, 1) = colorOut[1];
        out(x, y, 0, 2) = colorOut[2];
    }
}
}

ImageProcessor::ImageProcessor(const std::string& fileName)
{
    _image.load(fileName.c_str());
    _image /= 255.f;
}

ImageProcessor::ImageProcessor(const Image& image)
    : _image(image)
{
}

auto ImageProcessor::getImage() const -> Image
{
    return _image;
}

void ImageProcessor::saveImage(const std::string& fileName)
{
    const CImg<unsigned char> image(_image * 255.f);
    image.save(fileName.c_str());
}

void ImageProcessor::apply(const Filter& filter)
{
    ::apply(filter, _image, _image);
}

void ImageProcessor::apply(const Filter& filter, Image& image)
{
    image.assign(_image.width(), _image.height(), 1, 3);

    ::apply(filter, _image, image);
}

