#pragma once
#include<fn/ecs/systems/System.hpp>
#include <fn\core\gl\Sprite.hpp>

namespace S {
	/**
	 * @brief Sistema che gestisce le Combo e i Blitz.
	 *
	 * La classe Combo consente di gestire le combo di frutti all'interno del gioco.
	 * 
	 * I componenti coinvolti dal sistema sono C::Sprite e C::Particle.
	 */
	class Combo : public S::System
	{
	public:
		/** @brief Livelli di blitz. */
		enum struct BlitzLevel : int {
			none = 0,
			combo = 5,
			great = 10,
			awesome = 15,
		};

		/** @brief Costruttore. */
		Combo(State* state, fn::Database& database);
		virtual void update(core::seconds delta_t) override;

		/** @brief Notifica un nuovo split di un frutto. */
		void addSplit() {
			this->m_lastSplit = core::seconds{ 0 };
			m_splitCount++;
		}
		BlitzLevel blitzLevel() const;

	protected:
		/**
		 * @brief Funzione richiamata in caso di combo.
		 * 
		 * La funzione viene chiamata quando si verifica una combo, ovvero quando si tagliano almeno tre frutti
		 * entro il tempo di timeout.
		 */
		void onCombo();

		/**
		 * @brief Funzione richiamata in caso di blitz.
		 *
		 * La funzione viene chiamata quando si verifica un blitz, ovvero quando si effettuano almeno tre combo
		 * entro il tempo di timeout.
		 */
		void onBlitz();

		/** @brief Aggiorna gli elementi grafici */
		void updateParticles(core::seconds delta_t);

		/** @brief Aggiorna gli elementi grafici */
		void updateArrowBlitz(core::seconds delta_t);


		fn::Database& m_database;
		State* m_state;
		// COMBO
		unsigned int m_splitCount;					//!< Conta il numero di split effettuati fino a questo momento
		core::seconds m_lastSplit;					//!< Tempo trascorso dall'ultimo split
		const core::seconds m_splitTimeout{ 0.5 };	//!< Tempo massimo dal `lastSplit` prima che `splitCount` si resetti

		// BLITZ
		unsigned int m_comboCount;					//!< Conta il numero di combo effettuate fino a questo momento
		core::seconds m_lastCombo;					//!< Tempo trascorso dall'ultima combo
		const core::seconds m_comboTimeout{ 8.0 };	//!< Tempo massimo dal `lastCombo` prima che `comboCount` si resetti

		const core::seconds m_blitzTimeout{ 16.0 };	//!< Tempo massimo dal `m_lastBlitz` prima che `m_lastBlitzLevel` si resetti
		const core::seconds m_blitzDelay{ 8.0 };	//!< Tempo che deve trascorrere per il rinnovo della blitz di massimo livello
		core::seconds m_lastBlitz;					//!< Tempo trascorso dall'ultimo blitz
		BlitzLevel m_lastBlitzLevel;

		SpriteSP m_arrowSprite1;
		SpriteSP m_arrowSprite2;

	private:
		static inline constexpr unsigned int MAX_COMBO_COUNT = 6;
		static inline constexpr BlitzLevel MAX_BLITZ_LEVEL = BlitzLevel::awesome;
		static inline constexpr core::seconds PARTICLE_LIFETIME{ 1.2 };
	};
}


