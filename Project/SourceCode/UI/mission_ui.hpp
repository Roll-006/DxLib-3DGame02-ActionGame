#pragma once
#include "../JSON/json_loader.hpp"
#include "../Data/mission_ui_data.hpp"
#include "../GameTime/game_time_manager.hpp"
#include "../Calculation/math.hpp"

class MissionUI final
{
public:
	MissionUI();
	~MissionUI();

	void Init();
	void LateUpdate();
	void Draw() const;

	void Activate() { m_is_active = true; }

private:
	void CalcAlphaBlendNum();
	void CalcWaitTime();
	void CreateScreen();

private:
	MissionUIData					mission_ui_data;

	std::shared_ptr<ScreenCreator>	m_screen;
	Vector2D<int>					m_text_center_pos;
	int								m_alpha_blend_num;
	float							m_wait_timer;
	bool							m_is_active;
	bool							m_is_wait;

	friend void from_json(const nlohmann::json& j_data, MissionUI& mission_ui);
	friend void to_json  (nlohmann::json& j_data, const MissionUI& mission_ui);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, MissionUI& mission_ui)
{
	j_data.at("mission_ui_data").get_to(mission_ui.mission_ui_data);
}

inline void to_json(nlohmann::json& j_data, const MissionUI& mission_ui)
{
	j_data = nlohmann::json
	{
		{ "mission_ui_data", mission_ui.mission_ui_data },
	};
}
#pragma endregion
