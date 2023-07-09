// network.hpp

#pragma once

#include <type_traits> // std::is_fundamental
#include <cstring>     // std::memcpy
#include <vector>
#include <cassert>

namespace gravity
{
   using uint64 = unsigned long long;
   using  int64 =   signed long long;
   using uint32 = unsigned int;
   using  int32 =   signed int;
   using uint16 = unsigned short;
   using  int16 =   signed short;
   using  uint8 = unsigned char;
   using   int8 =   signed char;

   struct byte_stream {
      byte_stream() = default;

      int  m_size{ 0 };
      char m_buffer[1024] = {};
   };

   struct byte_stream_writer {
      byte_stream_writer(byte_stream &stream)
         : m_stream(stream)
         , m_cursor(stream.m_buffer + stream.m_size)
         , m_end(stream.m_buffer + sizeof(stream.m_buffer))
      {
      }

      template <typename T>
      bool serialize(T value) {
         static_assert(std::is_fundamental_v<T>, "T needs to be a fundamental datatype!");
         if ((m_cursor + sizeof(T)) >= m_end) {
            return false;
         }

         std::memcpy(m_cursor, &value, sizeof(T));
         m_cursor += sizeof(T);
         m_stream.m_size = int32_t(m_cursor - m_stream.m_buffer);
         return true;
      }

      byte_stream &m_stream;
      char        *m_cursor{ nullptr };
      char        *m_end{ nullptr };
   };

   struct byte_stream_reader {
      byte_stream_reader(byte_stream &stream)
         : m_stream(stream)
         , m_cursor(stream.m_buffer)
         , m_end(stream.m_buffer + stream.m_size)
      {
      }

      inline uint8 peek() const {
         return m_cursor[0];
      }

      inline bool has_data() const {
         return m_cursor < m_end;
      }

      template <typename T>
      bool serialize(T &value) {
         static_assert(std::is_fundamental_v<T>, "T needs to be a fundamental datatype!");
         if ((m_cursor + sizeof(T)) > m_end) {
            return false;
         }

         std::memcpy(&value, m_cursor, sizeof(T));
         m_cursor += sizeof(T);
         return true;
      }

      byte_stream &m_stream;
      char        *m_cursor{ nullptr };
      char        *m_end{ nullptr };
   };

   struct network
   {
      struct error
      {
         error() = default;
         error(int code);
         error(const error &rhs);

         bool  is_critical() const;
         int32 as_code() const;
         const char *as_string() const;

         const int32 m_code{ 0 };
      };

      static error get_last_error();

      network();
      ~network();
   };

   struct ip_address
   {
      static bool get_local_addresses(std::vector<ip_address> &addresses);

      static constexpr uint32 k_any_host = 0;
      static constexpr uint16 k_any_port = 0;

      ip_address() = default;
      ip_address(const ip_address &rhs);
      ip_address(const char *name, uint16 port = k_any_port);
      ip_address(const uint32 host, uint16 port = k_any_port);
      ip_address(uint8 a, uint8 b, uint8 c, uint8 d, uint16 port = k_any_port);

      bool operator==(const ip_address &rhs) const;
      bool operator!=(const ip_address &rhs) const;

      uint8 a() const;
      uint8 b() const;
      uint8 c() const;
      uint8 d() const;

      uint32 m_host{ k_any_host };
      uint16 m_port{ k_any_port };
   };

   struct udp_socket
   {
      udp_socket();

      bool valid() const;
      bool open(const ip_address &address);
      void close();

      bool send(const ip_address &address, const byte_stream &stream);
      bool receive(ip_address &address, byte_stream &stream);

      bool get_address(ip_address &address);

      uint64 m_handle;
   };
} // !gravity

