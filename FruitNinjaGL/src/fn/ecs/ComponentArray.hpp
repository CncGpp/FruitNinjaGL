#pragma once
#include"ecs_types.hpp"

namespace fn {
	/**
	 * @brief Interfaccia ad un array delle componenti.
	 * 
	 * Un ComponentArray non è altro che un array di lunghezza fn::ecs::MAX_ENTITY_COUNT che memorizza
	 * le componenti delle varie entità create dal database. Ci saranno dunque tanti ComponentArray quante sono le componenti
	 * registrate all'interno del database.
	 * L'accesso alla componente dell'entità avviene tramite eid.
	 * 
	 * Per conoscere velocemente quali delle posizioni dell'array sono effettivamente occupate ci si 
	 * affida ad un bitset di validità.
	 */
	class IComponentArray {
	public:
		virtual ~IComponentArray() = default;
		virtual void remove(const unsigned int ix) = 0;
		[[nodiscard]] virtual const std::bitset<fn::ecs::MAX_ENTITY_COUNT>& validity() = 0;
	};

	/**
	 * @brief ComponentArray per una specifica componente T.
	 * 
	 * @tparam T struct o classe della componente.
	 */
	template<typename T>
	class ComponentArray : public IComponentArray {
	public:
		static inline const Cid cid = fn::ecs::get_component_id<T>();

		/**
		 * @brief Setta alla posizione ix dell'array la componente T passando un rvalue.
		 * 
		 * \param ix indice (eid) della componente da settare.
		 * \param component componente
		 */
		void set(const unsigned int ix, T component) {
			this->m_validity[ix] = true;
			this->m_data.at(ix) = component;
		}
		/**
		 * @brief Costruisce e poi setta alla posizione ix dell'array la componente T passando i parametri del suo costruttore.
		 * 
		 * \param ix indice (eid) della componente da settare.
		 * \param args parametri necessari alla costruzione della componente.
		 */
		template<class... Args>
		void set(const unsigned int ix, Args&&... args) {
			this->m_validity[ix] = true;
			this->m_data.at(ix) = T{ std::forward<Args>(args)... }; //TODO sostituire con emplace?
		}
		/**
		 * @brief Accede ad una componente.
		 * 
		 * \param ix indice (eid) della componente da recuperare.
		 * \return puntatore alla ix-esima componente se valida altrimenti nullptr. 
		 */
		[[nodiscard]] T* get(const unsigned int ix) {
			return has(ix) ? &this->m_data.at(ix) : nullptr;
		}
		/** \return true se la ix-esima componente è valida.*/
		[[nodiscard]] bool has(const unsigned int ix) {
			return this->m_validity[ix];
		}
		/**
		 * @brief Rimuove dal component array la ix-esima componente.
		 * 
		 * \param ix indice (eid) della componente da rimuovere.
		 */
		void remove(const unsigned int ix) override {
			this->m_validity[ix] = false;
			//this->m_data.at(ix).~T(); TODO: Non so perché ma lo shared pointer della classe Shader (solo il suo) viene distrutto troppe volte ed il contatore diventa 0 causando nullptr
		}

		/**
		 * Bitmap con la validità dell'array.
		 * 
		 * \return bitset con le componenti valide.
		 */
		[[nodiscard]] const std::bitset<fn::ecs::MAX_ENTITY_COUNT>& validity() override { return m_validity; }
		
	private:
		std::bitset<fn::ecs::MAX_ENTITY_COUNT> m_validity;
		std::array<T, fn::ecs::MAX_ENTITY_COUNT> m_data;
	};
}