#pragma once
#include <queue>
#include<fn/ecs/systems/System.hpp>
#include <fn\ecs\entities\Fruit.hpp>
#include <fn\ecs\components\components.hpp>

namespace S {
	/**
	 * @brief Sistema che gestisce la le ondate di frutti.
	 *
	 * La classe Physics consente di gestire il lancio di frutti, raggruppandole in `ondate`
	 * di vario tipo e stabilendo i tempi di lancio.
	 * I componenti coinvolti dal sistema sono: C::Position, C::Movement e C::Fruit.
	 */
	class Wave : public S::System
	{
	public:
		/** @brief Costruttore. */
		Wave(fn::Database& database);
		virtual void update(core::seconds delta_t) override;

	protected:
		/**
		 * @brief Tipologia di wave.
		 * 
		 * Ognuna di queste tipologie corrisponde ad una specializzazione del tameplate Wave::mk_wave
		 */
		enum struct WaveType : int {
			RANDOM = 0,
			LEFT_TO_RIGHT,
			RIGHT_TO_LEFT,
			SPOT,
		};
		/**
		 * @brief Componenti di un generico frutto da lanciare.
		 * 
		 * La struct racchiude le componenti da utilizzare per la costruzione di un frutto.
		 * Il delay rappresenta il tempo di lancio dalla creazione.
		 */
		struct Spawn {
			C::Position position;
			C::Movement movement;
			C::Fruit fruit;
			core::seconds delay;
		};

		/**
		 * @brief Costruisce una ondata.
		 * 
		 * Esiste una specializzazione di questa funzione per ogni possibile valore in WaveType.
		 * - RANDOM: I frutti sono creati in maniera casuale per posizione, rotazione, velocità, spin e fruit.
		 * - LEFT_TO_RIGHT: I frutti sono tutti dello stesso tipo e lanciati da sinistra verso destra.
		 * - RIGHT_TO_LEFT: Simile al precedente ma l'ordine è da destra verso sinistra.
		 * - SPOT: I frutti sono tutti dello stesso tipo e lanciati casualmente.
		 * 
		 * @tparam type tipologia dell'ondata.
		 * \param count numero di frutti dell'ondata.
		 * \return Vettore con gli spawn creati.
		 */
		template <WaveType type>
		[[nodiscard]] std::vector<Spawn> mk_wave(const unsigned int count = 2);

		/** @brief Costruisce una entità da uno Spawn. */
		[[nodiscard]] E::Fruit mk_fruit(Spawn& s);

		/** @brief Genera la prossima ondata. */
		void next_wave();

	private:
		fn::Database& m_database;
		std::queue<Spawn> m_sheduled_waves{};
	};
}
