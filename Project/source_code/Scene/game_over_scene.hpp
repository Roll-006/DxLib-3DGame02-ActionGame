#pragma once
#include "../Base/scene_base.hpp"

class GameOverScene : public SceneBase
{
public:
	GameOverScene();
	~GameOverScene()override;

	void Init()		 override;
	void Update()	 override;
	void Draw()const override;

private:

};