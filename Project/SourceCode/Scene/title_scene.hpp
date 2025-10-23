#pragma once
#include "../Interface/i_scene.hpp"

#include "../Object/title_character.hpp"
#include "../ObjectPool/title_scene_effect_object_pool.hpp"

#include "../Light/light_holder.hpp"
#include "../VirtualCamera/virtual_camera.hpp"

#include "../Tab/title_tab.hpp"

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
	void StartFadeIn() override;

private:
	bool		m_is_active;
	SceneKind	m_scene_kind;

	std::shared_ptr<TitleCharacter>				m_title_character;
	std::shared_ptr<TitleTab>					m_title_tab;
	std::shared_ptr<TitleSceneEffectObjectPool>	m_title_scene_effect_object_pool;

	// TODO : ÇÃÇøÇ…ï ÉNÉâÉXÇ÷à⁄çs
	std::shared_ptr<VirtualCamera>				m_title_camera;
	std::shared_ptr<Transform>					m_aim_transform;
	std::shared_ptr<Transform>					m_smoke_transform;
	float										m_sin;
	int											m_smoke_delete_handle;
};
