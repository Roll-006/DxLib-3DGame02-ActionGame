#pragma once
#include "../Base/game_obj.hpp"

class GameObjManager
{
public:
	static void Generate();
	static void Delete();
	static GameObjManager* GetInstance();

	void AddObj	  (const std::shared_ptr<GameObj> obj);
	void RemoveObj(const std::string& obj_name);

	[[nodiscard]] std::shared_ptr<GameObj> GetObj(const std::string& obj_name);

private:
	GameObjManager();
	~GameObjManager();

private:
	std::unordered_map<std::string, std::shared_ptr<GameObj>> m_objects;

	static GameObjManager* m_instance;
};
