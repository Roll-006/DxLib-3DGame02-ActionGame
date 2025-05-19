#include "collision_calculator.hpp"

#pragma region 衝突判定
bool CollisionCalculator::IsHitPointAndLine(const VECTOR& point, const Line* line)
{
    return math::GetDistancePointToLine(point, line) <= 0.0f;
}

bool CollisionCalculator::IsHitPointAndSegment(const VECTOR& point, const Segment* segment)
{
    return math::GetDistancePointToSegment(point, segment) <= 0.0f;
}

bool CollisionCalculator::IsHitPointAndPlane(const VECTOR& point, const Plane* plane)
{
    return math::GetDistancePointToPlane(point, plane) == 0.0f;
}

bool CollisionCalculator::IsHitPointAndTriangle(const VECTOR& point, const Triangle* triangle)
{
    Plane plane = Plane(triangle->GetCentroid(), triangle->GetNormalVector(), 0);
    if (!IsHitPointAndPlane(point, &plane)) { return false; }

    VECTOR v1 = triangle->GetPosition(0) - triangle->GetPosition(1);
    VECTOR v2 = triangle->GetPosition(1) - triangle->GetPosition(2);
    VECTOR v3 = triangle->GetPosition(2) - triangle->GetPosition(0);

    VECTOR n_v1 = math::GetNormalVector(v1, point - triangle->GetPosition(1));
    VECTOR n_v2 = math::GetNormalVector(v2, point - triangle->GetPosition(2));
    VECTOR n_v3 = math::GetNormalVector(v3, point - triangle->GetPosition(0));

    return (triangle->GetNormalVector() != n_v1
        && triangle->GetNormalVector() != n_v2
        && triangle->GetNormalVector() != n_v3);
}

bool CollisionCalculator::IsHitPointAndSquare(const VECTOR& point, const Square* square)
{
    return IsHitPointAndTriangle(point, square->GetTriangle(0)) || IsHitPointAndTriangle(point, square->GetTriangle(1));
}

bool CollisionCalculator::IsHitPointAndCapsule(const VECTOR& point, const Capsule* capsule)
{
    float distance = math::GetDistancePointToSegment(point, capsule->GetSegment());
    return distance <= capsule->GetRadius();
}

bool CollisionCalculator::IsHitLineAndLine(const Line* line1, const Line* line2)
{
    return math::GetDistanceLineToLine(line1, line2) <= 0.0f;
}

bool CollisionCalculator::IsHitLineAndPlane(const Line* line, const Plane* plane)
{
    VECTOR v = line->GetPosition() - plane->GetPosition();

    bool is_vertical = math::IsVertical(line->GetDirection(), plane->GetNormalVector());
    bool is_contain_plane = math::IsVertical(v, plane->GetNormalVector());

    return is_contain_plane || !is_vertical;
}

bool CollisionCalculator::IsHitSegmentAndSegment(const Segment* segment1, const Segment* segment2)
{
    return math::GetDistanceSegmentToSegment(segment1, segment2) <= 0.0f;
}

bool CollisionCalculator::IsHitSegmentAndPlane(const Segment* segment, const Plane* plane)
{
    return math::GetDistanceSegmentToPlane(segment, plane) == 0.0f;
}

bool CollisionCalculator::IsHitSegmentAndTriangle(const Segment* segment, const Triangle* triangle)
{
    Plane plane = Plane(triangle->GetCentroid(), triangle->GetNormalVector(), 0);
    if (!IsHitSegmentAndPlane(segment, &plane)) { return false; }

    VECTOR v1 = segment->GetBeginPosition() - triangle->GetPosition(0);
    VECTOR v2 = segment->GetEndPosition() - triangle->GetPosition(0);
    float d1 = math::GetDistancePointToPlane(segment->GetBeginPosition(), &plane);
    float d2 = math::GetDistancePointToPlane(segment->GetEndPosition(), &plane);
    float a = d1 / (d1 + d2);
    VECTOR v3 = (1 - a) * v1 + a * v2;

    // 貫通点
    VECTOR p3 = triangle->GetPosition(0) + v3;

    // 貫通点が三角形と衝突しているか
    return IsHitPointAndTriangle(p3, triangle);
}

bool CollisionCalculator::IsHitSegmentAndSquare(const Segment* segment, const Square* square)
{
    return IsHitSegmentAndTriangle(segment, square->GetTriangle(0)) || IsHitSegmentAndTriangle(
        segment, square->GetTriangle(1));
}

