#pragma once
#include<fn/fn.hpp>
#include <fn\core\gl\Shader.hpp>
#include <fn\core\gl\Texture.hpp>
#include <fn\core\gl\Mesh.hpp>
#include <fn\core\assets\assets.hpp>

class Sprite;
using SpriteSP = std::shared_ptr<Sprite>;

/**
 * @brief Astrazione di uno sprite 2D.
 * 
 * Il rendering avviene utilizzando una semplice mesh rettangolare composta da due triangoli sulla quale è
 * applicata l'immagine da mostrare come texture.
 * 
 * @section Projection-Matrix Matrice di proiezione
 * 
 * Gli sprite sono considerati come elementi di GUI e quindi la loro rappresentazione avviene rispetto ad
 * una matrice di proiezione fissata a priori.
 * 
 *  @code{.cpp}
 *   constexpr auto vsize = gl::displayport_size();
 *   this->m_projection = glm::ortho(0.0f, vsize.x, vsize.y, 0.0f, -1.0f, 200.1f);
 *	@endcode
 * 
 * dove @code{.cpp} gl::displayport_size() @endcode specifica le dimensioni virtuali dello schermo sul 
 * quale collocare lo sprite.
 * 
 * 
 * @section Auto-Size Determinazione automatica delle dimensioni
 * 
 * Specificando lo @code{.cpp} shape @endcode dello sprite è possibile determinare le dimensioni (
 * lunghezza e largezza) dello sprite. Lo shape può anche essere calcolato automaticamente, infatti:
 * - se entrambi le componenti sono -1 usa le dimensioni originali,
 * - se una delle due componenti è -1 questa è calcolata dalla restante cercando 
 *   di mantenere le proporzioni dello sprite originale.
 * 
 * @see gl::displayport_size(), gl::vw, gl::vh
 */
class Sprite {
public:
	/**
	 * @ brief Costruisce uno sprite.
	 * 
	 * \param spritepath percorso alla texture/immagine da visualizzare.
	 * \param position posizione dove collocare lo sprite (relativa al vertice in alto a destra)
	 * \param shape dimensioni (in larghezza e altezza) che deve avere lo sprite.
	 *        - se entrambi le componenti sono -1 usa le dimensioni originali,
	 *        - se una delle due componenti è -1 questa è calcolata dalla restante
	 *          cercando di mantenere le proporzioni dello sprite originale.
	 * \param scaleXY Fattore di scala delle sprite.
	 */
	Sprite(const fs::path& spritepath, const glm::vec3& position, const glm::vec2& shape, const float scaleXY=1.0f);
	Sprite(const fs::path& spritepath, const glm::vec3& position, const float scaleXY = 1.0f);
	Sprite(const fs::path& spritepath, const glm::vec2& shape, const float scaleXY = 1.0f);
	Sprite(const fs::path& spritepath, const float scaleXY = 1.0f);
	

	virtual Sprite& setPosition(const glm::vec3& position);
	const glm::vec3& position() const noexcept { return m_position; }
	virtual Sprite& setShape(const glm::vec2& shape);
	virtual Sprite& setSource(const fs::path& spritepath);
	void setAlpha(float alpha) { this->alpha = alpha; }

	[[nodiscard]] virtual float width() const { return compute_shape().x; }
	[[nodiscard]] virtual float height() const { return compute_shape().y; }
	[[nodiscard]] virtual glm::vec2 shape() const { return compute_shape(); }

	/**
	 * Disegna lo sprite.
	 * 
	 * Utilizza lo Shader passato per disegnare lo sprite rispetto la camera.
	 * Lo shader deve essere passato già attivato e con le uniform opportunamente settate.
	 * 
	 * \param camera Camera rispetto la quale disegnare lo sprite.
	 * \param shader Shader da utilizzare per il disegno.
	 */
	virtual void draw(const fn::Camera& camera, ShaderSP& shader);

	/**
	 * Disegna lo sprite.
	 *
	 * Utilizza lo Shader di default della classe Sprite per disegnare lo sprite.
	 * Attivazione e set delle uniform sono gestite da questa funzione.
	 *
	 * \param camera Camera rispetto la quale disegnare lo sprite.
	 * 
	 * @see Sprite::S_DEFAULT_VERTEX_SHADER, Sprite::S_DEFAULT_FRAGMENT_SHADER
	 */
	virtual void draw(const fn::Camera& camera);
	virtual void onResize();

private:
	/**
	 * Calcola la shape dello sprite.
	 * \return vettore contenente la shape calcolata
	*/
	[[nodiscard]] virtual glm::vec2 compute_shape() const;
	glm::vec3 m_position;		//!< Posizione topleft dello sprite secondo la viewprojection matrix
	glm::vec2 m_shape;			//!< Shape in unità di viewprojection, se una componente è -1 viene determinata dall'altra dall'aspect ratio, se entrambe -1 si usa lo shape della texture
	float m_scaleXY;			//!< Scala dello sprite ha effetto solo se m_shape ha entrambi -1
	
	// Richiesti per il disegno
	glm::mat4 m_projection;
	glm::mat4 m_view = glm::mat4(1.0f);
	glm::mat4 m_model = glm::mat4(1.0f);
	ShaderSP m_shader;
	TextureSP m_texture;
	MeshSP m_mesh;

	// Attributi
	float alpha = 1.0f;

protected:
	static inline const auto S_DEFAULT_VERTEX_SHADER = assets::dir::shaders / "sprites" / "default.vglsl";			//!< Shader di default per il disegno dei vertici
	static inline const auto S_DEFAULT_FRAGMENT_SHADER = assets::dir::shaders / "sprites" / "default.fglsl";        //!< Shader di default che consente di scegliere anche trasparenza
};

