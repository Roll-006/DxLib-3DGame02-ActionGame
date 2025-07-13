#pragma once
#include <memory>

#include "transform.hpp"
#include "../FPS/fps.hpp"

class CameraBody final
{
public:
	CameraBody(const std::shared_ptr<Transform> camera_transform);
	~CameraBody();

	/// @brief ターゲットを追尾する
	void TrackTarget()		  { m_is_track = true; }
	/// @brief ターゲットの追尾を停止する
	void StopTrackingTarget() { m_is_track = false; }


	#pragma region Attach / Detach
	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& target_correct_pos);
	void DetachTarget();
	#pragma endregion


	#pragma region Setter
	void SetCameraPos		(const VECTOR& camera_pos)			{ m_camera_transform->SetPos(CoordinateKind::kWorld, camera_pos); }
	void SetCameraCorrectDir(const VECTOR& camera_correct_dir)	{ m_camera_correct_dir	= camera_correct_dir; }
	void SetDistanceToTarget(const float   distance_to_target)	{ m_distance_to_target	= distance_to_target; }
	void SetDamping			(const VECTOR& damping)				{ m_damping				= damping; }
	void SetDampingYaw		(const float   damping_yaw)			{ m_damping_yaw			= damping_yaw; }
	#pragma endregion


	#pragma region Getter
	//[[nodiscard]] VECTOR GetCameraPos()			const;
	[[nodiscard]] VECTOR GetCameraCorrectDir()	const { return m_camera_correct_dir; }
	[[nodiscard]] float  GetDistanceToTarget()	const { return m_distance_to_target; }
	[[nodiscard]] VECTOR GetDamping()			const { return m_damping; }
	[[nodiscard]] float  GetDampingYaw()		const { return m_damping_yaw; }
	#pragma endregion

private:
	void CalcPos();

private:
	std::shared_ptr<Transform> m_camera_transform;
	std::shared_ptr<Transform> m_target_transform;

	VECTOR	m_target_correct_pos;	// ターゲットの補正座標(オフセット)
	VECTOR	m_camera_correct_dir;	// カメラの位置補正方向(オフセット)
	float	m_distance_to_target;	// ターゲットとの距離
	VECTOR	m_damping;				// 追尾遅延
	float	m_damping_yaw;			// ヨー角回転の追尾遅延

	bool	m_is_track;				// 追尾を行うかを判定
};
