#include "components.hpp"

glm::mat4 C::Position::model_matrix() const
{
	// Matrice di rotazione
	auto R = glm::mat4(1.0f);
	R = glm::rotate(R, this->rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	R = glm::rotate(R, this->rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	R = glm::rotate(R, this->rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

	// Matrice di traslazione nella posizione finale
	auto T = glm::translate(glm::mat4(1.0f), this->position);

	// Calcolo la matrice finale
	return T * R;
}

std::ostream& operator << (std::ostream& o, const C::Position& obj) {
	auto* ptr = &obj;
	o << fn::format_component(ptr, {
			{".position", fmt::format("[{}, {}, {}]", ptr->position.x, ptr->position.y, ptr->position.z), typeid(ptr->position).name()},
			{".rotation", fmt::format("[{}, {}, {}]", ptr->rotation.x, ptr->rotation.y, ptr->rotation.z), typeid(ptr->rotation).name()}
		});
	return o;
}

std::ostream& operator<< (std::ostream& o, const C::Movement& obj) {
	auto* ptr = &obj;
	o << fn::format_component(ptr, {
			{".velocity", fmt::format("[{}, {}, {}]", ptr->velocity.x, ptr->velocity.y, ptr->velocity.z), typeid(ptr->velocity).name()},
			{".spin", fmt::format("[{}, {}, {}]", ptr->spin.x, ptr->spin.y, ptr->spin.z), typeid(ptr->spin).name()}
		});
	return o;
}


