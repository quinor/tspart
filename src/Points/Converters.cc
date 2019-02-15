#include "Points/Converters.hh"
#include <iostream>

namespace {
  sf::Color scale_color(const sf::Color& a, float val)
  {
    return sf::Color(a.r*val, a.g*val, a.b*val);
  }
}

GrayscaleImageToScalarField::GrayscaleImageToScalarField()
: in(this)
, out(this)
, scale_input(this)
{
  name = "GrayscaleImageToScalarField";
  scale_manual.set_data(2);
  scale_input.connect(scale_manual);
}

void GrayscaleImageToScalarField::compute()
{
  size_t scale = scale_input.get_data(); //for readability reasons

  logger.log(Logger::Level::Verbose)<<"Conversion scale is "<<scale;

  auto img = in.get_data().copyToImage();
  auto img_size = img.getSize(); //I use it alot
  const uint8_t* img_data = img.getPixelsPtr();

  auto& output = data_hook(out);
  output = ScalarField<uint8_t>(img_size, scale);

  auto progress = get_logger().progress(Logger::Level::Verbose, "Upscale", output.size.y);
  for (size_t y = 0; y<output.size.y; y++)
  {
    if (y%int(output.size.y/10) == 0)
      progress.update(y);
    for (size_t x = 0; x<output.size.x; x++)
    {
      int px = x/scale;
      int py = y/scale;
      int kx = std::min(px+1, (int)img_size.x-1);
      int ky = std::min(py+1, (int)img_size.y-1);
      float dx = (x%scale)/float(scale);
      float dy = (y%scale)/float(scale);

      uint8_t v00=img_data[(py*img_size.x+px)*4];
      uint8_t v01=img_data[(ky*img_size.x+px)*4];
      uint8_t v10=img_data[(py*img_size.x+kx)*4];
      uint8_t v11=img_data[(ky*img_size.x+kx)*4];
      double v = v00*(1-dx)*(1-dy) + v10*dx*(1-dy) + v01*(1-dx)*(dy) + v11*dx*dy;
      uint8_t val = std::min(std::max(0, int(v)), 255);
      output.data[y*output.size.x+x] = val;
    }
  }

  progress.finish();
}


ColorImageToScalarField::ColorImageToScalarField()
: in(this)
, out(this)
, scale_input(this)
{
  name = "ColorImageToScalarField";
  scale_manual.set_data(2);
  scale_input.connect(scale_manual);
}

void ColorImageToScalarField::compute()
{
  size_t scale = scale_input.get_data(); //for readability reasons

  logger.log(Logger::Level::Verbose)<<"Conversion scale is "<<scale;

  auto img = in.get_data().copyToImage();
  auto img_size = img.getSize(); //I use it alot
  const sf::Color* img_data = reinterpret_cast<const sf::Color*>(img.getPixelsPtr());

  auto& output = data_hook(out);
  output = ScalarField<sf::Color>(img_size, scale);

  auto progress = get_logger().progress(Logger::Level::Verbose, "Upscale", output.size.y);
  for (size_t y = 0; y<output.size.y; y++)
  {
    if (y%int(output.size.y/10) == 0)
      progress.update(y);
    for (size_t x = 0; x<output.size.x; x++)
    {
      int px = x/scale;
      int py = y/scale;
      int kx = std::min(px+1, (int)img_size.x-1);
      int ky = std::min(py+1, (int)img_size.y-1);
      float dx = (x%scale)/float(scale);
      float dy = (y%scale)/float(scale);

      sf::Color v00=img_data[(py*img_size.x+px)];
      sf::Color v01=img_data[(ky*img_size.x+px)];
      sf::Color v10=img_data[(py*img_size.x+kx)];
      sf::Color v11=img_data[(ky*img_size.x+kx)];
      sf::Color v =
        scale_color(v00, (1-dx)*(1-dy)) +
        scale_color(v10, dx*(1-dy)) +
        scale_color(v01, (1-dx)*(dy)) +
        scale_color(v11, dx*dy);
      output.data[y*output.size.x+x] = v;
    }
  }

  progress.finish();
}


ScalarFieldMassPrefixSum::ScalarFieldMassPrefixSum()
: in(this)
, out(this)
{
  name = "ScalarFieldMassPrefixSum";
}

void ScalarFieldMassPrefixSum::compute()
{
  auto& input = in.get_data();
  auto& output = data_hook(out);
  output = ScalarField<WeightedElement>(input.size/(unsigned int)(input.scale), input.scale);

  output.data[-1] = {0, 0, 0, 0};
  for (size_t y=0; y<input.size.y; y++)
    for (size_t x=0; x<input.size.x; x++)
    {
      size_t pos = y*input.size.x+x;
      output.data[pos] = output.data[pos-1] + WeightedElement(x, y)*input.data[pos];
    }
}


ScalarFieldColorPrefixSum::ScalarFieldColorPrefixSum()
: in(this)
, out(this)
{
  name = "ScalarFieldColorPrefixSum";
}

void ScalarFieldColorPrefixSum::compute()
{
  auto& input = in.get_data();
  auto& output = data_hook(out);
  output = ScalarField<WeightedElement>(input.size/(unsigned int)(input.scale), input.scale);

  output.data[-1] = {0, 0, 0, 0};
  for (size_t y=0; y<input.size.y; y++)
    for (size_t x=0; x<input.size.x; x++)
    {
      size_t pos = y*input.size.x+x;
      auto col = input.data[pos];
      output.data[pos] = output.data[pos-1] + WeightedElement(col.r, col.g, col.b);
    }
}
