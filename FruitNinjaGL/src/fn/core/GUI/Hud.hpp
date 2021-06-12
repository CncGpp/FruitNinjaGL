#pragma once
#include <fn/fn.hpp>
#include "GUI.hpp"
#include <fn\core\gl\Sprite.hpp>
#include <fn\core\GUI\Text.hpp>

/**
 * @brief Classe che gestisce l'hud di gioco.
 * 
 * Si occupa di disegnare:
 * - Il punteggio (con l'icona a fianco)
 * - Il timer del tempo residuo.
 */
class Hud : public GUI{
public:
	Hud();
	/**
	 * @brief Setta il punteggio attuale nell'hud.
	 * 
	 * \param score punteggio da mostrare
	 */
	void setScore(const unsigned int score);

	/**
	 * @brief Setta il tempo restante nell'hud.
	 * 
	 * \param time tempo da mostrare
	 */
	void setTimer(const core::seconds time);

	void draw(const fn::Camera& camera) override;
	void onResize() override;

private:
	SpriteSP m_score_icon;

	sp<Text> m_score_text;
	sp<Text> m_timer_text;

	// Inherited via GUI
	virtual glm::vec3 position() override;
	virtual glm::vec2 shape() override;
};

