#include <SFML/Graphics.hpp>
#include <cstdint>
#include <cmath>
#include <sstream>

#include "HilbertGenerator.hh"
#include "Visualizer.hh"
#include "Constants.hh"
#include "Logger.hh"
#include "Config.hh"

struct MassElement
{
  unsigned long long wx;
  unsigned long long wy;
  unsigned long long ws;

  MassElement operator + (const MassElement& other)
  {
    return {wx+other.wx, wy+other.wy, ws+other.ws};
  }

  MassElement operator += (const MassElement& other)
  {
    wx+=other.wx;
    wy+=other.wy;
    ws+=other.ws;
    return *this;
  }

  sf::Vector2f middle()
  {
    return {float(wx)/ws, float(wy)/ws};
  }
};


sf::Image img;
uint8_t* data;
sf::Vector2u img_size, img2_size;
uint8_t* img2_ptr;
MassElement* prefix_sums;

void load_image(const char* fname)
{
  get_logger().enter(Logger::Level::Info, "Image loading");

  sf::Image tmp;
  if(!tmp.loadFromFile(fname))
  {
    get_logger().log(Logger::Level::Error, "Invalid image file!");
    exit(-1);
  }

  get_logger().log(Logger::Level::Verbose, "Maximizing");

  sf::Vector2f s = sf::Vector2f(tmp.getSize());
  float scale = 1500.f/std::max(s.x, s.y);

  {
    std::ostringstream stream;
    stream<<"Size before maximizing: ("<<s.x<<", "<<s.y<<")";
    get_logger().log(Logger::Level::Verbose, stream.str().c_str());
  }

  s*=scale;

  {
    std::ostringstream stream;
    stream<<"Size after maximizing: ("<<s.x<<", "<<s.y<<")";
    get_logger().log(Logger::Level::Verbose, stream.str().c_str());
  }

  
  sf::RenderTexture rtex;
  rtex.create(s.x, s.y);

  sf::Texture tex;
  tex.loadFromImage(tmp);
  sf::Sprite sp(tex);
  sp.scale({scale, scale});

  rtex.draw(sp);
  rtex.display();

  img = rtex.getTexture().copyToImage();
  img_size = img.getSize();

  get_logger().exit();
}

void apply_filters()
{
  get_logger().enter(Logger::Level::Info, "Applying filters");
  auto img_ptr = img.getPixelsPtr();
  data = new uint8_t[img_size.x*img_size.y+7];

  get_logger().log(Logger::Level::Verbose, "Applying grayscale (to [0...255])");
  {
    std::ostringstream stream;
    stream
      <<"Applying sigmoid function: atan((x + "
      <<sigmoid_beta<<") * "<<sigmoid_alpha<<")";
    get_logger().log(Logger::Level::Verbose, stream.str().c_str());
  }

  auto progress = get_logger().progress(
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
      v = atan((v+sigmoid_beta)/sigmoid_alpha)/M_PI+0.5;
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
  get_logger().exit();
}

void upscale()
{
  get_logger().enter(Logger::Level::Info, "Upscaling");
  img2_size = img_size*scale_factor;
  img2_ptr = new uint8_t[img2_size.x*img2_size.y+7];

  {
    std::ostringstream stream;
    stream<<"Upscaling with factor "<<scale_factor;
    get_logger().log(Logger::Level::Verbose, stream.str().c_str());
  }

  auto progress = get_logger().progress(Logger::Level::Info, "Upscale", img2_size.y);
  for (size_t y = 0; y<img2_size.y; y++)
  {
    if (y%int(img2_size.y/10) == 0)
      progress.update(y);
    for (size_t x = 0; x<img2_size.x; x++)
    {
      int px = x/scale_factor;
      int py = y/scale_factor;
      float dx = (x%scale_factor)/float(scale_factor);
      float dy = (y%scale_factor)/float(scale_factor);

      int v00=data[py*img_size.x+px];
      int v01=data[(py+1)*img_size.x+px];
      int v10=data[py*img_size.x+px+1];
      int v11=data[(py+1)*img_size.x+px+1];
      double v = v00*(1-dx)*(1-dy) + v10*dx*(1-dy) + v01*(1-dx)*(dy) + v11*dx*dy;
      uint8_t val = std::min(std::max(0, int(v)), 255);
      img2_ptr[y*img2_size.x+x] = val;
    }
  }
  progress.finish();

  delete [] data;

  get_logger().exit();
}

void calculate_prefix_sums()
{
  get_logger().enter(Logger::Level::Info, "Calculating prefix sums");
  MassElement* prefix_sums = new MassElement[img2_size.x*img2_size.y+7];
  auto progress = get_logger().progress(
    Logger::Level::Info,
    "Prefix summarize",
    img2_size.y
  );
  for (size_t y = 0; y<img2_size.y; y++)
  {
    if (y%int(img2_size.y/10) == 0)
      progress.update(y);
    for (size_t x = 0; x<img2_size.x; x++)
    {
      unsigned long long mass = img2_ptr[y*img2_size.x+x];
      prefix_sums[y*img2_size.x+x] += {x*mass, y*mass, mass};
      prefix_sums[y*img2_size.x+x+1] = prefix_sums[y*img2_size.x+x];
    }
  }
  progress.finish();

  get_logger().exit();
}

std::vector<sf::Vector2f> create_base_points ()
{
  get_logger().enter(Logger::Level::Info, "Creating basic points");

  int order = 0;
  while ((unsigned)(1<<order) < std::max(img2_size.x, img2_size.y))
    order++;

  HilbertGenerator generator(order);
  std::vector<sf::Vector2f> pts;

  auto progress = get_logger().progress(
    Logger::Level::Info,
    "Progress through Hilbert curve",
    1<<(2*order)
  );
  int val = 0;
  for (int i=0; i<(1<<(2*order)); i++)
  {
    if (i%(1<<(2*order-4)) == 0)
      progress.update(i);
    auto p = generator.get();
    generator.next();
    if (p.x>=(int)img2_size.x || p.y>=(int)img2_size.y)
      continue;
    val+=img2_ptr[p.y*img2_size.x+p.x];
    if (val > threshold)
    {
      val-=threshold;
      pts.emplace_back(p.x, p.y);
    }
  }
  progress.finish();

  get_logger().exit();
  return pts;
}

int main (int argc, char** argv)
{
  get_logger().set_log_level(Logger::Level::Max);



  if (argc!=2)
  {
    get_logger().log(Logger::Level::Error, "Invalid number of arguments!");
    return 0;
  }

  load_image(argv[1]);

  apply_filters();

  upscale();

  calculate_prefix_sums();

  auto pts = create_base_points();

  visualize_polyline(img2_size, pts);
}
