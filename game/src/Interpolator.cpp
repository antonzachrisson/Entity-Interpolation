//Interpolator.cpp

#include "Interpolator.hpp"

namespace gravity
{
	bool Interpolator::InterpolateEntities(sf::Time current_time, sf::Vector2f positions[])
	{
		if (packets[0].m_index == -1)
			return false;
		
		float time = current_time.asSeconds() - 0.2f;

		packet before;
		packet after;

		int i = 0;
		while (packets[i].m_time < time)
			i++;
		if (i < 1)
			before = packets[9];
		else
			before = packets[i - 1];
		after = packets[i];

		float alpha = (time - before.m_time) / (after.m_time - before.m_time);

		positions[0].x = before.m_msg.X1 + (after.m_msg.X1 - before.m_msg.X1) * alpha;
		positions[1].x = before.m_msg.X2 + (after.m_msg.X2 - before.m_msg.X2) * alpha;
		positions[2].x = before.m_msg.X3 + (after.m_msg.X3 - before.m_msg.X3) * alpha;
		positions[3].x = before.m_msg.X4 + (after.m_msg.X4 - before.m_msg.X4) * alpha;
		positions[4].x = before.m_msg.X5 + (after.m_msg.X5 - before.m_msg.X5) * alpha;

		positions[0].y = before.m_msg.Y1 + (after.m_msg.Y1 - before.m_msg.Y1) * alpha;
		positions[1].y = before.m_msg.Y2 + (after.m_msg.Y2 - before.m_msg.Y2) * alpha;
		positions[2].y = before.m_msg.Y3 + (after.m_msg.Y3 - before.m_msg.Y3) * alpha;
		positions[3].y = before.m_msg.Y4 + (after.m_msg.Y4 - before.m_msg.Y4) * alpha;
		positions[4].y = before.m_msg.Y5 + (after.m_msg.Y5 - before.m_msg.Y5) * alpha;

		return true;
	}

	bool Interpolator::add_packet(game_state_message msg, sf::Time time)
	{
		bool found_packet{ false };

		for (int i = 0; i < 10; i++)
		{
			if (packets[i].m_index == -1)
			{
				found_packet = true;
				packets[i].m_index = msg.sequence_number;
				packets[i].m_msg = msg;
				packets[i].m_time = time.asSeconds();
				break;
			}
		}

		if (found_packet == false)
		{
			packets[packet_override_index].m_index = -1;
			packet_override_index++;
			if (packet_override_index >= 10)
				packet_override_index = 0;
			add_packet(msg, time);
		}
		
		return true;
	}
}