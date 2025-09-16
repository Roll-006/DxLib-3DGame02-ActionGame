#pragma once
#include "../Base/obj_base.hpp"
#include "../Command/command_handler.hpp"

class Selector final : public ObjBase
{
public:
	Selector();
	~Selector() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void AddToObjManager()			override;
	void RemoveToObjManager()		override;

	[[nodiscard]] float	GetDeltaTime() const override;

private:

};
