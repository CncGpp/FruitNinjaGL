#pragma once
#include<assert.h>
#include<unordered_map>
#include<map>
#include <memory>
#include <array>
#include <bitset>
#include <iostream>
#include <stack>
#include <functional>

namespace fn {
	/** namespace interno dell ecs */
	namespace ecs {
		static inline constexpr auto MAX_ENTITY_COUNT = 1024;					//!< Numero massimo di entità presenti nel ecs simultaneamente
		static inline constexpr auto MAX_COMPONENT_COUNT = 32;					//!< Numero massimo di componenti possibili
	}
	using Eid = unsigned int;													//!< Entity di, Identificativo univoco delle entità
	using Signature = std::bitset<fn::ecs::MAX_COMPONENT_COUNT>;				//!< Firma digitale delle componenti di un'entità
	using Cid = Signature;														//!< Component id, identificativo univoco di una componente

	namespace ecs {
		static inline std::unordered_map<Cid, std::string> cid_to_ctypename{};  //!< Map che consente la conversione fra Cid e typename del componente
		static inline std::map<std::string, Cid> ctypename_to_cid{};			//!< Mappatura inversa fra typename e Cid

		/**
		 * @brief Genera un Cid univoco per una componente.
		 * 
		 * Funzione di utility, genera un Cid univoco progressivo.
		 * Il Cid è una potenza di due usabile in OR per generare una signature
		 * \return Identificativo univoco da usare per una componente
		 */
		[[nodiscard]] static inline Cid generate_component_id() {
			static Cid curr_component_id = 1;
			auto tmp = curr_component_id;
			curr_component_id <<= 1;
			return tmp;
		}
		/**
		 * @brief Restituisce il typename C++ di una componente.
		 * 
		 * @tparam T struct o class di una componente.
		 * Ritorna il typename di una componente dal suo template.
		 * \return stringa contenente il typename della componente
		 */
		template<typename T> [[nodiscard]] constexpr std::string get_component_typename() {
			static std::string component_typename = typeid(T).name();
			return component_typename;
		}
		/**
		 * @brief Restituisce il Cid di una componente.
		 *
		 * @tparam T struct o class di una componente.
		 * Ritorna il Cid che identifica univocamente una componente dal suo template
		 * \return Cid che identifica la componente passata nel template
		 */
		template<typename T> [[nodiscard]] constexpr Cid get_component_id() {
			static Cid component_id = generate_component_id();
			static std::string component_typename = get_component_typename<T>();
			if (ctypename_to_cid.find(component_typename) == ctypename_to_cid.end()) {
				ctypename_to_cid.emplace(component_typename, component_id);
				cid_to_ctypename.emplace(component_id, component_typename);
			}
			return component_id;
		}
	}

	// Funzione base usata Sign
	template<typename T>
	[[nodiscard]] Signature Sign_template() {
		return fn::ecs::get_component_id<T>();
	}
	// Funzione ricorsiva usata da Sign
	template<typename T1, typename T2, typename ...Ts>
	[[nodiscard]] Signature Sign_template() {
		return Sign_template<T1>() | Sign_template<T2, Ts ...>();
	}
	/**
	 * @brief Signature di un insieme di componenti.
	 * 
	 * Template variable che consente di calcolare la firma fornendo più entità.
	 * Ad esempio
	 * <code>
	 * auto s = Sign<C::Position, C::Movement, C::Render>
	 * </code>
	 * Calcola la signature di un entità avente le componenti C::Position, C::Movement e C::Render.
	 */
	template<typename ...Ts> Signature Sign = Sign_template<Ts ...>();





	class Entity;
	class IComponentArray;
	template<typename T>
	class ComponentArray;
	class Database;

	/**
	 * @ Interfaccia base per le componenti.
	 * 
	 * @tparam T struct o class di una componente.
	 * 
	 * Classe base dalla quale tutte le Componenti devono derivare.
	 * La classe determina staticamente il cid e typename della componente oltre
	 * che a fornire alcuni metodi di base per il debug.
	 */
	template<typename T>
	class Component {
	public:
		static inline const Cid cid = fn::ecs::get_component_id<T>();
		static inline const std::string ctypename = fn::ecs::get_component_typename<T>();
		
		/** @brief Stampa alcune info sulla componente. */
		void constexpr info() const {
			std::cout << "Component<T>::cid = " << cid << std::endl;
			std::cout << "Component<T>::ctypename = " << ctypename << std::endl;
		}
		
	};

}