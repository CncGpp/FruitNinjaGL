#pragma once
#include <fn\ecs\Database.hpp>
#include <fn\core\gl\Shader.hpp>
#include <fn\core\assets\assets.hpp>
#include <fn\core\Camera.hpp>
#include <unordered_set>

typedef unsigned int pickid;												//!< identificativo interno dell'oggetto disegnato durante il picking

/**
 * @brief Classe che implementa il picking delle entità dalla scena.
 * 
 * Il funzionamento è molto semplice e simile ad una chiamata draw
 *  1. Si chiama la funzione begin() la quale pulisce il color buffer con colore nero.
 *     e attiva lo shader necessario per il picking il quale disegnerà ogni oggetto con
 *     un diverso colore a seconda del suo eid.
 *  2. Tutte le entità interessate al picking sono disegnate con la funzione Picker::draw
 *  3. Passando poi le posizioni del mouse alle funzioni pick_color o pick verranno restituiti
 *     gli eid che sono stati toccati.
 *  4. In fine va richiamata la funzione end() per pulire il framebuffer e disabilitare lo shader
 */
class Picker
{
public:
	/**
	 * @brief Costruttore.
	 * 
	 * \param database Database con le entità.
	 */
	Picker(fn::Database& database);

	/** @brief Inizializza il picking */
	void begin();

	/** @brief Finalizza il picking */
	void end();

	/**
	 * @brief Disegna il volume di un entità sul framebuffer per il picking.
	 * 
	 * \param camera Camera rispetto la quale disegnare.
	 * \param eid Identificativo dell'entità da disegnare.
	 */
	void draw(const fn::Camera& camera, fn::Eid eid);

	/**
	 * @brief Effettua il picking di una collezione di coordinate schermo.
	 * 
	 * @tparam Itr Iterabile
	 * 
	 * \param coords Collezione di coordinate schermo da pickare.
	 * \return Set delle entità che sono state pickate.
	 */
	template<typename Itr>
	[[nodiscard]] std::unordered_set<fn::Eid> pick(const Itr& coords);

protected:
	/**
	 * @brief Converte un Eid in un pickid.
	 * 
	 * Per evitare possibili errori della funzione glReadPixels() alla lettura di valori molto vicini,
	 * l'eid è trasformato in un pickid, un identificativo univoco con una differenza più marcata. 
	 * 
	 * @return colore dell'entità da usare per il draw.
	 */
	[[nodiscard]] pickid to_pickid(fn::Eid input) const { return input * 17 + 1; };   
	[[nodiscard]] pickid to_pickid(fn::Entity input) const { return input.eid() * 17 + 1; };   //!< @overload

	/**
	 * @brief Converte un pickid in un Eid.
	 * 
	 * Effettua la trasformazione inversa da colore a Eid.
	 * 
	 * @return Eid corrispondente al colore pickato.
	 */
	[[nodiscard]] fn::Eid to_eid(pickid input) const { return (input-1) / 17; };



private:
	bool inbegin = false;
	fn::Database& m_database;

	ShaderSP shader;

protected:
	static inline const auto S_DEFAULT_VERTEX_SHADER = assets::dir::shaders / "picking" / "default.vglsl";				//!< Shader di default
	static inline const auto S_DEFAULT_FRAGMENT_SHADER = assets::dir::shaders / "picking" / "color_picking.fglsl";		//!< Shader di default, disegna con un colore fissato

};

template<typename Itr>
inline std::unordered_set<fn::Eid> Picker::pick(const Itr& coords)
{
	assert(this->inbegin == true);
	std::unordered_set<fn::Eid> ret;
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	for (auto& p : coords) {
		pickid id;
		// Controllo che p appartenga alla finestra
		if (viewport[3] - p.y < viewport[1] || viewport[3] - p.y > viewport[1] + viewport[3] || p.x < viewport[0] || p.x > viewport[0] + viewport[2]) {
			continue;
		}
		// Leggo il colore
		glReadPixels((GLint)p.x, viewport[3] - (GLint)p.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &id);
		// Rimuovo la parte relativa al canale alpha
		id = 0x00FFFFFF & id;
		if(id && to_eid(id) < fn::ecs::MAX_ENTITY_COUNT)
			ret.emplace(to_eid(id));
	}

	for (auto x : ret) {
		fn::log::trace("(picking) eid = {}.\n", x);
	}
		
	return ret;
}
