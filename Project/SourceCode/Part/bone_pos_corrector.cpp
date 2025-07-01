#include "bone_pos_corrector.hpp"

void BonePosCorrector::CorrectGunPoseBone(const int model_handle, const VECTOR& look_dir, const MATRIX& rot, const bool m_is_ready_gun)
{
	const int frame_spine = MV1SearchFrame(model_handle, BonePath.SPINE);

	// データのシフト
	m_bone_angle[BonePath.SPINE][TimeKind::kPrev] = m_bone_angle[BonePath.SPINE][TimeKind::kCurrent];

	// 武器を構えていない場合は初期値に戻す
	if (!m_is_ready_gun)
	{
		MV1ResetFrameUserLocalMatrix(model_handle, frame_spine);
		return;
	}

	// カメラの行列からボーンの回転を取得
	bool is_gimbal_lock = false;
	m_bone_angle[BonePath.SPINE][TimeKind::kCurrent] = -math::ConvertRotMatrixToEulerAngles(MGetRotElem(rot), is_gimbal_lock);

	if (is_gimbal_lock)
	{
		// ジンバルロックが発生した場合は、発生していなかった状態の値を採用
		m_bone_angle[BonePath.SPINE][TimeKind::kCurrent] = m_bone_angle[BonePath.SPINE][TimeKind::kPrev];
	}
	else
	{
		m_bone_angle[BonePath.SPINE][TimeKind::kCurrent].y = 0.0f;

		// HACK : ワールドZ軸に対してforwardが90°を超えるとボーンが反転する現象が起きているためその条件下のみ反転し直す
		//		  ConvertRotMatrixToEulerAngles関数の修正が必要な可能性あり
		const auto forward = math::ConvertRotMatrixToAxes(rot).z_axis;
		if (math::GetAngleBetweenTwoVector(forward, axis::GetWorldZAxis()) >= 90.0f * math::kDegreesToRadian)
		{
			m_bone_angle[BonePath.SPINE][TimeKind::kCurrent].x -= DX_PI_F;
			m_bone_angle[BonePath.SPINE][TimeKind::kCurrent].z -= DX_PI_F;
		}
	}

	const MATRIX result_m = math::ConvertEulerAnglesToRotMatrix(m_bone_angle[BonePath.SPINE][TimeKind::kCurrent]);

	// 行列情報を適用
	MV1SetFrameUserLocalMatrix(model_handle, frame_spine, result_m);
}
