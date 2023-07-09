// application.hpp

#pragma once

#include "batch.hpp"
#include "network.hpp"
#include "protocol.hpp"
#include "Interpolator.hpp"

namespace gravity
{
   class application final
   {
   public:
      application() = default;

      void run();

   private:
      bool enter();
      void exit();
      bool update();
      void render();

   private:
      void on_mouse_move(int x, int y);
      void on_key_pressed(const sf::Keyboard::Key key);
      void on_key_released(const sf::Keyboard::Key key);
      void on_button_pressed(const sf::Mouse::Button button);
      void on_button_released(const sf::Mouse::Button button);

   private:
      sf::RenderWindow m_window;
      sf::Clock        m_clock;
      sf::Time         m_apptime;
      sf::Time         m_deltatime;
      sf::Time         m_ticktime;
      sf::Time         m_simulationrate;

   private:
      bool             m_running{ true };
      bool             m_connected{ false };
      network          m_network;
      udp_socket       m_socket;
      ip_address       m_address;

      connection_message m_connection_message;
      disconnection_message m_disconnection_message;
      keep_alive_message m_keep_alive_message;
      game_state_message m_game_state_message;

      primitive_batch batch;
      sf::Color yellow = sf::Color{ 0xff, 0xcc, 0x00, 0xff };

      Interpolator m_interpolator;

      sf::Vector2f positions[5];
   };
} // !gravity
