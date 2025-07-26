#pragma once
#include <memory>

#include "transform.hpp"
#include "../FPS/fps.hpp"

class CameraBody final
{
public:
	CameraBody(const std::shared_ptr<Transform> owner_transform);
	~CameraBody();

	void CalcPos();
	void CalcDampedPos();

	/// @brief ターゲットを追尾する
	void TrackTarget()		  { m_is_track = true; }
	/// @brief ターゲットの追尾を停止する
	void StopTrackingTarget() { m_is_track = false; }


	#pragma region Attach / Detach
	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform);
	void DetachTarget();
	#pragma endregion


	#pragma region Setter
	void SetCameraCorrectPos(const VECTOR& camera_correct_pos)	{ m_camera_correct_pos  = camera_correct_pos; }
	void SetDamping			(const VECTOR& damping);
	void SetDampingYaw		(const float   damping_yaw);
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] VECTOR GetCameraCorrectPps()	const { return m_camera_correct_pos; }
	[[nodiscard]] VECTOR GetDamping()			const { return m_damping; }
	[[nodiscard]] float  GetDampingYaw()		const { return m_damping_yaw; }
	#pragma endregion

private:

private:
	static constexpr float kMaxDampingNum = 20.0f;

	std::shared_ptr<Transform> m_owner_transform;
	std::shared_ptr<Transform> m_target_transform;

	VECTOR m_destination_pos;		// 目的とする座標
	VECTOR m_current_pos;			// 現在の座標

	VECTOR m_camera_correct_pos;	// カメラの補正座標(オフセット)

	VECTOR m_damping;				// 追尾遅延
	float  m_damping_yaw;			// ヨー角回転の追尾遅延

	bool   m_is_track;				// 追尾を行うかを判定
};
