#include "third_person_controller_component.hpp"

ThirdPersonControllerComponent::ThirdPersonControllerComponent(GameObj* owner_obj, TransformComponent* camera_transform) :
	ComponentBase		(owner_obj),
	m_camera_transform	(*camera_transform)
{

}

ThirdPersonControllerComponent::~ThirdPersonControllerComponent()
{

}

void ThirdPersonControllerComponent::Init()
{

}

void ThirdPersonControllerComponent::Update()
{
	Move();
}

void ThirdPersonControllerComponent::LateUpdate()
{

}

void ThirdPersonControllerComponent::Draw()const
{

}

void ThirdPersonControllerComponent::Move()
{
	VECTOR camera_dir = VTransform(VGet(0.0f, 0.0f, 1.0f), m_camera_transform.GetRotationMatrix());

	if (InputChecker::GetInstance()->GetKeyInputState(KEY_INPUT_4) == InputState::kSingle)
	{
		m_owner_obj->GetComponent<TransformComponent>()->SetPos(VGet(-100.0f, 0.0f, 0.0f));
	}
	if (InputChecker::GetInstance()->GetKeyInputState(KEY_INPUT_5) == InputState::kSingle)
	{
		m_owner_obj->GetComponent<TransformComponent>()->SetPos(VGet(0.0f, 0.0f, 0.0f));
	}
	if (InputChecker::GetInstance()->GetKeyInputState(KEY_INPUT_6) == InputState::kSingle)
	{
		m_owner_obj->GetComponent<TransformComponent>()->SetPos(VGet(100.0f, 0.0f, 0.0f));
	}
}
