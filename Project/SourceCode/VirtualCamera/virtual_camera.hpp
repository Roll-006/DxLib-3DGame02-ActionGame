#pragma once
#include "../Part/camera_body.hpp"
#include "../Part/camera_aim.hpp"
#include "../Part/camera_noise.hpp"

#include "../Kind/blend_activation_policy_kind.hpp"

class CinemachineBrain;

class VirtualCamera final : public std::enable_shared_from_this<VirtualCamera>
{
public:
	VirtualCamera(const std::string& name, const BlendActivationPolicyKind blend_policy_kind);
	~VirtualCamera();

	void Init();
	void Update();
	void LateUpdate();

	void AttachTarget(const std::shared_ptr<Transform> target_transform);
	void DetachTarget();

	void SetPriority(const int priority);

	/// @brief オブジェクトをアクティブ化する
	void Activate()		{ m_is_active = true; }
	/// @brief オブジェクトを非アクティブ化する(削除せずに機能を停止)
	void Deactivate()	{ m_is_active = false; }


	#pragma region Getter
	[[nodiscard]] float							GetDeltaTime()					const;
	[[nodiscard]] int							GetCameraHandle()				const { return m_camera_handle; }
	[[nodiscard]] std::string					GetName()						const { return m_name; }
	[[nodiscard]] std::shared_ptr<Transform>	GetTransform()					const { return m_transform; }
	[[nodiscard]] std::shared_ptr<CameraBody>	GetBody()						const { return m_body; }
	[[nodiscard]] std::shared_ptr<CameraAim>	GetAim()						const { return m_aim; }
	[[nodiscard]] std::shared_ptr<CameraNoise>	GetNoise()						const { return m_noise; }
	[[nodiscard]] int							GetPriority()					const { return m_priority; }
	[[nodiscard]] BlendActivationPolicyKind		GetBlendActivationPolicyKind()	const { return m_blend_activation_policy_kind; }
	[[nodiscard]] bool							IsActive()						const { return m_is_active; }
	#pragma endregion

private:
	/// @brief body, aim, noiseによりトランスフォーム値を計算する
	void CalcTransform();

private:
	std::string						m_name;
	int								m_camera_handle;
	int								m_priority;							// 優先度
	bool							m_is_active;

	std::shared_ptr<Transform>		m_transform;
	std::shared_ptr<CameraBody>		m_body;
	std::shared_ptr<CameraAim>		m_aim;
	std::shared_ptr<CameraNoise>	m_noise;

	std::shared_ptr<Transform>		m_target_transform;

	BlendActivationPolicyKind		m_blend_activation_policy_kind;		// ブレンド時のアクティブ処理の方針
};
