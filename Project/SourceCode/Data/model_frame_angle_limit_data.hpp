#pragma once
#include <nlohmann/json.hpp>

struct ModelFrameAngleLimitData
{
	float max_angle		= 0.0f;
	float min_angle		= 0.0f;
	bool  is_limited	= false;	// êßå¿Ç™Ç©Ç©Ç¡ÇΩ
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, ModelFrameAngleLimitData& model_frame_angle_limit_data)
{
	j_data.at("max_angle").get_to(model_frame_angle_limit_data.max_angle);
	j_data.at("min_angle").get_to(model_frame_angle_limit_data.min_angle);
}

inline void to_json(nlohmann::json& j_data, const ModelFrameAngleLimitData& model_frame_angle_limit_data)
{
	j_data = nlohmann::json
	{
		{ "max_angle",	model_frame_angle_limit_data.max_angle },
		{ "min_angle",	model_frame_angle_limit_data.min_angle },
	};
}
#pragma endregion
