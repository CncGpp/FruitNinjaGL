#include "Model.hpp"
#include <fn/core/assets/AssetManager.hpp>

TextureSP TextureFromFile(const char* path, std::string directory, Texture::Type type)
{
    //Generate texture ID and load texture data
    std::string filename = std::string(path);
    //Pezzotto TODO: AGGIUSTARE
    std::string to_repl("Food - Fruits - Textures");
    filename.replace(filename.find(to_repl), to_repl.length(), "Textures");

    filename = directory + '/' + filename;
    return AssetManager::loadTexture(filename, type);
}


void Model::draw(ShaderSP& shader){
    for (GLuint i = 0; i < this->m_meshes.size(); i++){
        this->m_meshes[i]->draw(shader);
    }
}


void Model::loadModel(const fs::path& modelpath)
{
    // Leggo il file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelpath.string(), aiProcess_Triangulate | aiProcess_FlipUVs);

    // Controllo se ci sono stati eventuali errori
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        fn::log::error(
            "[ERRORE::ASSIMP] Errore durante la lettura del modello {} <image=nullptr>\n", importer.GetErrorString()
        );
        return;
    }
    // Determino la directory che contiene il modelpath
    this->m_dir = modelpath.parent_path();

    // Partendo dal root node processo l'albero della scena in maniera ricorsiva.
    this->processNode(scene, scene->mRootNode);
    calculateAABB();
}


void Model::processNode(const aiScene* scene, const aiNode* node)
{
    // Itero su tutte le mesh contenute nel nodo corrente.
    for (GLuint i = 0; i < node->mNumMeshes; i++){
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];            // Gli aiNode contengono in realtà soltanto gli indici alle mesh. (servono per organizzare)
                                                                    // È l'aiScene che contiene tutti i dati. Le relazioni tra i dati sono definite dai nodi.
        this->m_meshes.emplace_back(this->processMesh(scene, mesh));
    }

    // Dopo aver processato il nodo si prosegue ricorsivamente con una DFS processando eventuali nodi figli.
    for (GLuint i = 0; i < node->mNumChildren; i++){
        this->processNode(scene, node->mChildren[i]);
    }
}

MeshSP Model::processMesh(const aiScene* scene, const aiMesh* mesh)
{
    // Dati della mesh
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<TextureSP> textures;
    
    /*:::::::  FASE 1 - Lettura dei vertici della mesh                                             :::::::*/
    vertices.reserve(mesh->mNumVertices);
    for (GLuint i = 0; i < mesh->mNumVertices; i++){                                                        // Itero su ogni vertice della mesh
        Vertex vertex;

        vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };             // Estraggo le posizioni
        vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };                  // Estraggo le normali

        if (mesh->mTextureCoords[0]) {                                                                      // Estraggo le coordinate delle texture
            vertex.texCoords = { mesh->mTextureCoords[0][i].x , mesh->mTextureCoords[0][i].y};              // /!\ Un vertice in ASSIMP può avere fino a 8 differenti coordinate 
                                                                                                            //     texture per vertice. Nel codice però prendiamo solo la prima        
            #ifdef _DEBUG                                                                                   //     assumendo che il modello caricato ne abbia solo 1.
            if (mesh->mTextureCoords[1]) {
                fn::log::warn("La texture ha più di una coordinata texture per vertice!");
            }
            #endif // DEBUG
        }
        else
            vertex.texCoords = {0.0f, 0.0f};

        vertices.push_back(vertex);
    }

    /*:::::::  FASE 2 - Lettura degli indici delle facce della mesh.                      :::::::*/
    for (GLuint i = 0; i < mesh->mNumFaces; i++){                                                  // Itero su ogni faccia della mesh per recuperare gli indici dei vertici
        aiFace face = mesh->mFaces[i];                                                             // Una faccia non è altro che un triangolo (terna di vertici)
        for (GLuint j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    /*:::::::  FASE 3 - Lettura dei materiali.                                            :::::::*/
    if (mesh->mMaterialIndex >= 0){                                                               // Se il modello caricato contiene dei materiali...
        const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];                     // /!\ Per convenzione ogni variabile sampler degli shader è chiamata concatenando
                                                                                                  //     il nome della texture-materiale con un progressivo 1-based: <name><prg>.
                                                                                                  //     es: DIFFUSE: texture_diffuse1, texture_diffuse2, ...
                                                                                                  //         SPECULAR: texture_specular1, texture_specular2, texture_specular3, ...
        // a. Diffuse
        std::vector<TextureSP> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // b. Specular
        std::vector<TextureSP> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    /*:::::::  FASE 4 - Costruzione e ritorno della mesh con i dati estratti.             :::::::*/
    return std::make_shared<Mesh>(vertices, indices, textures);
}

void Model::calculateAABB()
{
    glm::vec3 vmax = glm::vec3{ std::numeric_limits<float>::min() };
    glm::vec3 vmin = glm::vec3{ std::numeric_limits<float>::max() };

    // Semplicemente determino i valori x, y, z minimi e massimi.
    for (auto& m : this->m_meshes) {
        for (auto& v : m->vertices) {
            vmax = glm::max(vmax, v.position);          //glm::max e min sono component wise!
            vmin = glm::min(vmin, v.position);
        }
    }

    this->m_AABB = { vmin, vmax };
}



std::vector<TextureSP> Model::loadMaterialTextures(const aiMaterial* mat, const aiTextureType type, const std::string& typeName)
{
    std::vector<TextureSP> textures;
    textures.reserve(mat->GetTextureCount(type));

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString matTexPath;
        mat->GetTexture(type, i, &matTexPath);
        Texture::Type TexType;
        switch (type)
        {
        case aiTextureType_DIFFUSE:
            TexType = Texture::Type::diffuse;
            break;
        case aiTextureType_SPECULAR:
            TexType = Texture::Type::specular;
            break;
        default:
            TexType = Texture::Type::unknow;
            break;
        }
        //auto t = AssetManager::loadTexture(this->m_dir / matTexPath.C_Str(), TexType);
        auto t = TextureFromFile(matTexPath.C_Str(), this->m_dir.string(), TexType);
        textures.push_back(t);
    }

    return textures;
}
