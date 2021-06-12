#pragma once
#include"System.hpp"

namespace S {
	/**
	 * @brief Sistema che gestisce le collisioni.
	 * 
	 * La classe Collision si occupa di cestire ciò che avviene quando due oggetti collidono. 
	 * implementando sia la rilevazione che l'azione alla collisione.
	 * I componenti coinvolti dal sistema sono: C::Position, C::Movement, C::AABB e C::Fruit.
	 * 
	 * Le collisioni sono calcolate soltanto per le entità dotate delle precedenti quattro componenti 
	 * ristretta ulteriormente ai soli frutti interi. Infatti per scelta i frutti già tagliati sono intangibili.
	 * 
	 * @section detection Rilevamento delle collisioni.
	 * 
	 * Il rilevamento delle collisioni avviene utilizzando un approccio ibrido fra [OOB](https://en.wikipedia.org/wiki/Minimum_bounding_box#Object-oriented_minimum_bounding_box) e la semplice sfera.
	 * 1. Al momento del caricamento del modello, viene calcolata anche la [AABB](https://en.wikipedia.org/wiki/Minimum_bounding_box#Axis-aligned_minimum_bounding_box)
	 *    relativa alle sue coordinate model.
	 * 2. Durante il check delle collisioni al AABB viene trasformata in OOB applicando le stesse trasformazioni di modellazione dell'entità al box.
	 * 3. A partire da questo nuovo box viene calcolato il raggio della sfera centrato nel centro tangente al suo lato più lungo. 
	 * Il raggio individua una sfera che sarà utilizzata per la rilevazione.
	 * 
	 * Si è scelto questo approccio perché semplice e si adatta bene con la quasi totalità dei frutti i quali sono piuttosto tondeggianti.
	 * 
	 * @section action Risposta alle collisioni.
	 * 
	 * Quando due frutti collidono, la risposta alla collisione cerca di imitare gli urti elastici della fisica classica.
	 * In particolare, si tiene conto delle velocità iniziali e della massa dei frutti per calcolare le velocità relative a seguito dell'impatto.
	 * La formula utilizzata è: 
	 * @f[
	 * {\begin{aligned}\mathbf {v} '_{1}&=\mathbf {v} _{1}-{\frac {2m_{2}}{m_{1}+m_{2}}}\ {\frac {\langle \mathbf {v} _{1}-\mathbf {v} _{2},\,\mathbf {x} _{1}-\mathbf {x} _{2}\rangle }{\|\mathbf {x} _{1}-\mathbf {x} _{2}\|^{2}}}\ (\mathbf {x} _{1}-\mathbf {x} _{2}),\\\mathbf {v} '_{2}&=\mathbf {v} _{2}-{\frac {2m_{1}}{m_{1}+m_{2}}}\ {\frac {\langle \mathbf {v} _{2}-\mathbf {v} _{1},\,\mathbf {x} _{2}-\mathbf {x} _{1}\rangle }{\|\mathbf {x} _{2}-\mathbf {x} _{1}\|^{2}}}\ (\mathbf {x} _{2}-\mathbf {x} _{1})\end{aligned}}
	 * @f]
	 * Dove @f$ \mathbf{x}_1 @f$ e @f$ \mathbf{x}_2 @f$ sono i centri dei due oggetti al momento dell'impatto, @f$ \mathbf{v}_1 @f$ e @f$ \mathbf{v}_2 @f$ le velocità dei frutti ed in fine 
	 * @f$ m_1 @f$ e @f$ m_2 @f$ le loro masse.
	 */
	class Collision : public System
	{
	public:
		/**
		 * @brief Costruttore.
		 * 
		 * \param database istanza del database su cui agire.
		 */
		Collision(fn::Database& database);
		virtual void update(core::seconds delta_t) override;

	private:
		fn::Database& m_database;
		glm::vec3 calculate_relative_velocity(float m1, float m2, glm::vec3 v1, glm::vec3 v2, glm::vec3 c1, glm::vec3 c2) const;
	};
}

