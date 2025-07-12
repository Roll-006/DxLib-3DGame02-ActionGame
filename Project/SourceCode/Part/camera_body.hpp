#pragma once
#include <memory>

#include "transform.hpp"
#include "../FPS/fps.hpp"

class CameraBody final
{
public:
	CameraBody(const std::shared_ptr<Transform> camera_transform);
	~CameraBody();

	void Update();

	/// @brief ターゲットを追尾する
	void TrackTarget()		  { m_is_track = true; }
	/// @brief ターゲットの追尾を停止する
	void StopTrackingTarget() { m_is_track = false; }


	#pragma region Attach / Detach
	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform);
	void AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& camera_correct_pos);
	void DetachTarget();
	#pragma endregion


	#pragma region Setter
	void SetCameraPos		(const VECTOR& camera_pos)			{ m_camera_transform->SetPos(CoordinateKind::kWorld, camera_pos); }
	void SetCameraCorrectPos(const VECTOR& camera_correct_pos)	{ m_camera_correct_pos = camera_correct_pos; }
	void SetDamping			(const VECTOR& damping)				{ m_damping				= damping; }
	void SetDampingYaw		(const float   damping_yaw)			{ m_damping_yaw			= damping_yaw; }
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] VECTOR GetCameraPos()			const;
	[[nodiscard]] VECTOR GetCameraCorrectPos()	const { return m_camera_correct_pos; }
	[[nodiscard]] VECTOR GetDamping()			const { return m_damping; }
	[[nodiscard]] float  GetDampingYaw()		const { return m_damping_yaw; }
	#pragma endregion


private:
	std::shared_ptr<Transform> m_camera_transform;
	std::shared_ptr<Transform> m_target_transform;

	VECTOR m_camera_correct_pos;	// カメラの補正座標(オフセット)
	VECTOR m_damping;				// 追尾遅延
	float  m_damping_yaw;			// ヨー角回転の追尾遅延

	bool   m_is_track;				// 追尾を行うかを判定
};
