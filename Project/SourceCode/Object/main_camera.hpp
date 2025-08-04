#pragma once
#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"

class MainCamera final : public PhysicalObjBase
{
public:
	MainCamera();
	~MainCamera() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

private:
	/// @brief 見る方向を設定
	void SetAim();

	/// @brief レイキャストトリガーの位置を計算
	void CalcRayPos();
};
