#pragma once
#include<fn/engine/states/State.hpp>
#include <fn\core\gl\Sprite.hpp>
#include <fn\core\GUI\Hud.hpp>

/**
 * @brief Stato di fine gioco.
 *
 * Stato finale del gioco, creato al termine di una partita.
 * Ha il compito di mostrare una scritta game over con il punteggio attuale.
 * 
 * @todo *NB* Non sono implementati pulsanti per ricominciare una partita...
 */
class GameOverState : public State
{
public:
	GameOverState(unsigned int score);

	// Inherited via State
	virtual void notifyEvent(Event* e) override;
	virtual void update(core::seconds delta_t) override;
	virtual void render() override;

private:
	SpriteSP m_background;
	fn::Camera m_camera;
	Hud m_hud;
};

