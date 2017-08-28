#include "FunctionAPI/Image.hh"

ImageViewer& ImageMixin::image_viewer(DataPromise<sf::Texture>& data)
{
  auto ret = new ImageViewer();
  blocks.push_back(ret);
  ret->in.connect(data);
  return *ret;
}

template<int W, int H>
ImageMultiViewer<W, H>& ImageMixin::image_multi_viewer()
{
  auto ret = new ImageMultiViewer<W, H>();
  blocks.push_back(ret);
  return *ret;
}

ImageLoader& ImageMixin::image_loader(std::string name)
{
  auto ret = new ImageLoader();
  blocks.push_back(ret);
  if (name != "")
    ret->filename_manual.set_data(name);
  return *ret;
}

ImageSaver& ImageMixin::image_saver(DataPromise<sf::Texture>& data, std::string name)
{
  auto ret = new ImageSaver();
  blocks.push_back(ret);
  ret->in.connect(data);
  if (name != "")
    ret->filename_manual.set_data(name);
  return *ret;
}

ImageMaximizer& ImageMixin::image_maximizer(DataPromise<sf::Texture>& data, int max_size)
{
  auto ret = new ImageMaximizer();
  blocks.push_back(ret);
  ret->in.connect(data);
  if (max_size != 0)
    ret->max_size_manual.set_data(max_size);
  return *ret;
}

ImageFilterGrayscale& ImageMixin::image_filter_grayscale(DataPromise<sf::Texture>& data)
{
  auto ret = new ImageFilterGrayscale();
  blocks.push_back(ret);
  ret->in.connect(data);
  return *ret;
}

ImageFilterInverse& ImageMixin::image_filter_inverse(DataPromise<sf::Texture>& data)
{
  auto ret = new ImageFilterInverse();
  blocks.push_back(ret);
  ret->in.connect(data);
  return *ret;
}

ImageFilterSigmoid& ImageMixin::image_filter_sigmoid(DataPromise<sf::Texture>& data, std::pair<float, float> shape)
{
  auto ret = new ImageFilterSigmoid();
  blocks.push_back(ret);
  ret->in.connect(data);
  if (shape != std::pair<float, float>(0, 0))
    ret->shape_manual.set_data(shape);
  return *ret;
}

ImageFilterGamma& ImageMixin::image_filter_gamma(DataPromise<sf::Texture>& data, float shape)
{
  auto ret = new ImageFilterGamma();
  blocks.push_back(ret);
  ret->in.connect(data);
  if (shape != 0.f)
    ret->shape_manual.set_data(shape);
  return *ret;
}

ImageFilterLogarithm& ImageMixin::image_filter_logarithm(DataPromise<sf::Texture>& data, float shape)
{
  auto ret = new ImageFilterLogarithm();
  blocks.push_back(ret);
  ret->in.connect(data);
  if (shape != 0.f)
    ret->shape_manual.set_data(shape);
  return *ret;
}

ImageFilterGaussianBlur& ImageMixin::image_filter_gaussian_blur(DataPromise<sf::Texture>& data, float sigma)
{
  auto ret = new ImageFilterGaussianBlur();
  blocks.push_back(ret);
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
  blocks.push_back(ret);
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
  blocks.push_back(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  return *ret;
}

ImageCompositorAbsoluteDifference& ImageMixin::image_compositor_absolute_difference(
  DataPromise<sf::Texture>& left,
  DataPromise<sf::Texture>& right)
{
  auto ret = new ImageCompositorAbsoluteDifference();
  blocks.push_back(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  return *ret;
}

ImageCompositorRatio& ImageMixin::image_compositor_ratio(
  DataPromise<sf::Texture>& left,
  DataPromise<sf::Texture>& right)
{
  auto ret = new ImageCompositorRatio();
  blocks.push_back(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  return *ret;
}
