#include "Points/Converters.hh"

#include <sstream>

#include <iostream>

ImageToScalarFieldConverter::ImageToScalarFieldConverter()
: in(this)
, out(this)
, scale_input(this)
{
  name = "ImageToScalarFieldConverter";
  scale_manual.set_data(2);
  scale_input.connect(scale_manual);
}

void ImageToScalarFieldConverter::compute()
{
  size_t scale = scale_input.get_data(); //for readability reasons
  {
    std::ostringstream stream;
    stream<<"Conversion scale is "<<scale;
    logger.log(Logger::Level::Verbose, stream.str().c_str());
  }

  auto img = in.get_data().copyToImage();
  auto img_size = img.getSize(); //I use it alot
  const uint8_t* img_data = img.getPixelsPtr();

  ScalarField& output = data_hook(out);
  output = ScalarField(img_size, scale);

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

      int v00=img_data[(py*img_size.x+px)*4];
      int v01=img_data[(ky*img_size.x+px)*4];
      int v10=img_data[(py*img_size.x+kx)*4];
      int v11=img_data[(ky*img_size.x+kx)*4];
      double v = v00*(1-dx)*(1-dy) + v10*dx*(1-dy) + v01*(1-dx)*(dy) + v11*dx*dy;
      uint8_t val = std::min(std::max(0, int(v)), 255);
      output.data[y*output.size.x+x] = val;
    }
  }

  progress.finish();
}
