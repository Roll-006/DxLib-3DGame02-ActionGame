#include "patrol_route_giver.hpp"

std::unordered_map<std::string, std::unordered_map<int, VECTOR>> PatrolRouteGiver::m_routes = {};

PatrolRouteGiver::PatrolRouteGiver(const PatrolKind patrol_kind, const std::string& route_id) :
	m_patrol_kind	(patrol_kind),
	m_route_id		(route_id),
	m_current_step	(1),
	m_max_step		(0),
	m_is_back		(false),
	m_is_end		(false)
{
	if (m_routes.empty())
	{
		nlohmann::json j_data;
		if (json_loader::Load("Data/JSON/patrol_route.json", j_data))
		{
			const auto route_size = j_data["patrol_route"].size();
			for (size_t i = 0; i < route_size; i++)
			{
				const auto step_size = j_data["patrol_route"][std::to_string(i)].size();
				for (size_t j = 0; j < step_size; ++j)
				{
					const auto pos = j_data["patrol_route"][std::to_string(i)][std::to_string(j)].get<VECTOR>();
					m_routes[std::to_string(i)][j] = pos;
				}
			}
		}
	}

	m_max_step = static_cast<int>(m_routes.at(route_id).size()) - 1;
}

PatrolRouteGiver::~PatrolRouteGiver()
{

}

bool PatrolRouteGiver::ChangeDestination(VECTOR& destination_pos, const VECTOR& current_pos)
{
	if (m_is_end) { return false; }

	if (VSize(destination_pos - current_pos) < kChangeThresholdDistance)
	{
		switch (m_patrol_kind)
		{
		case PatrolKind::kLoop:
			return LoopParolRoute(destination_pos);

		case PatrolKind::kBack:
			return BackParolRoute(destination_pos);

		case PatrolKind::kStop:
			return StopParolRoute(destination_pos);
		}
	}

	destination_pos = m_routes.at(m_route_id).at(m_current_step);
	return false;
}

bool PatrolRouteGiver::LoopParolRoute(VECTOR& destination_pos)
{
	// 終点の場合は始点に戻す
	m_current_step = m_current_step >= m_max_step ? 0 : m_current_step + 1;

	m_current_step = std::clamp(m_current_step, 0, m_max_step);
	destination_pos = m_routes.at(m_route_id).at(m_current_step);
	return true;
}

bool PatrolRouteGiver::BackParolRoute(VECTOR& destination_pos)
{
	if (m_is_back)
	{
		// 始点に到達した場合、バックを終了し前進させる
		if (m_current_step <= 0)
		{
			m_current_step	= 1;
			m_is_back		= false;
		}
		else
		{
			--m_current_step;
		}
	}
	else
	{
		// 終点に到達した場合、前進を終了しバックさせる
		if (m_current_step >= m_max_step)
		{
			m_current_step	= m_max_step - 1;
			m_is_back		= true;
		}
		else
		{
			++m_current_step;
		}
	}

	m_current_step = std::clamp(m_current_step, 0, m_max_step);
	destination_pos = m_routes.at(m_route_id).at(m_current_step);
	return true;
}

bool PatrolRouteGiver::StopParolRoute(VECTOR& destination_pos)
{
	// すでに終点の場合は早期return
	if (m_current_step >= m_max_step)
	{
		m_is_end = true;
		return false;
	}

	++m_current_step;

	m_current_step = std::clamp(m_current_step, 0, m_max_step);
	destination_pos = m_routes.at(m_route_id).at(m_current_step);
	return true;
}
