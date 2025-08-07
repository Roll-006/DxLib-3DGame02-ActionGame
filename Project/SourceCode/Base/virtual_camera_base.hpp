#pragma once
#include "obj_base.hpp"

#include "../Part/camera_body.hpp"
#include "../Part/camera_aim.hpp"
#include "../Part/camera_noise.hpp"

#include "../Data/Kind/scene_kind.hpp"
#include "../Data/Kind/virtual_camera_kind.hpp"

class VirtualCameraBase abstract : public ObjBase
{
public:
	VirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind);
	virtual ~VirtualCameraBase() = default;

	void AddToObjManager() override;

	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform);
	void DetachTarget();

	[[nodiscard]] int				GetPriority()		const { return m_priority; }
	[[nodiscard]] VirtualCameraKind GetCameraKind()		const { return m_camera_kind; }

protected:
	/// @brief body, aim, noiseによりトランスフォーム値を計算する
	void CalcTransform();

protected:
	std::shared_ptr<CameraBody>		m_body;
	std::shared_ptr<CameraAim>		m_aim;
	std::shared_ptr<CameraNoise>	m_noise;

	std::shared_ptr<Transform>		m_target_transform;

	std::vector<SceneKind>			m_active_scene_kind;	// どのシーンでアクティブ化されるかを格納
	int								m_priority;				// 優先度

private:
	VirtualCameraKind				m_camera_kind;			// カメラの種類
};
