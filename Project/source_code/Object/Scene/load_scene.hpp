#pragma once
#include "../../Base/scene_obj.hpp"

class LoadScene : public SceneObj
{
public:
	LoadScene() : SceneObj(SceneObjName.LOAD, ObjTag.SCENE)
	{

	}
};
