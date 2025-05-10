#pragma once
#include "../Data/Name/scene_obj_name.hpp"
#include "../Manager/game_obj_manager.hpp"

class SceneObj : public ObjBase
{
public:
	SceneObj(const std::string& name, const std::string& tag);
	~SceneObj()override;

	void Init()			override;
	void Update()		override;
	void Draw()const	override;

	void AttachObj(const std::shared_ptr<GameObj> obj);
	void DetachObj(const std::string& obj_name);

	[[nodiscard]] std::shared_ptr<GameObj> GetObj(const std::string& obj_name);

private:
	std::unordered_map<std::string, std::shared_ptr<GameObj>> m_objects;
};
