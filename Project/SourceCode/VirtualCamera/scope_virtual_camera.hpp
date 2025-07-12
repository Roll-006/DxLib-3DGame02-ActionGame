#pragma once
#include "../Base/virtual_camera_base.hpp"

class ScopeVirtualCamera final : public VirtualCameraBase
{
public:
	ScopeVirtualCamera();
	~ScopeVirtualCamera() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

private:
	static constexpr int kPriority = 10;		// バーチャルカメラの優先度
};
