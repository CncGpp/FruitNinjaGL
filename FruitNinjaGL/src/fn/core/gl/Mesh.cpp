#include "Mesh.hpp"
#include <fn/core/assets/AssetManager.hpp>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<TextureSP>& textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    this->initMesh();
}

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, TextureSP texture){
    this->vertices = vertices;
    this->indices = indices;
    this->textures = { texture };

    this->initMesh();
}

Mesh::~Mesh()
{
    GL_CHECK( glDeleteBuffers(1, &this->VBO) );
    GL_CHECK( glDeleteBuffers(1, &this->EBO) );
    GL_CHECK( glDeleteVertexArrays(1, &this->VAO) );
}

void Mesh::draw(ShaderSP& shader) const
{
    // Bind delle texture
    GLuint diffuseNr = 1;
    GLuint specularNr = 1;

    for (GLuint i = 0; i < this->textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // Attivo la texture unit prima del binding
        // Determino il numero progressivo della texture
        std::string number;
        const std::string type = this->textures[i]->type();

        if (type == "texture_diffuse"){
            number = std::to_string(diffuseNr++);
        }
        else if (type == "texture_specular"){
            number = std::to_string(specularNr++);
        }

        // Setto i sampler nel fragment shader
        glUniform1i(glGetUniformLocation(shader->program(), (type + number).c_str()), i);
        //Effettuo in bind della texture.
        glBindTexture(GL_TEXTURE_2D, this->textures[i]->id());
    }

    // Disegno la Mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Disattivo tutti i bindings
    for (GLuint i = 0; i < this->textures.size(); i++){
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

MeshSP Mesh::rectangle(const glm::vec3& topLeft, const glm::vec3& bottomRight, const TextureSP& texture)
{
    std::vector<Vertex> vertices = {
        Vertex{
            .position = topLeft,
            .normal = {0.0f, 0.0f, -1.0f},
            .texCoords = {0.0f, 1.0f}
        },
        Vertex{
            .position = glm::vec3(topLeft.x, bottomRight.y, topLeft.z) ,
            .normal = {0.0f, 0.0f, -1.0f},
            .texCoords = {0.0f, 0.0f}
        },
        Vertex{
            .position = bottomRight ,
            .normal = {0.0f, 0.0f, -1.0f},
            .texCoords = {1.0f, 0.0f}
        },
        Vertex{
            .position = glm::vec3(bottomRight.x, topLeft.y, bottomRight.z) ,
            .normal = {0.0f, 0.0f, -1.0f},
            .texCoords = {1.0f, 1.0f}
        },
    };
    std::vector<GLuint> indices = { 0, 1, 2, 2, 3, 0 };

    return std::make_shared<Mesh>(vertices, indices, texture);
}

MeshSP Mesh::rectangle(const glm::vec2& topLeft, const glm::vec2& bottomRight, const TextureSP& texture)
{
    return Mesh::rectangle({topLeft.x, topLeft.y, 0.0f}, { bottomRight.x, bottomRight.y, 0.0f }, texture);
}

MeshSP Mesh::rectangle(const TextureSP& texture)
{
    return Mesh::rectangle({ -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f }, texture);
}


void Mesh::initMesh(){
    // Genero i buffer e gli array in opengl
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);

    // Carico i dati dei vertici all'interno del vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);
    // Carico i dati degli indici 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Definisco il layout dei vertici, ovvero come opengl deve interpretare i dati nel vertex buffer
    // Il layout utilizzato è semplicemente quello della struct Vertex
    /*
        +----------------------------+--------------------------+------------------+
        |       Position (12B)       |       Normal (12B)       |  texCoords (8B)  |
        +----------------------------+--------------------------+------------------+
        ^ (0)                        ^ (12)                     ^ (24)
        offset position              offsetof(Vertex, normal)   offsetof(Vertex, texCoords)
    */
    // Posizioni del vertice
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Normali del vertice
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
    // Coordinate Texture del vertice
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

