#pragma once
#include "../Interface/i_scene.hpp"

class LoadScene final : public IScene
{
public:
	LoadScene(const SceneKind next_scene_kind);
	~LoadScene() override;

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

private:
	bool		m_is_active;
	SceneKind	m_scene_kind;
	SceneKind	m_next_scene_kind;
};
