#pragma once
#include"ecs_types.hpp"
#include"Entity.hpp"

namespace fn {
	/**
	 * @brief Classe che gestisce la costruzione, recupero e accesso delle entità.
	 * 
	 * Rappresenta appunto un database di tutte le possibili entità e consente di effettuare 'query'
	 * - recuperare una specifica entità dato il suo eid.
	 * - recuperare un sottoinsieme delle componenti di un entità dato il suo eid
	 * - restituire tutte le entità con una specifica firma o sottoinsieme di componenti.
	 * - iterare lungo i component array applicando funzioni lambda
	 */
	class Database {
	public:
		Database();

		/** @brief La copia di un oggetto Database non è consentita */
		Database(Database const&) = delete;

		/** @brief L'assegnazione di un oggetto Database non è consentita */
		Database& operator=(Database const&) = delete;

		/**
		 * @brief Registra la componente nel database.
		 * 
		 * Aggiunge un nuovo ComponentArray<T> al database.
		 * 
		 * @tparam T dtype della componente da registrare.
		 */
		template<typename T>
		constexpr void register_component() {
			const Cid cid = ComponentArray<T>::cid;
			m_components.insert({ cid, std::make_shared<ComponentArray<T>>() });
		}

		/**
		 * @brief Verifica se la componente è registrata.
		 * 
		 * @tparam T componente da verificare.
		 * \return @code{.cpp} true @endcode se la componente è registrata.
		 */
		template<typename T>
		[[nodiscard]] constexpr bool registered() {
			const Cid cid = ComponentArray<T>::cid;
			return m_components.find(cid) != m_components.end();
		}

		/**
		 * @brief Setta una componente di una entità.
		 * 
		 * Consente di settare una componente per una specifica entità.
		 * La componente da settare è specificabile attraverso il template @code{.cpp} T @endcode.
		 * 
		 * @code{.cpp} 
		 * fn::Eid e = database.create_entity();
		 * database.set<C::Movement>(e, glm::vec3{1.0f, 16.0f, -2.5f}, 
		 *                              glm::vec3{2.0f, -1.0f, 2.0f});
		 * @endcode
		 * 
		 * Tutti i parametri in args saranno utilizzati per costruire la componente.
		 * 
		 * @tparam T componente da settare.
		 * @tparam Args parametri da inoltrare al costruttore di @code{.cpp} T @endcode.
		 * \param eid identificativo dell'entità a cui si vuole settare la componente.
		 * \param args argomenti da inoltrare al costruttore di @code{.cpp} T @endcode.
		 */
		template<typename T, class... Args>
		constexpr void set(const Eid eid, Args&&... args) {
			[[unlikely]] if (!this->registered<T>())
				register_component<T>();

			assert(this->registered<T>() && "set<T> di un componente non registrato nel database.");
			getComponentArray<T>()->set(eid, T{ std::forward<Args>(args)... });
		}

		/**
		 * @brief Setta una componente di una entità.
		 *
		 * Consente di settare una componente per una specifica entità.
		 * La componente da settare è specificabile attraverso il template @code{.cpp} T @endcode.
		 *
		 * @code{.cpp}
		 * fn::Eid e = database.create_entity();
		 * database.set<C::Movement>(e, {
		 *		.velocity = glm::vec3{1.0f, 16.0f, -2.5f},
		 *		.spin = glm::vec3{2.0f, -1.0f, 2.0f}
		 * });
		 * @endcode
		 *
		 *
		 * @tparam T componente da settare.
		 * \param eid identificativo dell'entità a cui si vuole settare la componente.
		 * \param component componente da settare.
		 */
		template<typename T>
		constexpr void set(const Eid eid, T& component) {
			if (!this->registered<T>()) [[unlikely]]
				register_component<T>();

			assert(this->registered<T>() && "set<T> di un componente non registrato nel database.");
			getComponentArray<T>()->set(eid, component);
		}

		/**
		 * @brief Recupera il puntatore a una singola componente di un entità.
		 * 
		 * Consente di ottenere il puntatore ad una componente per una entità.
		 * La componente è specificabile attraverso il template @code{.cpp} T @endcode.
		 * 
		 * @code{.cpp}
		 * auto* pos = database.get<C::Position>(eid);
		 * pos->position;			      // accesso alla posizione della componente
		 * pos->rotation;			      // accesso alla rotazione della componente
		 * pos->translate({1.0f, 1.0f}); // accesso alle funzioni della componente
		 * @endcode
		 * 
		 * @tparam T componente a cui si vuole accedere.
		 * \param eid identificativo dell'entità a cui si vuole accedere.
		 * \return puntatore ad una componente T dell'entità eid.
		 */
		template<typename T>
		[[nodiscard]] constexpr T* get(const Eid eid) {
			return getComponentArray<T>()->get(eid);
		}

