#pragma once
#include "../Base/scene_base.hpp"

class PlayScene : public SceneBase
{
public:
	PlayScene();
	~PlayScene()override;

	void Init()		 override;
	void Update()	 override;
	void Draw()const override;

private:

};