bool CollisionCalculator::IsHitSegmentAndCircumference(const Segment* segment, const Circle* circle)
{
    Plane plane = Plane(circle->GetPosition(), circle->GetNormalVector(), 0); // 円がある平面
    auto intersection = math::GetIntersectionSegmentAndPlane(segment, &plane); // 交点

    // 線分が平面と交点を持つ場合
    if (intersection)
    {
        float minus_nan_ind = intersection->x;

        // 一点で交わっている場合
        if (std::isfinite(minus_nan_ind))
        {
            float distance = VSize(*intersection - circle->GetPosition());

            if (distance == circle->GetRadius())
            {
                return true;
            }
        }
        // 線分と平面が同一平面上にある場合
        else
        {
            float begin_distance = VSize(segment->GetBeginPosition() - circle->GetPosition());
            float end_distance = VSize(segment->GetEndPosition() - circle->GetPosition());

            // 始点から円の中心の距離と、終点から円の中心の距離が、半径より短い場合 ➡ 非衝突
            if (begin_distance < circle->GetRadius() && end_distance < circle->GetRadius())
            {
                return false;
            }

            // どちらか片方の距離が半径より短く、もう片方が半径より長い場合 ➡ 衝突
            if ((begin_distance < circle->GetRadius() && end_distance > circle->GetRadius())
                || (begin_distance > circle->GetRadius() && end_distance < circle->GetRadius()))
            {
                return true;
            }

            // 始点と終点がともに円の外にある場合
            // 線分と円の中心の最短距離が、半径以下の場合 ➡ 衝突
            if (math::GetDistancePointToSegment(circle->GetPosition(), segment) <= circle->GetRadius())
            {
                return true;
            }
            return false;
        }
    }
    return false;
}

bool CollisionCalculator::IsHitSegmentAndCapsule(const Segment* segment, const Capsule* capsule)
{
    float distance = math::GetDistanceSegmentToSegment(segment, capsule->GetSegment());
    return distance <= capsule->GetRadius();
}

bool CollisionCalculator::IsHitPlaneAndCapsule(const Plane* plane, const Capsule* capsule)
{
    return math::GetDistancePlaneToCapsule(plane, capsule) == 0.0f;
}

bool CollisionCalculator::IsHitTriangleAndCapsule(const Triangle* triangle, const Capsule* capsule)
{
    return math::GetDistanceSegmentToTriangle(capsule->GetSegment(), triangle) <= capsule->GetRadius();
}

bool CollisionCalculator::IsHitSquareAndCapsule(const Square* square, const Capsule* capsule)
{
    float distance1 = math::GetDistanceSegmentToTriangle(capsule->GetSegment(), square->GetTriangle(0));
    float distance2 = math::GetDistanceSegmentToTriangle(capsule->GetSegment(), square->GetTriangle(1));

    return distance1 <= capsule->GetRadius() || distance2 <= capsule->GetRadius();
}

bool CollisionCalculator::IsHitSphereAndSphere(const Sphere* sphere1, const Sphere* sphere2)
{
    VECTOR p1 = sphere1->GetPosition();
    VECTOR p2 = sphere2->GetPosition();
    float distance = VSize(p2 - p1);

    return distance <= sphere1->GetRadius() + sphere2->GetRadius();
}

bool CollisionCalculator::IsHitSphereAndCapsule(const Sphere* sphere, const Capsule* capsule)
{
    VECTOR p = sphere->GetPosition();
    float distance = math::GetDistancePointToSegment(p, capsule->GetSegment());

    return distance <= sphere->GetRadius() + capsule->GetRadius();
}

bool CollisionCalculator::IsHitCapsuleAndCapsule(const Capsule* capsule1, const Capsule* capsule2)
{
    float distance = math::GetDistanceSegmentToSegment(capsule1->GetSegment(), capsule2->GetSegment());
    return distance <= capsule1->GetRadius() + capsule2->GetRadius();
}

bool CollisionCalculator::IsHitCircumferenceAndCapsuleLowPrecision(const Circle* circle, const Capsule* capsule)
{
    Plane plane = Plane(circle->GetPosition(), circle->GetNormalVector(), 0);

    if (!IsHitPlaneAndCapsule(&plane, capsule)) { return false; }

    // 始点から平面の最短距離座標を取得
    float distance1 = math::GetDistancePointToPlane(capsule->GetSegment()->GetBeginPosition(), &plane);
    VECTOR v = plane.GetNormalVector() * distance1;
    if (math::IsPointAheadOfPlane(capsule->GetSegment()->GetBeginPosition(), &plane))
    {
        v *= -1;
    }
    VECTOR p = capsule->GetSegment()->GetBeginPosition() + v;

    // 斜辺の長さを取得
    VECTOR hypotenuse_direction = capsule->GetSegment()->GetDirection();
    float angle1 = math::GetAngleBetweenTwoVector(capsule->GetSegment()->GetDirection(), v);
    float angle2 = math::GetAngleBetweenTwoVector(-capsule->GetSegment()->GetDirection(), v);
    float hypotenuse_length = distance1 / cos(angle1);
    if (angle1 > angle2)
    {
        hypotenuse_length = distance1 / cos(angle2);
        hypotenuse_direction *= -1;
    }

    // TODO : 欠陥あり
    // カプセルの上部が少しだけ平面に衝突した場合や、カプセルが傾いて衝突した場合に計算がずれる
    // 交点を取得し、半径を考慮し衝突判定を行う
    VECTOR intersection = capsule->GetSegment()->GetBeginPosition() + hypotenuse_direction * hypotenuse_length;
    float distance2 = VSize(circle->GetPosition() - intersection);

    return distance2 >= circle->GetRadius() - capsule->GetRadius()
        && distance2 <= circle->GetRadius() + capsule->GetRadius();
}
#pragma endregion


