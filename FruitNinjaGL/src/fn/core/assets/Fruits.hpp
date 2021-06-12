#pragma once
#include<fn/fn.hpp>
#include<array>

/**
 * @file
 * @brief Class @ref Fruits, @ref Fruits::Species, @ref Fruits::Model
 */

/**
 * @brief Asset e meta-dati sui frutti.
 * 
 * La classe è implementata imitando il funzionamento di un enum. Oltre alle informazioni dei singoli 
 * frutti sono presenti anche metodi di utility.
 */
class Fruits {
public:
	static const Fruits apple_red;
	static const Fruits banana;
	static const Fruits coconut;
	static const Fruits honeydew_melon;
	static const Fruits kiwi;
	static const Fruits lemon;
	static const Fruits lime;
	static const Fruits orange;
	static const Fruits pear;
	static const Fruits pineapple;
	static const Fruits plum;
	static const Fruits pomegranate;
	static const Fruits strawberry;
	static const Fruits watermelon;
	static const Fruits tomato;
	static const Fruits pepper_green;
	static const Fruits pepper_pointed;
	static const Fruits pepper_red;
	static const Fruits pepper_yellow;

	/** Specie di appartenenza del frutto. */
	enum class Species {
		apple,
		banana,
		coconut,
		honeydew_melon,
		melon,
		kiwi,
		lemon,
		lime,
		orange,
		pear,
		pineapple,
		plum,
		pomegranate,
		strawberry,
		watermelon,
		tomato,
		pepper,
	};

	/** Tipologie di modelli 3d possibili per il frutto. */
	enum class Model {
		whole, half_front, half_back
	};

	std::string name;						//!< Nome descrittivo del frutto
	Species species;						//!< Specie del frutto (mela, banana, cocco...)
	fs::path modeldir;                      //!< Path alla cartella contenente tutti i modelli
	std::string whole_filename;				//!< Nome del file+estensione contenuto in `modeldir` che rappresenta il modello del frutto intero
	std::string half_back_filename;			//!< Nome del file+estensione contenuto in `modeldir` che rappresenta il modello della prima metà del frutto
	std::string half_front_filename;		//!< Nome del file+estensione contenuto in `modeldir` che rappresenta il modello della seconda metà del frutto
	float weight;							//!< Peso del frutto, in kg.

	/**
	 * Percorso del modello richiesto.
	 * 
	 * \param mtype tipo del modello per il quale si vuole il percorso.
	 * \return path completo al file contenente il modello.
	 * 
	 * @see @ref Fruits::Model
	 */
	inline fs::path modelpath(const Fruits::Model mtype) const {
		switch (mtype)
		{
		case Fruits::Model::whole:
			return modeldir / whole_filename;
		case Fruits::Model::half_back:
			return modeldir / half_back_filename;
		case Fruits::Model::half_front:
			return modeldir / half_front_filename;
		default:
			return "";
		}
	}

	/**
	 * Ritorna tutti i frutti.
	 * 
	 * \return Array di 19 elementi contenente tutti i frutti.
	 */
	static const std::array<const Fruits*, 19>& list() {
		static std::array<const Fruits*, 19> tmp = {
			&apple_red, &banana, &coconut, &honeydew_melon, &kiwi,
			&lemon, &lime, &orange, &pear, &pineapple,
			&plum, &pomegranate, &strawberry, &watermelon, &tomato,
			&pepper_pointed, &pepper_green, &pepper_red, &pepper_yellow
		};
		return tmp;
	}

	/**
	 * Restituisce un frutto random.
	 * 
	 * \return Frutto casuale fra quelli disponibili in Fruits::list()
	 */
	static const Fruits& random() {
		auto& tmp = list();
		return *tmp[std::rand() % tmp.size()];
	}
};