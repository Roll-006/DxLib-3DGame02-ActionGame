#pragma once
#include "../Base/character_base.hpp"

#include "../GameTime/game_time_manager.hpp"
#include "../Part/player_animator.hpp"
#include "../Part/subject.hpp"

#include "assault_rifle.hpp"
#include "rocket_launcher.hpp"
#include "knife.hpp"
#include "../Part/weapon_shortcut_selecter.hpp"
#include "../Part/bone_pos_corrector.hpp"

#include "../Manager/camera_manager.hpp"

class PlayerStateController;

class Player final : public CharacterBase
{
public:
	Player();
	~Player() override;

	void Init()						override;
	void Update()					override;
	void LateUpdate()				override;
	void DrawToShadowMap()	const	override;
	void Draw()				const	override;

	void OnCollide(const ColliderPairOneToOneData& hit_collider_pair) override;

	void NotifyShotRocketLauncher();


	#pragma region アイテム
	/// @brief アイテムの所持登録 
	template<obj_concepts::ItemT ItemT>
	void AddItem(const std::shared_ptr<ItemT> item)
	{
		const auto item_kind = item->GetItemKind();

		if (std::find(m_items[item_kind].begin(), m_items[item_kind].end(), item) == m_items[item_kind].end())
		{
			m_items[item_kind].emplace_back(item);
		}
	}
	/// @brief アイテムの所持登録を解除
	template<obj_concepts::ItemT ItemT>
	void RemoveItem(const std::shared_ptr<ItemT> item)
	{
		const auto item_kind = item->GetItemKind();

		m_items[item_kind].erase(std::remove(m_items[item_kind].begin(), m_items[item_kind].end(), item), m_items[item_kind].end());
	}
	#pragma endregion


	#pragma region 武器
	/// @brief 武器を装備させる
	/// @brief ナイフの登録は許可しない
	template<obj_concepts::WeaponT WeaponT>
	void EquipWeapon(const std::shared_ptr<WeaponT> weapon)
	{
		if (weapon->GetWeaponKind() == WeaponKind::kKnife) { return; }

		m_current_equip_weapon = weapon;
	}
	/// @brief 武器の装備を解除
	void UnequipWeapon();

	/// @brief ナイフを装備させる
	template<obj_concepts::KnifeT KnifeT>
	void EquipKnife(const std::shared_ptr<KnifeT> knife)
	{
		m_current_equip_knife = knife;
	}
	/// @brief ナイフの装備を解除
	void UnequipKnife();
	#pragma endregion


	#pragma region State
	//void CalcMoveDirFirstFrame();

	void Move();

	/// @brief エイミング時の見る方向を修正するための値を設定する
	void SetLookDirOffsetValueForAim();

	/// @brief 進行方向を向く
	void DirOfMovement();
	/// @brief カメラのforward(Y軸は0)方向を向く
	void DirOfCameraForward();

	/// @brief 移動を停止する際の速度を計算
	void CalcMoveSpeedStop();
	/// @brief しゃがみ時の移動速度を計算
	void CalcMoveSpeedCrouch();
	/// @brief ダッシュ時の移動速度を計算
	void CalcMoveSpeedRun();

	/// @brief 切り裂く攻撃の移動補正値を計算
	//void CalcMoveOffsetSideSlashKnife();
	//void Test() { m_move_dir[TimeKind::kNext] = m_look_dir[TimeKind::kCurrent]; }
	#pragma endregion


	#pragma region Getter
	[[nodiscard]] float	GetDeltaTime() const override;
	[[nodiscard]] std::shared_ptr<Subject<Player>>			GetSubject()				const { return m_subject; }
	[[nodiscard]] std::shared_ptr<PlayerStateController>	GetStateController()		const { return m_state; }
	[[nodiscard]] std::shared_ptr<BonePosCorrector>			GetBonePosCorrector()		const { return m_bone_pos_corrector; }
	[[nodiscard]] std::vector<std::shared_ptr<IItem>>		GetCurrentHaveItem(const ItemKind item_kind) const { return m_items.at(item_kind); }
	[[nodiscard]] WeaponKind								GetCurrentEquipWeaponKind();
	[[nodiscard]] std::shared_ptr<WeaponBase>				GetCurrentEquipWeapon()		const { return m_current_equip_weapon; }
	[[nodiscard]] std::shared_ptr<KnifeBase>				GetCurrentEquipKnife()		const { return m_current_equip_knife; }
	[[nodiscard]] std::shared_ptr<WeaponShortcutSelecter>	GetWeaponShortcutSelecter()	const { return m_weapon_shortcut_selecter; }
	#pragma endregion

private:
	/// @brief 入力されたスティックの傾きから移動速度を計算
	/// @param input_slope スティックの傾き
	void CalcMoveSpeed(const float input_slope);

