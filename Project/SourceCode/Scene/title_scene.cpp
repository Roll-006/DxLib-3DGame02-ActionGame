#include "title_scene.hpp"
#include "../Manager/scene_manager.hpp"

TitleScene::TitleScene() : 
	m_is_active		(true),
	m_scene_kind	(SceneKind::kTitle),
	m_title_tab		(std::make_shared<TitleTab>()),
	m_warning_tab	(std::make_shared<WarningTab>())
{
	TabDrawer::GetInstance()->AddTab(m_title_tab);
	TabDrawer::GetInstance()->AddTab(m_warning_tab);
}

TitleScene::~TitleScene()
{
	TabDrawer::GetInstance()->RemoveTab(typeid(TitleTab));
	TabDrawer::GetInstance()->RemoveTab(typeid(WarningTab));
}

void TitleScene::Init()
{

}

void TitleScene::Update()
{
	m_title_tab		->Update();
	m_warning_tab	->Update();

	if (m_title_tab->IsActivateWarningTab())
	{
		m_warning_tab->Activate();
		m_title_tab->StopSelect();
	}

	if (m_warning_tab->IsBack())
	{
		m_warning_tab->Deactivate();
		m_title_tab->AllowSelect();
	}
}

void TitleScene::LateUpdate()
{

}

void TitleScene::DrawToShadowMap() const
{

}

void TitleScene::Draw() const
{
	DrawFormatString(0, 40, 0xffffff, "Current Scene : TITLE");
	DrawFormatString(0, 60, 0xffffff, "SPACE / A でゲームスタート");
}

std::shared_ptr<IScene> TitleScene::ChangeScene()
{
	const auto command = CommandHandler::GetInstance();

	// プレイ
	if (m_title_tab->IsGameStart())
	{
		return std::make_shared<PlayScene>();
	}

	return nullptr;
}
