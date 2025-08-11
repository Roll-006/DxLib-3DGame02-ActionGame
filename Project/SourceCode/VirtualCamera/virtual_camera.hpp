#pragma once
#include "../Base/virtual_camera_base.hpp"

/// @brief 回転コントロールカメラ
class VirtualCamera final : public VirtualCameraBase
{
public:
	VirtualCamera(const BlendActivationPolicyKind blend_policy_kind);
	~VirtualCamera() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;
};
