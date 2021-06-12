#pragma once
#include<fn/fn.hpp>
#include"GUI.hpp"
#include <fn\core\gl\Sprite.hpp>

/**
 * @brief Classe che gestisce il disegno di un testo.
 * 
 * Il testo è semplicemente disegnato ricorrendo ad uno `spritefont` ovvero
 * ogni carattere corrisponde ad uno sprite.
 * 
 * **NOTA**: Al momento sono disponibili soltanto gli sprite dei i caratteri numerici e il `:`.
 */
class Text : public GUI{
public:
	/**
	 * @brief Crea un testo.
	 * 
	 * \param text stringa corrispondente al testo da visualizzare
	 * \param position posizione del testo (rispetto l'angolo in alto a sinistra)
	 * \param scaleXY fattore di scala del testo.
	 */
	Text(const std::string& text, glm::vec3 position, const float scaleXY = 1.0f);

	/**
	 * @brief Crea un testo con posizione di default a {0,0,0}.
	 *
	 * \param text stringa corrispondente al testo da visualizzare
	 * \param scaleXY fattore di scala del testo.
	 */
	Text(const std::string& text, const float scaleXY = 1.0f);

	/**
	 * Disegna il testo.
	 *
	 * Utilizza lo Shader di default della classe Sprite per disegnare il testo.
	 * Attivazione e set delle uniform sono gestite da questa funzione.
	 *
	 * \param camera Camera rispetto la quale disegnare il testo.
	 *
	 * @see Sprite
	 */
	virtual void draw(const fn::Camera& camera) override;

	/** @brief Ridimensiona il testo al resize della finestra. */
	virtual void onResize() override;

	/**
	 * Setta il testo da mostrare.
	 * 
	 * \param text testo da mostrare.
	 * \return reference a @code{.cpp} this @endcode per chiamate a cascata.
	 */
	Text& setText(const std::string& text);

	/** @return testo attuale. */
	const std::string& text() const { return m_text; }

private:
	std::string m_text;							//!< Testo in formato stringa
	std::vector<SpriteSP> m_characters;			//!< Sprite che rappresentano i singoli caratteri
	glm::vec3 m_position = glm::vec3{ 0.0f };   //!< Posizione del testo
	glm::vec2 m_shape;
	float m_scaleXY = 1.0f;
	// Inherited via GUI
	[[nodiscard]] virtual glm::vec3 position() override;
	[[nodiscard]] virtual glm::vec2 shape() override;
};


