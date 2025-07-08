#pragma once
#include "../Manager/obj_manager.hpp"
#include "../Manager/collision_manager.hpp"

class MainCamera final : public PhysicalObjBase
{
public:
	MainCamera();
	~MainCamera() override;

	void Init()			override;
	void Update()		override;
	void LateUpdate()	override;
	void Draw() const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

private:
	/// @brief Œ©‚é•ûŒü‚ğİ’è
	void SetAim();
};
