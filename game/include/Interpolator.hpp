//Inerpolator.hpp

#pragma once

#include "protocol.hpp"
#include <initializer_list>
#include <SFML/Graphics.hpp>

namespace gravity
{
	class Interpolator final
	{
	public:
		Interpolator() = default;

		bool InterpolateEntities(sf::Time current_time, sf::Vector2f positions[]);
		bool add_packet(game_state_message msg, sf::Time time);

	private:
		struct packet
		{
			packet() {};
			packet(game_state_message msg, sf::Time time)
			{
				m_msg = msg;
				m_index = msg.sequence_number;
				m_time = time.asSeconds();
			}

			int m_index{ -1 };
			game_state_message m_msg;
			float m_time{ 0.0f };
		};

		packet packets[10];
		int packet_override_index{ 0 };
	};
}