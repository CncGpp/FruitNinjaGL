#pragma once
#include <fn/fn.hpp>

#include "irrKlang.h"
using namespace irrklang;

/**
 * @file
 * @brief Class @ref Audio
 */

class Audio;
using AudioSP = std::shared_ptr<Audio>;		//!< Alias

/**
 * @brief Gestisce la gestione e caricamento dei file audio.
 * 
 * La classe Audio basa il suo funzionamento su [irrKlang](https://www.ambiera.com/irrklang/) un 
 * sound engine e libreria audio che consente di caricare e eseguire numerosissimi file audio ad alta risoluzione sia in 2D che 3D.
 */
class Audio
{
public:
	/**
	 * @brief Costruttore.
	 * 
	 * Carica il file specificato e costruisce un oggetto Audio.
	 * 
	 * \param audiopath percorso al file audio da caricare.
	 */
	Audio(const fs::path& audiopath);

	/** @brief La copia di un oggetto Audio non è permessa. */
	Audio(Audio const&) = delete;

	/** @brief L'assegnazione di un oggetto Audio non è permessa. */
	Audio& operator=(Audio const&) = delete;

	/**
	 * @brief Esegue l'audio caricato in 2D.
	 * 
	 * \param volume Volume dell'audio da eseguire [0, 100].
	 * \param loop se @code{.cpp} true @endcode l'audio è eseguito in loop.
	 */
	void play2D(float volume=1.0f, bool loop=false);

	/**
	 * @brief Esegue l'audio caricato in 3D.
	 *
	 * \param position posizione della sorgente audio in coordinate word.
	 * \param volume Volume dell'audio da eseguire [0, 100].
	 * \param loop se @code{.cpp} true @endcode l'audio è eseguito in loop.
	 */
	void play3D(glm::vec3 position = glm::vec3(), float volume = 1.0f, bool loop = false);

	/** @brief Termina l'esecuzione di tutti i suoni. */
	static void stopAllSounds(){ s_audio_engine->stopAllSounds(); }

protected:
	static ISoundEngine* s_audio_engine;				//!< Puntatore all'engine di irrKlang
	void loadAudio(const fs::path& audiopath);			//!< Esegue il caricamento del file audio

private:
	ISoundSource* m_soundSource;						//!< Puntatore alla sorgente audio di irrKlang relativa all'audio caricato.
};

