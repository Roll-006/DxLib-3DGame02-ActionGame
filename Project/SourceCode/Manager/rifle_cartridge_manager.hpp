#pragma once
#include "../Base/singleton_base.hpp"

#include "../ObjectPool/object_pool_holder.hpp"
#include "../ObjectPool/rifle_cartridge_object_pool.hpp"

/// @brief ’eŠÛ‚¨‚æ‚Ñ–òä°‚ÌŠÇ—ƒNƒ‰ƒX
class RifleCartridgeManager final : public SingletonBase<RifleCartridgeManager>
{
public:
	void Update();
	void LateUpdate();
	void Draw() const;

	/// @brief ’eŠÛ‚Ì”­Ë
	void SearchValidRifleCartidge(GunBase& gun);

	/// @brief g—pÏ‚İ’eŠÛ‚ğíœ
	void DeleteBullet(const std::shared_ptr<ObjBase>& bullet);

private:
	RifleCartridgeManager();
	~RifleCartridgeManager() override;

	void AddRifleCartridge(const std::shared_ptr<ObjBase>& rifle_cartridge);
	void ReturnPool();

private:
	std::unordered_map<std::string, std::vector<std::shared_ptr<ObjBase>>> m_rifle_cartridge;

	friend SingletonBase<RifleCartridgeManager>;
};
