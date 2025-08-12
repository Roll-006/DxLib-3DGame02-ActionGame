#pragma once
#include "../Data/Kind/virtual_camera_controller_kind.hpp"

class IVirtualCameraController abstract
{
public:
	virtual ~IVirtualCameraController() = default;

	virtual void Update()	  abstract;
	virtual void LateUpdate() abstract;

	/// @brief アクティブ化する
	virtual void Activate()   abstract;
	/// @brief 非アクティブ化する(削除せずに機能を停止)
	virtual void Deactivate() abstract;

	virtual [[nodiscard]] VirtualCameraControllerKind GetVirtualCameraControllerKind() const abstract;
	virtual [[nodiscard]] int GetControllerHandle() const abstract;
	virtual [[nodiscard]] bool IsActive() const abstract;
};
