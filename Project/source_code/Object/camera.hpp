#pragma once
#include "../Base/obj_base.hpp"
#include "../Manager/obj_manager.hpp"

class Camera final : public ObjBase
{
public:
	enum class TimeState
	{
		kCurrect,
		kNext,
	};

	Camera();
	~Camera();

	void Init();
	void Update();
	void Draw()const;

	void AttachTarget(const std::shared_ptr<ObjBase> obj);
	void AttachTarget(const std::string& obj_name);
	void DetachTarget();

private:
	static constexpr float kNear	= 10.0f;
	static constexpr float kFar		= 2500.0f;
	static constexpr float kFOV		= 60.0f;

	std::shared_ptr<Transform>				m_target_transform;
	std::unordered_map<TimeState, VECTOR>	m_target_pos;

	float m_move_speed;
	float m_distance_to_target;
};