		/**
		 * @brief Recupera il puntatore a molteplici componenti di un entità.
		 *
		 * Consente di ottenere il puntatore a più componenti per una entità.
		 * Le componenti sono specificabili attraverso il template @code{.cpp} T1 @endcode, 
		 * @code{.cpp} T2 @endcode, @code{.cpp} ...Ts @endcode. La funzione è un template ricorsivo
		 * definito in termini di Database::get<T>.
		 *
		 * @code{.cpp}
		 * // Accedo alle tre componenti dividendo la tupla con un unpacking
		 * auto [pos, mov, ren] = database.get<C::Position, C::Movement, C::Render>(eid);
		 * pos->translate({1.0f, 1.0f, 1.0f});
		 * pos->rotate({1.0f, 1.0f, 1.0f});
		 * mov->accelerate({0.0f, 1.0f, 0.0f});
		 * ren->model->draw(...);
		 * @endcode
		 *
		 * @tparam T1 prima componente a cui si vuole accedere.
		 * @tparam T2 seconda componente a cui si vuole accedere.
		 * @tparam Ts altre componenti a cui si vuole accedere.
		 * \param eid identificativo dell'entità a cui si vuole accedere.
		 * \return una std::tuple contenente i puntatori alle componenti.
		 */
		template<typename T1, typename T2, typename ...Ts> [[nodiscard]] constexpr std::tuple<T1*, T2*, Ts*...> get(const Eid eid);

		/**
		 * @brief Verifica se un'entità ha una componente.
		 * 
		 * Consente di verificare la presenza di una componente per una entità.
		 * La componente è specificabile attraverso il template @code{.cpp} T @endcode.
		 * 
		 * @code{.cpp}
		 * bool has = database.has<C::AABB>(eid);
		 * @endcode
		 * 
		 * \param eid identificativo dell'entità.
		 * \return @code{.cpp} true @endcode se l'entità ha una componente @code{.cpp} T @endcode valida.
		 */
		template<typename T>
		[[nodiscard]] constexpr bool has(const Eid eid) {
			if (this->registered<T>()) {
				std::shared_ptr<ComponentArray<T>> x = getComponentArray<T>();
				bool y = x->has(eid);
			}

			return this->registered<T>() && getComponentArray<T>()->has(eid);
		}

		/**
		 * @brief Verifica se un'entità ha molteplici componenti.
		 *
		 * Consente di verificare la presenza di più componenti per una entità.
		 * Le componenti sono specificabili attraverso il template @code{.cpp} T1 @endcode,
		 * @code{.cpp} T2 @endcode, @code{.cpp} ...Ts @endcode. La funzione è un template ricorsivo
		 * definito in termini di Database::has<T>.
		 *
		 * @code{.cpp}
		 * bool has = database.has<C::Position, C::Movement, C::Render>(eid);
		 * @endcode
		 *
		 * @tparam T1 prima componente.
		 * @tparam T2 seconda componente.
		 * @tparam Ts eventuali altre componenti.
		 * \param eid identificativo dell'entità.
		 * \return @code{.cpp} true @endcode se l'entità ha tutte le componenti specificate.
		 */
		template<typename T1, typename T2, typename ...Ts> [[nodiscard]] constexpr bool has(const Eid eid);

		/**
		 * @brief Rimuove una componente di una entità.
		 *
		 * Consente di rimuovere una componente per una entità.
		 * La componente da rimuovere è specificabile attraverso il template @code{.cpp} T @endcode.
		 *
		 * @code{.cpp}
		 * database.remove<C::Sprite>(e)
		 * @endcode
		 *
		 * @tparam T componente da rimuovere.
		 * \param eid identificativo dell'entità a cui si vuole settare la componente.
		 */
		template<typename T>
		constexpr void remove(const Eid eid) {
			getComponentArray<T>()->remove(eid);
		}


		Signature signature(const Eid eid);

		/**
		 * @brief Crea una nuova entità.
		 * 
		 * \return un nuovo oggetto fn::Entity che wrappa l'enntità creata.
		 */
		[[nodiscard]] Entity create_entity();

		/** @brief distrugge una entità */
		void destroy_entity(Eid eid);

		//Query
		[[nodiscard]] std::vector<Entity> having(const Signature signature);
		template<typename T, typename ...Ts> [[nodiscard]] std::vector<Entity> having();
		void for_each(const Signature signature, std::function<void(Eid e)> fun);
		void for_each(const Signature signature, std::function<void(Entity& e)> fun);

