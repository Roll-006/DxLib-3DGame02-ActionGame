#include "collision_calculator.hpp"

#pragma region 衝突判定
bool collision::IsHitPointAndLine       (const VECTOR&      point,      const Line&     line)
{
    return math::GetDistancePointToLine(point, line) <= 0.0f;
}

bool collision::IsHitPointAndSegment    (const VECTOR&      point,      const Segment&  segment)
{
    return math::GetDistancePointToSegment(point, segment) <= 0.0f;
}

bool collision::IsHitPointAndPlane      (const VECTOR&      point,      const Plane&    plane)
{
    return math::GetDistancePointToPlane(point, plane) == 0.0f;
}

bool collision::IsHitPointAndTriangle   (const VECTOR&      point,      const Triangle& triangle)
{
    const Plane plane = Plane(triangle.GetCentroid(), triangle.GetNormalVector());
    if (!IsHitPointAndPlane(point, plane)) { return false; }

    const VECTOR v1 = triangle.GetPos(0) - triangle.GetPos(1);
    const VECTOR v2 = triangle.GetPos(1) - triangle.GetPos(2);
    const VECTOR v3 = triangle.GetPos(2) - triangle.GetPos(0);

    const VECTOR n_v1 = math::GetNormalVector(v1, point - triangle.GetPos(1));
    const VECTOR n_v2 = math::GetNormalVector(v2, point - triangle.GetPos(2));
    const VECTOR n_v3 = math::GetNormalVector(v3, point - triangle.GetPos(0));

    return (triangle.GetNormalVector() != n_v1
         && triangle.GetNormalVector() != n_v2
         && triangle.GetNormalVector() != n_v3);
}

bool collision::IsHitPointAndSquare     (const VECTOR&      point,      const Square&   square)
{
    return IsHitPointAndTriangle(point, square.GetTriangle(0)) || IsHitPointAndTriangle(point, square.GetTriangle(1));
}

bool collision::IsHitPointAndCapsule    (const VECTOR&      point,      const Capsule&  capsule)
{
    const float distance = math::GetDistancePointToSegment(point, capsule.GetSegment());
    return distance <= capsule.GetRadius();
}

bool collision::IsHitLineAndLine        (const Line&        line1,      const Line&     line2)
{
    return math::GetDistanceLineToLine(line1, line2) <= 0.0f;
}

bool collision::IsHitLineAndPlane       (const Line&        line,       const Plane&    plane)
{
    const VECTOR v = line.GetPos() - plane.GetPos();

    const bool is_vertical = math::IsVertical(line.GetDir(), plane.GetNormalVector());
    const bool is_contain_plane = math::IsVertical(v, plane.GetNormalVector());

    return is_contain_plane || !is_vertical;
}

bool collision::IsHitSegmentAndSegment  (const Segment&     segment1,   const Segment&  segment2)
{
    return math::GetDistanceSegmentToSegment(segment1, segment2) <= 0.0f;
}

bool collision::IsHitSegmentAndPlane    (const Segment&     segment,    const Plane&    plane)
{
    return math::GetDistanceSegmentToPlane(segment, plane) == 0.0f;
}

bool collision::IsHitSegmentAndTriangle (const Segment&     segment,    const Triangle& triangle)
{
    const Plane plane = Plane(triangle.GetCentroid(), triangle.GetNormalVector());
    if (!IsHitSegmentAndPlane(segment, plane)) { return false; }

    const VECTOR v1 = segment.GetBeginPos() - triangle.GetPos(0);
    const VECTOR v2 = segment.GetEndPos() - triangle.GetPos(0);
    const float d1  = math::GetDistancePointToPlane(segment.GetBeginPos(), plane);
    const float d2  = math::GetDistancePointToPlane(segment.GetEndPos(), plane);
    const float a   = d1 / (d1 + d2);
    const VECTOR v3 = (1 - a) * v1 + a * v2;

    // 貫通点
    const VECTOR p3 = triangle.GetPos(0) + v3;

    // 貫通点が三角形と衝突しているか
    return IsHitPointAndTriangle(p3, triangle);
}

