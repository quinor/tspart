#include "utils/tinyfiledialogs.h"
#include "FunctionAPI/Graph.hh"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <TGUI/TGUI.hpp>

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <memory>
#include <cmath>

const int PANEL_HEIGHT = 120;

std::shared_ptr<std::string> getImageFile()
{
    std::vector<const char*> filePatterns = {
            "*.png", "*.jpg", "*.bmp"
    };

    auto result = tinyfd_openFileDialog(
            "Choose an image",
            "",
            filePatterns.size(),
            filePatterns.data(),
            "Images",
            0
    );

    if (result == nullptr)
        return nullptr;
    return std::make_shared<std::string>(result);
}

std::shared_ptr<std::string> saveImageFile()
{
    std::vector<const char*> filePatterns = {
            "*.png", "*.jpg", "*.bmp"
    };

    auto result = tinyfd_saveFileDialog(
            "Save an image",
            "",
            filePatterns.size(),
            filePatterns.data(),
            "SVG file"
    );

    if (result == nullptr)
        return nullptr;
    return std::make_shared<std::string>(result);
}


tgui::VerticalLayout::Ptr named_column(
  std::vector<std::pair<const char*, tgui::Widget::Ptr>> widgets,
  int whitespace=0)
{
  auto layout = tgui::VerticalLayout::create();
  for (auto& p : widgets)
  {
    auto row = tgui::HorizontalLayout::create();
    tgui::Label::Ptr label = tgui::Label::create();
    label->setText(p.first);
    label->setTextSize(10);
    label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Right);
    label->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
    row->add(label);
    row->add(p.second);
    row->setRatio(0, 0.35);
    layout->add(row);
    layout->addSpace(0.1);
  }
  layout->addSpace(1.1f*whitespace);
  return layout;
}

tgui::HorizontalLayout::Ptr slider(int from, int def, int to, std::function<void(int)> callback)
{
  auto lt = tgui::HorizontalLayout::create();

  tgui::Slider::Ptr slider = tgui::Slider::create();
  tgui::EditBox::Ptr box = tgui::EditBox::create();

  auto update = [=](int val)->void
  {
    int new_val = std::min(std::max(from, val), to);
    if (int(slider->getValue()) != new_val)
      slider->setValue(new_val);
    if (val >= from && box->getText() != std::to_string(new_val))
      box->setText(std::to_string(new_val));
    callback(new_val);
  };

  box->setInputValidator(tgui::EditBox::Validator::UInt);
  box->connect("TextChanged", [=]() {
    std::string val = box->getText();
    if (val.size() == 0)
      return;
    int r = std::stoi(val);
    if (r < from)
      return;
    update(r);
  });

  slider->setMinimum(from);
  slider->setMaximum(to);
  slider->connect("ValueChanged", [=]() {
    update(slider->getValue());
  });

  update(def);

  lt->add(slider);
  lt->add(box);
  lt->setRatio(0, 0.7);
  lt->setRatio(1, 0.3);
   return lt;
}


