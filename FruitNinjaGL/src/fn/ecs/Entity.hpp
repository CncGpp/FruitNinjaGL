#pragma once
#include "ecs_types.hpp"


namespace fn {
	/**
	* @brief Wrapper per un entità.
	* 
	 * La classe Entity è un semplice wrapper che consente in maniera conveniente di gestire le entità di base.
	 * Ogni fn::Entity mantiene al suo interno l'eid dell'entità e un puntatore al database che l'ha creata
	 * tramite ciò consente di applicare le varie funzioni del database senza specificare eid e database ogni volta. 
	 * 
	 * Un fn::Entity è anche costruito dal database quando viene creata una nuova entity.
	 * 
	 * @code{.cpp}
	 * fn::Entity e = database.create_entity();
	 * @endcode
	 * 
	 * Sono possibili tutte le operazioni su entità effettuabili mediante il database:
	 * 
	 * @code{.cpp}
	 * bool has = e.has<C::AABB>();		                // 1. Verifica 
	 * e.set<C::Sprite>({...});							// 2. set
	 * auto [p, m] = e.get<C::Position, C::Movement>();	// 3. get
	 * @endcode
	 * 
	 */
	class Entity {
	public:
		Entity(const Eid eid, Database* databse, Signature signature = 0) : m_eid(eid), m_database(databse), m_signature(signature) {}

		template<typename T> [[nodiscard]] constexpr bool has();
		template<typename T1, typename T2, typename ...Ts> [[nodiscard]] constexpr bool has();
		template<typename T, class... Args> constexpr Entity& set(Args&&... args);
		template<typename T> constexpr Entity& set(T component);
		template<typename T> constexpr Entity& remove();
		template<typename T> [[nodiscard]] constexpr T* get();
		template<typename T1, typename T2, typename ...Ts> [[nodiscard]] constexpr std::tuple<T1*, T2*, Ts*...> get();
		[[nodiscard]] Eid eid() const noexcept { return m_eid; }
		[[nodiscard]] Signature signature() const noexcept { return m_signature; }

	private:
		Signature m_signature;
		Eid m_eid;
		Database* m_database;

		void m_update_signature();
	};

}

