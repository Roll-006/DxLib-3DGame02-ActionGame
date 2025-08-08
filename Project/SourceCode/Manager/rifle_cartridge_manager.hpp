#pragma once
#include "../Base/singleton_base.hpp"

#include "../ObjectPool/object_pool_holder.hpp"
#include "../ObjectPool/rifle_cartridge_object_pool.hpp"

/// @brief 弾丸および薬莢の管理クラス
class RifleCartridgeManager final : public SingletonBase<RifleCartridgeManager>
{
public:
	void Update();
	void LateUpdate();
	void DrawToShadowMap()	const;
	void Draw()				const;

	/// @brief 弾丸の発射
	void Shot(GunBase& gun);

	/// @brief 使用済み弾丸を削除
	void DeleteBullet(std::shared_ptr<ObjBase> bullet);


	// TODO : 仮実装。のちに削除
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

	void AddRifleCartridge(const std::shared_ptr<ObjBase> rifle_cartridge);

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<ObjBase>>> m_rifle_cartridge;

	std::queue<VECTOR> m_hit_pos;	// TODO : 仮の衝突座標。のちに削除

	friend SingletonBase<RifleCartridgeManager>;
};
