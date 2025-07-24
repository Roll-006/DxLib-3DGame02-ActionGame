#pragma once
#include <memory>
#include <algorithm>

#include "transform.hpp"
#include "../Calculation/vector_2d.hpp"
#include "../FPS/fps.hpp"

class CameraAim final
{
public:
	CameraAim(const std::shared_ptr<Transform> camera_transform);
	~CameraAim();

	/// @brief ターゲットを追尾する
	void TrackTarget()			{ m_is_track = true; }
	/// @brief ターゲットの追尾を停止する
	void StopTrackingTarget()	{ m_is_track = false; }


	#pragma region Attach / Detach
	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& target_correct_pos);
	void DetachTarget();
	#pragma endregion


	#pragma region Setter
	/// @brief 回転量を設定する
	/// @brief WARNING : 回転量の補正値は自動的に適用されるため考慮する必要はない
	/// @param rot_matrix 回転行列
	void SetRot					(const MATRIX&			rot_matrix);
	void SetAimCorrect			(const VECTOR&			aim_correct)		{ m_aim_correct			= aim_correct;		}
	void SetHorizontalDamping	(const float			horizontal_damping)	{ m_horizontal_damping	= horizontal_damping;	}
	void SetVerticalDamping		(const float			vertical_damping)	{ m_vertical_damping	= vertical_damping;		}
	/// @brief ターゲットをスクリーンのどこに位置させるかを設定
	/// @param screen 指定可能範囲 : 0.0～1.0
	void SetScreen				(const Vector2D<float>& screen);
	/// @brief デッドゾーンを設定
	/// @param dead_zone 指定可能範囲 : 0.0～1.0
	void SetDeadZone			(const Vector2D<float>& dead_zone);
	/// @brief ソフトゾーンを設定
	/// @param soft_zone 指定可能範囲 : 0.0～1.0
	void SetSoftZone			(const Vector2D<float>& soft_zone);
	/// @brief デッドゾーンおよびソフトゾーンのずらし量を設定
	/// @param bias 指定可能範囲 : -0.5～0.5
	void SetBias				(const Vector2D<float>& bias);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] VECTOR			GetAimPos()				const;
	[[nodiscard]] VECTOR			GetAimCorrect()			const { return m_aim_correct; }
	[[nodiscard]] float				GetHorizontalDamping()	const { return m_horizontal_damping; }
	[[nodiscard]] float				GetVerticalDamping()	const { return m_vertical_damping; }
	[[nodiscard]] Vector2D<float>	GetScreen()				const { return m_screen; }
	[[nodiscard]] Vector2D<float>	GetDeadZone()			const { return m_dead_zone; }
	[[nodiscard]] Vector2D<float>	GetSoftZone()			const { return m_soft_zone; }
	[[nodiscard]] Vector2D<float>	GetBias()				const { return m_bias; }
	#pragma endregion

private:
	/// @brief 姿勢を計算する
	//void CalcRotMatrix();

private:
	static constexpr float kMaxDampingNum = 20.0f;

	std::shared_ptr<Transform> m_camera_transform;
	std::shared_ptr<Transform> m_target_transform;

	VECTOR			m_target_correct_pos;	// ターゲットの補正座標(オフセット)
	VECTOR			m_aim_correct;			// ターゲットを見る方向の補正(オフセット)
	float			m_horizontal_damping;	// 水平方向の追尾遅延
	float			m_vertical_damping;		// 垂直方向の追尾遅延
	Vector2D<float>	m_screen;				// ターゲットをスクリーンのどこに位置させるか
	Vector2D<float>	m_dead_zone;			// デッドゾーン
	Vector2D<float>	m_soft_zone;			// ソフトゾーン
	Vector2D<float> m_bias;					// デッドゾーンおよびソフトゾーンのずらし量

	bool			m_is_track;				// 追尾を行うかを判定
};
