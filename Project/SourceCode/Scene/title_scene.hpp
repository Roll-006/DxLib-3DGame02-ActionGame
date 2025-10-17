#pragma once
#include "../Interface/i_scene.hpp"

#include "../Tab/tab_drawer.hpp"
#include "../Tab/title_tab.hpp"
#include "../Tab/warning_tab.hpp"

class TitleScene final : public IScene
{
public:
	TitleScene();
	~TitleScene() override;

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
	bool		m_is_active;
	SceneKind	m_scene_kind;

	std::shared_ptr<TitleTab>	m_title_tab;
	std::shared_ptr<WarningTab> m_warning_tab;
};
