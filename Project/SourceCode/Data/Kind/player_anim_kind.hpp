#pragma once

enum class PlayerAnimKind
{
	kNone = -1,


	#pragma region UPPER BODY
	kEquipKnife,
	kAimKnife,
	kStabKnife,
	kFirstSideSlashKnife,
	kSecondSideSlashKnife,
	kParry,
	kEquipGun,
	kAimGun,
	kReload,
	#pragma endregion


	#pragma region LOWER BODY
	kIdle,
	kCrouch,

	kMoveForward,
	kMoveBackward,
	kMoveLeft,
	kMoveRight,
	kMoveForwardLeft,
	kMoveForwardRight,
	kMoveBackwardLeft,
	kMoveBackwardRight,

	kMoveForwardCrouch,
	kMoveBackwardCrouch,
	kMoveLeftCrouch,
	kMoveRightCrouch,
	kMoveForwardLeftCrouch,
	kMoveForwardRightCrouch,
	kMoveBackwardLeftCrouch,
	kMoveBackwardRightCrouch,

	kMoveForwardRun,
	#pragma endregion
};

///// @brief プレイヤーのアニメーションの種類
///// @brief MEMO : 8方向移動の定義順は必ず
/////				　[Forward ➡ Backward ➡ Left ➡ Right ➡ ForwardLeft ➡ ForwardRight ➡ BackwardLeft ➡ BackwardRight]
/////				　とする必要あり
///// @brief WARNING : 8方向移動の定義順を変えるとアニメーションのアタッチに影響するため注意が必要
//enum class PlayerAnimKind
//{
//	kNone = -1,
//
//
//
//	#pragma region IDLE
//
//	#pragma region 通常
//	kIdle,									// IDLE
//	kIdleEquipKnifeped,						// ナイフ装備状態
//	kIdleAimKnife,							// ナイフを構える
//	kIdleStabKnife,							// 突き刺す(ナイフ)
//	kIdleSideSlashKnife,					// 大振り(ナイフ)
//	kIdleParry,								// パリィ
//	kIdleEquipGun,						// 銃装備状態
//	kIdleAimGun,							// 銃を構える
//	kIdleShot,								// 銃を撃つ
//	kIdleReload,							// リロード
//	#pragma endregion
//
//	#pragma region しゃがみ
//	kIdleCrouch,							// しゃがみ
//	kIdleCrouchEquipKnifeped,				// しゃがみ(ナイフ装備状態)
//	kIdleCrouchEquipGun,					// しゃがみ(銃装備状態)
//	kIdleCrouchReload,						// しゃがみ状態でリロード
//	#pragma endregion
//
//	#pragma region 振り向き
//	kIdleTurnAround,						// 振り向く
//	kIdleTurnAroundEquipKnifeped,			// 振り向く(ナイフ装備状態)
//	kIdleTurnAroundEquipGun,				// 振り向く(銃装備状態)
//	#pragma endregion
//
//	#pragma region しゃがみ振り向き
//	kIdleCrouchTurnAround,					// しゃがみ状態で振り向く
//	kIdleCrouchTurnAroundEquipKnifeped,		// しゃがみ状態で振り向く(ナイフ装備状態)
//	kIdleCrouchTurnAroundEquipGun,		// しゃがみ状態で振り向く(銃装備状態)
//	#pragma endregion
//
//	#pragma endregion
//
//
//
//	#pragma region 移動
//
//	#pragma region 通常
//	kMoveForward,							// 前方に移動
//	kMoveBackward,							// 後方に移動
//	kMoveLeft,								// 左に移動
//	kMoveRight,								// 右に移動
//	kMoveForwardLeft,						// 左前方に移動
//	kMoveForwardRight,						// 右前方に移動
//	kMoveBackwardLeft,						// 左後方に移動
//	kMoveBackwardRight,						// 右後方に移動
//	#pragma endregion
//
//	#pragma region 移動(ナイフ装備状態)
//	kMoveForwardEquipKnifeped,				// 前方に移動(ナイフ装備状態)
//	kMoveBackwardEquipKnifeped,				// 後方に移動(ナイフ装備状態)
//	kMoveLeftEquipKnifeped,					// 左に移動(ナイフ装備状態)
//	kMoveRightEquipKnifeped,				// 右に移動(ナイフ装備状態)
//	kMoveForwardLeftEquipKnifeped,			// 左前方に移動(ナイフ装備状態)
//	kMoveForwardRightEquipKnifeped,			// 右前方に移動(ナイフ装備状態)
//	kMoveBackwardLeftEquipKnifeped,			// 左後方に移動(ナイフ装備状態)
//	kMoveBackwardRightEquipKnifeped,		// 右後方に移動(ナイフ装備状態)
//	#pragma endregion
//
//	#pragma region ナイフを構えながら移動
//	kMoveForwardAimKnife,					// ナイフを構えながら前方に移動
//	kMoveBackwardAimKnife,					// ナイフを構えながら後方に移動
//	kMoveLeftAimKnife,						// ナイフを構えながら左に移動
//	kMoveRightAimKnife,						// ナイフを構えながら右に移動
//	kMoveForwardLeftAimKnife,				// ナイフを構えながら左前方に移動
//	kMoveForwardRightAimKnife,				// ナイフを構えながら右前方に移動
//	kMoveBackwardLeftAimKnife,				// ナイフを構えながら左後方に移動
//	kMoveBackwardRightAimKnife,				// ナイフを構えながら右後方に移動
//	#pragma endregion
//
//	#pragma region 移動(銃装備状態)
//	kMoveForwardEquipGun,				// 前方に移動(銃装備状態)
//	kMoveBackwardEquipGun,				// 後方に移動(銃装備状態)
//	kMoveLeftEquipGun,					// 左に移動(銃装備状態)
//	kMoveRightEquipGun,					// 右に移動(銃装備状態)
//	kMoveForwardLeftEquipGun,			// 左前方に移動(銃装備状態)
//	kMoveForwardRightEquipGun,			// 右前方に移動(銃装備状態)
//	kMoveBackwardLeftEquipGun,			// 左後方に移動(銃装備状態)
//	kMoveBackwardRightEquipGun,			// 右後方に移動(銃装備状態)
//	#pragma endregion
//
//	#pragma region 銃を構えながら移動
//	kMoveForwardAimGun,						// 銃を構えながら前方に移動
//	kMoveBackwardAimGun,					// 銃を構えながら後方に移動
//	kMoveLeftAimGun,						// 銃を構えながら左に移動
//	kMoveRightAimGun,						// 銃を構えながら右に移動
//	kMoveForwardLeftAimGun,					// 銃を構えながら左前方に移動
//	kMoveForwardRightAimGun,				// 銃を構えながら右前方に移動
//	kMoveBackwardLeftAimGun,				// 銃を構えながら左後方に移動
//	kMoveBackwardRightAimGun,				// 銃を構えながら右後方に移動
//	#pragma endregion
//
//	#pragma region 銃を撃ちながら移動
//	kMoveForwardShot,						// 銃を撃ちながら前方に移動
//	kMoveBackwardShot,						// 銃を撃ちながら後方に移動
//	kMoveLeftShot,							// 銃を撃ちながら左に移動
//	kMoveRightShot,							// 銃を撃ちながら右に移動
//	kMoveForwardLeftShot,					// 銃を撃ちながら左前方に移動
//	kMoveForwardRightShot,					// 銃を撃ちながら右前方に移動
//	kMoveBackwardLeftShot,					// 銃を撃ちながら左後方に移動
//	kMoveBackwardRightShot,					// 銃を撃ちながら右後方に移動
//	#pragma endregion
//
//	#pragma region リロードしながら移動
//	kMoveForwardReload,						// リロードしながら前方に移動
//	kMoveBackwardReload,					// リロードしながら後方に移動
//	kMoveLeftReload,						// リロードしながら左に移動
//	kMoveRightReload,						// リロードしながら右に移動
//	kMoveForwardLeftReload,					// リロードしながら左前方に移動
//	kMoveForwardRightReload,				// リロードしながら右前方に移動
//	kMoveBackwardLeftReload,				// リロードしながら左後方に移動
//	kMoveBackwardRightReload,				// リロードしながら右後方に移動
//	#pragma endregion
//
//	#pragma region しゃがみ移動
//	kMoveForwardCrouch,						// しゃがみ状態で前方に移動
//	kMoveBackwardCrouch,					// しゃがみ状態で後方に移動
//	kMoveLeftCrouch,						// しゃがみ状態で左に移動
//	kMoveRightCrouch,						// しゃがみ状態で右に移動
//	kMoveForwardLeftCrouch,					// しゃがみ状態で左前方に移動
//	kMoveForwardRightCrouch,				// しゃがみ状態で右前方に移動
//	kMoveBackwardLeftCrouch,				// しゃがみ状態で左後方に移動
//	kMoveBackwardRightCrouch,				// しゃがみ状態で右後方に移動
//	#pragma endregion
//	
//	#pragma region しゃがみ移動(ナイフ装備状態)
//	kMoveForwardCrouchEquipKnifeped,		// しゃがみ状態で前方に移動(ナイフ装備状態)
//	kMoveBackwardCrouchEquipKnifeped,		// しゃがみ状態で後方に移動(ナイフ装備状態)
//	kMoveLeftCrouchEquipKnifeped,			// しゃがみ状態で左に移動(ナイフ装備状態)
//	kMoveRightCrouchEquipKnifeped,			// しゃがみ状態で右に移動(ナイフ装備状態)
//	kMoveForwardLeftCrouchEquipKnifeped,	// しゃがみ状態で左前方に移動(ナイフ装備状態)
//	kMoveForwardRightCrouchEquipKnifeped,	// しゃがみ状態で右前方に移動(ナイフ装備状態)
//	kMoveBackwardLeftCrouchEquipKnifeped,	// しゃがみ状態で左後方に移動(ナイフ装備状態)
//	kMoveBackwardRightCrouchEquipKnifeped,	// しゃがみ状態で右後方に移動(ナイフ装備状態)
//	#pragma endregion
//
//	#pragma region しゃがみ移動(銃装備状態)
//	kMoveForwardCrouchEquipGun,			// しゃがみ状態で前方に移動(銃装備状態)
//	kMoveBackwardCrouchEquipGun,			// しゃがみ状態で後方に移動(銃装備状態)
//	kMoveLeftCrouchEquipGun,				// しゃがみ状態で左に移動(銃装備状態)
//	kMoveRightCrouchEquipGun,			// しゃがみ状態で右に移動(銃装備状態)
//	kMoveForwardLeftCrouchEquipGun,		// しゃがみ状態で左前方に移動(銃装備状態)
//	kMoveForwardRightCrouchEquipGun,		// しゃがみ状態で右前方に移動(銃装備状態)
//	kMoveBackwardLeftCrouchEquipGun,		// しゃがみ状態で左後方に移動(銃装備状態)
//	kMoveBackwardRightCrouchEquipGun,	// しゃがみ状態で右後方に移動(銃装備状態)
//	#pragma endregion
//
//	#pragma region リロードしながら移動
//	kMoveForwardCrouchReload,				// しゃがみ状態でリロードしながら前方に移動
//	kMoveBackwardCrouchReload,				// しゃがみ状態でリロードしながら後方に移動
//	kMoveLeftCrouchReload,					// しゃがみ状態でリロードしながら左に移動
//	kMoveRightCrouchReload,					// しゃがみ状態でリロードしながら右に移動
//	kMoveForwardLeftCrouchReload,			// しゃがみ状態でリロードしながら左前方に移動
//	kMoveForwardRightCrouchReload,			// しゃがみ状態でリロードしながら右前方に移動
//	kMoveBackwardLeftCrouchReload,			// しゃがみ状態でリロードしながら左後方に移動
//	kMoveBackwardRightCrouchReload,			// しゃがみ状態でリロードしながら右後方に移動
//	#pragma endregion
//
//	#pragma region ダッシュ移動
//	kMoveForwardRun,						// 前方に走る
//	kMoveForwardRunEquipKnifeped,			// 前方に走る(ナイフ装備状態)
//	kMoveForwardRunEquipGun,				// 前方に走る(銃装備状態)
//	kMoveForwardRunReload,					// 前方に走りながらリロード
//	#pragma endregion
//
//	#pragma endregion
//};
