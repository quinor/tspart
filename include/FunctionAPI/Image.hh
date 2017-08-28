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

  ImageLoader& image_loader(std::string name="");
  ImageSaver& image_saver(DataPromise<sf::Texture>& data, std::string name="");

  ImageMaximizer& image_maximizer(DataPromise<sf::Texture>& data, int max_size=0);

  ImageFilterGrayscale& image_filter_grayscale(DataPromise<sf::Texture>& data);
  ImageFilterInverse& image_filter_inverse(DataPromise<sf::Texture>& data);
  ImageFilterSigmoid& image_filter_sigmoid(DataPromise<sf::Texture>& data, std::pair<float, float> shape={0, 0});
  ImageFilterGamma& image_filter_gamma(DataPromise<sf::Texture>& data, float shape=0);
  ImageFilterLogarithm& image_filter_logarithm(DataPromise<sf::Texture>& data, float shape=0);
  ImageFilterGaussianBlur& image_filter_gaussian_blur(DataPromise<sf::Texture>& data, float sigma=0);

  ImageCompositorAverage& image_compositor_average(
    DataPromise<sf::Texture>& left,
    DataPromise<sf::Texture>& right,
    float ratio=.5f);
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
    float steepness=30);
};

template<int W, int H>
ImageMultiViewer<W, H>& ImageMixin::image_multi_viewer()
{
  auto ret = new ImageMultiViewer<W, H>();
  register_block(ret);
  return *ret;
}
