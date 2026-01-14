#pragma once
#include <DxLib.h>

#include "../Path/frame_path.hpp"

class HumanoidFrameGetter final
{
public:
	#pragma region Index
	[[nodiscard]] int GetArmatureIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.ARMATURE); }
	
	[[nodiscard]] int GetHipsIndex				(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.HIPS); }
	[[nodiscard]] int GetSpineIndex				(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.SPINE); }
	[[nodiscard]] int GetSpine1Index			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.SPINE_1); }
	[[nodiscard]] int GetSpine2Index			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.SPINE_2); }
	[[nodiscard]] int GetNeckIndex				(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.NECK); }
	[[nodiscard]] int GetHeadIndex				(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.HEAD); }
	[[nodiscard]] int GetHeadTopEndIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.HEAD_TOP_END); }
	

	[[nodiscard]] int GetLeftShoulderIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_SHOULDER); }
	[[nodiscard]] int GetLeftArmIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_ARM); }
	[[nodiscard]] int GetLeftForeArmIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_FORE_ARM); }
	[[nodiscard]] int GetLeftHandIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND); }
	
	[[nodiscard]] int GetLeftHandThumb1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_THUMB_1); }
	[[nodiscard]] int GetLeftHandThumb2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_THUMB_2); }
	[[nodiscard]] int GetLeftHandThumb3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_THUMB_3); }
	[[nodiscard]] int GetLeftHandThumb4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_THUMB_4); }
	
	[[nodiscard]] int GetLeftHandIndex1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_INDEX_1); }
	[[nodiscard]] int GetLeftHandIndex2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_INDEX_2); }
	[[nodiscard]] int GetLeftHandIndex3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_INDEX_3); }
	[[nodiscard]] int GetLeftHandIndex4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_INDEX_4); }
	
	[[nodiscard]] int GetLeftHandMiddle1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_MIDDLE_1); }
	[[nodiscard]] int GetLeftHandMiddle2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_MIDDLE_2); }
	[[nodiscard]] int GetLeftHandMiddle3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_MIDDLE_3); }
	[[nodiscard]] int GetLeftHandMiddle4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_MIDDLE_4); }
	
	[[nodiscard]] int GetLeftHandRing1Index		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_RING_1); }
	[[nodiscard]] int GetLeftHandRing2Index		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_RING_2); }
	[[nodiscard]] int GetLeftHandRing3Index		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_RING_3); }
	[[nodiscard]] int GetLeftHandRing4Index		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_RING_4); }
	
	[[nodiscard]] int GetLeftHandPinky1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_PINKY_1); }
	[[nodiscard]] int GetLeftHandPinky2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_PINKY_2); }
	[[nodiscard]] int GetLeftHandPinky3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_PINKY_3); }
	[[nodiscard]] int GetLeftHandPinky4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_HAND_PINKY_4); }
	
	[[nodiscard]] int GetRightShoulderIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_SHOULDER); }
	[[nodiscard]] int GetRightArmIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_ARM); }
	[[nodiscard]] int GetRightForeArmIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_FORE_ARM); }
	[[nodiscard]] int GetRightHandIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND); }

	[[nodiscard]] int GetRightHandThumb1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_THUMB_1); }
	[[nodiscard]] int GetRightHandThumb2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_THUMB_2); }
	[[nodiscard]] int GetRightHandThumb3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_THUMB_3); }
	[[nodiscard]] int GetRightHandThumb4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_THUMB_4); }
	
	[[nodiscard]] int GetRightHandIndex1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_INDEX_1); }
	[[nodiscard]] int GetRightHandIndex2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_INDEX_2); }
	[[nodiscard]] int GetRightHandIndex3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_INDEX_3); }
	[[nodiscard]] int GetRightHandIndex4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_INDEX_4); }
	
	[[nodiscard]] int GetRightHandMiddle1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_MIDDLE_1); }
	[[nodiscard]] int GetRightHandMiddle2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_MIDDLE_2); }
	[[nodiscard]] int GetRightHandMiddle3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_MIDDLE_3); }
	[[nodiscard]] int GetRightHandMiddle4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_MIDDLE_4); }
	
	[[nodiscard]] int GetRightHandRing1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_RING_1); }
	[[nodiscard]] int GetRightHandRing2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_RING_2); }
	[[nodiscard]] int GetRightHandRing3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_RING_3); }
	[[nodiscard]] int GetRightHandRing4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_RING_4); }
	
	[[nodiscard]] int GetRightHandPinky1Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_PINKY_1); }
	[[nodiscard]] int GetRightHandPinky2Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_PINKY_2); }
	[[nodiscard]] int GetRightHandPinky3Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_PINKY_3); }
	[[nodiscard]] int GetRightHandPinky4Index	(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_HAND_PINKY_4); }
	

	[[nodiscard]] int GetLeftUpLegIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_UP_LEG); }
	[[nodiscard]] int GetLeftLegIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_LEG); }
	[[nodiscard]] int GetLeftFootIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_FOOT); }
	[[nodiscard]] int GetLeftToeBaseIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_TOE_BASE); }
	[[nodiscard]] int GetLeftToeEndIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.LEFT_TOE_END); }
	
	[[nodiscard]] int GetRightUpLegIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_UP_LEG); }
	[[nodiscard]] int GetRightLegIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_LEG); }
	[[nodiscard]] int GetRightFootIndex			(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_FOOT); }
	[[nodiscard]] int GetRightToeBaseIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_TOE_BASE); }
	[[nodiscard]] int GetRightToeEndIndex		(const int model_handle) const { return MV1SearchFrame(model_handle, FramePath.RIGHT_TOE_END); }
	#pragma endregion
};