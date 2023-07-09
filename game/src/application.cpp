// application.cpp

#include "application.hpp"

namespace gravity
{
   void application::run()
   {
      const sf::VideoMode mode{ 800, 600 };
      const sf::Uint32 flags = sf::Style::Titlebar | sf::Style::Close;
      m_window.create(mode, "gravity", flags);
      if (!m_window.isOpen() || !enter()) {
         return;
      }

      m_window.setKeyRepeatEnabled(false);
      while (m_window.isOpen()) {
         sf::Event event;
         while (m_window.pollEvent(event)) {
            if (event.type == sf::Event::MouseMoved) {
               on_mouse_move(event.mouseMove.x, event.mouseMove.y);
            }
            else if (event.type == sf::Event::MouseButtonPressed) {
               on_button_pressed(event.mouseButton.button);
            }
            else if (event.type == sf::Event::MouseButtonReleased) {
               on_button_released(event.mouseButton.button);
            }
            else if (event.type == sf::Event::KeyPressed) {
               on_key_pressed(event.key.code);
            }
            else if (event.type == sf::Event::KeyReleased) {
               on_key_released(event.key.code);
            }
            else if (event.type == sf::Event::Closed) {
               m_window.close();
            }
         }

         if (!update()) {
            m_window.close();
         }

         render();
      }

      exit();
   }

   bool application::enter()
   {
       m_address = ip_address(127, 0, 0, 1, 54321);
       m_socket.open({});
       byte_stream stream{};
       byte_stream_writer writer = byte_stream_writer(stream);
       m_connection_message.write(writer);
       m_socket.send(m_address, stream);

       return true;
   }

   void application::exit()
   {
   }

   bool application::update()
   {
      m_deltatime = m_clock.restart();
      m_apptime  += m_deltatime;
      m_ticktime += m_deltatime;
      m_simulationrate += m_deltatime;

      if (m_ticktime.asSeconds() >= 0.1f)
      {
          m_ticktime = m_clock.restart();
          m_keep_alive_message.acknowledge_number = m_game_state_message.sequence_number;
          byte_stream stream{};
          byte_stream_writer writer = byte_stream_writer(stream);
          m_keep_alive_message.write(writer);
          m_socket.send(m_address, stream);
          m_keep_alive_message.sequence_number++;
      }

      ip_address server_ip;
      byte_stream stream1{};

      if (m_socket.receive(server_ip, stream1))
      {
          byte_stream_reader reader = byte_stream_reader(stream1);
          m_game_state_message.read(reader);
          m_interpolator.add_packet(m_game_state_message, m_apptime);
      }

      if (m_simulationrate.asSeconds() >= 0.0167f)
      {
          m_simulationrate = m_clock.restart();
          m_interpolator.InterpolateEntities(m_apptime, positions);
      }

      return m_running;
   }

   void application::render()
   {
      m_window.clear(sf::Color{ 0x44, 0x55, 0x66, 0xff });
      batch.clear();

      for (int i = 0; i < 5; i++)
      {
          batch.draw_circle(positions[i], 15.0f, 20, yellow);
      }

      batch.present(m_window);
      m_window.display();
   }

   void application::on_mouse_move(int x, int y) 
   {
   }

   void application::on_key_pressed(const sf::Keyboard::Key key)
   {
   }

   void application::on_key_released(const sf::Keyboard::Key key)
   {
   }

   void application::on_button_pressed(const  sf::Mouse::Button button)
   {
   }

   void application::on_button_released(const sf::Mouse::Button button)
   {
   }
} // !gravity
