#include "ImageProcessor.hh"
#include <sstream>
#include <cmath>


uint8_t* UpscaleImageProcessor::apply_filters(const sf::Image& img)
{
  logger.enter(Logger::Level::Info, "Applying filters");
  auto img_size = img.getSize();
  auto img_ptr = img.getPixelsPtr();
  auto data = new uint8_t[img_size.x*img_size.y+7];

  logger.log(Logger::Level::Verbose, "Applying grayscale (to [0...255])");
  {
    std::ostringstream stream;
    stream
      <<"Applying sigmoid function: atan((x + "
      <<config.sigmoid_beta<<") * "<<config.sigmoid_alpha<<")";
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }

  auto progress = logger.progress(
    Logger::Level::Info,
    "Filters",
    img_size.x*img_size.y
  );

  for (size_t i=0; i<img_size.x*img_size.y; i++)
  {
    if (i%(int(img_size.x*img_size.y)/10) == 0)
      progress.update(i);
    double v = 0.3*img_ptr[i*4] + 0.59*img_ptr[i*4+1] + 0.11*img_ptr[i*4+2];
    if (true)
      v = atan((v+config.sigmoid_beta)/config.sigmoid_alpha)/M_PI+0.5;
    else
      v = v/255.0;
    v = pow(v, 0.666);
    v = 1-v;
    v = (v-0.05)/0.95;
    v*=255;
    uint8_t val = std::min(std::max(0, int(v)), 255);
    data[i] = val;
  }
  progress.finish();
  logger.exit();
  return data;
}


std::pair<sf::Vector2u, uint8_t*>
  UpscaleImageProcessor::upscale(sf::Vector2u img_size, const uint8_t* data)
{
  get_logger().enter(Logger::Level::Info, "Upscaling");
  sf::Vector2u img2_size = img_size*config.scale_factor;
  auto img2_ptr = new uint8_t[img2_size.x*img2_size.y+7];

  {
    std::ostringstream stream;
    stream<<"Upscaling with factor "<<config.scale_factor;
    get_logger().log(Logger::Level::Verbose, stream.str().c_str());
  }

  auto progress = get_logger().progress(Logger::Level::Info, "Upscale", img2_size.y);
  for (size_t y = 0; y<img2_size.y; y++)
  {
    if (y%int(img2_size.y/10) == 0)
      progress.update(y);
    for (size_t x = 0; x<img2_size.x; x++)
    {
      int px = x/config.scale_factor;
      int py = y/config.scale_factor;
      int kx = std::min(px+1, (int)img_size.x-1);
      int ky = std::min(py+1, (int)img_size.y-1);
      float dx = (x%config.scale_factor)/float(config.scale_factor);
      float dy = (y%config.scale_factor)/float(config.scale_factor);

      int v00=data[py*img_size.x+px];
      int v01=data[ky*img_size.x+px];
      int v10=data[py*img_size.x+kx];
      int v11=data[ky*img_size.x+kx];
      double v = v00*(1-dx)*(1-dy) + v10*dx*(1-dy) + v01*(1-dx)*(dy) + v11*dx*dy;
      uint8_t val = std::min(std::max(0, int(v)), 255);
      img2_ptr[y*img2_size.x+x] = val;
    }
  }

  progress.finish();
  get_logger().exit();
  return {img2_size, img2_ptr};
}


std::pair<sf::Vector2u, uint8_t*> UpscaleImageProcessor::process(const sf::Image& img)
{
  logger.enter(Logger::Level::Info, "Processing image");

  auto data = apply_filters(img);

  auto out = upscale(img.getSize(), data);

  delete[] data;
  logger.exit();
  return out;
}