bool collision::IsHitSegmentAndSquare   (const Segment&     segment,    const Square&   square)
{
    return IsHitSegmentAndTriangle(segment, square.GetTriangle(0)) || IsHitSegmentAndTriangle(
        segment, square.GetTriangle(1));
}

//bool collision::IsHitSegmentAndCircumference(const Segment* segment, const Circle* circle)
//{
//    Plane plane = Plane(circle->GetPos(), circle->GetNormalVector(), 0); // 円がある平面
//    auto intersection = math::GetIntersectionSegmentAndPlane(segment, &plane); // 交点
//
//    // 線分が平面と交点を持つ場合
//    if (intersection)
//    {
//        float minus_nan_ind = intersection->x;
//
//        // 一点で交わっている場合
//        if (std::isfinite(minus_nan_ind))
//        {
//            float distance = VSize(*intersection - circle->GetPos());
//
//            if (distance == circle->GetRadius())
//            {
//                return true;
//            }
//        }
//        // 線分と平面が同一平面上にある場合
//        else
//        {
//            float begin_distance = VSize(segment->GetBeginPosition() - circle->GetPos());
//            float end_distance = VSize(segment->GetEndPosition() - circle->GetPos());
//
//            // 始点から円の中心の距離と、終点から円の中心の距離が、半径より短い場合 ➡ 非衝突
//            if (begin_distance < circle->GetRadius() && end_distance < circle->GetRadius())
//            {
//                return false;
//            }
//
//            // どちらか片方の距離が半径より短く、もう片方が半径より長い場合 ➡ 衝突
//            if ((begin_distance < circle->GetRadius() && end_distance > circle->GetRadius())
//                || (begin_distance > circle->GetRadius() && end_distance < circle->GetRadius()))
//            {
//                return true;
//            }
//
//            // 始点と終点がともに円の外にある場合
//            // 線分と円の中心の最短距離が、半径以下の場合 ➡ 衝突
//            if (math::GetDistancePointToSegment(circle->GetPos(), segment) <= circle->GetRadius())
//            {
//                return true;
//            }
//            return false;
//        }
//    }
//    return false;
//}

bool collision::IsHitSegmentAndCapsule  (const Segment&     segment,    const Capsule&  capsule)
{
    const float distance = math::GetDistanceSegmentToSegment(segment, capsule.GetSegment());
    return distance <= capsule.GetRadius();
}

bool collision::IsHitPlaneAndCapsule    (const Plane&       plane,      const Capsule&  capsule)
{
    return math::GetDistancePlaneToCapsule(plane, capsule) == 0.0f;
}

bool collision::IsHitTriangleAndCapsule (const Triangle&    triangle,   const Capsule&  capsule)
{
    return math::GetDistanceSegmentToTriangle(capsule.GetSegment(), triangle) <= capsule.GetRadius();
}

bool collision::IsHitSquareAndCapsule   (const Square&      square,     const Capsule&  capsule)
{
    const float distance1 = math::GetDistanceSegmentToTriangle(capsule.GetSegment(), square.GetTriangle(0));
    const float distance2 = math::GetDistanceSegmentToTriangle(capsule.GetSegment(), square.GetTriangle(1));

    return distance1 <= capsule.GetRadius() || distance2 <= capsule.GetRadius();
}

bool collision::IsHitSphereAndSphere    (const Sphere&      sphere1,    const Sphere&   sphere2)
{
    const VECTOR p1 = sphere1.GetPos();
    const VECTOR p2 = sphere2.GetPos();
    const float distance = VSize(p2 - p1);

    return distance <= sphere1.GetRadius() + sphere2.GetRadius();
}

bool collision::IsHitSphereAndCapsule   (const Sphere&      sphere,     const Capsule&  capsule)
{
    const VECTOR p = sphere.GetPos();
    const float distance = math::GetDistancePointToSegment(p, capsule.GetSegment());

    return distance <= sphere.GetRadius() + capsule.GetRadius();
}

