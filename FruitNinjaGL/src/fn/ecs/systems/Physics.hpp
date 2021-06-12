#pragma once
#include<fn/ecs/systems/System.hpp>

namespace S {
	/**
	 * @brief Sistema che gestisce la fisica.
	 * 
	 * La classe Physics consente di gestire lo stato fisico delle entità aggiornandone
	 * posizione, rotazione, velocità e rendendoli soggetti alla forza gravitazionale.
	 * I componenti coinvolti dal sistema sono: C::Position, C::Movement.
	 * 
	 * La classe ha anche il compito di eliminare quelle entità che a causa della gravità escono dallo schermo.
	 */
	class Physics : public S::System
	{
	public:
		Physics(fn::Database& database) : m_database(database) {}
		virtual void update(core::seconds delta_t) override;
	private:
		fn::Database& m_database;
	};
}


