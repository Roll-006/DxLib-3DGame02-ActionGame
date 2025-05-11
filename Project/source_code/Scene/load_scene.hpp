#pragma once
#include "../Base/scene_base.hpp"

class LoadScene : public SceneBase
{
public:
	LoadScene();
	~LoadScene()override;

	void Init()		 override;
	void Update()	 override;
	void Draw()const override;

private:

};