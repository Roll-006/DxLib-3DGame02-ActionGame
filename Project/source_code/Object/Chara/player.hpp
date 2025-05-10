#pragma once
#include "../../Component/transform_component.hpp"
#include "../../Component/Model/modeler_component.hpp"
#include "../../Component/Model/animator_component.hpp"
#include "../../Component/third_person_controller_component.hpp"

#include "../../Data/Kind/player_kind.hpp"

#include "../../Object/Camera/camera.hpp"

class Player : public GameObj
{
public:
	Player() : GameObj(GameObjName.PLAYER, ObjTag.PLAYER)
	{
		AttachComponent<TransformComponent>				(this, VGet(0.0f, 0.0f, 0.0f));
		AttachComponent<ModelerComponent>				(this, ModelPath.CHARA_01);
		AttachComponent<AnimatorComponent>				(this, 3.0f);
		AttachComponent<ThirdPersonControllerComponent>	(this, GameObjManager::GetInstance()->GetObj(GameObjName.CAMERA)->GetComponent<TransformComponent>().get());

		GetComponent<AnimatorComponent>()->AddAnimHandle(static_cast<int>(PlayerAnimState::kIdle), AnimPath.IDLE_01, AnimTag.NONE, 20.0f, true);
		GetComponent<AnimatorComponent>()->AddAnimHandle(static_cast<int>(PlayerAnimState::kWalk), AnimPath.WALK_01, AnimTag.WALK, 20.0f, true);
		GetComponent<AnimatorComponent>()->AddAnimHandle(static_cast<int>(PlayerAnimState::kJog),  AnimPath.JOG_01,  AnimTag.WALK, 20.0f, true);
		GetComponent<AnimatorComponent>()->AddAnimHandle(static_cast<int>(PlayerAnimState::kRun),  AnimPath.RUN_01,  AnimTag.WALK, 20.0f, true);
		GetComponent<AnimatorComponent>()->AddAnimHandle(static_cast<int>(PlayerAnimState::kJump), AnimPath.JUMP_01, AnimTag.NONE, 20.0f, true);
		GetComponent<AnimatorComponent>()->AddAnimHandle(static_cast<int>(PlayerAnimState::kFall), AnimPath.FALL_01, AnimTag.NONE, 20.0f, true);
	}
};