		/**
		 * @brief Applica una lambda a tutte le entità con una specifica componente.
		 * 
		 * Consente applicare una funzione lambda iterando su tutte le entità che hanno una specifica
		 * componente. La funzione applicata è del tipo @code{.cpp} void(Entity& e) @endcode dunque consente
		 * di accedere completamente all'entità.
		 * La componente richiesta è specificabile attraverso il template @code{.cpp} T @endcode.
		 * 
		 * @code{.cpp}
		 * database.for_each<C::Particle>([](Entity& e){ 
		 *		// fai qualcosa con `e` ...
		 * });
		 * @endcode
		 * 
		 * La funzione si divide in due parti, la query per individuare nel database le entità coinvolte 
		 * e l'applicazione di fun a ciascuna di queste.
		 * 
		 * @tparam T componente.
		 * @param fun funzione da applicare.
		 */
		template<typename T> void for_each(std::function<void(Entity& e)> fun);

		/**
		 * @brief Applica una lambda a tutte le entità con un insieme di componenti.
		 *
		 * Consente applicare una funzione lambda iterando su tutte le entità che hanno le
		 * componenti specificate. La funzione applicata è del tipo @code{.cpp} void(Entity& e) @endcode dunque consente
		 * l'accesso completo all'entità.
		 * Le componenti richiesta sono specificabili attraverso i template @code{.cpp} T1 @endcode,
		 * @code{.cpp} T2 @endcode, @code{.cpp} ...Ts @endcode.
		 *
		 * @code{.cpp}
		 * database.for_each<C::Position, C::Movement>([](Entity& e){ 
		 *		// fai qualcosa con `e` ...
		 * });
		 * @endcode
		 *
		 * La funzione si divide in due parti, la query per individuare nel database le entità coinvolte
		 * e l'applicazione di fun a ciascuna di queste.
		 *
		 * @tparam T1 prima componente.
		 * @tparam T2 seconda componente.
		 * @tparam Ts eventuali altre componenti.
		 * @param fun funzione da applicare.
		 */
		template<typename T1, typename T2, typename ...Ts> void for_each(std::function<void(Entity& e)> fun);

		/**
		 * @brief Applica una lambda a tutte le entità con un insieme di componenti.
		 * 
		 * @code{.cpp}
		 * database.for_each<C::Render>([](C::Render& r){
		 *		// fai qualcosa con `r` ...
		 * });
		 * @endcode
		 * 
		 * Simile alle precedenti, ma la funzione lambda è del tipo @code{.cpp} void(T&) @endcode ovvero
		 * fornisce direttamente un reference alla componente quando si itera piuttosto che una entità.
		 */
		template<typename T> void for_each(std::function<void(T& e)> fun);

		/**
		 * @brief Applica una lambda a tutte le entità con un insieme di componenti.
		 * 
		 * Simile alle precedenti, ma la funzione lambda è del tipo @code{.cpp} void(T&, Ts&...) @endcode ovvero
		 * fornisce direttamente un reference alle componenti quando si itera piuttosto che una entità.
		 */
		template<typename T, typename ...Ts> void for_each(std::function<void(T&, Ts&...)> fun);

		/**
		 * @brief Applica una lambda a tutte le entità con un insieme di componenti.
		 *
		 * Simile alle precedenti, ma la funzione lambda è del tipo @code{.cpp} void(fn::Eid, Ts&...) @endcode ovvero
		 * fornisce anche il fn::Eid insieme ai reference alle componenti quando si itera.
		 */
		template<typename ...Ts> void for_each(std::function<void(fn::Eid, Ts&...)> fun);


	private:
		std::unordered_map<Cid, std::shared_ptr<IComponentArray>> m_components{};
		template<typename T>
		[[nodiscard]] std::shared_ptr<ComponentArray<T>> getComponentArray() {
			assert(this->registered<T>() && "Errore, componente non registrato nel database.");
			const Cid cid = ComponentArray<T>::cid;
			return std::static_pointer_cast<ComponentArray<T>>(m_components[cid]);
		}

		// Funzioni e template di utility
		// Effettua una call di una funzione unpackando una tupla
		template<typename Function, typename Tuple, size_t ... I>
		inline auto call(Function f, fn::Eid eid, Tuple t, std::index_sequence<I ...>){ return f(eid, *std::get<I>(t) ...); }
		template<typename Function, typename Tuple, size_t ... I>
		inline auto call(Function f, Tuple t, std::index_sequence<I ...>) { return f(*std::get<I>(t) ...); }

		// Per la gestione delle entità:
		std::stack<Eid> eid_pool{};				//Pool di id disponibili, contiene inizialmente MAX_ENTITY_COUNT valori

	};

	template<typename T1, typename T2, typename ...Ts>
	constexpr inline bool Database::has(const Eid eid)
	{
		return this->has<T1>(eid) && this->has<T2, Ts...>(eid);
	}

