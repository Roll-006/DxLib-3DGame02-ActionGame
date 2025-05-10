#pragma once
#include "../../Base/game_obj.hpp"

#include "../../Component/transform_component.hpp"
#include "../../Component/Camera/camera_component.hpp"
#include "../../Component/tracker_component.hpp"
#include "../../Component/Camera/camera_controller_component.hpp"
#include "../../Component/frustum_component.hpp"

class Camera : public GameObj
{
public:
	Camera() : GameObj(GameObjName.CAMERA, ObjTag.CAMERA)
	{
		AttachComponent<TransformComponent>			(this, VGet(0.0f, 0.0f, -300.0f));
		AttachComponent<TrackerComponent>			(this);
		AttachComponent<CameraControllerComponent>	(this);
		AttachComponent<FrustumComponent>			(this);
		AttachComponent<CameraComponent>			(this);

		GetComponent<TrackerComponent>()->SetTrackSpeed(10.0f);
	}
};
