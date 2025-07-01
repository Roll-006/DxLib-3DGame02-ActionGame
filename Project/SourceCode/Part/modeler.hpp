#pragma once
#include <dxlib_helper.h>

#include "../Manager/handle_keeper.hpp"
#include "../Data/Path/bone_path.hpp"
#include "transform.hpp"

class Modeler final
{
public:
	Modeler(const std::shared_ptr<Transform> transform, const std::string& file_path, const VECTOR& basic_angle);
	Modeler(const std::shared_ptr<Transform> transform, const std::string& file_path);
	Modeler(const std::shared_ptr<Transform> transform, const int model_handle,		  const VECTOR& basic_angle);
	Modeler(const std::shared_ptr<Transform> transform, const int model_handle);
	~Modeler();

	void Draw() const;

	/// @brief 不透明度を設定
	void SetOpacity(const float opacity) { m_opacity = opacity; }

	/// @brief モデルに不透明度処理を適用
	void ApplyOpacity() const;

	/// @brief モデルに行列情報を適用させる
	/// @brief MEMO : 通常描画時に適用されるが、武器などがモデルの行列情報を基準とするため先行して適用
	void ApplyMatrix() const;

	[[nodiscard]] int GetModelHandle() const { return m_model_handle; }

private:
	int    m_model_handle;
	float  m_opacity;			// モデルの不透明度(0.0f～1.0f)
	VECTOR m_basic_angle;		// モデルの基礎回転値(モデルが元から持つ回転を修正するための回転値)

	std::shared_ptr<Transform> m_transform;
};
