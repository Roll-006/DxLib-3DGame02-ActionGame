#include "math.hpp"

MATRIX math::ConvertQuaternionToMatrix(const Quaternion& q)
{
	// 参考URL : [ http://marupeke296.com/DXG_No58_RotQuaternionTrans.html ]
	MATRIX mat = MGetIdent();

	//X軸
	mat.m[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	mat.m[0][1] = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
	mat.m[0][2] = 2.0f * q.x * q.z - 2.0f * q.w * q.y;

	//Y軸
	mat.m[1][0] = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
	mat.m[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	mat.m[1][2] = 2.0f * q.y * q.z + 2.0f * q.w * q.x;

	//Z軸
	mat.m[2][0] = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
	mat.m[2][1] = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
	mat.m[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

	// 転置関係にある式を参考にしているため、計算後に転置し元に戻す
	return MTranspose(mat);
}

Quaternion math::ConvertMatrixToQuaternion(const MATRIX& mat)
{
	// FIXME : おそらく不具合を起こす。行列の成分の配置が違う可能性高
	// 参考URL : [ http://marupeke296.com/DXG_No58_RotQuaternionTrans.html ]

	std::array<float, 4> e
	{
		 mat.m[0][0] - mat.m[1][1] - mat.m[2][2] + 1.0f,
		-mat.m[0][0] + mat.m[1][1] - mat.m[2][2] + 1.0f,
		-mat.m[0][0] - mat.m[1][1] + mat.m[2][2] + 1.0f,
		 mat.m[0][0] + mat.m[1][1] + mat.m[2][2] + 1.0f
	};

	int max_index = 0;
	for (int i = 0; i < e.size(); ++i)
	{
		if (e.at(i) > e.at(max_index))
		{
			max_index = i;
		}
	}

	std::array<float, 4> q{ 0.0f, 0.0f, 0.0f, 0.0f };
	float v = sqrtf(e.at(max_index)) * 0.5f;
	q.at(max_index) = v;
	float mult = 0.25f / v;

	Quaternion ret_q(q.at(0), q.at(1), q.at(2), q.at(3));
	switch (max_index)
	{
	case 0:
		ret_q.y = (mat.m[0][1] + mat.m[1][0]) * mult;
		ret_q.z = (mat.m[2][0] + mat.m[0][2]) * mult;
		ret_q.w = (mat.m[1][2] + mat.m[2][1]) * mult;
		break;

	case 1:
		ret_q.x = (mat.m[0][1] + mat.m[1][0]) * mult;
		ret_q.z = (mat.m[1][2] + mat.m[2][1]) * mult;
		ret_q.w = (mat.m[2][0] + mat.m[0][2]) * mult;
		break;

	case 2:
		ret_q.x = (mat.m[2][0] + mat.m[0][2]) * mult;
		ret_q.y = (mat.m[1][2] + mat.m[2][1]) * mult;
		ret_q.w = (mat.m[0][1] + mat.m[1][0]) * mult;
		break;

	case 3:
		ret_q.x = (mat.m[1][2] + mat.m[2][1]) * mult;
		ret_q.y = (mat.m[2][0] + mat.m[0][2]) * mult;
		ret_q.z = (mat.m[0][1] + mat.m[1][0]) * mult;
		break;
	}
	return ret_q;
}

VECTOR math::GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q)
{
	// 参考URL : [ https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7 ]

	Quaternion q_pos(pos.x, pos.y, pos.z, 0.0f);
	Quaternion n_rotate_q = quat::GetNormalizedQuaternion(rotate_q);
	Quaternion totated_q = n_rotate_q * q_pos * quat::GetInverseQuaternion(n_rotate_q);
	return VGet(totated_q.x, totated_q.y, totated_q.z);
}
