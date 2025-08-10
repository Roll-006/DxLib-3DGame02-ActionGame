#pragma once
#include "../Base/virtual_camera_base.hpp"

/// @brief 回転コントロールカメラ
class VirtualCamera final : public VirtualCameraBase
{
public:
	VirtualCamera();
	~VirtualCamera()		override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;
};
