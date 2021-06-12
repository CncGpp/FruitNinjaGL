#include "Blade.hpp"
#include <fn\core\assets\AssetManager.hpp>

Blade::Blade() {
	this->m_shader = AssetManager::loadShader(S_DEFAULT_VERTEX_SHADER, S_DEFAULT_FRAGMENT_SHADER);
	Texture::Parameteri m_parameteri = {
		{ GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT },
		{ GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT },
		{ GL_TEXTURE_MIN_FILTER, GL_NEAREST },
		{ GL_TEXTURE_MAG_FILTER, GL_NEAREST },
	};
	this->blade_texture = AssetManager::loadTexture(assets::res::blades[2], m_parameteri);
}

inline float Blade::thickness(const std::vector<glm::vec2>& pos, const size_t i) {
	return 11.5f * (1.0f - i / (float) pos.size());
}

void Blade::update(core::seconds delta_t) {}

void Blade::draw(const fn::Camera& camera) {
	if (Mouse::positions().size() < 5)
		return;

	const auto vs = gl::viewport_size();
	this->m_projection = glm::ortho(0.0f, vs.x, vs.y, 0.0f, -1.0f, 1.0f);

	auto positions = std::vector(Mouse::positions().begin(), Mouse::positions().end());
	positions = polylineSmooth(positions);
	positions = polylineSmooth(positions);

	// Preparo le strutture dati necessarie alla costruzione della mesh
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<sp<Texture>> textures = { this->blade_texture };

	// Fase 1. Creo la punta
	GLuint ix = 0;
	{
		const auto pp = positions[0];	// previous point
		const auto cp = positions[1];	// current point
		const auto disp = cp - pp;
		const auto perp = thickness(positions, 1) * glm::normalize(glm::vec2(-disp.y, disp.x));
		const auto p1 = cp + perp;
		const auto p2 = cp - perp;
		const auto tip = 2.0f * thickness(positions, 1) * glm::normalize(-disp);
		vertices.push_back(make_vertex(pp + tip, positions, 0, 0)); //0
		vertices.push_back(make_vertex(p1, positions, 1, -1.0)); //1
		vertices.push_back(make_vertex(cp, positions, 1,  0.0)); //2
		vertices.push_back(make_vertex(p2, positions, 1, +1.0)); //3
		indices.insert(indices.end(), { 3, 0, 2 });
		indices.insert(indices.end(), { 2, 0, 1 });
		ix += 4;
	}

	// Fase 2. Creo le due stride a partire dalle posizioni
	for (size_t i = 2; i < positions.size() - 1; i++) {
		const auto pp = positions[i - 1];	// previous point
		const auto cp = positions[i];		// current point
		const auto np = positions[i + 1];	// next point

		// Determino il vettore perpendicolare -- è il perp all vettore scostamento medio fra (pp, cp) e (cp, np)
		const auto mean = ((cp - pp) + (np - cp)) / 2.0f;
		const auto perp = thickness(positions, i) * glm::normalize(glm::vec2(-mean.y, mean.x));

		// Determino i nuovi punti applicando `perp` e `-perp` su cp
		const auto p1 = cp + perp;
		const auto p2 = cp - perp;

		vertices.push_back(make_vertex(p1, positions, i, -1.0));
		vertices.push_back(make_vertex(cp, positions, i, 0.0));
		vertices.push_back(make_vertex(p2, positions, i, +1.0));

		// p1_old   =  ix - 3
		// cp_old   =  ix - 2
		// p2_old   =  ix - 1
		// p1_new   =  ix + 0
		// cp_new   =  ix + 1
		// p2_new   =  ix + 2

		// Stride di sotto
		indices.insert(indices.end(), { ix + 1, ix - 2, ix - 3 });
		indices.insert(indices.end(), { ix + 1, ix - 3, ix + 0 });

		// Stride di sopra
		indices.insert(indices.end(), { ix + 2, ix - 1, ix - 2 });
		indices.insert(indices.end(), { ix + 2, ix - 2, ix + 1 });

		ix += 3;
	}

	const Mesh mesh = Mesh(vertices, indices, textures);

	m_shader->active();
	m_shader->set("projection", m_projection);
	m_shader->set("view", m_view);
	m_shader->set("model", m_model);
	mesh.draw(m_shader);
	m_shader->deactive();
}



inline Vertex Blade::make_vertex(const glm::vec2& p, const std::vector<glm::vec2>& positions, const size_t idx, const float sign) {
	float tex_y = idx % 2;
	float tex_x = sign/2.5f + 0.5;

	return {
		.position = glm::vec3(p.x, p.y, 1.0f),
		.normal = glm::vec3(0.0f),
		.texCoords = glm::vec2(tex_x, tex_y),
	};
}

// http://www.cs.unc.edu/~dm/UNC/COMP258/LECTURES/Chaikins-Algorithm.pdf
/*
It should be noted that Chaikin’s curve has been shown to be equivalent to a quadratic B-spline curve
[2](a piecewise quadratic Bezier curve).However, Chaikin’s method avoids the analytical definition of ´
B - splines and provides a simple, elegant curve drawing mechanism.
*/
std::vector<glm::vec2> Blade::polylineSmooth(const std::vector<glm::vec2>& input)
{
	constexpr auto a = 3.0f / 4.0f;
	static_assert(a > 0.5f && a < 1.0f);

	std::vector<glm::vec2> output;
	output.reserve(input.size() * 2);
	output.emplace_back(input[0]);

	for (size_t i = 0; i < input.size() - 1; i++) {
		const auto cp = input[i];
		const auto np = input[i + 1];

		const auto Q = a * cp + (1.0f - a) * np;
		const auto R = (1.0f - a) * cp + a * np;
		output.emplace_back(Q);
		output.emplace_back(R);
	}

	output.emplace_back(input[input.size()-1]);
	return output;
}