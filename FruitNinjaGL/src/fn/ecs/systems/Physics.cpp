#include "Physics.hpp"
#include <fn\ecs\ecs.hpp>
#include <fn\ecs\components\components.hpp>

void S::Physics::update(core::seconds delta_t)
{
	m_database.for_each<C::Position, C::Movement>([delta_t](C::Position& p, C::Movement& m) {
		const float dt = (float)delta_t.count();

		// :::::: Aggiornamento della rotazione 
		p.rotation += m.spin * dt;
		for (auto i = 0; i < 3; i++) {
			constexpr auto two_pi = glm::two_pi<float>();
			p.rotation[0] = p.rotation[0] - two_pi * glm::floor(p.rotation[0] / two_pi);
		}

		// :::::: Aggiornamento della posizione considerando la gravità
		constexpr auto g = -9.80665f;
		m.velocity.y += g * dt;
		p.position += m.velocity * dt;
	});

	m_database.for_each<C::Position, C::Movement>([&](fn::Entity& e) {
		if (e.get<C::Position>()->position.y < -11.0f && e.get<C::Movement>()->velocity.y < 0) {
			m_database.destroy_entity(e.eid());
			fn::log::trace("(physics) entità con eid {} fuori schermo distrutta.\n", e.eid());
		}
	});
}
