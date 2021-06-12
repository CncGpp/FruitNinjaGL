#include "Picker.hpp"
#include <fn\core\assets\AssetManager.hpp>
#include <fn\ecs\components\components.hpp>

Picker::Picker(fn::Database& database) : m_database(database)
{
	this->shader = AssetManager::loadShader(S_DEFAULT_VERTEX_SHADER, S_DEFAULT_FRAGMENT_SHADER);
}

void Picker::begin()
{
	inbegin = true;
	shader->active();
	GL_CHECK( glClearColor(0.0f, 0.0f, 0.0f, 0.0f) );
	GL_CHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}

void Picker::end()
{
	inbegin = false;
	shader->deactive();
	GL_CHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
}

void Picker::draw(const fn::Camera& camera, fn::Eid eid)
{
	assert(this->inbegin == true);
	assert( (m_database.has<C::Position, C::Render>(eid) == true) );

	auto [p, r] = m_database.get<C::Position, C::Render>(eid);
	pickid id = to_pickid(eid);
	assert(id < 0x00FFFFFF);						// Il byte del canale alpha � lasciato a FF

	unsigned char* color = (unsigned char*)&id;
	shader->active();
	shader->set("r", (int)color[0]);
	shader->set("g", (int)color[1]);
	shader->set("b", (int)color[2]);
	shader->set("a", (int)255);

	shader->set("projection", camera.projection());
	shader->set("view", camera.view());
	shader->set("model", p->model_matrix());
	r->model->draw(shader);
}
