
#include <TRandom.h>

#include <TGUI/TGUI.hpp>
#include <stdexcept>

#include "TH1F.h"
#include "geometrical_entities.h"
#include "geometrical_methods.h"

Setup::Setup(const tgui::Gui &gui)
    : m_y_0(-gui.get<tgui::EditBoxSlider>("y_0")->getValue()),
      m_theta_0(-gui.get<tgui::EditBoxSlider>("theta_0")->getValue()),
      m_l(gui.get<tgui::EditBoxSlider>("l")->getValue()),
      m_r_1(gui.get<tgui::EditBoxSlider>("r_1")->getValue()),
      m_r_2(gui.get<tgui::EditBoxSlider>("r_2")->getValue()) {}

Setup::Setup(const float &y_0, const float &theta_0, const float &l,
             const float &r_1, const float &r_2)
    : m_y_0(-y_0), m_theta_0(-theta_0), m_l(l), m_r_1(r_1), m_r_2(r_2) {}

// making system drawable
void Setup::makeDrawableSystem(sf::CircleShape &ball, sf::VertexArray &top_line,
                               sf::VertexArray &bottom_line,
                               const float &scale) {
  Setup s = *this;

  // creation of top line
  top_line[0].position = sf::Vector2f(0, scale * s.get_r_1());
  top_line[0].color = sf::Color::White;
  top_line[1].position = sf::Vector2f(scale * s.get_l(), scale * s.get_r_2());
  top_line[1].color = sf::Color::White;

  // creation of the bottom line
  bottom_line[0].position = sf::Vector2f(0, -scale * s.get_r_1());
  bottom_line[0].color = sf::Color::White;
  bottom_line[1].position =
      sf::Vector2f(scale * s.get_l(), -scale * s.get_r_2());
  bottom_line[1].color = sf::Color::White;

  // creation of the ball
  ball.setPosition(0, scale * s.get_y_0());
  ball.setFillColor(sf::Color::Green);
  ball.setOrigin(ball.getRadius(), ball.getRadius());
}

// running the program
void Setup::run(sf::CircleShape &ball, std::vector<Point> &animation_positions,
                Speed_and_scale &speed_and_scale, tgui::Gui &gui,
                sf::VertexArray &top_line, sf::VertexArray &bottom_line) {
  animation_positions.clear();
  Setup setup = (gui);

  // rescaling speed and scale  in base  of the  higest parameter
  float scale_reference =
      std::max({setup.get_l() / 30, setup.get_r_1() / 8, setup.get_r_2() / 8});
  speed_and_scale.scale = 25 / scale_reference;
  speed_and_scale.speed = 1.5f * scale_reference;
  setup.makeDrawableSystem(ball, top_line, bottom_line, speed_and_scale.scale);

  System system(setup);

  Point last_interception{0, 0};
  Point new_interception{calculateFirstHit(setup.get_l(), system)};

  Angle_and_point result =
      getFinalPoint(new_interception, last_interception, system, setup.get_l(),
                    animation_positions, speed_and_scale);
  if (result.y == 0 && result.theta == 180) {
    gui.get<tgui::EditBox>("Final angle")->setText("Invalid throw");
    gui.get<tgui::EditBox>("Final point")->setText("Invalid throw");
  } else {
    gui.get<tgui::EditBox>("Final angle")
        ->setText(std::to_string(result.theta));
    gui.get<tgui::EditBox>("Final point")
        ->setText(std::to_string(setup.get_l()) + ";" +
                  std::to_string(result.y));
  }
}

// drowing the histograms of the normal distributions
void Setup::getNormalDistribution(tgui::Gui &gui) {
  // defining the inputs from the sliders
  float sigma_y_0 = gui.get<tgui::EditBoxSlider>("sigma y_0")->getValue();
  float sigma_theta_0 =
      gui.get<tgui::EditBoxSlider>("sigma theta_0")->getValue();
  int n = static_cast<int>(gui.get<tgui::EditBoxSlider>("n")->getValue());

  // creation of the histograms
  TH1F h1("Legend", "Final points", 20, -m_r_2, m_r_2);
  TH1F h2("Legend", "Final angles", 100, -90, 90);

  // filling the histograms n times
  for (int i{0}; i < n; i++) {
    float theta = static_cast<float>(gRandom->Gaus(m_theta_0, sigma_theta_0));
    float y = static_cast<float>(gRandom->Gaus(m_y_0, sigma_y_0));

    Setup setup_gaus{y, theta, m_l, m_r_1,
                     m_r_2};  // here the setup is flipped twice
    System system(setup_gaus);
    if (std::abs(y) < m_r_1) {
      Point last_interception{0, 0};
      Point new_interception{calculateFirstHit(setup_gaus.get_l(), system)};

      nThrowsSimulation(new_interception, last_interception, system,
                        setup_gaus.get_l(), h1, h2);
    }
  }

  // drawing histograms and final outputs on the Gui
  TCanvas canvas{};
  canvas.Divide(2, 1);
  canvas.cd(1);
  h1.Draw();
  canvas.cd(2);
  h2.Draw();
  canvas.Print("histos.png");

  gui.get<tgui::EditBox>("Position mean")
      ->setText(std::to_string(h1.GetMean()));
  gui.get<tgui::EditBox>("Position stddev")
      ->setText(std::to_string(h1.GetRMS()));
  gui.get<tgui::EditBox>("Position skewedness")
      ->setText(std::to_string(h1.GetSkewness()));
  gui.get<tgui::EditBox>("Position kurtosis")
      ->setText(std::to_string(h1.GetKurtosis()));

  gui.get<tgui::EditBox>("Angle mean")->setText(std::to_string(h2.GetMean()));
  gui.get<tgui::EditBox>("Angle stddev")->setText(std::to_string(h2.GetRMS()));
  gui.get<tgui::EditBox>("Angle skewedness")
      ->setText(std::to_string(h2.GetSkewness()));
  gui.get<tgui::EditBox>("Angle kurtosis")
      ->setText(std::to_string(h2.GetKurtosis()));
}
