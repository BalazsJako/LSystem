#pragma once

#include <string>
#include <map>
#include <set>
#include <vector>

namespace Serialize
{

	void Serialize(std::ostream& a, const uint32_t s);
	void Serialize(std::ostream& a, const uint16_t s);
	void Serialize(std::ostream& a, const int32_t s);
	void Serialize(std::ostream& a, const int16_t s);
	void Serialize(std::ostream& a, const std::string& s);

	template <typename V>
	void Serialize(std::ostream& a, const std::vector<V>& s)
	{
		Serialize(a, s.size());
		for (auto e : s)
			Serialize(a, e);
	}

	template <typename K, typename V>
	void Serialize(std::ostream& a, const std::map<K, V>& s)
	{
		Serialize(a, s.size());
		for (auto e : s)
		{
			Serialize(a, e.first);
			Serialize(a, e.second);
		}
	}
}
