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

	void Update();

	/// @brief ターゲットを追尾する
	void TrackTarget()			{ m_is_track = true; }
	/// @brief ターゲットの追尾を停止する
	void StopTrackingTarget()	{ m_is_track = false; }


	#pragma region Attach / Detach
	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform);
	void AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& aim_correct_dir, const float horizontal_damping, const float vertical_damping);
	void DetachTarget();
	#pragma endregion


	#pragma region Setter
	void SetRot					(const MATRIX&			rot_matrix);
	void SetAimCorrect			(const VECTOR&			aim_correct)		{ m_aim_correct			= aim_correct;		}
	void SetHorizontalDamping	(const float			horizontal_damping)	{ m_horizontal_damping	= horizontal_damping;	}
	void SetVerticalDamping		(const float			vertical_damping)	{ m_vertical_damping	= vertical_damping;		}
	void SetScreen				(const Vector2D<float>& screen);
	void SetDeadZone			(const Vector2D<float>& dead_zone);
	void SetSoftZone			(const Vector2D<float>& soft_zone);
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
	std::shared_ptr<Transform> m_camera_transform;
	std::shared_ptr<Transform> m_target_transform;

	VECTOR			m_aim_correct;			// ターゲットを見る方向の補正(オフセット)
	float			m_horizontal_damping;	// 水平方向の追尾遅延
	float			m_vertical_damping;		// 垂直方向の追尾遅延
	Vector2D<float>	m_screen;				// ターゲットをスクリーンのどこに位置させるか
	Vector2D<float>	m_dead_zone;			// デッドゾーン
	Vector2D<float>	m_soft_zone;			// ソフトゾーン
	Vector2D<float> m_bias;					// デッドゾーンおよびソフトゾーンのずらし量

	bool			m_is_track;				// 追尾を行うかを判定
};
