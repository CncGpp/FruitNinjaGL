#include "Database.hpp"
#include "ComponentArray.hpp"

namespace fn {
	Database::Database()
	{
		for (unsigned int i = 0; i < fn::ecs::MAX_ENTITY_COUNT; i++)
			eid_pool.push(fn::ecs::MAX_ENTITY_COUNT - i - 1);
	}

	Entity Database::create_entity()
	{
		auto eid = this->eid_pool.top();
		this->eid_pool.pop(); //TODO: controllare se � vuoto
		return Entity(eid, this);
	}

	void Database::destroy_entity(Eid eid)
	{
		this->eid_pool.push(eid);
		for (auto& [key, value] : this->m_components) {
			value->remove(eid);
		}
	}


	Signature Database::signature(const Eid eid)
	{
		Signature s{};
		for (auto& [key, value] : this->m_components) {
			if (value->validity()[eid])
				s = s | key;
		}
		return s;
	}

	std::vector<Entity> Database::having(const Signature signature)
	{
		auto bitmap = std::move(std::bitset<ecs::MAX_ENTITY_COUNT>{}.set());

		for (auto& [cid, component_array] : this->m_components) {
			if ((signature & cid).any())										//Se nella firma ci sta il cid allora posso procedere
				bitmap = bitmap & component_array->validity();
		}

		std::vector<Entity> result;
		result.reserve(bitmap.count());
		for (auto i = 0; i < bitmap.size(); i++) {
			if (bitmap[i]) [[unlikely]] {
				result.emplace_back(i, this, signature);
			}
		}
		return result;
	}


	void Database::for_each(const Signature signature, std::function<void(Eid e)> fun)
	{
		auto bitmap = std::move(std::bitset<ecs::MAX_ENTITY_COUNT>{}.set());

		for (auto& [cid, component_array] : this->m_components) {
			if ((signature & cid).any())										//Se nella firma ci sta il cid allora posso procedere
				bitmap = bitmap & component_array->validity();
		}

		for (auto i = 0; i < bitmap.size(); i++) {
			if (bitmap[i]) [[unlikely]] {
				fun(i);
			}
		}
	}
	void Database::for_each(const Signature signature, std::function<void(Entity& e)> fun)
	{
		auto bitmap = std::move(std::bitset<ecs::MAX_ENTITY_COUNT>{}.set());

		for (auto& [cid, component_array] : this->m_components) {
			if ((signature & cid).any())										//Se nella firma ci sta il cid allora posso procedere
				bitmap = bitmap & component_array->validity();
		}

		for (unsigned int i = 0; i < bitmap.size(); i++) {
			if (bitmap[i]) [[unlikely]] {
				Entity e{i, this, signature};
				fun(e);
			}
		}
	}

}
