#pragma once
#include "../Base/singleton_base.hpp"

#include "../ObjectPool/object_pool_holder.hpp"
#include "../Object/Bullet.hpp"
#include "../Object/shell_casing.hpp"

/// @brief 弾丸および薬莢の管理クラス
class RifleCartridgeManager final : public SingletonBase<RifleCartridgeManager>
{
public:
	void Update();
	void LateUpdate();
	void Draw() const;

	/// @brief 弾丸の発射
	void Shot(GunBase& gun);

	/// @brief 使用済み弾丸を削除
	void DeleteBullet(const int obj_handle);

	void AddHitPos(const VECTOR& hit_pos)
	{
		if (m_hit_pos.size() == 20)
		{
			m_hit_pos.pop();
		}
		m_hit_pos.push(hit_pos);
	}

private:
	RifleCartridgeManager();
	~RifleCartridgeManager() override;

	void AddBullet		(const std::shared_ptr<Bullet>		bullet);
	void AddShellCasing	(const std::shared_ptr<ShellCasing> shell_casing);

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<ObjBase>>> m_rifle_cartridge;

	std::queue<VECTOR> m_hit_pos;	// MEMO : 仮の衝突座標

	friend SingletonBase<RifleCartridgeManager>;
};
