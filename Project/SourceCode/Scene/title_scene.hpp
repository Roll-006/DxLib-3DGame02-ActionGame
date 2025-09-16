#pragma once
#include "../Interface/i_scene.hpp"

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

	[[nodiscard]] bool IsActive() const override { return m_is_active; }
	[[nodiscard]] std::shared_ptr<IScene> ChangeScene() override;

private:
	bool m_is_active;
};
