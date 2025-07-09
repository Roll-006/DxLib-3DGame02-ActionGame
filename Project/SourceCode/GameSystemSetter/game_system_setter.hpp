#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>

#include "../Base/one_instance_singleton_base.hpp"

#include "../Window/window.hpp"
#include "../Manager/input_checker.hpp"
#include "../Random/random_generator.hpp"

class GameSystemSetter final : public OneInstanceSingletonBase<GameSystemSetter>
{
public:
	GameSystemSetter();
	~GameSystemSetter();

private:
	void SetUpGameSystem();
};
