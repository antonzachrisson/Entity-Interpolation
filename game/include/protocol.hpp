// protocol.hpp

#pragma once

#include "network.hpp" // byte_stream_reader, byte_stream_writer, ..

namespace gravity
{
   enum class message_type
   {
      connection,
      disconnect,
      keep_alive,
      game_state,
   };

   struct connection_message {
      connection_message()
         : type(uint8(message_type::connection))
      {
      }

      inline bool read(byte_stream_reader &stream)
      {
         return serialize(stream);
      }

      inline bool write(byte_stream_writer &stream)
      {
         return serialize(stream);
      }

      template <typename stream_type>
      bool serialize(stream_type &stream)
      {
         bool success = true;
         success &= stream.serialize(type);
         return success;
      }

      uint8 type{ 0 };
   };

   struct disconnection_message {
      disconnection_message()
         : type(uint8(message_type::disconnect))
      {
      }

      inline bool read(byte_stream_reader &stream)
      {
         return serialize(stream);
      }

      inline bool write(byte_stream_writer &stream)
      {
         return serialize(stream);
      }

      template <typename stream_type>
      bool serialize(stream_type &stream)
      {
         bool success = true;
         success &= stream.serialize(type);
         return success;
      }

      uint8 type{ 0 };
   };

   struct keep_alive_message {
      keep_alive_message()
         : type(uint8(message_type::keep_alive))
      {
      }

      inline bool read(byte_stream_reader& stream)
      {
          return serialize(stream);
      }

      inline bool write(byte_stream_writer& stream)
      {
          return serialize(stream);
      }

      template <typename stream_type>
      bool serialize(stream_type& stream)
      {
          bool success = true;
          success &= stream.serialize(type);
          success &= stream.serialize(sequence_number);
          success &= stream.serialize(acknowledge_number);
          return success;
      }

      uint16 sequence_number{ 0 };
      uint16 acknowledge_number{ 0 };
      uint8 type{ 0 };
   };

   struct game_state_message {
      game_state_message()
         : type(uint8(message_type::game_state))
      {
      }

      inline bool read(byte_stream_reader& stream)
      {
          return serialize(stream);
      }

      inline bool write(byte_stream_writer& stream)
      {
          return serialize(stream);
      }

      template <typename stream_type>
      bool serialize(stream_type& stream)
      {
          bool success = true;
          success &= stream.serialize(type);
          success &= stream.serialize(sequence_number);
          success &= stream.serialize(acknowledge_number);
          success &= stream.serialize(X1);
          success &= stream.serialize(Y1);
          success &= stream.serialize(X2);
          success &= stream.serialize(Y2);
          success &= stream.serialize(X3);
          success &= stream.serialize(Y3);
          success &= stream.serialize(X4);
          success &= stream.serialize(Y4);
          success &= stream.serialize(X5);
          success &= stream.serialize(Y5);
          return success;
      }

      uint16 sequence_number{ 0 };
      uint16 acknowledge_number{ 0 };
      float X1{ 0 };
      float Y1{ 0 };
      float X2{ 0 };
      float Y2{ 0 };
      float X3{ 0 };
      float Y3{ 0 };
      float X4{ 0 };
      float Y4{ 0 };
      float X5{ 0 };
      float Y5{ 0 };
      uint8 type{ 0 };
   };
} // !gravity
