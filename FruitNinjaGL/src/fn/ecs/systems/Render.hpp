#pragma once
#include<fn/ecs/systems/System.hpp>

namespace S {
	/**
	 * @brief Sistema che gestisce il disegno.
	 *
	 * La classe Render consente di gestire il disegno delle entità.
	 * I componenti coinvolti dal sistema sono: C::Position, C::Render per gli oggetti tridimensionali
	 * e C::Sprite per gli oggetti bidimensionali.
	 */
	class Render : public System
	{
	public:
		/** @brief Costruttore. */
		Render(fn::Database& database);
		virtual void update(core::seconds delta_t) override;

		/**
		 * @brief Disegna le entità..
		 * 
		 * Disegna le entità in due fasi, prima quelle dotate di 
		 * (C::Position, C::Render) e poi quelle con C::Sprite.
		 * 
		 * \param camera
		 */
		virtual void draw(const fn::Camera& camera);

	private:
		fn::Database& m_database;
	};
}