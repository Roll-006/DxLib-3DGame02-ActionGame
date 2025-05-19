#include "../Data/IncludeList/shape.hpp"
#include "math.hpp"

#pragma region 変換
MATRIX math::ConvertQuaternionToMatrix(const Quaternion& q)
{
	MATRIX mat = MGetIdent();

	//X軸
	mat.m[0][0] = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
	mat.m[0][1] =        2.0f * q.x * q.y + 2.0f * q.w * q.z;
	mat.m[0][2] =        2.0f * q.x * q.z - 2.0f * q.w * q.y;

	//Y軸
	mat.m[1][0] =        2.0f * q.x * q.y - 2.0f * q.w * q.z;
	mat.m[1][1] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
	mat.m[1][2] =        2.0f * q.y * q.z + 2.0f * q.w * q.x;

	//Z軸
	mat.m[2][0] =        2.0f * q.x * q.z + 2.0f * q.w * q.y;
	mat.m[2][1] =        2.0f * q.y * q.z - 2.0f * q.w * q.x;
	mat.m[2][2] = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;

	// 計算後に転置し元に戻す
	return MTranspose(mat);
}

Quaternion math::ConvertMatrixToQuaternion(const MATRIX& mat)
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
bool math::IsHorizontal(const VECTOR& vector1, const VECTOR& vector2)
{
    VECTOR cross = VCross(vector1, vector2);

    // 小数点第5位より後は誤差とみなして四捨五入
    cross = RoundOff(cross, -5);
    return cross == v3d::GetZeroVector();
}

bool math::IsVertical(const VECTOR& vector1, const VECTOR& vector2)
{
    float dot = VDot(vector1, vector2);

    // 小数点第5位より後は誤差とみなして四捨五入
    dot = RoundOff(dot, -5);
    return dot == 0.0f;
}

VECTOR math::GetNormalVector(const VECTOR& vector)
{
    VECTOR normal_v = v3d::GetZeroVector();

    if (vector != VGet(1, 0, 0))
    {
        normal_v = math::GetNormalVector(vector, VGet(1, 0, 0));
    }
    else if (vector != VGet(0, 1, 0))
    {
        normal_v = math::GetNormalVector(vector, VGet(0, 1, 0));
    }
    else if (vector != VGet(0, 0, 1))
    {
        normal_v = math::GetNormalVector(vector, VGet(0, 0, 1));
    }
    return v3d::GetNormalizedVector(normal_v);
}

VECTOR math::GetNormalVector(const VECTOR& vector1, const VECTOR& vector2)
{
    // [法線ベクトル = v1とv2の外積]から成分を取得
    return v3d::GetNormalizedVector(VCross(vector1, vector2));
}
#pragma endregion


#pragma region 角度
bool math::IsAcuteAngle(const VECTOR& vector1, const VECTOR& vector2)
{
    const float radian = VDot(vector1, vector2) / (VSize(vector1) * VSize(vector2));
    return (radian < 90.0f * kDegreesToRadian && radian > 0.0f);
}

bool math::IsAcuteAngle(const float radian)
{
    return (radian < 90.0f * kDegreesToRadian && radian > 0.0f);
}

float math::GetAngleBetweenTwoVector(const VECTOR& vector1, const VECTOR& vector2)
{
    if (vector1 == v3d::GetZeroVector() || vector2 == v3d::GetZeroVector()) { return 0.0f; }

    const float length1 = VSize(vector1);
    const float length2 = VSize(vector2);

    return acos(VDot(vector1, vector2) / (length1 * length2));
}
#pragma endregion


#pragma region 回転
VECTOR math::GetRotatedPos(const VECTOR& pos, const Quaternion& rotate_q)
{
	// 参考URL : [ https://zenn.dev/mebiusbox/books/132b654aa02124/viewer/2966c7 ]

	const Quaternion q_pos(pos.x, pos.y, pos.z, 0.0f);
	const Quaternion n_rotate_q = quat::GetNormalizedQuaternion(rotate_q);
	const Quaternion totated_q = n_rotate_q * q_pos * quat::GetInverseQuaternion(n_rotate_q);
	return VGet(totated_q.x, totated_q.y, totated_q.z);
}

float math::GetYaw(const VECTOR& vector)
{
    return atan2f(-vector.x, -vector.z);
}

VECTOR math::GetYawRotateVector(const VECTOR& vector)
{
    return VGet(0.0f, GetYaw(vector), 0.0f);
}
#pragma endregion


#pragma region 重心
VECTOR math::GetCentroidOfATriangle(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3)
{
    VECTOR centroid = v3d::GetZeroVector();
    centroid.x = (pos1.x + pos2.x + pos3.x) / 3.0f;
    centroid.y = (pos1.y + pos2.y + pos3.y) / 3.0f;
    centroid.z = (pos1.z + pos2.z + pos3.z) / 3.0f;
    return centroid;
}

VECTOR math::GetCentroidOfAQuadrilateral(const VECTOR& pos1, const VECTOR& pos2, const VECTOR& pos3, const VECTOR& pos4)
{
    const VECTOR centroid_triangle1 = GetCentroidOfATriangle(pos1, pos2, pos3);
    const VECTOR centroid_triangle2 = GetCentroidOfATriangle(pos3, pos4, pos1);
    const Line   line1(centroid_triangle1, v3d::GetNormalizedVector(centroid_triangle2 - centroid_triangle1));

    const VECTOR centroid_triangle3 = GetCentroidOfATriangle(pos2, pos3, pos4);
    const VECTOR centroid_triangle4 = GetCentroidOfATriangle(pos4, pos1, pos2);
    const Line   line2(centroid_triangle3, v3d::GetNormalizedVector(centroid_triangle4 - centroid_triangle3));

    // 四角形の時点でnulloptを返すことはないが、
    // 引数が正常でなかった場合nulloptである可能性があるため判定を行う
    const auto intersection = GetIntersectionLineAndLine(line1, line2);
    return intersection ? *intersection : centroid_triangle1;
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


#pragma region 最短距離
float math::GetDistancePointToLine          (const VECTOR&      point,      const Line&     line)
{
    VECTOR h = v3d::GetZeroVector();
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
    VECTOR h = v3d::GetZeroVector();
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
    VECTOR h1 = v3d::GetZeroVector();
    VECTOR h2 = v3d::GetZeroVector();
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
    VECTOR h1 = v3d::GetZeroVector();
    VECTOR h2 = v3d::GetZeroVector();
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
