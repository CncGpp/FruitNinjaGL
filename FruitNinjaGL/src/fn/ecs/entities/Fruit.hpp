#pragma once
#include <fn\ecs\Entity.hpp>
#include <fn\core\assets\Fruits.hpp>
#include <fn\core\assets\assets.hpp>

namespace E{
	/**
	 * @brief Frutto come entità.
	 * 
	 * La classe Fruit specializza il concetto di Entity consentendo per composizione di creare
	 * una entità di tipo Frutto aggiungendo gli opportuni componenti.
	 * 
	 * Le componenti richieste sono: C::Position, C::Movement, C::Fruit, C::Render e C::AABB.
	 * 
	 * Si tratta dunque di un builder che permette la costruzione di una entità con le giuste componenti
	 * tali da essere considerata un Frutto.
	 * Oltre a ciò la classe similmente ad fn::Entity può essere usata come wrapper di un entità e mette
	 * a disposizione alcuni metodi per la gestione dei frutti (es lo split di un frutto).
	 */
	class Fruit : public fn::Entity
	{
	public:
		/**
		 * @brief Costruisce una nuova entità con componenti fruit.
		 * 
		 * \param db database necessario alla creazione dell'entità.
		 * \param fruit Tipologia del frutto da costruite.
		 * \param model_kind tipo di modello del frutto da costruire.
		 * \return Una entity con le componenti necessarie.
		 */
		[[nodiscard]] static Fruit make(fn::Database& db, const Fruits& fruit, const Fruits::Model model_kind = Fruits::Model::whole);

		/**
		 * @brief Effettua lo split di un frutto in due entità.
		 * 
		 * Divide un frutto con model_kind::whole nelle due sue parti,
		 * creando due entità a partire da quella attuale che viene poi distrutta.
		 * 
		 * Le due perti generate avranno due nuovi vettori velocità e rotazione
		 * calcolati a partire dal frutto originario.
		 * 
		 * \param db Database da utilizzare.
		 * \param eid identificativo dell'entità frutto da splittare.
		 */
		[[nodiscard]] static void split(fn::Database& db, const fn::Eid eid);
	protected:
	private:
		Fruit() = delete;
		Fruit(const fn::Eid eid, fn::Database* db) : fn::Entity(eid, db){};
		Fruit(const fn::Entity& e) : fn::Entity(e) {};
		static inline const auto S_DEFAULT_VERTEX_SHADER = assets::dir::shaders / "fruits" / "default.vglsl";
		static inline const auto S_DEFAULT_FRAGMENT_SHADER = assets::dir::shaders / "fruits" / "default.fglsl";
	};
}