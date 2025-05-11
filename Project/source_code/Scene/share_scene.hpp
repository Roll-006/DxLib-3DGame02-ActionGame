#pragma once
#include "../Base/scene_base.hpp"

class ShareScene : public SceneBase
{
public:
	ShareScene();
	~ShareScene()override;

	void Init()		 override;
	void Update()	 override;
	void Draw()const override;

private:

};