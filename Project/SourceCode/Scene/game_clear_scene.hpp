#pragma once
#include "../Base/scene_base.hpp"

class GameClearScene final : public SceneBase
{
public:
	GameClearScene();
	~GameClearScene() override;

	void Init()		  override;
	void Update()	  override;
	void LateUpdate() override;
	void Draw() const override;

private:

};