	void CalcVelocity();
	void CalcMoveDir(const VECTOR& velocity);
	void CalcLookDir();

	/// @brief カメラに見られる座標を計算
	//void CalcCameraAimPos();

	// パッド入力での速度ベクトルを取得
	[[nodiscard]] VECTOR GetVelocityFromPad();

	/// @brief カメラのforwardをもとにした進行方向ベクトルを取得する
	/// @return カメラのforwardのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveForward();
	/// @brief カメラのrightをもとにした進行方向ベクトルを取得する
	/// @return カメラのrightのY軸を消し飛ばしたベクトル
	[[nodiscard]] VECTOR GetMoveRight();

	/// @brief 無加工のボーンをカメラが追尾するかを判定
	/// @return true : ボーンそのものをカメラが追尾, false : ボーンを同じ高さにある位置を追尾
	//[[nodiscard]] bool IsTrackCameraOriginBone() const;

private:
	static constexpr VECTOR kBasicAngle							= { 0.0f, DX_PI_F, 0.0f };
	static constexpr float  kBasicScale							= 0.3f;

	static constexpr float kCrouchWalkSpeed						= 10.0f;
	static constexpr float kSlowWalkSpeed						= 17.0f;
	static constexpr float kWalkSpeed							= 27.0f;
	static constexpr float kRunSpeed							= 60.0f;
	static constexpr float kAcceleration						= 1.0f;					// 加速度(減速度も共通)

	static constexpr float kMoveDirOffsetSpeed					= 5.0f;					// 移動方向の補正速度
	static constexpr float kLookDirOffsetAngle					= 0.1f;					// 見る方向の補正角度
	static constexpr float kLookDirOffsetAngleForAim			= 0.3f;					// エイミング時の見る方向を回転させる角度
	static constexpr float kConfirmMoveDirThresholdDistance		= 0.08f;				// 目的の移動方向に到達したと判定する閾値
	static constexpr float kConfirmLookDirThresholdAngle		= 10.0f;				// 目的の見る方向に到達したと判定する閾値
	static constexpr float kConfirmLookDirThresholdAngleForAim	= 20.0f;				// エイミング時の目的の見る方向に到達したと判定する閾値

	static constexpr int   kWalkStickSlopeLimit					= 15000;				// 歩き状態とするスティック傾きの上限
	//static constexpr float kTurnAroundStickAngle				= 30.0f;				// 振り向きを行うスティックの入力角度

	static constexpr float kCapsuleRadius						= 8.0f;
	static constexpr float kLandingTriggerRadius				= 6.0f;

	static constexpr float kCameraAimOffsetBasicSpeed			= 40.0f;
	
	//static constexpr float kADSSpeed							= 70.0f;				// スコープをのぞき込む速度

private:
	std::shared_ptr<Subject<Player>>			m_subject;

	std::shared_ptr<PlayerStateController>		m_state;
	std::shared_ptr<BonePosCorrector>			m_bone_pos_corrector;
	std::shared_ptr<Transform>					m_camera_aim_transform;					// カメラのターゲットとなるトランスフォーム
	VECTOR										m_current_camera_aim_pos;

	std::unordered_map<TimeKind, VECTOR>		m_move_dir;								// 移動方向(TODO : 長さが1未満である時がある場合があるため命名を変更すべき)
	std::unordered_map<TimeKind, VECTOR>		m_look_dir;								// 向いている方向
	float										m_move_speed;

	float										m_look_dir_offset_angle;				// 見る方向の補正角度
	float										m_confirm_look_dir_threshold_angle;		// 目的の見る方向に到達したと判定する閾値
	
	std::unordered_map<ItemKind, std::vector<std::shared_ptr<IItem>>> m_items;			// 所持しているアイテム
	std::shared_ptr<WeaponBase>					m_current_equip_weapon;					// 現在装備している武器(ナイフ以外)
	std::shared_ptr<KnifeBase>					m_current_equip_knife;					// 現在装備しているナイフ
	std::shared_ptr<WeaponShortcutSelecter>		m_weapon_shortcut_selecter;				// ショートカットに登録されている武器
};
