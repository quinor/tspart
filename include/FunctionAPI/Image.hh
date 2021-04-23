#pragma once

#include "API.hh"
#include "Image/ImageTools.hh"
#include <string>

class ImageMixin : public virtual API
{
public:

  ImageViewer& image_viewer(DataPromise<sf::Texture>& data);
  template<int W, int H>
  ImageMultiViewer<W, H>& image_multi_viewer();

  ImageLoader& image_loader(Param<std::string> name=std::string(""));
  ImageSaver& image_saver(DataPromise<sf::Texture>& data, Param<std::string> name=std::string(""));

  ImageMaximizer& image_maximizer(DataPromise<sf::Texture>& data, Param<size_t> max_size=1024);

  ImageFilterGrayscale& image_filter_grayscale(DataPromise<sf::Texture>& data);
  ImageFilterInverse& image_filter_inverse(DataPromise<sf::Texture>& data);
  ImageFilterSigmoid& image_filter_sigmoid(
    DataPromise<sf::Texture>& data,
    Param<float> alpha=30.f,
    Param<float> beta=128.f);
  ImageFilterGamma& image_filter_gamma(DataPromise<sf::Texture>& data, Param<float> shape=0.3f);
  ImageFilterLogarithm& image_filter_logarithm(DataPromise<sf::Texture>& data, Param<float> shape=10.f);
  ImageFilterGaussianBlur& image_filter_gaussian_blur(DataPromise<sf::Texture>& data, Param<size_t> sigma=40.f);

  ImageCompositorAverage& image_compositor_average(
    DataPromise<sf::Texture>& left,
    DataPromise<sf::Texture>& right,
    Param<float> ratio=.5f);
  ImageCompositorDifference& image_compositor_difference(
    DataPromise<sf::Texture>& left,
    DataPromise<sf::Texture>& right);
  ImageCompositorAbsoluteDifference& image_compositor_absolute_difference(
    DataPromise<sf::Texture>& left,
    DataPromise<sf::Texture>& right);
  ImageCompositorRatio& image_compositor_ratio(
    DataPromise<sf::Texture>& left,
    DataPromise<sf::Texture>& right);
  DataPromise<sf::Texture>& image_normalization(
    DataPromise<sf::Texture>& data,
    Param<float> steepness=30,
    Param<float> graypoint=128,
    Param<size_t> radius=20);
};

template<int W, int H>
ImageMultiViewer<W, H>& ImageMixin::image_multi_viewer()
{
  auto ret = new ImageMultiViewer<W, H>();
  register_block(ret);
  return *ret;
}
