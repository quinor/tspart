#include "FunctionAPI/Image.hh"

ImageViewer& ImageMixin::image_viewer(DataPromise<sf::Texture>& data)
{
  auto ret = new ImageViewer();
  register_block(ret);
  ret->in.connect(data);
  return *ret;
}

ImageLoader& ImageMixin::image_loader(Param<std::string> name)
{
  auto ret = new ImageLoader();
  register_block(ret);
  ret->filename_input.connect(name.get_input(this));
  return *ret;
}

ImageSaver& ImageMixin::image_saver(DataPromise<sf::Texture>& data, Param<std::string> name)
{
  auto ret = new ImageSaver();
  register_block(ret);
  ret->in.connect(data);
  ret->filename_input.connect(name.get_input(this));
  return *ret;
}

ImageMaximizer& ImageMixin::image_maximizer(DataPromise<sf::Texture>& data, Param<size_t> max_size)
{
  auto ret = new ImageMaximizer();
  register_block(ret);
  ret->in.connect(data);
  ret->max_size_input.connect(max_size.get_input(this));
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

ImageFilterSigmoid& ImageMixin::image_filter_sigmoid(
  DataPromise<sf::Texture>& data,
  Param<float> alpha,
  Param<float> beta)
{
  auto ret = new ImageFilterSigmoid();
  register_block(ret);
  ret->in.connect(data);
  ret->alpha_input.connect(alpha.get_input(this));
  ret->beta_input.connect(beta.get_input(this));
  return *ret;
}

ImageFilterGamma& ImageMixin::image_filter_gamma(DataPromise<sf::Texture>& data, Param<float> shape)
{
  auto ret = new ImageFilterGamma();
  register_block(ret);
  ret->in.connect(data);
  ret->shape_input.connect(shape.get_input(this));
  return *ret;
}

ImageFilterLogarithm& ImageMixin::image_filter_logarithm(DataPromise<sf::Texture>& data, Param<float> shape)
{
  auto ret = new ImageFilterLogarithm();
  register_block(ret);
  ret->in.connect(data);
  ret->shape_input.connect(shape.get_input(this));
  return *ret;
}

ImageFilterGaussianBlur& ImageMixin::image_filter_gaussian_blur(DataPromise<sf::Texture>& data, Param<size_t> sigma)
{
  auto ret = new ImageFilterGaussianBlur();
  register_block(ret);
  ret->in.connect(data);
  ret->sigma_input.connect(sigma.get_input(this));
  return *ret;
}

ImageCompositorAverage& ImageMixin::image_compositor_average(
  DataPromise<sf::Texture>& left,
  DataPromise<sf::Texture>& right,
  Param<float> ratio)
{
  auto ret = new ImageCompositorAverage();
  register_block(ret);
  ret->in1.connect(left);
  ret->in2.connect(right);
  ret->ratio_input.connect(ratio.get_input(this));
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
  Param<float> steepness,
  Param<size_t> radius)
{
  return image_filter_sigmoid(
    image_compositor_ratio(
      image_filter_gaussian_blur(data, 1),
      image_filter_grayscale(image_filter_gaussian_blur(data, radius))),
    steepness);
}
