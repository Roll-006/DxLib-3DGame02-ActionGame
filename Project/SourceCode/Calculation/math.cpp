#include "../Data/IncludeList/shape.hpp"
#include "math.hpp"

#pragma region 変換
MATRIX math::ConvertQuaternionToRotMatrix(const MATRIX& mat, const Quaternion& q)
{
    MATRIX m = MTranspose(mat);

	//X軸
	m.m[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	m.m[0][1] =        2.0f * q.x * q.y + 2.0f * q.w * q.z;
	m.m[0][2] =        2.0f * q.x * q.z - 2.0f * q.w * q.y;

	//Y軸
	m.m[1][0] =        2.0f * q.x * q.y - 2.0f * q.w * q.z;
	m.m[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	m.m[1][2] =        2.0f * q.y * q.z + 2.0f * q.w * q.x;

	//Z軸
	m.m[2][0] =        2.0f * q.x * q.z + 2.0f * q.w * q.y;
	m.m[2][1] =        2.0f * q.y * q.z - 2.0f * q.w * q.x;
	m.m[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

	// 計算後に転置し元に戻す
	return MGetRotElem(MTranspose(m));
}

Quaternion math::ConvertRotMatrixToQuaternion(const MATRIX& mat)
{
    // TODO    : 処理の先頭と末尾で転置するべき
	// FIXME   : おそらく不具合を起こす。行列の成分の配置が違う可能性高
	// 参考URL : [ http://marupeke296.com/DXG_No58_RotQuaternionTrans.html ]

    MATRIX tmp_m = MTranspose(mat);

	std::array<float, 4> e
	{
         tmp_m.m[0][0] - tmp_m.m[1][1] - tmp_m.m[2][2] + 1.0f,
		-tmp_m.m[0][0] + tmp_m.m[1][1] - tmp_m.m[2][2] + 1.0f,
		-tmp_m.m[0][0] - tmp_m.m[1][1] + tmp_m.m[2][2] + 1.0f,
         tmp_m.m[0][0] + tmp_m.m[1][1] + tmp_m.m[2][2] + 1.0f
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
	const float v    = sqrtf(e.at(max_index)) * 0.5f;
	q.at(max_index)  = v;
	const float mult = 0.25f / v;

	Quaternion ret_q(q.at(0), q.at(1), q.at(2), q.at(3));
	switch (max_index)
	{
	case 0:
		ret_q.y = (tmp_m.m[0][1] + tmp_m.m[1][0]) * mult;
		ret_q.z = (tmp_m.m[2][0] + tmp_m.m[0][2]) * mult;
		ret_q.w = (tmp_m.m[1][2] + tmp_m.m[2][1]) * mult;
		break;

	case 1:
		ret_q.x = (tmp_m.m[0][1] + tmp_m.m[1][0]) * mult;
		ret_q.z = (tmp_m.m[1][2] + tmp_m.m[2][1]) * mult;
		ret_q.w = (tmp_m.m[2][0] + tmp_m.m[0][2]) * mult;
		break;

	case 2:
		ret_q.x = (tmp_m.m[2][0] + tmp_m.m[0][2]) * mult;
		ret_q.y = (tmp_m.m[1][2] + tmp_m.m[2][1]) * mult;
		ret_q.w = (tmp_m.m[0][1] + tmp_m.m[1][0]) * mult;
		break;

	case 3:
		ret_q.x = (tmp_m.m[1][2] + tmp_m.m[2][1]) * mult;
		ret_q.y = (tmp_m.m[2][0] + tmp_m.m[0][2]) * mult;
		ret_q.z = (tmp_m.m[0][1] + tmp_m.m[1][0]) * mult;
		break;
	}
	return ret_q;
}

MATRIX math::ConvertAxesToXYZRotMatrix(const Axes& axes, const Axes& parent_axes)
{
    // オイラー角を取得
    const VECTOR angle = ConvertAxesToEulerAngles(parent_axes, axes);

    MATRIX mat = MGetIdent();
    CreateRotationXYZMatrix(&mat, angle.x, angle.y, angle.z);
    return MGetRotElem(mat);
}

VECTOR math::ConvertAxesToEulerAngles(const Axes& axes, const Axes& parent_axes)
{
    const float angle_x = GetAngleBetweenTwoVector(parent_axes.x_axis, axes.x_axis);
    const float angle_y = GetAngleBetweenTwoVector(parent_axes.y_axis, axes.y_axis);
    const float angle_z = GetAngleBetweenTwoVector(parent_axes.z_axis, axes.z_axis);

    return VECTOR(angle_x, angle_y, angle_z);
}

VECTOR math::ConvertRotMatrixToEulerAngles(const MATRIX& mat)
{
    VECTOR angle = v3d::GetZeroV();
    const MATRIX m = mat;

    GetMatrixXYZRotation(&m, &angle.x, &angle.y, &angle.z);

    return angle;
}

VECTOR math::ConvertRotMatrixToEulerAngles(const MATRIX& mat, bool& is_gimbal_lock)
{
    VECTOR angle = v3d::GetZeroV();
    const MATRIX m = mat;

    is_gimbal_lock = GetMatrixXYZRotation(&m, &angle.x, &angle.y, &angle.z);

    return angle;
}

Axes math::ConvertRotMatrixToAxes(const MATRIX& mat)
{
    const MATRIX m = MGetRotElem(mat);

    const VECTOR scale
    {
        VSize(VGet(m.m[0][0], m.m[0][1], m.m[0][2])),
        VSize(VGet(m.m[0][0], m.m[0][1], m.m[0][2])),
        VSize(VGet(m.m[2][0], m.m[2][1], m.m[2][2]))
    };

    const VECTOR reciprocal
    {
        1.0f / scale.x,
        1.0f / scale.y,
        1.0f / scale.z
    };

    // スケールを打ち消す
    const VECTOR x_axis = VTransform(axis::GetWorldXAxis(), m) * reciprocal.x;
    const VECTOR y_axis = VTransform(axis::GetWorldYAxis(), m) * reciprocal.y;
    const VECTOR z_axis = VTransform(axis::GetWorldZAxis(), m) * reciprocal.z;

    return Axes(x_axis, y_axis, z_axis);
}

MATRIX math::ConvertEulerAnglesToRotMatrix(const VECTOR& angle)
{
    MATRIX mat = MGetIdent();
    CreateRotationXYZMatrix(&mat, angle.x, angle.y, angle.z);
    return MGetRotElem(mat);
}
#pragma endregion


#pragma region 修正
float math::ConnectMinusPiToPi(const float angle)
{
    // -π～πの値をループ
    float connected_angle = angle;
    if (connected_angle <= -DX_PI_F) { connected_angle += DX_TWO_PI_F; }
    if (connected_angle >=  DX_PI_F) { connected_angle -= DX_TWO_PI_F; }
    return connected_angle;
}
#pragma endregion


#pragma region 三平方の定理
float math::GetAdjacentLengthIsoscelesRightTriangle(const float hypotenuse_length)
{
    return sqrt(hypotenuse_length * hypotenuse_length * 0.5f);
}

float math::GetHypotenuseLengthIsoscelesRightTriangle(const float adjacent_length)
{
    return sqrt(adjacent_length * adjacent_length + adjacent_length * adjacent_length);
}

float math::GetAdjacentLengthRightTriangle(const float opposite_length, const float hypotenuse_length)
{
    return sqrt(hypotenuse_length * hypotenuse_length - opposite_length * opposite_length);
}

float math::GetHypotenuseLengthRightTriangle(const float adjacent_length, const float opposite_length)
{
    return sqrt(adjacent_length * adjacent_length + opposite_length * opposite_length);
}
#pragma endregion


#pragma region ベクトル
bool math::IsHorizontal(const VECTOR& v1, const VECTOR& v2)
{
    VECTOR cross = VCross(v1, v2);

    // 小数点第5位より後は誤差とみなして四捨五入
    cross = RoundOff(cross, -5);
    return cross == v3d::GetZeroV();
}

bool math::IsVertical(const VECTOR& v1, const VECTOR& v2)
{
    float dot = VDot(v1, v2);

    // 小数点第5位より後は誤差とみなして四捨五入
    dot = RoundOff(dot, -5);
    return dot == 0.0f;
}

VECTOR math::GetNormalVector(const VECTOR& v)
{
    VECTOR normal_v = v3d::GetZeroV();

    if (v != axis::GetWorldXAxis())
    {
        normal_v = math::GetNormalVector(v, axis::GetWorldXAxis());
    }
    else if (v != axis::GetWorldYAxis())
    {
        normal_v = math::GetNormalVector(v, axis::GetWorldYAxis());
    }
    else if (v != axis::GetWorldZAxis())
    {
        normal_v = math::GetNormalVector(v, axis::GetWorldZAxis());
    }
    return v3d::GetNormalizedV(normal_v);
}

VECTOR math::GetNormalVector(const VECTOR& v1, const VECTOR& v2)
{
    // [法線ベクトル = v1とv2の外積]から成分を取得
    return v3d::GetNormalizedV(VCross(v1, v2));
}

VECTOR math::GetProjectionVector(const VECTOR& projected_v, const VECTOR& base_v)
{
    const float dot         = VDot(base_v, projected_v);
    const float square_base = VSquareSize(base_v);
    return (dot / square_base) * base_v;
}
#pragma endregion


#pragma region 角度
bool math::IsAcuteAngle(const VECTOR& v1, const VECTOR& v2)
{
    const float radian = VDot(v1, v2) / (VSize(v1) * VSize(v2));
    return (radian < 90.0f * kDegreesToRadian && radian > 0.0f);
}

bool math::IsAcuteAngle(const float radian)
{
    return (radian < 90.0f * kDegreesToRadian && radian > 0.0f);
}

float math::GetAngleBetweenTwoVector(const VECTOR& v1, const VECTOR& v2)
{
    const float length1 = VSize(v1);
    const float length2 = VSize(v2);

    if (length1 == 0.0f || length2 == 0.0f) { return 0.0f; }

    // 誤差を消し飛ばす
    float dot = VDot(v1, v2) / (length1 * length2);
    dot = std::clamp(dot, -1.0f, 1.0f);

    return acos(dot);
}
#pragma endregion


#pragma region 回転
VECTOR math::GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q)
{
	// 参考URL : [ https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7 ]

	const Quaternion q_pos(pos.x, pos.y, pos.z, 0.0f);
	const Quaternion n_rotate_q = quat::GetNormalizedQuaternion(rotate_q);
	const Quaternion totated_q  = n_rotate_q * q_pos * quat::GetInverseQuaternion(n_rotate_q);

	return VGet(totated_q.x, totated_q.y, totated_q.z);
}

float math::GetYaw(const VECTOR& v)
{
    return atan2f(v.x, v.z);
}

VECTOR math::GetYawRotVector(const VECTOR& v)
{
    return VGet(0.0f, GetYaw(v), 0.0f);
}

Axes math::GetAxes(const VECTOR& dir, const Axes& parent_axes)
{
    // directionを基準として各軸を取得
    const VECTOR local_dir_z = v3d::GetNormalizedV(dir);
    const VECTOR local_dir_x = math::GetNormalVector(parent_axes.y_axis, local_dir_z);
    const VECTOR local_dir_y = math::GetNormalVector(local_dir_z, local_dir_x);

    return Axes(local_dir_x, local_dir_y, local_dir_z);
}
#pragma endregion


#pragma region 重心
VECTOR math::GetCentroidOfATriangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3)
{
    VECTOR centroid = v3d::GetZeroV();
    centroid.x = (pos1.x + pos2.x + pos3.x) / 3.0f;
    centroid.y = (pos1.y + pos2.y + pos3.y) / 3.0f;
    centroid.z = (pos1.z + pos2.z + pos3.z) / 3.0f;
    return centroid;
}

VECTOR math::GetCentroidOfATriangle(const Triangle& triangle)
{
    const VECTOR pos1 = triangle.GetPos(0);
    const VECTOR pos2 = triangle.GetPos(1);
    const VECTOR pos3 = triangle.GetPos(2);
    return GetCentroidOfATriangle(pos1, pos2, pos3);
}

VECTOR math::GetCentroidOfAQuadrilateral(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4)
{
    const VECTOR centroid_triangle1 = GetCentroidOfATriangle(pos1, pos2, pos3);
    const VECTOR centroid_triangle2 = GetCentroidOfATriangle(pos3, pos4, pos1);
    const Line   line1(centroid_triangle1, v3d::GetNormalizedV(centroid_triangle2 - centroid_triangle1));

    const VECTOR centroid_triangle3 = GetCentroidOfATriangle(pos2, pos3, pos4);
    const VECTOR centroid_triangle4 = GetCentroidOfATriangle(pos4, pos1, pos2);
    const Line   line2(centroid_triangle3, v3d::GetNormalizedV(centroid_triangle4 - centroid_triangle3));

    // 四角形の時点でnulloptを返すことはないが、
    // 引数が正常でなかった場合nulloptである可能性があるため判定を行う
    const auto intersection = GetIntersectionLineAndLine(line1, line2);
    return intersection ? *intersection : centroid_triangle1;
}

VECTOR math::GetCentroidOfAQuadrilateral(const Square& square)
{
    const VECTOR pos1 = square.GetPos(0);
    const VECTOR pos2 = square.GetPos(1);
    const VECTOR pos3 = square.GetPos(2);
    const VECTOR pos4 = square.GetPos(3);
    return GetCentroidOfAQuadrilateral(pos1, pos2, pos3, pos4);
}
#pragma endregion


#pragma region 図形同士の関係
std::optional<VECTOR> math::GetIntersectionLineAndLine(const Line& line1, const Line& line2)
{
    //            pos1 + direction1 * t1 = pos2 + direction2 * t2
    // direction1 * t1 - direction2 * t2 = pos2 - pos1

    const VECTOR p1p2         = line2.GetPos() - line1.GetPos();
    const VECTOR cross_v      = VCross(line1.GetDir(), line2.GetDir());
    const float  cross_length = VDot(cross_v, cross_v);

    if (cross_length == 0.0f) { return std::nullopt; }

    const float t1 = VDot(VCross(p1p2, line1.GetDir()), cross_v) / cross_length;
    const float t2 = VDot(VCross(p1p2, line2.GetDir()), cross_v) / cross_length;

    const VECTOR p1 = line1.GetPoint(t1);
    const VECTOR p2 = line2.GetPoint(t2);

    if (VSize(p2 - p1) == 0.0f)
    {
        return p1;
    }
    return std::nullopt;
}

std::optional<VECTOR> math::GetIntersectionSegmentAndPlane(const Segment& segment, const Plane& plane)
{
    const VECTOR a = segment.GetBeginPos();
    const VECTOR b = segment.GetEndPos();
    const VECTOR ab = b - a;
    const VECTOR pa = a - plane.GetPos();
    const VECTOR pb = b - plane.GetPos();
    const VECTOR n = plane.GetNormalVector();

    if ((VDot(pa, n) >= 0 && VDot(pb, n) <= 0) || (VDot(pa, n) <= 0 && VDot(pb, n) >= 0))
    {
        const float dot_pa_n = std::abs(VDot(pa, n));
        const float dot_pb_n = std::abs(VDot(pb, n));

        // 同一平面上にある場合は不定値を返す
        return a + ab * (dot_pa_n / (dot_pa_n + dot_pb_n));
    }
    return std::nullopt;
}

bool math::IsSameLine(const Line& line1, const Line& line2)
{
    if (!IsHorizontal(line1.GetDir(), line2.GetDir())) { return false; }

    const VECTOR direction = line2.GetDir() - line1.GetDir();
    return IsHorizontal(line1.GetDir(), direction) ? true : false;
}

VECTOR math::GetClosestPosOnPlaneFromSegment(const Plane& plane, const Segment& segment)
{
    const float distance1 = GetDistancePointToPlane(segment.GetBeginPos(), plane);
    const float distance2 = GetDistancePointToPlane(segment.GetEndPos(), plane);
    std::optional<VECTOR> closest_pos = GetIntersectionSegmentAndPlane(segment, plane);

    // 交点を持つ場合
    if (closest_pos)
    {
        return std::isfinite(closest_pos->x) ? *closest_pos : segment.GetBeginPos();
    }

    if (distance1 < distance2)
    {
        closest_pos = segment.GetBeginPos() + plane.GetNormalVector() * distance1;
        if (GetDistancePointToPlane(*closest_pos, plane) == 0.0f)
        {
            return *closest_pos;
        }
        return segment.GetBeginPos() - plane.GetNormalVector() * distance1;
    }

    closest_pos = segment.GetEndPos() + plane.GetNormalVector() * distance2;
    if (GetDistancePointToPlane(*closest_pos, plane) == 0.0f)
    {
        return *closest_pos;
    }
    return segment.GetEndPos() - plane.GetNormalVector() * distance2;
}

VECTOR math::GetClosestPosOnSegmentFromPlane(const Segment& segment, const Plane& plane)
{
    const VECTOR p = GetClosestPosOnPlaneFromSegment(plane, segment);
    const float distance = GetDistanceSegmentToPlane(segment, plane);

    const VECTOR closest_pos = p + plane.GetNormalVector() * distance;
    if (GetDistancePointToSegment(closest_pos, segment) == 0.0f)
    {
        return closest_pos;
    }
    return p - plane.GetNormalVector() * distance;
}

bool math::IsPointAheadOfPlane(const VECTOR& point, const Plane& plane)
{
    const VECTOR v = point - plane.GetPos();
    const float angle = GetAngleBetweenTwoVector(plane.GetNormalVector(), v);

    return angle < 90 * kDegreesToRadian;
}

bool math::IsPointOnSphereSurface(const VECTOR& point, const Sphere& sphere)
{
    return VDot(point - sphere.GetPos(), point - sphere.GetPos()) == sqrt(sphere.GetRadius());
}
#pragma endregion


#pragma region 図形
Vector2D<int> math::GetRandomPointInCircle2D(const Vector2D<int>& center_pos, const float radius)
{
    // 角度・距離を乱数で設定
    float angle     = DX_TWO_PI_F * ((GetRand(DX_TWO_PI_F - 1.0f) + 1.0f) / DX_TWO_PI_F);
    float distance  = radius * ((GetRand(radius - 1) + 1) / radius);

    Vector2D<int> pos;
    pos.x = center_pos.x + distance * cosf(angle);
    pos.y = center_pos.y + distance * sinf(angle);
    return pos;
}

VECTOR math::GetRandomPointInCircle(const Circle& circle)
{
    // 角度・距離を乱数で設定
    // FIXME : GetRandがint型のためデータが失われている
    float angle     = DX_TWO_PI_F * ((GetRand(DX_TWO_PI_F - 1.0f) + 1.0f) / DX_TWO_PI_F);
    float distance  = circle.GetRadius() * ((GetRand(circle.GetRadius() - 1) + 1) / circle.GetRadius());

    // 座標を距離分移動させ、回転させる
    VECTOR pos = circle.GetPos() + math::GetNormalVector(circle.GetNormalVector()) * distance;
    return math::GetRotatedPos(pos, quat::CreateQuaternion(circle.GetNormalVector(), angle));
}
#pragma endregion


#pragma region 最短距離
float math::GetDistancePointToLine          (const VECTOR&      point,      const Line&     line)
{
    VECTOR h = v3d::GetZeroV();
    float  t = 0.0f;
    return GetDistancePointToLine(point, line, h, t);
}

float math::GetDistancePointToLine          (const VECTOR&      point,      const Line&     line,       VECTOR& h, float& t)
{
    const VECTOR v = line.GetDir();
    t = 0.0f;

    if (VDot(v, v) > 0.0f)
    {
        t = VDot(v, (point - line.GetPos())) / VDot(v, v);
    }

    h = line.GetPos() + v * t;
    return VSize(h - point);
}

float math::GetDistancePointToSegment       (const VECTOR&      point,      const Segment&  segment)
{
    VECTOR h = v3d::GetZeroV();
    float  t = 0.0f;
    return GetDistancePointToSegment(point, segment, h, t);
}

float math::GetDistancePointToSegment       (const VECTOR&      point,      const Segment&  segment,    VECTOR& h, float& t)
{
    // MEMO : 点が線分の端点から伸びる垂線の内側にある ➡ 点と直線の最短距離を求める
    //                                      外側にある ➡ 端点までの距離を求める

    const VECTOR seg_dir    = segment.GetEndPos() - segment.GetBeginPos();
    const Line   line       = Line(segment.GetBeginPos(), seg_dir);
    const float  distance   = GetDistancePointToLine(point, line, h, t);

    const VECTOR a = segment.GetBeginPos() - point;
    const VECTOR b = segment.GetBeginPos() - segment.GetEndPos();
    const VECTOR c = segment.GetEndPos() - point;
    const VECTOR d = segment.GetEndPos() - segment.GetBeginPos();

    // 始点側の外側であった場合
    if (!math::IsAcuteAngle(a, b))
    {
        h = segment.GetBeginPos();
        return VSize(h - point);
    }

    // 終点側の外側であった場合
    if (!math::IsAcuteAngle(c, d))
    {
        h = segment.GetEndPos();
        return VSize(h - point);
    }

    // 内側であった場合
    return distance;
}

float math::GetDistancePointToPlane         (const VECTOR&      point,      const Plane&    plane)
{
    const VECTOR v1 = point - plane.GetPos();
    return std::abs(VDot(plane.GetNormalVector(), v1)) / VSize(plane.GetNormalVector());
}

float math::GetDistancePointToTriangle      (const VECTOR&      point,      const Triangle& triangle)
{
    return Triangle_Point_MinLength(triangle.GetPos(0), triangle.GetPos(1), triangle.GetPos(2), point);
}

float math::GetDistancePointToSquare        (const VECTOR&      point,      const Square&   square)
{
    const float distance1 = GetDistancePointToTriangle(point, square.GetTriangle(0));
    const float distance2 = GetDistancePointToTriangle(point, square.GetTriangle(1));

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceLineToLine           (const Line&        line1,      const Line&     line2)
{
    VECTOR h1 = v3d::GetZeroV();
    VECTOR h2 = v3d::GetZeroV();
    float  t1 = 0.0f;
    float  t2 = 0.0f;
    return GetDistanceLineToLine(line1, line2, h1, h2, t1, t2);
}

float math::GetDistanceLineToLine           (const Line&        line1,      const Line&     line2,      VECTOR& h1, VECTOR& h2, float& t1, float& t2)
{
    // 平行であった場合
    if (math::IsHorizontal(line1.GetDir(), line2.GetDir()))
    {
        h1 = line1.GetPos();
        t1 = 0.0f;
        return GetDistancePointToLine(line1.GetPos(), line2, h2, t2);
    }

    // ねじれ関係であった場合
    const float  dot_v1v1 = VDot(line1.GetDir(), line1.GetDir());
    const float  dot_v1v2 = VDot(line1.GetDir(), line2.GetDir());
    const float  dot_v2v2 = VDot(line2.GetDir(), line2.GetDir());
    const VECTOR p21p11   = VSub(line1.GetPos(), line2.GetPos());

    t1 = (dot_v1v2 * VDot(line2.GetDir(), p21p11) - dot_v2v2 * VDot(line1.GetDir(), p21p11))
        / (dot_v1v1 * dot_v2v2 - dot_v1v2 * dot_v1v2);
    h1 = line1.GetPoint(t1);
    t2 = VDot(line2.GetDir(), h1 - line2.GetPos()) / dot_v2v2;
    h2 = line2.GetPoint(t2);

    return VSize(h2 - h1);
}

float math::GetDistanceSegmentToSegment     (const Segment&     segment1,   const Segment&  segment2)
{
    VECTOR h1 = v3d::GetZeroV();
    VECTOR h2 = v3d::GetZeroV();
    float  t1 = 0.0f;
    float  t2 = 0.0f;
    return GetDistanceSegmentToSegment(segment1, segment2, h1, h2, t1, t2);
}

float math::GetDistanceSegmentToSegment     (const Segment&     segment1,   const Segment&  segment2,   VECTOR& h1, VECTOR& h2, float& t1, float& t2)
{
    // どちらかの線分が縮退している場合
    if (segment1.GetLength() <= 0.0f || segment2.GetLength() <= 0.0f)
    {
        // どちらも縮退している場合
        if (segment1.GetLength() <= 0.0f && segment2.GetLength() <= 0.0f)
        {
            const float distance = VSize(segment2.GetBeginPos() - segment1.GetBeginPos());
            h1 = segment1.GetBeginPos();
            h2 = segment2.GetBeginPos();
            t1 = t2 = 0.0f;
            return distance;
        }

        // 線分1が縮退している場合
        if (segment1.GetLength() <= 0.0f)
        {
            const float distance = GetDistancePointToSegment(segment1.GetBeginPos(), segment2, h2, t2);
            h1 = segment1.GetBeginPos();
            t1 = 0.0f;
            t2 = std::clamp(t2, 0.0f, segment2.GetLength());
            return distance;
        }

        // 線分2が縮退している場合
        const float distance = GetDistancePointToSegment(segment2.GetBeginPos(), segment1, h1, t1);
        h2 = segment2.GetBeginPos();
        t2 = 0.0f;
        t1 = std::clamp(t1, 0.0f, segment1.GetLength());
        return distance;
    }

    // 平行であった場合
    if (math::IsHorizontal(segment1.GetDir(), segment2.GetDir()))
    {
        float distance = GetDistancePointToSegment(segment1.GetBeginPos(), segment2, h2, t2);
        h1 = segment1.GetBeginPos();
        t1 = 0.0f;
        if (t2 >= 0 && t2 <= segment2.GetLength())
        {
            return distance;
        }
    }
    // ねじれ関係であった場合
    else
    {
        Line line1 = Line(segment1.GetBeginPos(), segment1.GetDir());
        Line line2 = Line(segment2.GetBeginPos(), segment2.GetDir());
        float distance = GetDistanceLineToLine(line1, line2, h1, h2, t1, t2);
        if ((t1 >= 0.0f && t1 <= segment1.GetLength()) && (t2 >= 0.0f && t2 <= segment2.GetLength()))
        {
            return distance;
        }
    }

    // 垂線の足が外にある場合
    // 線分1をクランプ
    t1 = std::clamp(t1, 0.0f, segment1.GetLength());
    h1 = segment1.GetPoint(t1);
    float distance = GetDistancePointToSegment(h1, segment2, h2, t2);
    if (t2 >= 0.0f && t2 <= segment2.GetLength())
    {
        return distance;
    }

    // 線分2側が外であったため、線分2をクランプ
    t2 = std::clamp(t2, 0.0f, segment2.GetLength());
    h2 = segment2.GetPoint(t2);
    distance = GetDistancePointToSegment(h2, segment1, h1, t1);
    if (t1 >= 0.0f && t1 <= segment1.GetLength())
    {
        return distance;
    }

    // お互いの端点が最短であった場合
    t1 = std::clamp(t1, 0.0f, segment1.GetLength());
    h1 = segment1.GetPoint(t1);
    return VSize(h2 - h1);
}

float math::GetDistanceSegmentToPlane       (const Segment&     segment,    const Plane&    plane)
{
    const VECTOR v1 = plane.GetPos() - segment.GetBeginPos();
    const VECTOR v2 = plane.GetPos() - segment.GetEndPos();

    if (VDot(v1, plane.GetNormalVector()) * VDot(v2, plane.GetNormalVector()) <= 0)
    {
        return 0.0f;
    }

    const float distance1 = GetDistancePointToPlane(segment.GetBeginPos(), plane);
    const float distance2 = GetDistancePointToPlane(segment.GetEndPos(), plane);

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceSegmentToTriangle    (const Segment&     segment,    const Triangle& triangle)
{
    return Segment_Triangle_MinLength(segment.GetBeginPos(), segment.GetEndPos(),
        triangle.GetPos(0), triangle.GetPos(1), triangle.GetPos(2));
}

float math::GetDistanceSegmentToSquare      (const Segment&     segment,    const Square&   square)
{
    const float distance1 = GetDistanceSegmentToTriangle(segment, square.GetTriangle(0));
    const float distance2 = GetDistanceSegmentToTriangle(segment, square.GetTriangle(1));

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistancePlaneToCapsule       (const Plane&       plane,      const Capsule&  capsule)
{
    const float distance = GetDistanceSegmentToPlane(capsule.GetSegment(), plane) - capsule.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}

float math::GetDistanceTriangleToTriangle   (const Triangle&    triangle1,  const Triangle& triangle2)
{
    return Triangle_Triangle_MinLength(
        triangle1.GetPos(0), triangle1.GetPos(1), triangle1.GetPos(2),
        triangle2.GetPos(0), triangle2.GetPos(1), triangle2.GetPos(2));
}

float math::GetDistanceTriangleToSquare     (const Triangle&    triangle,   const Square&   square)
{
    const float distance1 = GetDistanceTriangleToTriangle(triangle, square.GetTriangle(0));
    const float distance2 = GetDistanceTriangleToTriangle(triangle, square.GetTriangle(1));

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceTriangleToSphere(const Triangle& triangle, const Sphere& sphere)
{
    const float distance = GetDistancePointToTriangle(sphere.GetPos(), triangle) - sphere.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}

float math::GetDistanceTriangleToCapsule(const Triangle& triangle, const Capsule& capsule)
{
    const float distance = GetDistanceSegmentToTriangle(capsule.GetSegment(), triangle) - capsule.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}

float math::GetDistanceSquareToSquare       (const Square&      square1,    const Square&   square2)
{
    const float distance1 = GetDistanceTriangleToSquare(square1.GetTriangle(0), square2);
    const float distance2 = GetDistanceTriangleToSquare(square1.GetTriangle(1), square2);

    return distance1 < distance2 ? distance1 : distance2;
}

float math::GetDistanceSquareToCapsule      (const Square&      square,     const Capsule&  capsule)
{
    const float distance = GetDistanceSegmentToSquare(capsule.GetSegment(), square) - capsule.GetRadius();
    return distance < 0.0f ? 0.0f : distance;
}
#pragma endregion
