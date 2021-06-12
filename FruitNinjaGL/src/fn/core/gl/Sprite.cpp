#include "Sprite.hpp"
#include <fn\core\assets\AssetManager.hpp>

Sprite::Sprite(const fs::path& spritepath, const glm::vec3& position, const glm::vec2& size, const float scaleXY) : m_position(position), m_shape(size), m_scaleXY(scaleXY)
{
	this->m_shader = AssetManager::loadShader(S_DEFAULT_VERTEX_SHADER, S_DEFAULT_FRAGMENT_SHADER);
	onResize();
	setSource(spritepath);
}
Sprite::Sprite(const fs::path& spritepath, const glm::vec3& position, const float scaleXY) : Sprite(spritepath, position, { -1.0f, -1.0f }, scaleXY){}
Sprite::Sprite(const fs::path& spritepath, const glm::vec2& shape, const float scaleXY) : Sprite(spritepath, {0.0f, 0.0f , 0.0f}, shape, scaleXY) {}
Sprite::Sprite(const fs::path& spritepath, const float scaleXY) : Sprite(spritepath, { 0.0f, 0.0f , 0.0f }, { -1.0f, -1.0f }, scaleXY) {}


Sprite& Sprite::setPosition(const glm::vec3& position)
{
	this->m_position = position;
	this->m_model = glm::translate(glm::mat4(1.0f), this->m_position);
	return setSource(this->m_texture->filepath());
}

Sprite& Sprite::setShape(const glm::vec2& size)
{
	this->m_shape = size;
	return setSource(this->m_texture->filepath());
}

Sprite& Sprite::setSource(const fs::path& spritepath)
{
	this->m_texture = AssetManager::loadTexture(spritepath);

	this->m_mesh = Mesh::rectangle({ 0.0f, 0.0f, 0.0f }, glm::vec3(compute_shape(), 0.0f) , this->m_texture);
	this->m_model = glm::translate(glm::mat4(1.0f), this->m_position);
	return *this;
}

glm::vec2 Sprite::compute_shape() const
{
	auto tex = this->m_texture->shape();					
	const float ratio = tex.x / (float)tex.y;
	glm::vec2 tmp;
	if (m_shape.x == -1.0f && m_shape.y > 0.0f) {			
		tmp = { ratio * m_shape.y, m_shape.y };
	}
	else if (m_shape.x > 0.0f && m_shape.y == -1.0f) {
		tmp = { m_shape.x , m_shape.x / ratio };
	}
	else if (m_shape.x > 0.0f && m_shape.y > 0.0f) {
		tmp = m_shape;
	}
	else
		tmp = m_scaleXY * tex;

	return tmp;
}

void Sprite::draw(const fn::Camera& camera, ShaderSP& shader)
{
	shader->active();
	shader->set("projection", this->m_projection);
	shader->set("view", this->m_view);
	shader->set("model", this->m_model);

	shader->set("alpha", this->alpha);
	this->m_mesh->draw(shader);
	shader->deactive();
}

void Sprite::draw(const fn::Camera& camera)
{
	this->draw(camera, m_shader);
}

void Sprite::onResize()
{
	const auto vsize = gl::displayport_size();
	this->m_projection = glm::ortho(0.0f, vsize.x, vsize.y, 0.0f, -1.0f, 200.1f);
}
