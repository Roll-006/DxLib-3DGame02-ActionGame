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

	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform);
	void AttachTarget(const std::shared_ptr<Transform> target_transform, const VECTOR& follow_correct_pos, const VECTOR& damping, const float damping_yaw);
	void DetachTarget();


	#pragma region Setter
	void SetTrackCorrectPos	(const VECTOR& track_correct_pos)	{ m_track_correct_pos	= track_correct_pos; }
	void SetDamping			(const VECTOR& damping)				{ m_damping				= damping; }
	void SetDampingYaw		(const float   damping_yaw)			{ m_damping_yaw			= damping_yaw; }
	#pragma endregion


	#pragma region Getter
	VECTOR GetTrackCorrectPos() const { return m_track_correct_pos; }
	VECTOR GetDamping()			const { return m_damping; }
	float  GetDampingYaw()		const { return m_damping_yaw; }
	#pragma endregion

private:
	void CalcCameraPos();

private:
	std::shared_ptr<Transform> m_camera_transform;
	std::shared_ptr<Transform> m_target_transform;

	VECTOR m_track_correct_pos;		// 補正座標(オフセット)
	VECTOR m_damping;				// 追尾遅延
	float  m_damping_yaw;			// ヨー角回転の追尾遅延

	bool   m_is_track;				// 追尾を行うかを判定
};
