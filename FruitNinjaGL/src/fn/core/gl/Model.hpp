#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include<fn/core/gl/Mesh.hpp>

/**
 * @file
 * @brief Class @ref Model
 */

class Model;
using ModelSP = std::shared_ptr<Model>;			//!< Alias

[[nodiscard]] TextureSP TextureFromFile(const char* path, std::string directory, Texture::Type type);

/// <summary>
/// La classe Model consente di caricare da file, memorizzare e gestire un modello 3D.
/// <para>Il caricamento avviene appoggiandosi alla libreria <a href="http://assimp.org/">asslib</a>.</para>
/// </summary>
class Model
{
public:

	/**
	 * Costruttore.
	 * 
	 * \param modelpath Filepath al modello da caricare.
	 */
	Model(const fs::path& modelpath) {
		this->loadModel(modelpath);
	}
	/** @brief La copia di un Model non è permessa */
	Model(Model const&) = delete;

	/** @brief L'assegnamento di un Model non è permesso */
	Model& operator=(Model const&) = delete;

	/// <summary>
	///  Disegna il modello nella finestra. Il disegno avviene disegnando una per una le singole <code>Mesh</code> <see cref='Mesh'>Mesh</see>
	///  che compongono il modello. Il disegno avviene secondo uno shader passato in input.
	/// </summary>
	/// <param name='shader'>Shader da utilizzare per il disegno del modello.</param>
	void draw(ShaderSP& shader);

	glm::mat2x3 AABB() const { return m_AABB; };

private:
	fs::path m_dir;							// Parent dir del modello da leggere.
	std::vector<MeshSP> m_meshes;           // Mesh che compongono il model.
	glm::mat2x3 m_AABB;

	// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	/// <summary>
	/// Carica il modello da file utilizzando la libreria ASSIMP. Le mesh e le texture risultanti sono salvate localmente.
	/// </summary>
	/// <param name="modelpath">path al modello da caricare</param>
	void loadModel(const fs::path& modelpath);

	/// <summary>
	/// Processa i nodi di una <code>aiScene</code> in maniera ricorsiva. Per ogni nodo vengono processate le su mesh e si procede ricorsivamente sui suoi figli (se ci sono)
	/// </summary>
	/// <param name="scene">Scena di ASSIMP che rappresenta l'intero modello</param>
	/// <param name="node">Nodo di partenza della ricerca</param>
	void processNode(const aiScene* scene, const aiNode* node);

	/// <summary>
	/// Funzione base per processare le Mesh contenute all'interno dei nodi.
	/// </summary>
	/// <param name="scene"></param>
	/// <param name="mesh"></param>
	/// <returns>Ritorna un <code>Mesh</code> costruita con i dati caricati.</returns>
	[[nodiscard]] MeshSP processMesh(const aiScene* scene, const aiMesh* mesh);

	void calculateAABB();

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.

	/// <summary>
	/// Controlla tutte le material texture di un determinato `type` e le carica se non sono ancora state caricate.
	/// utilizza <code>textures_cache</code> come memoria cache.
	/// </summary>
	/// <param name="mat">Puntatore all'oggetto <code>aiMaterial</code> contenente informazioni sui materiali.</param>
	/// <param name="type">ASSIMP type (enum) della texture da caricare.</param>
	/// <param name="typeName">Nome di base della tipologia della texture del materiale.</param>
	/// <returns>Un vettore di <code>Texture</code> contenente le texture lette.</returns>
	[[nodiscard]] std::vector<TextureSP> loadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName);
};