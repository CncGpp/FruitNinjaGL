#include "Text.hpp"

Text::Text(const std::string& text, glm::vec3 position, const float scaleXY)
{
	m_position = position;
	m_scaleXY = scaleXY;
	setText(text);
}

Text::Text(const std::string& text, const float scaleXY) : Text(text, glm::vec3{}, scaleXY) {}

void Text::draw(const fn::Camera& camera)
{
	float tot_w = 0.0f;	//Width totale dal primo carattere a quello che attualmente si sta disegnando
	for (size_t i = 0; i < m_text.size(); i++) {
		m_characters[i]->setPosition({ m_position.x + tot_w, m_position.y, 0.0f });
		m_characters[i]->draw(camera);

		tot_w += m_characters[i]->width();
	}
}

void Text::onResize()
{
	for (auto& x : m_characters) {
		x->onResize();
	}
}

Text& Text::setText(const std::string& text){
	if(text == m_text)
		return *this;
	

	this->m_text = text;
	m_characters.clear();
	m_characters.reserve(m_text.size());
	for (int i = 0; i < m_text.size(); i++) {
		char character = m_text[i];
		glm::vec2 shape = {-1.0f, 5*gl::vh * m_scaleXY };
		m_characters.push_back(std::make_shared<Sprite>(assets::res::ui::alphanum[character], m_position, shape, m_scaleXY));
	}

	return *this;
}

glm::vec3 Text::position()
{
	return m_position;
}

glm::vec2 Text::shape()
{
	return glm::vec2();
}
