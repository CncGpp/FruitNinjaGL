#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stack>
#include <iostream>

#include "fn/core/core.hpp"
#include <fn\engine\states\State.hpp>

/** @file
 * @brief Class @ref fn::Engine
 */

namespace fn {
	/**
	 * @brief Motore di gioco.
	 * 
	 * La classe @ref Engine è un'astrazione del motore grafico del gioco, si occupa di
	 * - Inizializzare librerie, singleton ed il contesto di OpenGL (es. glfw, glew, Mouse)
	 * - Gestire il game loop con le sue varie fasi (input handle, update, render)
	 * - Gestire i vari @ref State in cui si trova il gioco.
	 */
	class Engine {
	public:
		/**
		 * @brief Costruttore.
		 * 
		 * \param window_width larghezza in pixel della finestra da creare
		 * \param window_height altezza in pixel della finestra da creare
		 * 
		 * @attention L'aspect ratio della finestra deve essere 16:9.
		 */
		Engine(int window_width, int window_height) noexcept;

		/** @brief Distruttore. */
		~Engine();

		/**
		 * @brief Esegue una singola iterazione del game loop.
		 * 
		 * La funzione esegue un loop dello stato correntemente attivo, in sequenza
		 * 1. Aggiorna i singleton per le classi di input (@ref Mouse::update)
		 * 2. Pool degli eventi glwf (glfwPollEvents)
		 * 3. loop dello stato:
		 *    - gestione degli input (@code{.cpp} state.handelInput() @endcode)
		 *    - update (@code{.cpp} state.update() @endcode)
		 *    - render (@code{.cpp} state.render() @endcode)
		 *    - gestione degli eventi di gioco (@code{.cpp} state.handleEvent() @endcode)
		 */
		void loop();

		/**
		 * @brief Inserisce un nuovo stato in cima allo stack.
		 * 
		 * Il nuovo stato aggiunto si troverà sul top e sarà quindi attivo e renderizzato da @ref loop()
		 * @param state unique_ptr ad un nuovo stato da aggiungere.
		 */
		template<class S,
			typename Requires = std::enable_if_t<std::is_base_of<State, S>::value>>      //Qualsiasi S che estende State (che non sia astratto)
		void pushState(std::unique_ptr<S>& state) { m_states.emplace(std::move(state)); }

		/** @brief rimuove l'ultimo stato dallo stack */
		void popState() { m_states.pop(); }

		/**
		 * @brief Ritorna lo stato in cima allo stack.
		 * \return Reference all'unique_ptr allo stato.
		 */
		[[nodiscard]] std::unique_ptr<State>& topState() { return m_states.top(); }

		/** 
		 * @brief Finestra glfw. 
		 * \return Puntatore alla finestra di glfw.
		 */
		[[nodiscard]] inline GLFWwindow* window() const { return this->m_window; }

	protected:
		bool init_context();
		bool init_window();
		bool init_gl_loader();
		bool init_gl();
		bool init_view();
		/** @brief Non implementato... */
		bool init_imgui() { return true; };

		/** @brief Inizializzazione del motore. */
		void init();

	protected:
		static constexpr auto* glsl_version = "#version 130";				//!< Versione di GLSL da usare in imgui @todo aggiungere imgui?
		static constexpr auto VERSION_MAJOR = 4;							//!< Minima versione major.minor di opengl che GLFW può usare
		static constexpr auto VERSION_MINOR = 6;                            //!< Ricerca sempre la dll più aggiornata

	private:
		static inline int m_window_height, m_window_width;
		static inline bool resized = false;
		GLFWwindow* m_window = nullptr;

		std::stack<std::unique_ptr<State>> m_states{};						//!< Stack degli stati, quello attivo è al top.
		core::seconds last_t, delta_t;
	};
}