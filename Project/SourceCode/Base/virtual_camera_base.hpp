#pragma once
#include "obj_base.hpp"

#include "../Part/camera_body.hpp"
#include "../Part/camera_aim.hpp"
#include "../Part/camera_noise.hpp"

#include "../Data/Kind/virtual_camera_kind.hpp"
#include "../Data/Kind/blend_activation_policy_kind.hpp"

class CameraManager;

class VirtualCameraBase abstract : public ObjBase
{
public:
	VirtualCameraBase(const std::string& name, const VirtualCameraKind camera_kind, const BlendActivationPolicyKind blend_policy_kind);
	virtual ~VirtualCameraBase() = default;

	void AddToObjManager() override;

	/// @brief ターゲットをアタッチする(上書き可)
	void AttachTarget(const std::shared_ptr<Transform> target_transform);
	void DetachTarget();

	void SetPriority(const int priority);

	[[nodiscard]] float							GetDeltaTime()					const override;
	[[nodiscard]] VirtualCameraKind				GetCameraKind()					const { return m_camera_kind; }
	[[nodiscard]] std::shared_ptr<CameraBody>	GetBody()						const { return m_body; }
	[[nodiscard]] std::shared_ptr<CameraAim>	GetAim()						const { return m_aim; }
	[[nodiscard]] std::shared_ptr<CameraNoise>	GetNoise()						const { return m_noise; }
	[[nodiscard]] int							GetPriority()					const { return m_priority; }
	[[nodiscard]] BlendActivationPolicyKind		GetBlendActivationPolicyKind()	const { return m_blend_activation_policy_kind; }

protected:
	/// @brief body, aim, noiseによりトランスフォーム値を計算する
	void CalcTransform();

protected:
	std::shared_ptr<CameraBody>		m_body;
	std::shared_ptr<CameraAim>		m_aim;
	std::shared_ptr<CameraNoise>	m_noise;

	std::shared_ptr<Transform>		m_target_transform;

	int								m_priority;							// 優先度
	BlendActivationPolicyKind		m_blend_activation_policy_kind;		// ブレンド時のアクティブ処理の方針

private:
	VirtualCameraKind				m_camera_kind;						// カメラの種類
};
