#pragma once
#include <unordered_map>

#include "../Data/Kind/sort_kind.hpp"

namespace algorithm
{
	#pragma region 変換
	/// @brief std::vector<std::pair<key, value>>をstd::unorderd_map<key, value>へ変換
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::unordered_map<KeyT, ValueT> ConvertPairsToUmap(const std::vector<std::pair<KeyT, ValueT>>& pairs)
	{
		std::unordered_map<KeyT, ValueT> u_map(pairs.begin(), pairs.end());
		return u_map;
	}

	/// @brief std::unorderd_map<key, value>をstd::vector<std::pair<key, value>>へ変換
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::vector<std::pair<KeyT, ValueT>> ConvertUmapToPairs(const std::unordered_map<KeyT, ValueT>& u_map)
	{
		std::vector<std::pair<KeyT, ValueT>> pairs(u_map.begin(), u_map.end());
		return pairs;
	}
	#pragma endregion


	#pragma region ソート
	/// @brief unorderd_mapのvalueをソート
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::unordered_map<KeyT, ValueT> Sort(const std::unordered_map<KeyT, ValueT>& u_map, const SortKind sort_kind)
	{
		std::vector<std::pair<KeyT, ValueT>> pairs = ConvertUmapToPairs(u_map);

		std::sort(pairs.begin(), pairs.end(), [=](const auto& a, const auto& b)
		{
			switch (sort_kind)
			{
			case SortKind::kAscending:	return a.second < b.second; break;
			case SortKind::kDescending:	return a.second > b.second;	break;
			}
		});

		return ConvertPairsToUmap(pairs);
	}

	/// @brief unorderd_mapのvalueをソート
	/// @brief main_u_mapとsub_u_mapは同じデータ型、同じ個数である必要あり
	/// @param main_u_map ソートされるunorderd_map
	/// @param sub_u_map main_u_mapに同じvalueがあった場合、sub_u_mapでソートを行う
	template<typename KeyT, typename ValueT>
	[[nodiscard]] std::unordered_map<KeyT, ValueT> Sort(const std::unordered_map<KeyT, ValueT>& main_u_map, const std::unordered_map<KeyT, ValueT>& sub_u_map, const SortKind sort_kind)
	{
		std::vector<std::pair<KeyT, ValueT>> pairs = ConvertUmapToPairs(main_u_map);

		std::sort(pairs.begin(), pairs.end(), [=](const auto& a, const auto& b)
		{
			switch (sort_kind)
			{
			case SortKind::kAscending:
				if (a.second != b.second) { return a.second < b.second; }

				// main_u_mapが同じ値であった場合はsub_u_mapでソート
				return sub_u_map.at(a.first) < sub_u_map.at(b.first);
				break;

			case SortKind::kDescending:
				if (a.second != b.second) { return a.second > b.second; }

				return sub_u_map.at(a.first) > sub_u_map.at(b.first);
				break;
			}
		});

		return ConvertPairsToUmap(pairs);
	}
	#pragma endregion
}