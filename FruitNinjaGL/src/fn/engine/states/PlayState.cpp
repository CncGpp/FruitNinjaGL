#include "PlayState.hpp"
//#include <fn\ecs\systems\Wave.cpp>
#include <fn\ecs\entities\Fruit.hpp>
#include <fn\ecs\components\components.hpp>
#include <fn\core\audio\Audio.hpp>
#include <fn\core\assets\AssetManager.hpp>
#include <fn\engine\states\GameOverState.hpp>
#include <thread>

PlayState::PlayState() : State(), m_database(), m_wave_system(m_database), m_physics_system(m_database),  
                                  m_picker(m_database), m_combo_system(this, m_database), m_collison_system(m_database),
	                              m_render_system(m_database)
{
	// Registro le varie componenti
	m_database.register_component<C::Position>();
	m_database.register_component<C::Movement>();
	m_database.register_component<C::Fruit>();
	m_database.register_component<C::Render>();
	m_database.register_component<C::Sprite>();
	m_database.register_component<C::Particle>();
	m_database.register_component<C::AABB>();

	m_background = std::make_shared<Sprite>(assets::res::backgrounds[2], glm::vec3{ 0.0f, 0.0f, -200.0f }, gl::viewport_size(), 1.0f);

	// Imposto il timer
	this->m_timer = core::seconds(92);
	AssetManager::loadAudio(assets::res::sound::game_start)->play2D();
}

fn::Database& PlayState::database()
{
    return m_database;
}

void PlayState::handleInputs()
{
	// Determino le entità che sono state prese dal pick
	m_picker.begin();
	m_database.for_each<C::Position, C::Render, C::Fruit>([&](fn::Entity& e) {
		if(e.get<C::Fruit>()->model_kind == Fruits::Model::whole)
			m_picker.draw(this->m_camera, e.eid());
	});
	auto picked = m_picker.pick(Mouse::positions());
	m_picker.end();

	// Gestione degli input di picking.
	for (auto x : picked) {
		auto p = m_database.get<C::Position>(x)->position;
		E::Fruit::split(m_database, x);
		m_score++;
		m_hud.setScore(m_score);
		m_combo_system.addSplit();
		AssetManager::loadAudio(assets::res::sound::impact[x % 4])->play3D(p);
	}
}

void PlayState::handleEvents()
{
	while (!m_event_system.empty()){
		auto* event = m_event_system.front();

		switch (event->type()){
		case Event::Type::COMBO: {
			auto* e = dynamic_cast<ComboEvent*>(event);
			m_hud.setScore(m_score += e->comboLevel);
			AssetManager::loadAudio(assets::res::sound::combo[e->comboLevel % 8])->play2D();
			break;
		}
		case Event::Type::BLITZ: {
			auto* e = dynamic_cast<BlitzEvent*>(event);
			m_hud.setScore(m_score += e->blitzLevel);
			AssetManager::loadAudio(assets::res::sound::blitz[(e->blitzLevel/5 - 1) % 6])->play2D();
			break;
		}
		default:
			break;
		}

		m_event_system.front()->execute();
		delete m_event_system.front();
		m_event_system.pop();
	}
}

void PlayState::notifyEvent(Event* e)
{
	this->m_event_system.push(e);
}

void PlayState::update(core::seconds delta_t)
{
	m_timer -= delta_t;
	
	if (m_timer < core::seconds(0)) {
		Audio::stopAllSounds();
		AssetManager::loadAudio(assets::res::sound::game_end)->play2D();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		auto state = std::make_unique<GameOverState>(m_score);
		fn::engine->pushState(state);
		return;
	}
	
	m_hud.setTimer(m_timer);
	m_wave_system.update(delta_t);
	m_physics_system.update(delta_t);
	m_collison_system.update(delta_t);
	m_combo_system.update(delta_t);
	m_render_system.update(delta_t);
}

void PlayState::render()
{
	GL_CHECK( glClearColor(0.15f, 0.15f, 0.15f, 1.0f) );
	GL_CHECK( glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT) );
	
	m_background->draw(m_camera);
	m_render_system.draw(m_camera);
	m_blade.draw(m_camera);
	m_hud.draw(m_camera);

	glfwSwapBuffers(fn::engine->window());
}


