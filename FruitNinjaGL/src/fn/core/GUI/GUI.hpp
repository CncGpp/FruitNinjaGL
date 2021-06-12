#pragma once
#include <fn\core\Camera.hpp>

/** @brief Astrazione di base per tutti i componenti della GUI */
class GUI {
public:
	/**
	 * Disegna il componente.
	 *
	 * Utilizza lo Shader di default del componente per disegnarlo.
	 * Attivazione e set delle uniform sono gestite da questa funzione.
	 *
	 * \param camera Camera rispetto la quale disegnare lo sprite.
	 *
	 * @see Sprite
	 */
	virtual void draw(const fn::Camera& camera) = 0;
	virtual void onResize() {};
	virtual glm::vec3 position() = 0;
	virtual glm::vec2 shape() = 0;
};