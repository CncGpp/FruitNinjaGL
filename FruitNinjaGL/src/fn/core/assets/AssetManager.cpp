#include "AssetManager.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

std::unordered_map<std::string, TextureSP> AssetManager::s_textureCache;
std::unordered_map<std::string, ModelSP> AssetManager::s_modelCache;
std::unordered_map<std::string, ShaderSP> AssetManager::s_shaderCache;
std::unordered_map<std::string, AudioSP> AssetManager::s_audioCache;