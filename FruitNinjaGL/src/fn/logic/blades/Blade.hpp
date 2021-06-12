#pragma once
#include<fn/fn.hpp>
#include <fn\core\core.hpp>
#include <fn\core\gl\Shader.hpp>
#include <fn\core\gl\Texture.hpp>
#include <fn\core\assets\assets.hpp>
#include <fn\core\gl\Mesh.hpp>

/**
 * @brief Lama che taglia i frutti.
 * 
 * La classe rappresenta visivamente gli input da trascinamento del mouse
 * sotto-forma di una `Lama` capace di tagliare i frutti.
 * 
 * La lama è generata à partire dalle Mouse::positions() le quali sono prima
 * pre-processate attraverso un smoothing (utilizzando l'algoritmo di Chaikin) 
 * e successivamente utilizzate per la costruzione di una mesh.
 */
class Blade
{
public:
	/** @brief costruttore. */
	Blade();

	virtual void update(core::seconds delta_t);
	virtual void draw(const fn::Camera& camera);

private:
	ShaderSP m_shader;
	TextureSP blade_texture;
	glm::mat4 m_projection;
	glm::mat4 m_view = glm::mat4(1.0f);
	glm::mat4 m_model = glm::mat4(1.0f);

	/** Calcola lo spessore da dare alla lama all'i-esimo indice. */
	inline float thickness(const std::vector<glm::vec2>& pos, const size_t i);

	/** Costruisce un vertice della mesh */
	Vertex make_vertex(const glm::vec2& p, const std::vector<glm::vec2>& positions, const size_t idx, const float sign);

	/** Algoritmo di Chaikin */
	std::vector<glm::vec2> polylineSmooth(const std::vector<glm::vec2>& input);

private:
	static inline const auto S_DEFAULT_VERTEX_SHADER = assets::dir::shaders / "blades" / "default.vglsl";
	static inline const auto S_DEFAULT_FRAGMENT_SHADER = assets::dir::shaders / "blades" / "default.fglsl";
};

