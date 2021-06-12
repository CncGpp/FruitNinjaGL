#pragma once
#include<fn/engine/states/State.hpp>
#include <fn\core\gl\Sprite.hpp>

/**
 * @brief Stato di caricamento.
 * 
 * Stato iniziale del gioco, creato all'avvio dell'applicazione.
 * Ha il compito di mostrare una breve splash screen mente carica in background
 * i principali asset di gioco. Modelli, texture, Shader e Audio.
 */
class LoadState : public State
{
public:
	/** @brief Costruttore. */
	LoadState();
	virtual void render() override;
	virtual void notifyEvent(Event* e) override;
	virtual void update(core::seconds delta_t) override;

protected:
	/** @brief Carica i modelli. */
	void loadModels();
	/** @brief Carica le texture. */
	void loadTextures();
	/** @brief Carica gli shader. */
	void loadShaders();
	/** @brief carica gli audio. */
	void loadAudios();

private:
	fn::Camera m_camera;
	SpriteSP m_splash_screen;
	core::seconds m_splash_lifetime{ 5.0 };
	core::seconds m_splash_elapsed{ 0.0 };

	SpriteSP m_background;
};

