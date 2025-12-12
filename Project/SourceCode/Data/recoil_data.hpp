#pragma once
#include <DxLib.h>
#include <nlohmann/json.hpp>

struct RecoilData
{
	float  max_yaw			= 0.0f;						// 指定可能範囲 :  0～1
	float  min_yaw			= 0.0f;						// 指定可能範囲 : -1～0
	float  yaw				= 0.0f;						// ヨー(Y軸)回転値 正の値で指定
	float  pitch			= 0.0f;						// ピッチ(X軸)回転値 正の値で指定
	float  recoil_time		= 0.0f;						// 最高地点に到達するまでの時間
	VECTOR return_damping	= { 1.0f, 1.0f, 1.0f };		// 元地点への復帰する際の減衰値
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, RecoilData& recoil_data)
{
	j_data.at("max_yaw")			.get_to(recoil_data.max_yaw);
	j_data.at("min_yaw")			.get_to(recoil_data.min_yaw);
	j_data.at("yaw")				.get_to(recoil_data.yaw);
	j_data.at("pitch")			.get_to(recoil_data.pitch);
	j_data.at("recoil_time")		.get_to(recoil_data.recoil_time);
	j_data.at("return_damping")	.get_to(recoil_data.return_damping);
}

inline void to_json(nlohmann::json& j_data, const RecoilData& recoil_data)
{
	j_data = nlohmann::json
	{
		{ "max_yaw",		recoil_data.max_yaw },
		{ "min_yaw",		recoil_data.min_yaw },
		{ "yaw",			recoil_data.yaw },
		{ "pitch",			recoil_data.pitch },
		{ "recoil_time",	recoil_data.recoil_time },
		{ "return_damping", recoil_data.return_damping },
	};
}
#pragma endregion
