#pragma once
#include "../Object/player.hpp"

class PlayerUICreator final
{
public:
	PlayerUICreator(const std::shared_ptr<Player> player);
	~PlayerUICreator();

	void LateUpdate();

	void TestDraw();

private:
	/// @brief e‚ÌƒNƒƒXƒwƒA‚ğ¶¬‚·‚é
	void CreateCrossHair();

	void CreateSniperRifleCrossHair();

private:
	std::shared_ptr<Player> m_player;

	std::shared_ptr<ShapeBase> m_2d_diffusion_shape;	// “ñŸŒ³ã‚ÌŠgU”ÍˆÍ}Œ`
};