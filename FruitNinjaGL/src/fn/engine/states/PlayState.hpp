#pragma once
#include<fn/engine/states/State.hpp>

#include <fn/core/core.hpp>
#include <fn\core\input\Picker.hpp>

#include <fn\ecs\ecs.hpp>
#include <fn\ecs\systems\Wave.hpp>
#include <fn\ecs\systems\Physics.hpp>
#include <fn\ecs\systems\Combo.hpp>

#include <fn\logic\blades\Blade.hpp>
#include <fn\core\gl\Sprite.hpp>
#include <fn\core\GUI\Hud.hpp>
#include <fn\engine\events\Event.hpp>
#include <fn\ecs\systems\Collision.hpp>
#include <fn\ecs\systems\Render.hpp>

/**
 * @brief Stato di gioco.
 *
 * È lo stato principale del gioco, che ne gestisce tutte le logiche ed il funzionamento.
 * Ha il compito di inizializzare il database per l'ECS nonchè i fari sistemi che gestiscono le entità.
 */
class PlayState : public State
{
public:
	/** @brief Costruttore. */
	PlayState();

	virtual void handleInputs() override;
	virtual void handleEvents() override;
	virtual void notifyEvent(Event* e) override;

	virtual void update(core::seconds delta_t) override;
	virtual void render() override;

	fn::Database& database();

protected:
	fn::Database m_database{};					//!< Database delle entità per l'ECS.

	S::Physics m_physics_system;				
	S::Wave m_wave_system;                   
	S::Combo m_combo_system;
	S::Collision m_collison_system;
	S::Render m_render_system;

	std::queue<Event*> m_event_system{};		//!< Coda degli eventi pending da gestire.

	SpriteSP m_background;
	Hud m_hud;

	Picker m_picker;							//!< Picker per l'input
	Blade m_blade;								//!< Blade da visualizzare

private:
	fn::Camera m_camera;

	unsigned int m_score;
	core::seconds m_timer;
};

