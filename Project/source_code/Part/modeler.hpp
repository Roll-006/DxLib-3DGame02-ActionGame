#pragma once
#include <dxlib_helper.h>

#include "../Manager/handle_keeper.hpp"
#include "../Data/Path/bone_path.hpp"
#include "transform.hpp"

class Modeler final
{
public:
	Modeler(const std::shared_ptr<Transform> transform, const std::string& file_path, const bool is_turn_around);
	Modeler(const std::shared_ptr<Transform> transform, const int model_handle,		  const bool is_turn_around);
	~Modeler();

	void Draw() const;

	/// @brief モデルに不透明度処理を適用
	void ApplyOpacity() const;

	/// @brief モデルに行列情報を適用させる
	/// @brief MEMO : 通常描画時に適用されるが、武器などがモデルの行列情報を基準とするため先行して適用
	void ApplyMatrix() const;

	[[nodiscard]] int GetModelHandle() const { return m_model_handle; }

private:
	int   m_model_handle;
	float m_opacity;			// モデルの不透明度(0.0f～1.0f)
	bool  m_is_turn_around;		// モデルをY軸180°回転させるかを判定

	std::shared_ptr<Transform> m_transform;
};
