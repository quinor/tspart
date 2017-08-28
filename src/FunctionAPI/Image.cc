#include "FunctionAPI/Image.hh"

ImageViewer& ImageMixin::image_viewer(DataPromise<sf::Texture>& data)
{
  auto ret = new ImageViewer();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

ImageLoader& ImageMixin::image_loader(std::string name)
{
  auto ret = new ImageLoader();
  register_block(ret);
  if (name != "")
    ret->filename_manual.set_data(name);
  return *ret;
}

ImageSaver& ImageMixin::image_saver(DataPromise<sf::Texture>& data, std::string name)
{
  auto ret = new ImageSaver();
  register_block(ret);
  ret->in.connect(data);
  if (name != "")
    ret->filename_manual.set_data(name);
  return *ret;
}

ImageMaximizer& ImageMixin::image_maximizer(DataPromise<sf::Texture>& data, int max_size)
{
  auto ret = new ImageMaximizer();
  register_block(ret);
  ret->in.connect(data);
  if (max_size != 0)
    ret->max_size_manual.set_data(max_size);
  return *ret;
}

ImageFilterGrayscale& ImageMixin::image_filter_grayscale(DataPromise<sf::Texture>& data)
{
  auto ret = new ImageFilterGrayscale();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

ImageFilterInverse& ImageMixin::image_filter_inverse(DataPromise<sf::Texture>& data)
{
  auto ret = new ImageFilterInverse();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

ImageFilterSigmoid& ImageMixin::image_filter_sigmoid(DataPromise<sf::Texture>& data, std::pair<float, float> shape)
{
  auto ret = new ImageFilterSigmoid();
  register_block(ret);
  ret->in.connect(data);
  if (shape != std::pair<float, float>(0, 0))
    ret->shape_manual.set_data(shape);
  return *ret;
}

ImageFilterGamma& ImageMixin::image_filter_gamma(DataPromise<sf::Texture>& data, float shape)
{
  auto ret = new ImageFilterGamma();
  register_block(ret);
  ret->in.connect(data);
  if (shape != 0.f)
    ret->shape_manual.set_data(shape);
  return *ret;
}

ImageFilterLogarithm& ImageMixin::image_filter_logarithm(DataPromise<sf::Texture>& data, float shape)
{
  auto ret = new ImageFilterLogarithm();
  register_block(ret);
  ret->in.connect(data);
  if (shape != 0.f)
    ret->shape_manual.set_data(shape);
  return *ret;
}

ImageFilterGaussianBlur& ImageMixin::image_filter_gaussian_blur(DataPromise<sf::Texture>& data, float sigma)
{
  auto ret = new ImageFilterGaussianBlur();
  register_block(ret);
  ret->in.connect(data);
  if (sigma != 0.f)
    ret->sigma_manual.set_data(sigma);
  return *ret;
}

ImageCompositorAverage& ImageMixin::image_compositor_average(
  DataPromise<sf::Texture>& left,
  DataPromise<sf::Texture>& right,
  float ratio)
{
  auto ret = new ImageCompositorAverage();
  register_block(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  if (ratio != .5f)
    ret->ratio_manual.set_data(ratio);
  return *ret;
}

ImageCompositorDifference& ImageMixin::image_compositor_difference(
  DataPromise<sf::Texture>& left,
  DataPromise<sf::Texture>& right)
{
  auto ret = new ImageCompositorDifference();
  register_block(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  return *ret;
}

ImageCompositorAbsoluteDifference& ImageMixin::image_compositor_absolute_difference(
  DataPromise<sf::Texture>& left,
  DataPromise<sf::Texture>& right)
{
  auto ret = new ImageCompositorAbsoluteDifference();
  register_block(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  return *ret;
}

ImageCompositorRatio& ImageMixin::image_compositor_ratio(
  DataPromise<sf::Texture>& left,
  DataPromise<sf::Texture>& right)
{
  auto ret = new ImageCompositorRatio();
  register_block(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  return *ret;
}

DataPromise<sf::Texture>& ImageMixin::image_normalization(
  DataPromise<sf::Texture>& data,
  float steepness)
{
  return image_filter_sigmoid(
    image_compositor_ratio(
      image_filter_gaussian_blur(data, 1),
      image_filter_grayscale(image_filter_gaussian_blur(data, 20))),
    {steepness, 128});
}
