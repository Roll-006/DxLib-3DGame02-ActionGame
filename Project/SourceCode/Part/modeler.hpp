#pragma once
#include "../Handle/handle_keeper.hpp"
#include "../Path/frame_path.hpp"
#include "../Data/material_data.hpp"
#include "transform.hpp"

class Modeler final
{
public:
	Modeler(const std::shared_ptr<Transform>& transform, const std::string& file_path, const VECTOR& basic_angle = v3d::GetZeroV(), const float basic_scale = 1.0f);
	Modeler(const int model_handle);
	~Modeler();

	void Draw() const;

	void InitMaterial() const;

	/// @brief 不透明度を設定
	void SetOpacity(const float opacity) { m_opacity = opacity; }

	/// @brief モデルに不透明度処理を適用
	void ApplyOpacity() const;

	/// @brief モデルに行列情報を適用させる
	/// @brief MEMO : 通常描画時に適用されるが、武器などがモデルの行列情報を基準とするため先行して適用
	void ApplyMatrix() const;

	[[nodiscard]] int						 GetModelHandle()		const { return m_model_handle; }
	[[nodiscard]] float						 GetOpacity()			const { return m_opacity; }
	[[nodiscard]] VECTOR					 GetBasicAngle()		const { return m_basic_angle; }
	[[nodiscard]] std::shared_ptr<Transform> GetTransform()			const { return m_transform; }

private:
	int				m_model_handle;
	float			m_opacity;			// モデルの不透明度(0.0f～1.0f)
	VECTOR			m_basic_angle;		// モデルの基礎回転値(モデルが元から持つ回転を修正するための回転値)
	VECTOR			m_basic_scale;		// モデルの基礎スケール

	std::shared_ptr<Transform> m_transform;

	friend void from_json(const nlohmann::json& j_data, Modeler& modeler);
	friend void to_json  (nlohmann::json& j_data, const Modeler& modeler);
};


#pragma region from / to JSON
inline void from_json(const nlohmann::json& j_data, Modeler& modeler)
{
	j_data.at("opacity")		.get_to(modeler.m_opacity);
	j_data.at("basic_angle")	.get_to(modeler.m_basic_angle);
	j_data.at("basic_scale")	.get_to(modeler.m_basic_scale);
}

inline void to_json(nlohmann::json& j_data, const Modeler& modeler)
{
	j_data = nlohmann::json
	{
		{ "opacity",		modeler.m_opacity },
		{ "basic_angle",	modeler.m_basic_angle },
		{ "basic_scale",	modeler.m_basic_scale }
	};
}
#pragma endregion
