#pragma once

#include <string>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <fn/core/gl/Shader.hpp>

#include <fn/core/gl/Texture.hpp>

/**
 * @file
 * @brief Class @ref Audio
 */

class Mesh;
using MeshSP = std::shared_ptr<Mesh>;           //!< Alias

/**
 * @brief Rappresentazione di un generico vertice della Mesh.
 * 
 * In OpenGL un vertice è molto più di un punto nello spazio, esso può infatti 
 * contenere informazioni quali, nomali, coordinate texture, colori, tangenti,
 * bitangenti...
 */
struct Vertex{
    glm::vec3 position;             //!< Vettore posizione del vertice in coord locali.
    glm::vec3 normal;               //!< Vettore normale al vertice
    glm::vec2 texCoords;            //!< Coordinate del vertice sulla texture
};

/**
 * @brief Astrazione di una Mesh.
 * 
 * La classe rappresenta un reticolo poligonale che definisce un oggetto nello spazio, la
 * rappresentazione avviene mediante vertici, indici e texture.
 * 
 * Nei vertici sono contenuti i dati dell'oggetto mentre negli indici contengono l'adjacency information 
 * cioè come i vertici sono connessi per formare gli spigoli e le facce della mesh.
 */
class Mesh
{
public:
    /*  Mesh Data - Insieme di dati che descrivo una Mesh  */
    std::vector<Vertex> vertices;                                               //!< Buffer dei vertici che descrivono una texture
    std::vector<GLuint> indices;                                                //!< Buffer degli indici che descrivono le terne di vertici da disegnare
    std::vector<TextureSP> textures;                                            //!< Insieme delle texture usate in questa mesh

    /**
     * @brief Costruttore.
     * 
     * @param vertices vettore di Vertex che definiscono i vertici della mesh
     * @param indices vettore di indici rappresentanti l'adjacency information
     * @param textures vettore di Texture da utilizzare per la mesh
     */
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<TextureSP>& textures);

    /**
     * @brief Costruttore.
     *
     * @param vertices vettore di Vertex che definiscono i vertici della mesh
     * @param indices vettore di indici rappresentanti l'adjacency information
     * @param texture Texture da utilizzare per la mesh
     */
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, TextureSP texture);

    /** @brief Distruttore */
    ~Mesh();

    /** @brief La copia di un oggetto Mesh non è permessa. */
    Mesh(Mesh const&) = delete;

    /** @brief L'assegnazione di un oggetto Mesh non è permessa. */
    Mesh& operator=(Mesh const&) = delete;

    /**
     * @brief Disegna la mesh sul framebuffer.
     * 
     * @param shader Oggetto Shader da utilizzare per il disegno.
     */
    void draw(ShaderSP& shader) const;

    /**
     * @brief Genera una mesh rettangolare.
     * 
     * Si tratta essenzialmente di una funzione di utility che consente la rapida creazione di un rettangolo con una immagine
     * al suo interno. Utile per la visualizzazione di Sprite e figure.
     * I quattro spigoli del rettangolo sono tali da corrispondere ai 4 estremi della texture.
     * 
     * @param topLeft vertice top-left del rettangolo nello spazio 3d.
     * @param bottomRight vertice bottom-right del rettangolo nello spazio 3d.
     * @param texture Texture da utilizzare sulla mesh.
     * @return Una mesh rettangolare a cui è stata applicata una texture.
     */
    [[nodiscard]] static MeshSP rectangle(const glm::vec3& topLeft, const glm::vec3& bottomRight, const TextureSP& texture);

    /**
     * @brief Genera una mesh rettangolare.
     * 
     * Specializzazione al caso bidimensionale della funzione precedente. La coordinata z è fissata a 0.
     * Il rettangolo è quindi creato sul piano XY.
     * 
     * @param topLeft vertice top-left del rettangolo nello spazio 2d.
     * @param bottomRight vertice bottom-right del rettangolo nello spazio 2d.
     * @param texture Texture da utilizzare sulla mesh.
     * @return Una mesh rettangolare a cui è stata applicata una texture.
     */
    [[nodiscard]] static MeshSP rectangle(const glm::vec2& topLeft, const glm::vec2& bottomRight, const TextureSP& texture);

    /**
     * @brief Genera una mesh rettangolare.
     * 
     * Specializzazione della funzione precedente. Il rettangolo costruito avrà:
     * - top-left = { -1.0f, -1.0f, 0.0f }
     * - bottom-right ={ 1.0f, 1.0f, 0.0f }
     * La funzione è utile per avere un rettangolo `unitario` a cui applicare successivamente trasformazioni.
     * 
     * @param texture Texture da utilizzare sulla mesh.
     * @return Una mesh rettangolare a cui è stata applicata una texture.
     */
    [[nodiscard]] static MeshSP rectangle(const TextureSP& texture);

private:
    /*  Render data - descrittori di bind di OpenGL */
    GLuint VAO = 0, VBO = 0, EBO = 0;

    // Inizializza tutti i buffer, oggetti e array e definisce il Layout di Vertex
    void initMesh();
};

