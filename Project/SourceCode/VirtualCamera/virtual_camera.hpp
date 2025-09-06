#pragma once
#include "../Base/virtual_camera_base.hpp"

class VirtualCamera final : public VirtualCameraBase
{
public:
	VirtualCamera(const std::string& name, const BlendActivationPolicyKind blend_policy_kind);
	~VirtualCamera() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;
};
