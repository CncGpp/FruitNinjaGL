#include "Wave.hpp"
#include <utl\random.hpp>
#include <fn\ecs\components\components.hpp>

namespace S {
	Wave::Wave(fn::Database& database) : m_database(database)
	{
		this->next_wave();
		m_sheduled_waves.front().delay = core::seconds(2.0);
	}

	void Wave::update(core::seconds delta_t)
	{
		// ::::::::: Verifico se devo lanciare un nuovo frutto ::::::
		if (m_sheduled_waves.empty()) {
			this->next_wave();
		}
		else {
			auto& next = m_sheduled_waves.front();
			next.delay -= delta_t;
			if (next.delay.count() <= 0) {
				[[maybe_unused]] auto f = this->mk_fruit(next);
				m_sheduled_waves.pop();
			}
		}
	}


	template<>
	inline std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::RANDOM>(const unsigned int count)
	{
		std::vector<Wave::Spawn> w{};
		w.reserve(count);

		const float hX = 16.0f;
		const float hY = 9.0f;
		const float hZ = 12.0f;

		for (unsigned int i = 0; i < count; i++) {
			// Step 1: Determino le coordinate di partenza dell'entità
			glm::vec3 position = {
				fn::rand::uniform(-hX, hX),			// x Casuale fra i due estremi dello schermo
				-hY,								// y sotto lo schermo
				fn::rand::uniform(-hZ, 0.0f)		// z da dietro fino al piano xy
			};
			// Abbasso la y tenendo conto della visione prospettica (diminuendo z devo diminuire y per essere visto)
			position.y += glm::sin(glm::pi<float>() / 6.0f) * position.z;		//Come angolo uso pi/6 basta che sia minore del fovy della campera.

			// Step 2: Determino il vettore velocità di partenza dell'entità
			// TODO:::        definire in base al versore (posizione-punto_random)
			glm::vec3 velocity = { -position.x, 18.0f, -position.z * 0.5 };
			glm::vec3 spin = fn::rand::uniform<3>(-2.0f, 2.0f);

			// Step 3: Lo creo e lo aggiungo a w
			w.push_back({
				{.position = position},
				{.velocity = velocity, .spin = spin },
				{.fruit = Fruits::random(), .model_kind=Fruits::Model::whole},
				core::seconds(fn::rand::uniform(0.0f, 1.0f))
			});
		}

		return w;
	}


	template<>
	inline std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::LEFT_TO_RIGHT>(const unsigned int count)
	{
		std::vector<Wave::Spawn> w{};
		w.reserve(count);

		constexpr float hX = 16.0f;
		constexpr float hY = 9.0f;
		constexpr float hZ = 12.0f;

		float step = 2.0f * hX / (count + 2);
		auto fruit = Fruits::random();

		for (unsigned int i = 0; i < count; i++) {
			// Step 1: Determino le coordinate di partenza dell'entità
			glm::vec3 position = {
				-hX + (i + 1) * step,				    // da sinistra
				-hY,								    // y sotto lo schermo
				fn::rand::uniform(-hZ/2.0f, 0.0f)		// z da dietro fino al piano xy
			};
			// Abbasso la y tenendo conto della visione prospettica (diminuendo z devo diminuire y per essere visto)
			position.y += glm::sin(glm::pi<float>() / 6.0f) * position.z;		//Come angolo uso pi/6 basta che sia minore del fovy della campera.

			// Step 2: Determino il vettore velocità di partenza dell'entità
			glm::vec3 velocity = { fn::rand::uniform(), 18.0f, fn::rand::uniform() };
			glm::vec3 spin = fn::rand::uniform<3>(-2.0f, 2.0f);

			// Step 3: Lo creo e lo aggiungo a w
			w.push_back({
				{.position = position},
				{.velocity = velocity, .spin = spin },
				{.fruit = fruit, .model_kind = Fruits::Model::whole},
				core::seconds(fn::rand::uniform(0.0f, 0.75f))
				});
		}

		return w;
	}

