// gameplay.hpp

#pragma once

#include <vector>

namespace sf { class Time; class RenderTarget; }

namespace gravity
{
   struct Component {
      virtual ~Component() = default;
      virtual void update(const sf::Time &deltatime) = 0;
      virtual void render(sf::RenderTarget &target) = 0;
   };

   struct Entity {
      Entity() = default;
      ~Entity();

      void update(const sf::Time &deltatime);
      void render(sf::RenderTarget &target);

      void add_component(Component *component);
      template <typename T, typename ...Ts>
      void add_component(Ts &&...ts) {
         add_component(new Component(std::forward<Ts>(ts)...));
      }

   private:
      std::vector<Component *> m_components;
   };
} // !gravity

namespace ecs
{
   using uint32 = unsigned int;

   class Component {
   public:
      Component() = delete;

      template <typename T>
      static uint32 type() {
         static const uint32 type_index{ ms_next++ };
         return type_index;
      }

   private:
      static inline uint32 ms_next{ 0 };
   };

   template <typename T>
   class Handle {
   public:
      static constexpr uint32 k_invalid = ~0u;

      Handle() = default;
      Handle(const uint32 index) : m_index(index) {}

      bool is_valid() const { return m_index != k_invalid; }
      auto index() const { return m_index; }

   private:
      uint32 m_index{ k_invalid };
   };

   class IComponentContainer {
   public:
      virtual ~IComponentContainer() = default;
   };

   template <typename T>
   class ComponentContainer final : public IComponentContainer {
   public:
      ComponentContainer() = default;

      template <typename ...Ts>
      Handle<T> create_component(Ts&& ...ts) {
         const uint32 index = static_cast<uint32>(m_components.size());
         m_components.emplace_back(std::forward<Ts>(ts)...);
         return Handle<T>(index);
      }

      T &get_component(const Handle<T> &handle) {
         assert(handle.is_valid());
         assert(handle.index() < m_components.size());
         return m_components[handle.index()];
      }

      const T &get_component(const Handle<T> &handle) const {
         assert(handle.is_valid());
         assert(handle.index() < m_components.size());
         return m_components[handle.index()];
      }

   private:
      std::vector<T> m_components;
   };

   class ComponentContext final {
   public:
      ComponentContext() = default;
      ~ComponentContext();

      template <typename T, typename ...Ts>
      Handle<T> create_component(Ts ...ts) {
         return get_container<T>().create_component(std::forward<Ts>(ts)...);
      }

      template <typename T>
      T &get_component(const Handle<T> &handle) {
         return get_container<T>().get_component(handle);
      }

      template <typename T>
      const T &get_component(const Handle<T> &handle) const {
         return get_container<T>().get_component(handle);
      }

   private:
      template <typename T>
      ComponentContainer<T> &get_container()
      {
         const uint32 index = Component::type<T>();
         if (m_containers.size() < (index + 1)) {
            m_containers.resize(index + 1);
            m_containers[index] = new ComponentContainer<T>;
         }

         assert(m_containers[index]);
         return *static_cast<ComponentContainer<T> *>(m_containers[index]);
      }

      std::vector<IComponentContainer *> m_containers;
   };

   class Registry final {
   public:
      Registry() = default;

      template <typename F, typename ...Ts>
      void create(F f, Ts ...ts) {
         f(m_components, std::forward<Ts>(ts)...);
      }

      template <typename F, typename ...Ts>
      void update(F f, Ts ...ts) {
         f(m_components, std::forward<Ts>(ts)...);
      }

   private:
      ComponentContext m_components;
   };
} // !ecs
