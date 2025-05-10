#pragma once
#include "../../Base/scene_obj.hpp"

#include "../Chara/player.hpp"
#include "../Camera/camera.hpp"

class ShareScene : public SceneObj
{
public:
	ShareScene() : SceneObj(SceneObjName.SHARE, ObjTag.SCENE)
	{
		AttachObj(std::make_shared<Camera>());
		AttachObj(std::make_shared<Player>());

		GetObj(GameObjName.CAMERA)->GetComponent<TrackerComponent>()->AttachMainTarget(GameObjName.PLAYER);
	}
};
