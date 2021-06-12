#pragma once
#include <string>
#include <unordered_map>
#include <fn/fn.hpp>
#include <stb_image.h>
#include <fn/core/gl/Shader.hpp>
#include <fn/core/gl/Texture.hpp>
#include <fn/core/gl/Model.hpp>
#include <fn/core/audio/Audio.hpp>

using ModelSP = std::shared_ptr<Model>;

/**
 * \brief Gestore di asset e risorse di gioco.
 * 
 * La classe mette a disposizione una serie di metodi per il caricamento di 
 * risorse e asset quali Shader, Model, Texture, Audio.
 * 
 * Il caricamento avviene sfruttando il pattern Flyweight separando la risorsa 
 * dal suo utilizzo.
 */
class AssetManager
{
public:
	/**
	 * \brief Carica, compila e linka uno shader in GL.
	 * 
	 * La funzione prende in input il percorso al codice sorgente di un 
	 * vertex shader e un fragment shader li legge, compila ed infine linka
	 * restituendo uno shared pointer ad un oggetto Shader.
	 * 
	 * Gli shader già caricati sono memorizzati in s_shaderCache, successive
	 * richieste allo stesso shader restituiranno lo stesso puntatore.
	 * 
	 * \param vertex_filepath percorso al file contenente il codice sorgente
	 *        del vertex shader.
	 * \param fragment_filepath percorso al file contenente il codice sorgente
	 *        del fragment shader.
	 * \param args eventuali parametri extra che saranno inoltrati al
	 *        costruttore dell'oggetto Shader da creare.
	 * \return Shared pointer allo shader caricato.
	 */
	template<typename ... Args> static constexpr ShaderSP loadShader(const fs::path& vertex_filepath, const fs::path& fragment_filepath, Args&& ... args);

	/**
	 * \brief Carica, un modello.
	 *
	 * La funzione prende in input il percorso al file del modello e
	 * restituisce uno shared pointer ad un oggetto Model.
	 * La funzione usa ASSIMP come back-end per il caricamento.
	 *
	 * I modelli già caricati sono memorizzati in s_modelCache, successive
	 * richieste allo stesso modello restituiranno lo stesso puntatore.
	 *
	 * \param filepath percorso al file del modello.
	 * \param args eventuali parametri extra che saranno inoltrati al
	 *        costruttore dell'oggetto Model da creare.
	 * \return Shared pointer al modello caricato.
	 */
	template<typename ... Args> static constexpr ModelSP loadModel(const fs::path& filepath, Args&& ... args);

	/**
	 * \brief Carica, una texture.
	 *
	 * La funzione prende in input il percorso al file di una texture e
	 * restituisce uno shared pointer ad un oggetto Texture.
	 * La funzione usa stb_image come back-end per il caricamento.
	 *
	 * Le texture già caricate sono memorizzate in s_textureCache, successive
	 * richieste alla stessa texture restituiranno lo stesso puntatore.
	 *
	 * \param filepath percorso al file della texture.
	 * \param args eventuali parametri extra che saranno inoltrati al
	 *        costruttore dell'oggetto Texture da creare.
	 * \return Shared pointer alla texture caricata.
	 */
	template<typename ... Args> static constexpr TextureSP loadTexture(const fs::path& filepath, Args&& ... args);

	/**
	 * \brief Carica, un file Audio.
	 *
	 * La funzione prende in input il percorso al file audio e
	 * restituisce uno shared pointer ad un oggetto Audio.
	 * La funzione usa irrKlang come back-end per il caricamento.
	 *
	 * Gli audio già caricati sono memorizzati in s_audioCache, successive
	 * richieste allo stesso audio restituiranno lo stesso puntatore.
	 *
	 * \param filepath percorso al file dell'audio.
	 * \param args eventuali parametri extra che saranno inoltrati al
	 *        costruttore dell'oggetto Audio da creare.
	 * \return Shared pointer all'audio caricato.
	 */
	template<typename ... Args> static constexpr AudioSP loadAudio(const fs::path& filepath, Args&& ... args);

private:
	static std::unordered_map<std::string, TextureSP> s_textureCache;				//!< cache per le texture.
	static std::unordered_map<std::string, ModelSP> s_modelCache;					//!< cache per i modelli.
	static std::unordered_map<std::string, ShaderSP> s_shaderCache;					//!< cache per gli shader.
	static std::unordered_map<std::string, AudioSP> s_audioCache;					//!< cache per gli audio.
};


template<typename ...Args>
inline constexpr ShaderSP AssetManager::loadShader(const fs::path& vertex_filepath, const fs::path& fragment_filepath, Args && ...args)
{
	// Verifico che il modello non sia in cache
	auto f = vertex_filepath.string() + "_" + fragment_filepath.string();
	if (AssetManager::s_shaderCache.find(f) != AssetManager::s_shaderCache.end())
		return AssetManager::s_shaderCache[f];

	auto shader = std::make_shared<Shader>(vertex_filepath, fragment_filepath, std::forward<Args>(args)...);
	AssetManager::s_shaderCache[f] = shader;
	return shader;
}

template<typename ...Args>
inline constexpr ModelSP AssetManager::loadModel(const fs::path& filepath, Args && ...args)
{
	// Verifico che il modello non sia in cache
	auto f = filepath.string();
	if (AssetManager::s_modelCache.find(f) != AssetManager::s_modelCache.end())
		return AssetManager::s_modelCache[f];

	auto model = std::make_shared<Model>(filepath, std::forward<Args>(args)...);
	AssetManager::s_modelCache[f] = model;
	return model;
}

template<typename ...Args>
inline constexpr TextureSP AssetManager::loadTexture(const fs::path& filepath, Args && ...args)
{
	// Verifico che la texture non sia in cache
	auto f = filepath.string();
	if (AssetManager::s_textureCache.find(f) != AssetManager::s_textureCache.end())
		return AssetManager::s_textureCache[f];

	auto texture = std::make_shared<Texture>(filepath, std::forward<Args>(args)...);
	AssetManager::s_textureCache[f] = texture;
	return texture;
}

template<typename ...Args>
inline constexpr AudioSP AssetManager::loadAudio(const fs::path& filepath, Args && ...args)
{
	// Verifico che la texture non sia in cache
	auto f = filepath.string();
	if (AssetManager::s_audioCache.find(f) != AssetManager::s_audioCache.end())
		return AssetManager::s_audioCache[f];

	auto audio = std::make_shared<Audio>(filepath, std::forward<Args>(args)...);
	AssetManager::s_audioCache[f] = audio;
	return audio;
}
