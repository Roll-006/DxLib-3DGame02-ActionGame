#pragma once
#include <nlohmann/json.hpp>

struct ModelFrameAngleLimitData
{
	float max_angle;
	float min_angle;
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& data, ModelFrameAngleLimitData& model_frame_angle_limit_data)
{
	data.at("max_angle").get_to(model_frame_angle_limit_data.max_angle);
	data.at("min_angle").get_to(model_frame_angle_limit_data.min_angle);
}

inline void to_json(nlohmann::json& data, const ModelFrameAngleLimitData& model_frame_angle_limit_data)
{
	data = nlohmann::json
	{
		{ "max_angle",	model_frame_angle_limit_data.max_angle },
		{ "min_angle",	model_frame_angle_limit_data.min_angle },
	};
}
#pragma endregion