	template<typename T1, typename T2, typename ...Ts>
	constexpr inline std::tuple<T1*, T2*, Ts*...> Database::get(const Eid eid)
	{
		//TODO: Ritornare errore se non ha il valore quell'entità? Ora torna nullptr...
		if constexpr (sizeof...(Ts) > 0)
			return std::tuple_cat(
				std::make_tuple(get<T1>(eid)),
				get<T2, Ts...>(eid)
			);
		else return std::make_tuple(get<T1>(eid), get<T2>(eid));
	}

	template<typename T, typename ...Ts>
	inline std::vector<Entity> Database::having()
	{
		const Signature sign = Sign<T, Ts...>;
		return std::move(this->having(sign));
	}



	template<typename T>
	inline void Database::for_each(std::function<void(Entity& e)> fun)
	{
		const Cid cid = ComponentArray<T>::cid;
		auto ca = this->getComponentArray<T>();
		for (unsigned int i = 0; i < ca->validity().size(); i++) {
			if (ca->validity()[i]) [[unlikely]] {
				Entity e{i, this};
				fun(e);
			}
		}
	}
	
	template<typename T1, typename T2, typename ...Ts>
	inline void Database::for_each(std::function<void(Entity& e)> fun)
	{
		const Signature sign = Sign<T1, T2, Ts...>;
		this->for_each(sign, fun);
		
	}
	
	template<typename T>
	inline void Database::for_each(std::function<void(T& e)> fun)
	{
		const Cid cid = ComponentArray<T>::cid;
		auto ca = this->getComponentArray<T>();
		for (unsigned int i = 0; i < ca->validity().size(); i++) {
			if (ca->validity()[i]) [[unlikely]] {
				fun(*this->get<T>(i));
			}
		}
	}

	template<typename T, typename ...Ts>
	inline void Database::for_each(std::function<void(T&, Ts&...)> fun)
	{
		static constexpr auto size = sizeof...(Ts) + 1;
		const Signature signature = Sign<T, Ts...>;
		auto bitmap = std::move(std::bitset<ecs::MAX_ENTITY_COUNT>{}.set());

		for (auto& [cid, component_array] : this->m_components) {
			if ((signature & cid).any())										//Se nella firma ci sta il cid allora posso procedere
				bitmap = bitmap & component_array->validity();
		}

		for (unsigned int i = 0; i < bitmap.size(); i++) {
			if (bitmap[i]) [[unlikely]] {
				call(fun, this->get<T, Ts...>(i), std::make_index_sequence<size>{});
			}
		}
	}

	template<typename ...Ts>
	inline void Database::for_each(std::function<void(fn::Eid, Ts&...)> fun)
	{
		static constexpr auto size = sizeof...(Ts);
		const Signature signature = Sign<Ts...>;
		auto bitmap = std::move(std::bitset<ecs::MAX_ENTITY_COUNT>{}.set());

		for (auto& [cid, component_array] : this->m_components) {
			if ((signature & cid).any())										//Se nella firma ci sta il cid allora posso procedere
				bitmap = bitmap & component_array->validity();
		}

		for (unsigned int i = 0; i < bitmap.size(); i++) {
			if (bitmap[i]) [[unlikely]] {
				call(fun, i, this->get<Ts...>(i), std::make_index_sequence<size>{});
			}
		}
		
		
	}


}


/***************************************************************************************/

namespace fn {
	template<typename T>
	constexpr inline bool Entity::has() {
		return m_database->has<T>(m_eid);
	}
	template<typename T1, typename T2, typename ...Ts>
	constexpr inline bool Entity::has()
	{
		return has<T1>() && has<T2, Ts...>();
	}

	template<typename T, class ...Args>
	constexpr inline Entity& Entity::set(Args && ...args)
	{
		m_database->set<T>(m_eid, args);
		m_update_signature();
		return *this;
	}

	template<typename T>
	constexpr inline Entity& Entity::set(T component)
	{
		m_database->set<T>(m_eid, component);
		m_update_signature();
		return *this;
	}

	template<typename T>
	constexpr inline Entity& fn::Entity::remove()
	{
		m_database->remove<T>(m_eid);
		m_update_signature();
		return *this;
	}

	template<typename T>
	constexpr inline T* fn::Entity::get()
	{
		//TODO: Ritornare errore se non ha il valore quell'entità? Ora torna nullptr...
		return m_database->get<T>(m_eid);
	}

	template<typename T1, typename T2, typename ...Ts> 
	constexpr inline std::tuple<T1*, T2*, Ts*...> fn::Entity::get()
	{
		//TODO: Ritornare errore se non ha il valore quell'entità? Ora torna nullptr...
		return std::make_tuple(m_database->get<T1>(m_eid), m_database->get<T2, Ts...>(m_eid));
	}

}

