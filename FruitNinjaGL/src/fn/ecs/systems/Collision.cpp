#include "Collision.hpp"
#include <fn\ecs\components\components.hpp>

namespace S {
	Collision::Collision(fn::Database& database) : m_database(database)
	{
	}

	void Collision::update(core::seconds delta_t)
	{
		// FASE 1: Prendo le C::AABB delle entità con C::Position e le trasformo in OOB utilizzando
		//         la loro projection matrix, salvo tutto in object
		struct Obj {
			fn::Eid eid;
			glm::mat2x4 obb;
			float r;
		};
		std::vector<Obj> objects;
		objects.reserve(24);			// Non penso ci siano più di 24 Obj.

		m_database.for_each<C::Position, C::AABB>([&](fn::Eid eid, C::Position& p, C::AABB& a) {
			glm::mat2x4 aabb;
			aabb[0] = glm::vec4(a.box[0], 1.0f);
			aabb[1] = glm::vec4(a.box[1], 1.0f);
			objects.push_back({ eid, p.model_matrix() * aabb, a.radius() });
		});

		// Fase 2: Per ogni possibile coppia distinta di object verifico la collisione
		//     NB: per semplicità e forma generica dei frutti utilizzo una sfera di 
		//         raggio pari all'half lenght massima della OOB
		//     NB: La collisione è calcolata solo per le entità che hanno C::Movement, C::Fruit
		//         altrimenti si snatura il gameplay originale (che già non prevedeva collisioni)
		for (int i = 0; i < objects.size(); i++) {
			for (int j = i+1; j < objects.size(); j++) {
				auto c1 = objects[i].obb[0] + objects[i].obb[1];		//Primo centroide
				auto r1 = objects[i].r;								    //Primo raggio
				auto c2 = objects[j].obb[0] + objects[j].obb[1];		//Secondo centroide
				auto r2 = objects[j].r;								    //Secondo raggio

				if (glm::length(glm::vec3(c1 - c2)) < r1 + r2) {
					if (!m_database.has<C::Movement, C::Fruit>(objects[i].eid) ||
						!m_database.has<C::Movement, C::Fruit>(objects[j].eid))
						continue;
					if (m_database.get<C::Fruit>(objects[i].eid)->model_kind != Fruits::Model::whole ||
						m_database.get<C::Fruit>(objects[j].eid)->model_kind != Fruits::Model::whole)
						continue;

					fn::log::trace("Rilevata una collisione fra le entita' {} e {}.\n", objects[i].eid, objects[j].eid);

					float disp = (r1 + r2 - glm::length(glm::vec3(c1 - c2)))/2.0f;
					m_database.get<C::Position>(objects[i].eid)->traslate(+disp * glm::normalize(glm::vec3(c1 - c2)));
					m_database.get<C::Position>(objects[j].eid)->traslate(-disp * glm::normalize(glm::vec3(c1 - c2)));

					auto [mi, fi] = m_database.get<C::Movement, C::Fruit>(objects[i].eid);
					auto [mj, fj] = m_database.get<C::Movement, C::Fruit>(objects[j].eid);
					mi->velocity = calculate_relative_velocity(fi->fruit.weight, fj->fruit.weight, mi->velocity, mj->velocity, c1, c2);
					mj->velocity = calculate_relative_velocity(fj->fruit.weight, fi->fruit.weight, mj->velocity, mi->velocity, c2, c1);
				}
			}
		}
	}
	glm::vec3 Collision::calculate_relative_velocity(float m1, float m2, glm::vec3 v1, glm::vec3 v2, glm::vec3 c1, glm::vec3 c2) const
	{
		assert(m1 > 0);
		assert(m2 > 0);
		float m = 2.0f * m2 / (m1 + m2);
		auto delta_v = v1 - v2;
		auto delta_c = c1 - c2;
		return v1 - m * glm::dot(delta_v, delta_c) / glm::pow(glm::length(delta_c), 2.0f) * delta_c;
	}
}