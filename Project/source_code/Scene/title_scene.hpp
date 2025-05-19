#pragma once
#include "../Base/scene_base.hpp"

class TitleScene final : public SceneBase
{
public:
	TitleScene();
	~TitleScene()override;

	void Init()		 override;
	void Update()	 override;
	void Draw()const override;

private:

};
