#pragma once
#include <fn\core\core.hpp>
#include <fn/ecs/Database.hpp>

namespace S {
	/**
	 * @brief Interfaccia base per i System.
	 * 
	 * Classe base che un sistema dovrebbe implementare. 
	 * È disponibile un unico metodo che consente di aggiornare lo stato interno.
	 */
	class System {
	public:
		/**
		 * @brief Aggiorna lo stato interno del sistema.
		 * 
		 * \param delta_t intervallo di tempo gioco trascorso.
		 */
		virtual void update(core::seconds delta_t) = 0;
	};
}