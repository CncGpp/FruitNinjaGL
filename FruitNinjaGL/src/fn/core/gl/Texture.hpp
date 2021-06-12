#pragma once

#include <unordered_map>
#include <fn/fn.hpp>
#include <stb_image.h>

class Texture;
using TextureSP = std::shared_ptr<Texture>;

/**
 * @brief Astrazione del concetto di texture.
 * 
 * La classe consente di costruire e caricare una texture dato un percorso.
 * � anche possibile specificare il tipo di texture nonch� i parametri OpenGl.
 * 
 * Il loading dell'immagine avviene usando la libreria stb_image.
 */
class Texture {
public:
	using Parameteri = std::unordered_map<GLint, GLint>;

	/** @brief Tipi di texture supportate. */
	enum struct Type : int {
		diffuse = 0, specular = 1, unknow = -1,
	};

	/**
	 * @brief Costruttore.
	 * 
	 * \param texpath percorso alla texture da caricare.
	 * \param type tipologia della texture.
	 * \param parameteri dizionario di parametri (key, value) che specifica le opzioni OpenGL legate a quella texture.
	 */
	Texture(const fs::path& texpath, const Texture::Type type = Texture::Type::diffuse, const Texture::Parameteri& parameteri = {});

	/**
	 * @brief Costruttore.
	 *
	 * \param texpath percorso alla texture da caricare.
	 * \param parameteri dizionario di parametri (key, value) che specifica le opzioni OpenGL legate a quella texture.
	 */
	Texture(const fs::path& texpath, const Texture::Parameteri& parameteri);

	/** @brief Distruttore */
	~Texture();

	/** @brief La copia di un oggetto Texture non � permessa. */
	Texture(Texture const&) = delete;

	/** @brief L'assegnazione di un oggetto Texture non � permessa. */
	Texture& operator=(Texture const&) = delete;

	[[nodiscard]] GLuint id() const { return m_id; }													//!< id della texture come restituito da OpenGL 
	[[nodiscard]] std::string type() const { return m_type; }                                           //!< tipologia della texture in formato stringa
	[[nodiscard]] glm::vec2 shape() const { return {m_width, m_height}; }								//!< shape della texture
	void shape(int* width, int* height) const { *width = m_width; *height = m_height; }					//!< shape della texture tramite puntatori
	[[nodiscard]] const fs::path& filepath() const { return m_path; }									//!< filepath originale della texture
	[[nodiscard]] const Texture::Parameteri& parameteri() const { return m_parameteri; }                //!< dizionario dei parametri


protected:
	/** @brief Funzione principale per il load della texture */
	void load();

private:
	GLuint m_id;                   //!< Identificativo della texture restituito dopo il bind
	std::string m_type;            //!< Tipologia della texture ('texture_diffuse' o 'texture_specular')
	fs::path m_path;               //!< Path alla texture sul disco
	int m_width, m_height;
	Texture::Parameteri m_parameteri = {
		{ GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT },
		{ GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT },
		{ GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR },
		{ GL_TEXTURE_MAG_FILTER, GL_LINEAR },
	};
};