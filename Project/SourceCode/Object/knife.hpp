#pragma once
#include "../Base/knife_base.hpp"
#include "../JSON/json_loader.hpp"

class Knife final : public KnifeBase
{
public:
	Knife();
	~Knife() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;
};
