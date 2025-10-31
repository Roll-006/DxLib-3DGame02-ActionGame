#pragma once
#include <string>
#include <unordered_map>

#include "../JSON/json_loader.hpp"
#include "../Calculation/math.hpp"

class PatrolRouteGiver final
{
public:
	enum class PatrolKind
	{
		kNone = -1,

		kLoop,	// ルートをループする(end ➡ start)
		kBack,	// ルートを戻る(end ➡ end - 1 … start ➡ start + 1)
		kStop,	// 終点で止まる(end ➡ end)
	};

public:
	PatrolRouteGiver(const PatrolKind patrol_kind, const std::string& route_id);
	~PatrolRouteGiver();

	/// @brief 目的地を変更する
	/// @param destination_pos 目的地(変更があった場合は結果を代入)
	/// @param current_pos 現在の座標
	/// @return true : 目的の変更あり, false : 目的地の変更なし
	bool ChangeDestination(VECTOR& destination_pos, const VECTOR& current_pos);

	/// @brief 巡回が終了したかを判定
	/// @brief PatrolKindがkStopの場合しか意味を持たない
	[[nodiscard]] bool IsEnd() const { return m_is_end; }

private:
	[[nodiscard]] bool LoopParolRoute(VECTOR& destination_pos);
	[[nodiscard]] bool BackParolRoute(VECTOR& destination_pos);
	[[nodiscard]] bool StopParolRoute(VECTOR& destination_pos);

private:
	static constexpr float kChangeThresholdDistance = 10.0f;

	static std::unordered_map<std::string, std::unordered_map<int, VECTOR>> m_routes;
	
	PatrolKind  m_patrol_kind;
	std::string m_route_id;
	int			m_current_step;
	int			m_max_step;
	bool		m_is_back;
	bool		m_is_end;
};
