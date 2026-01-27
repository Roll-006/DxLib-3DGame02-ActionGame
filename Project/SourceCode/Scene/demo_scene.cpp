#include "demo_scene.hpp"
#include "../Manager/scene_manager.hpp"

DemoScene::DemoScene() : 
	m_is_active			(true),
	m_is_start_process	(false),
	m_loop_count		(0),
	m_scene_kind		(SceneKind::kTitle),
	m_demo_movie		(std::make_shared<MoviePlayer>("Data/Movie/test.mp4", false)),
	m_started_fade		(false)
{

}

DemoScene::~DemoScene()
{

}

void DemoScene::Init()
{

}

void DemoScene::Update()
{
	StartFadeIn();

	m_is_start_process = SceneFader::GetInstance()->GetAlphaBlendNum() < UCHAR_MAX;
	if (!m_is_start_process) { return; }

	m_demo_movie->CreateMovieScreen();
}

void DemoScene::LateUpdate()
{
	if (!m_is_start_process) { return; }
}

void DemoScene::DrawToShadowMap() const
{
	if (!m_is_start_process) { return; }
}

void DemoScene::Draw() const
{
	if (!m_is_start_process) { return; }

	m_demo_movie->Play();
}

std::shared_ptr<IScene> DemoScene::ChangeScene()
{
	const auto is_change = IsChangeTitleScene();
	printfDx("デモ : is_change = &d\n", is_change);

	// タイトル
	if (is_change)
	{
		return std::make_shared<TitleScene>();
	}

	return nullptr;
}

void DemoScene::StartFadeIn()
{
	++m_loop_count;
	if (m_loop_count == 5)
	{
		const auto fader = SceneFader::GetInstance();
		fader->StartFade(0, 100.0f);
	}

	printfDx("デモ : loop_count = %d\n", m_loop_count);
}

const bool DemoScene::IsChangeTitleScene()
{
	if (!m_is_start_process) { return false; }

	const auto fader = SceneFader::GetInstance();

	// フェード開始
	if (m_demo_movie->IsStop() > 3.0f && !m_started_fade)
	{
		m_started_fade = true;
		fader->StartFade(UCHAR_MAX, 110.0f);
	}

	// フェード終了判定
	return fader->GetAlphaBlendNum() >= UCHAR_MAX;
}
