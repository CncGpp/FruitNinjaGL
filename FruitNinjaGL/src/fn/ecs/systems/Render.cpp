#include "Render.hpp"
#include <algorithm>
#include <fn\ecs\components\components.hpp>

S::Render::Render(fn::Database& database) : m_database(database)
{
}

void S::Render::update(core::seconds delta_t) {}

void S::Render::draw(const fn::Camera& camera)
{
	// Fase 1: Effettuo il rendering di tutti gli oggetti con componente C::Position, C::Render
	//         Ovvero tutti i modelli 3d dei frutti
	m_database.for_each<C::Position, C::Render>([&](C::Position& p, C::Render& r) {
		r.shader->active();
		r.shader->set("projection", camera.projection());
		r.shader->set("view", camera.view());
		r.shader->set("model", p.model_matrix());
		r.model->draw(r.shader);
		r.shader->deactive();
	});

	// Fase 2: Effettuo il rendering degli sprite.
	m_database.for_each<C::Sprite>([&](C::Sprite& s) { 
		s.sprite->draw(camera);
	});
}
