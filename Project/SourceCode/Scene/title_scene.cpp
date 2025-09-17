#include "title_scene.hpp"
#include "../Manager/scene_manager.hpp"

TitleScene::TitleScene() : 
	m_is_active(true)
{

}

TitleScene::~TitleScene()
{

}

void TitleScene::Init()
{

}

void TitleScene::Update()
{

}

void TitleScene::LateUpdate()
{

}

void TitleScene::DrawToShadowMap() const
{

}

void TitleScene::Draw() const
{
	DrawFormatString(0, 40, 0xffffff, "TITLE");
}

std::shared_ptr<IScene> TitleScene::ChangeScene()
{
	const auto command = CommandHandler::GetInstance();

	if (command->IsExecute(CommandKind::kDecide, TimeKind::kCurrent))
	{
		return std::make_shared<PlayScene>();
	}

	return nullptr;
}