	template<>
	inline std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::RIGHT_TO_LEFT>(const unsigned int count)
	{
		std::vector<Wave::Spawn> w{};
		w.reserve(count);

		constexpr float hX = 16.0f;
		constexpr float hY = 9.0f;
		constexpr float hZ = 12.0f;

		float step = 2.0f * hX / (count + 2);
		auto fruit = Fruits::random();

		for (unsigned int i = 0; i < count; i++) {
			// Step 1: Determino le coordinate di partenza dell'entità
			glm::vec3 position = {
				hX - (i + 1) * step,				    // da destra
				-hY,								    // y sotto lo schermo
				fn::rand::uniform(-hZ / 2.0f, 0.0f)		// z da dietro fino al piano xy
			};
			// Abbasso la y tenendo conto della visione prospettica (diminuendo z devo diminuire y per essere visto)
			position.y += glm::sin(glm::pi<float>() / 6.0f) * position.z;		//Come angolo uso pi/6 basta che sia minore del fovy della campera.
			glm::vec3 rotation = fn::rand::uniform<3>(-4.0f, 4.0f);

			// Step 2: Determino il vettore velocità di partenza dell'entità
			glm::vec3 velocity = { fn::rand::uniform(), 18.0f, fn::rand::uniform() };
			glm::vec3 spin = fn::rand::uniform<3>(-2.0f, 2.0f);

			// Step 3: Lo creo e lo aggiungo a w
			w.push_back({
				{.position = position, .rotation = rotation},
				{.velocity = velocity, .spin = spin },
				{.fruit = fruit, .model_kind = Fruits::Model::whole},
				core::seconds(fn::rand::uniform(0.0f, 0.75f))
				});
		}

		return w;
	}


	template<>
	inline std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::SPOT>(const unsigned int count)
	{
		std::vector<Wave::Spawn> w{};
		w.reserve(count);

		constexpr float hX = 16.0f;
		constexpr float hY = 9.0f;
		constexpr float hZ = 12.0f;

		auto fruit = Fruits::random();

		for (unsigned int i = 0; i < count; i++) {
			// Step 1: Determino le coordinate di partenza dell'entità
			glm::vec3 position = {
				fn::rand::uniform(-0.8f * hX, 0.8f * hX),  // x causale
				-hY,								       // y sotto lo schermo
				fn::rand::uniform(-hZ / 2.0f, 0.0f)		   // z da dietro fino al piano xy
			};
			// Abbasso la y tenendo conto della visione prospettica (diminuendo z devo diminuire y per essere visto)
			position.y += glm::sin(glm::pi<float>() / 6.0f) * position.z;		//Come angolo uso pi/6 basta che sia minore del fovy della campera.
			glm::vec3 rotation = fn::rand::uniform<3>(-4.0f, 4.0f);

			// Step 2: Determino il vettore velocità di partenza dell'entità
			glm::vec3 velocity = { fn::rand::uniform(), 18.0f, fn::rand::uniform(-1.0f, 7.0f) };
			glm::vec3 spin = fn::rand::uniform<3>(-4.0f, 4.0f);

			// Step 3: Lo creo e lo aggiungo a w
			w.push_back({
				{.position = position, .rotation = rotation},
				{.velocity = velocity, .spin = spin },
				{.fruit = fruit, .model_kind = Fruits::Model::whole},
				core::seconds(fn::rand::uniform(0.5f, 1.25f))
				});
		}

		return w;
	}



	E::Fruit Wave::mk_fruit(Spawn& s)
	{
		E::Fruit f = E::Fruit::make(m_database, s.fruit.fruit);
		f.set<C::Position>(s.position);
		f.set<C::Movement>(s.movement);
		return f;
	}
	void Wave::next_wave()
	{
		// Genero l'ondata
		std::vector<Spawn> wave;
		auto wawe_type = std::rand() % 5;
		switch (wawe_type)
		{
		case 0: {
			const unsigned int count = (unsigned int)glm::round(fn::rand::uniform(2.0f, 10.0f));
			wave = mk_wave<WaveType::RANDOM>(count);
			break;
		}
		case 1: {
			const unsigned int count = (unsigned int)glm::round(fn::rand::uniform(3.0f, 6.0f));
			wave = mk_wave<WaveType::LEFT_TO_RIGHT>(count);
			break;
		}
		case 2: {
			const unsigned int count = (unsigned int)glm::round(fn::rand::uniform(3.0f, 6.0f));
			wave = mk_wave<WaveType::RIGHT_TO_LEFT>(count);
			break;
		}
		case 3: {
			const unsigned int count = (unsigned int)glm::round(fn::rand::uniform(3.0f, 7.0f));
			wave = mk_wave<WaveType::SPOT>(count);
			break;
		}
		default:
			wave = mk_wave<WaveType::RANDOM>((unsigned int)glm::round(fn::rand::uniform(5.0f, 15.0f)));
			break;
		}
		

		// Aumento il delay di testa per staccare dalle successive
		wave[0].delay = core::seconds(fn::rand::uniform(1.0f, 3.0f));

		// Le aggiungo alla coda
		for (auto& w : wave) {
			this->m_sheduled_waves.push(std::move(w));
		}
	}


	template std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::RANDOM>(const unsigned int);
	template std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::LEFT_TO_RIGHT>(const unsigned int);
	template std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::RIGHT_TO_LEFT>(const unsigned int);
	template std::vector<Wave::Spawn> Wave::mk_wave<Wave::WaveType::SPOT>(const unsigned int);
}