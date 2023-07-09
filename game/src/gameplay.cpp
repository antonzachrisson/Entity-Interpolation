// gameplay.cpp

#include "gameplay.hpp"

namespace gravity
{
Entity::~Entity() {
   for (auto c : m_components) {
      delete c;
   }
}

void Entity::update(const sf::Time &deltatime)
{
   for (auto &c : m_components) {
      c->update(deltatime);
   }
}

void Entity::render(sf::RenderTarget &target)
{
   for (auto &c : m_components) {
      c->render(target);
   }
}

void Entity::add_component(Component *component) 
{
   m_components.push_back(component);
}
} // !gravity

namespace ecs
{
   ComponentContext::~ComponentContext()
   {
      for (auto &container : m_containers) {
         delete container;
      }
   }
} // !ecs
