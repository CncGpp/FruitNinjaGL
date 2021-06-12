#pragma once
#include<fn/fn.hpp>
#include "glm/gtx/string_cast.hpp"

namespace fn {
	template<typename ...Args>
	constexpr void print(fmt::text_style style, const std::string& S, Args ...args) {
		fmt::print(style, S, args...);
	}
	template<typename ...Args>
	constexpr void print(const std::string& S, Args ...args) {
		fmt::print(S, args...);
	}

	template<typename T> 
	std::string format_component(T* cmp, std::vector<std::tuple<std::string, std::string, std::string>> attrb) {
		auto ctypename = cmp->ctypename;
		auto cid = cmp->cid.to_string('.');
		auto s = fmt::format("[{} - {}]", ctypename, cid);
		if (attrb.size() == 0){
			return s;
		}
		s += "{\n";
		for (auto& [attr_name, attr_value, attr_type] : attrb) {
			s += fmt::format("    {}: {}  -  {}\n", attr_name, attr_value, attr_type);
		}
		s += "}\n";
		return s;
	} 
}