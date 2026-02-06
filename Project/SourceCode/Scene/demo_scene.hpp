#pragma once
#include "../Interface/i_scene.hpp"
#include "../Part/movie_player.hpp"
#include "../Part/scene_fader.hpp"
#include "../JSON/json_loader.hpp"

class DemoScene final : public IScene
{
public:
	DemoScene();
	~DemoScene() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void Activate()		override { m_is_active = true; }
	void Deactivate()	override { m_is_active = false; }

	[[nodiscard]] bool						IsActive()		const	override { return m_is_active; }
	[[nodiscard]] std::shared_ptr<IScene>	ChangeScene()			override;
	[[nodiscard]] virtual SceneKind			GetSceneKind()	const	override { return m_scene_kind; }

private:
	void StartFadeIn() override;
	[[nodiscard]] const bool IsChangeTitleScene();

private:
	bool							m_is_active;
	bool							m_is_start_process;
	int								m_loop_count;
	SceneKind						m_scene_kind;

	std::shared_ptr<MoviePlayer>	m_demo_movie;
	bool							m_started_fade;
};
