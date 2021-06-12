#pragma once
#include <deque>
#include <fn/fn.hpp>
#include <fn/engine/Engine.hpp>

typedef int scancode;   //!< Alias
typedef int action;     //!< Alias

/**
 * @brief Classe singleton che gestisce gli input del mouse.
 * 
 * Definisce un'interfaccia unificata per:
 * - Stabilire se uno dei pulsanti (left o right) del mouse è premuto
 * - Memorizza gli swipe le ultime posizioni del cursore quando il buttonLeft`è premuto.
 * 
 * Le posizioni sono memorizzati in una deque che viene svuotata con il passare del tempo.
 */
class Mouse {
public:
	/** @brief  Funzione di inizializzazione del Signleton */
	static void
	init(const fn::Engine* engine);
	/** @brief Restituisce un reference alla deque delle posizioni */
	[[nodiscard]] static const std::deque<glm::vec2>& positions() { return m_mouse.m_positions; }
	/** @brief Restituisce la dimensione massima delle posizioni nella deque */
	[[nodiscard]] static size_t buffer_size() { return m_mouse.m_max_buffer_size; }

	/** @brief Restituisce true se il pulsante sinistro del mouse è premuto */
	[[nodiscard]] static bool buttonLeftPressed() { return getAction(GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS; }
	/** @brief Restituisce true se il pulsante destro del mouse è premuto */
	[[nodiscard]] static bool buttonRightPressed() { return getAction(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS; }

	/**
	 * @brief Rimuove gli ultimi elementi dalla deque.
	 * \param count numero di valori da rimuovere, default 1
	 */
	static void pop_back(const size_t count = 1);

	/**
	 * @brief Aggiorna lo stato interno della classe, rimuovendo eventuali posizioni dal fondo della deque.
	 * \param delta_t trascorso dall'ultimo aggiornamento.
	 */
	static void update(core::seconds delta_t);

private:
	static Mouse m_mouse;														           //!< istanza del singleton
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);    //!< callback glfw per le posizioni del mouse
	static action getAction(scancode button);

	const fn::Engine* m_engine;															   //!< puntatore all'engine
	const size_t m_max_buffer_size = 32;												   //!< dimensione massima della deque
	std::deque<glm::vec2> m_positions;													   //!< struttura dati contenente le ultime posizioni
};