bool collision::IsHitCapsuleAndCapsule  (const Capsule&     capsule1,   const Capsule&  capsule2)
{
    const float distance = math::GetDistanceSegmentToSegment(capsule1.GetSegment(), capsule2.GetSegment());
    return distance <= capsule1.GetRadius() + capsule2.GetRadius();
}

//bool collision::IsHitCircumferenceAndCapsuleLowPrecision(const Circle* circle, const Capsule* capsule)
//{
//    Plane plane = Plane(circle->GetPos(), circle->GetNormalVector(), 0);
//
//    if (!IsHitPlaneAndCapsule(&plane, capsule)) { return false; }
//
//    // 始点から平面の最短距離座標を取得
//    float distance1 = math::GetDistancePointToPlane(capsule->GetSegment()->GetBeginPosition(), &plane);
//    VECTOR v = plane.GetNormalVector() * distance1;
//    if (math::IsPointAheadOfPlane(capsule->GetSegment()->GetBeginPosition(), &plane))
//    {
//        v *= -1;
//    }
//    VECTOR p = capsule->GetSegment()->GetBeginPosition() + v;
//
//    // 斜辺の長さを取得
//    VECTOR hypotenuse_direction = capsule->GetSegment()->GetDir();
//    float angle1 = math::GetAngleBetweenTwoVector(capsule->GetSegment()->GetDir(), v);
//    float angle2 = math::GetAngleBetweenTwoVector(-capsule->GetSegment()->GetDir(), v);
//    float hypotenuse_length = distance1 / cos(angle1);
//    if (angle1 > angle2)
//    {
//        hypotenuse_length = distance1 / cos(angle2);
//        hypotenuse_direction *= -1;
//    }
//
//    // TODO : 欠陥あり
//    // カプセルの上部が少しだけ平面に衝突した場合や、カプセルが傾いて衝突した場合に計算がずれる
//    // 交点を取得し、半径を考慮し衝突判定を行う
//    VECTOR intersection = capsule->GetSegment()->GetBeginPosition() + hypotenuse_direction * hypotenuse_length;
//    float distance2 = VSize(circle->GetPos() - intersection);
//
//    return distance2 >= circle->GetRadius() - capsule->GetRadius()
//        && distance2 <= circle->GetRadius() + capsule->GetRadius();
//}
#pragma endregion


#pragma region 押し戻し(衝突時の有効な速度ベクトルを取得)
VECTOR collision::GetValidVelocityAfterHitCapsuleAndSquare(const VECTOR& velocity, const Capsule& dynamic_capsule, const Square& static_square)
{
    // 未来の座標が衝突しているかを判定
    Capsule future_capsule = dynamic_capsule;
    future_capsule.Move(velocity);
    if (!IsHitSquareAndCapsule(static_square, future_capsule))
    {
        // 衝突していない場合、そのまま返す
        return velocity;
    }

    // 未来の座標と平面の距離を取得
    const Plane plane = Plane(static_square.GetCentroid(), static_square.GetNormalVector());
    VECTOR future_pos = future_capsule.GetSegment().GetBeginPos();
    const float next_to_plane_distance = math::GetDistancePointToPlane(future_pos, plane);

    // 線分の位置からどちら側に位置修正するべきか
    VECTOR closest_dir = plane.GetNormalVector();
    if (math::IsPointAheadOfPlane(future_pos, plane))
    {
        closest_dir *= -1;
    }

    // 有効velocityを取得
    future_pos += closest_dir * next_to_plane_distance;
    future_pos += plane.GetNormalVector() * dynamic_capsule.GetRadius();
    return future_pos - dynamic_capsule.GetSegment().GetBeginPos();
}

VECTOR collision::GetValidVelocityAfterHitCapsuleAndOBB(const VECTOR& velocity, const Capsule& dynamic_capsule, const OBB& static_obb)
{
    VECTOR valid_velocity = velocity;

    for (int i = 0; i < BoxData::kVertexNum; ++i)
    {
        valid_velocity = GetValidVelocityAfterHitCapsuleAndSquare(valid_velocity, dynamic_capsule, static_obb.GetSquare(static_cast<SquareKind>(i)));
    }
    return valid_velocity;
}
#pragma endregion
