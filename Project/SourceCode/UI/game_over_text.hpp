#pragma once
#include "../Part/screen_creator.hpp"
#include "../Data/game_over_text_data.hpp"
#include "../JSON/json_loader.hpp"
#include "../Calculation/math.hpp"

class GameOverText final
{
public:
	GameOverText();
	~GameOverText();

	void Init();
	void Update();
	void Draw() const;

private:
	void CreateResultScreen();

private:
	GameOverTextData				game_over_text_data;

	Vector2D<int>					m_text_center_pos;
	int								m_alpha_blend_num;
	float							m_scale;
	std::shared_ptr<ScreenCreator>	m_result_screen;

	friend void from_json(const nlohmann::json& j_data, GameOverText& game_over_text);
	friend void to_json  (nlohmann::json& j_data, const GameOverText& game_over_text);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, GameOverText& game_over_text)
{
	j_data.at("game_over_text_data").get_to(game_over_text.game_over_text_data);
}

inline void to_json(nlohmann::json& j_data, const GameOverText& game_over_text)
{
	j_data = nlohmann::json
	{
		{ "game_over_text_data", game_over_text.game_over_text_data },
	};
}
#pragma endregion
