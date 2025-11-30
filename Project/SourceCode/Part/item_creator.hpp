#pragma once
#include "../Object/player.hpp"

class ItemCreator final
{
public:
	ItemCreator(const std::shared_ptr<Player>& player);
	~ItemCreator();

	void Init();
	void Update();
	void LateUpdate();
	void Draw() const;

private:
	/// @brief “G‚ª€–S‚µ‚½Û‚ÌƒAƒCƒeƒ€‚ğ¶¬
	void CreateDeadEnemyItem(const DeadEnemyEvent& event);
	
	void RemoveItem(const GotItemEvent& event);

private:
	std::shared_ptr<Player> m_player;
	std::vector<std::shared_ptr<ObjBase>> m_items;
};