#pragma region 壁ずり
VECTOR CollisionCalculator::GetWallSlideVector(const VECTOR& velocity, const VECTOR& extra_velocity,
                                               const VECTOR& wall_normal_vector)
{
    /*VECTOR push_back_v = extra_velocity;
    MATRIX matrixY = MGetRotY(DX_PI_F / 2.0f);
    MATRIX matrixZ = MGetRotZ(DX_PI_F / 2.0f);
    VECTOR parallel_plane_vector = VTransform(push_back_v, matrix);
    float vector1_abs = VSize(parallel_plane_vector);
    if (vector1_abs <= 0)
    {
        vector1_abs = 1.0f;
    }

    const float multiply = VDot(parallel_plane_vector, push_back_v) / (vector1_abs * vector1_abs);

    VECTOR push_buck_v = VScale(push_back_v, multiply);
    //push_back_v = VDot(push_back_v, math::GetUnitVector(wall_normal_vector)) * math::GetUnitVector(wall_normal_vector);

    return VAdd(velocity ,push_buck_v);*/

    VECTOR push_back_v = extra_velocity;
    push_back_v = VDot(push_back_v, math::GetUnitVector(wall_normal_vector)) * math::GetUnitVector(wall_normal_vector);

    return velocity + push_back_v;
}
#pragma endregion


#pragma region 押し戻し(衝突時の有効な速度ベクトルを取得)
VECTOR CollisionCalculator::GetValidVelocityAfterHitCapsuleAndSquare(const VECTOR& velocity,
                                                                     const Capsule* dynamic_capsule,
                                                                     const Square* static_square, bool& is_hit)
{
    // 未来の座標が衝突しているかを判定
    Capsule tmp_capsule = *dynamic_capsule;
    VECTOR future_position = tmp_capsule.GetSegment()->GetBeginPosition();
    future_position += velocity;
    tmp_capsule.Update(future_position - tmp_capsule.GetSegment()->GetOffset());
    if (!IsHitSquareAndCapsule(static_square, &tmp_capsule))
    {
        is_hit = false;
        return velocity;
    }

    is_hit = true;

    // TODO : 辺と接触した場合の処理を行う
    //if (IsHitSegmentAndCapsule(static_square->GetEdge(0), dynamic_capsule))
    //{

    //}

    // 始点と平面の前後関係を調べ、押し戻す方向を決める22
    Plane plane = Plane(static_square->GetCentroid(), static_square->GetNormalVector(), 0);

    VECTOR next_pos = tmp_capsule.GetSegment()->GetBeginPosition();
    float next_to_plane_distance = math::GetDistancePointToPlane(next_pos, &plane);

    //プレイヤーの位置からどちら側に位置修正するべきか
    VECTOR closest_dir = plane.GetNormalVector();
    if (math::IsPointAheadOfPlane(next_pos, &plane))
    {
        closest_dir = -closest_dir;
    }

    //位置修正
    next_pos += closest_dir * (next_to_plane_distance);
    next_pos += plane.GetNormalVector() * dynamic_capsule->GetRadius();
    return next_pos - dynamic_capsule->GetSegment()->GetBeginPosition();
}

VECTOR CollisionCalculator::GetValidVelocityAfterHitCapsuleAndOBB(const VECTOR& velocity,
                                                                  const Capsule* dynamic_capsule, const OBB* static_obb,
                                                                  bool& is_hit)
{
    std::array<bool, 6> tmp_is_hit;
    VECTOR valid_velocity = velocity;
    is_hit = false;

    for (int i = 0; i < BoxData::kPlaneNum; ++i)
    {
        valid_velocity = GetValidVelocityAfterHitCapsuleAndSquare(valid_velocity, dynamic_capsule,
                                                                  static_obb->GetPlane(
                                                                      static_cast<BoxData::PlaneKind>(i)),
                                                                  tmp_is_hit.at(i));
    }

    for (auto& hit : tmp_is_hit)
    {
        if (hit)
        {
            is_hit = true;
            break;
        }
    }

    return valid_velocity;
}
#pragma endregion