void create_app(tgui::Gui& gui, Graph<ImageMixin, PointsMixin>& gr)
{
  auto& in_filename = gr.input<std::string>();
  auto& out_filename = gr.input<std::string>();
  auto& size = gr.input<size_t>();

  auto& gauss = gr.input<size_t>();
  auto& log = gr.input<float>();
  auto& sigm = gr.input<float>();

  auto& fill = gr.input<size_t>();


  auto& in = gr.image_maximizer(gr.image_loader(in_filename), size);
  auto& pre = gr.image_filter_sigmoid(
    gr.image_filter_logarithm(
      gr.image_filter_grayscale(
        gr.image_compositor_absolute_difference(
          gr.image_filter_gaussian_blur(in, 1),
          gr.image_filter_gaussian_blur(in, gauss)
        )
      ),
      log
    ),
    sigm
  );

  auto& inv = gr.grayscale_image_to_scalar_field(pre, 2);
  auto& pts = gr.n_voronoi_relaxation(
    gr.points_generator(inv, fill), // 25-1000
    gr.scalar_field_mass_prefix_sum(inv),
    5);

  auto& voronoi = gr.points_voronoi_delaunay(pts);
  auto& color_field = gr.scalar_field_color_prefix_sum(gr.color_image_to_scalar_field(in));
  auto& colors = gr.points_color_averager(pts, voronoi.voronoi, color_field);
  auto& color_voronoi = gr.polygon_visualizer(pts, voronoi.voronoi, colors);

  auto& saver = gr.image_saver(color_voronoi, out_filename);

  auto& in_pic = gr.output<sf::Texture>(in);
  auto& pre_pic = gr.output<sf::Texture>(pre);
  auto& out_pic = gr.output<sf::Texture>(color_voronoi);


  auto pic = tgui::Picture::create("misc/empty.png");
  pic->setPosition(0, PANEL_HEIGHT);
  pic->setSize(tgui::bindWidth(gui), tgui::bindHeight(gui)-PANEL_HEIGHT);
  pic->getRenderer()->getTexture().setSmooth(true);
  gui.add(pic);


  auto bar = tgui::HorizontalLayout::create();
  bar->setSize(tgui::bindWidth(gui), PANEL_HEIGHT);

  // file choosers
  tgui::Button::Ptr in_file = tgui::Button::create();

  auto new_file = [=, &gui, &in_filename, &in_pic](std::string path) -> void
  {
      in_file->setText(path);
      in_filename.set_data(path);
      auto str = sf::String(path);
      auto tex = tgui::Texture(path);
      auto size = tex.getImageSize();
      float ratio = size.x/size.y;

      auto width = tgui::bindWidth(gui);
      auto height = tgui::bindHeight(gui)-PANEL_HEIGHT;

      auto new_width = height*ratio;
      auto new_height = height;

      pic->getRenderer()->setTexture(in_pic.get_data());
      pic->getRenderer()->getTexture().setSmooth(true);
      pic->setSize(new_width, new_height);
      pic->setPosition((width - new_width)/2, (height-new_height)/2+PANEL_HEIGHT);
  };

  in_file->connect("Pressed", [=]() {
    auto path = getImageFile();
    if (path != nullptr)
      new_file(*path);
  });

  tgui::Button::Ptr out_file = tgui::Button::create();
  out_file->setText("Save file");
  out_file->connect("Pressed", [&, out_file]() {
      auto path = saveImageFile();
      if (path != nullptr)
      {
          out_file->setText(*path);
          out_filename.set_data(*path);
      }
  });
  out_file->setText("out.jpg");
  out_filename.set_data("out.jpg");

  auto size_slider = slider(100, 2000, 4000, [&](int val)->void
  {
    size.set_data(val);
  });

  bar->add(named_column(
  {
    {"Input file", in_file},
    {"Output file", out_file},
    {"Size", size_slider}
  }));

  // preprocessing params
  auto details = slider(0, 7, 15, [&](int val)->void //pow(1.5, x)
  {
    gauss.set_data(pow(1.5, 15-val));
  });
  auto brightness = slider(0, 7, 20, [&](int val)->void //pow(1.5, x)
  {
    log.set_data(pow(2, val));
  });
  auto contrast = slider(0, 10, 20, [&](int val)->void //pow(2,x)
  {
    sigm.set_data(pow(1.5, (10-val)));
  });

  bar->add(named_column(
  {
    {"Details", details},
    {"Brightness", brightness},
    {"Contrast", contrast}
  }));

  // algo params
  auto density = slider(0, 5, 10, [&](int val)->void //int(12*pow(1.5, -10x))
  {
    fill.set_data(int(50*pow(1.5, 10-val)));
  });

  tgui::Button::Ptr fire = tgui::Button::create();
  tgui::Button::Ptr prev = tgui::Button::create();

  fire->setText("Draw!");
  fire->connect("Pressed", [&, pic]()
  {
    saver.update();
    // view_saver.update();
    pic->getRenderer()->setTexture("misc/empty.png");
    pic->getRenderer()->setTexture(out_pic.get_data());
    pic->getRenderer()->getTexture().setSmooth(true);
  });

  prev->setText("Preview");
  prev->connect("Pressed", [&, pic]()
  {
    // pre_saver.update();
    pic->getRenderer()->setTexture("misc/empty.png");
    pic->getRenderer()->setTexture(pre_pic.get_data());
    pic->getRenderer()->getTexture().setSmooth(true);
});

  bar->add(named_column(
  {
    {"Density", density},
    {"", prev},
    {"", fire}
  },
  1));

  new_file("res/klaudia.jpg");

  tgui::Panel::Ptr panel = tgui::Panel::create();
  panel->setPosition(0, 0);
  panel->setSize(tgui::bindWidth(gui), PANEL_HEIGHT);
  panel->add(bar);
  gui.add(panel);
}


int main()
{
  sf::RenderWindow window(
    sf::VideoMode(1280, 720),
    "TSPArt",
    sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close
  );
  window.setVerticalSyncEnabled(true);

  auto graph = Graph<ImageMixin, PointsMixin>();
  graph.logger.set_log_level(Logger::Level::Verbose);
  tgui::Gui gui(window);
  auto theme = tgui::Theme{"misc/TransparentGrey.txt"};
  tgui::Theme::setDefault(&theme);

  create_app(gui, graph);

  sf::Event event;
  sf::Clock clock;

  while(window.isOpen())
  {
    while(window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        return 0;
      else if (event.type == sf::Event::Resized)
      {
        sf::View newView(sf::FloatRect(0.f, 0.f, event.size.width, event.size.height));
        window.setView(newView);
        gui.setView(newView);
      }
      gui.handleEvent(event);
    }

    window.clear();
    gui.draw();
    window.display();
  }
  return 0;
}
