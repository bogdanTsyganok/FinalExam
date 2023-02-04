#include "AnimationSystem.h"
#include <glm/gtx/easing.hpp>

void AnimationSystem::Process(const std::vector<AnimetedMesh>& entities, float dt)
{
	AnimetedMesh pAnimMesh;

	for (int i = 0; i < entities.size(); ++i)
	{
		pAnimMesh = entities[i];


		if (pAnimMesh.pAnim == 0 || pAnimMesh.pMesh == 0 )
			continue;

		if (!pAnimMesh.pAnim->playing)
			continue;


		pAnimMesh.pAnim->currentTime += dt * pAnimMesh.pAnim->speed;

		if (pAnimMesh.pAnim->currentTime > pAnimMesh.pAnim->duration || pAnimMesh.pAnim->currentTime < -0.1f)
		{
			if (pAnimMesh.pAnim->speed > 0)
				pAnimMesh.pAnim->currentTime = pAnimMesh.pAnim->duration;
			else
				pAnimMesh.pAnim->currentTime = 0;

			if (pAnimMesh.pAnim->repeat == true)
			{
				if (pAnimMesh.pAnim->speed > 0)
					pAnimMesh.pAnim->currentTime = 0;
				else
					pAnimMesh.pAnim->currentTime = pAnimMesh.pAnim->duration;
			}
			else
			{
				pAnimMesh.pAnim->playing = false;
				pAnimMesh.pMesh->bUseObjectDebugColour = false;
			}

			
		}

		// Find active keyframes
		if (pAnimMesh.pAnim->keyFramePositions.size() == 0
			|| pAnimMesh.pAnim->keyFrameScales.size() == 0
			|| pAnimMesh.pAnim->keyFrameRotations.size() == 0)
		{
			printf("Contains an empty keyframe vector");
			continue;
		}

		// Currently we are using only the current KeyFrame,
		// but we need to smoothly animate between two KeyFrames
		// If the current time is at 1.5, and two KeyFrames are at
		// 1, and 2, then we must find a point between the two 
		// positions
		// 
		//KeyFrame keyframe = FindCurrentKeyFrame(animationPtr, animationPtr->currentTime);
		//positionPtr->value = keyframe.position;

		// If there is only 1 KeyFrame in the animation, only use that
		if (pAnimMesh.pAnim->keyFramePositions.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			pAnimMesh.pMesh->positionXYZ = pAnimMesh.pAnim->keyFramePositions[0].position;
		}
		else
		{


			int currPosFrameidx = FindKeyFramePositionIndex(pAnimMesh.pAnim, pAnimMesh.pAnim->currentTime);

			// If we are at the last KeyFrame, only use that KeyFrame.
			if (currPosFrameidx == pAnimMesh.pAnim->keyFramePositions.size() - 1)
			{
				// We only need to set the position of the last KeyFrame.
				pAnimMesh.pMesh->positionXYZ = pAnimMesh.pAnim->keyFramePositions[currPosFrameidx].position;
			}
			else
			{
				/* UPDATE POSITIONS */

				// Find a position between the current KeyFrame, and the next KeyFrame
				int nextPosFrameidx = currPosFrameidx + 1;

				const KeyFramePosition& keyFramePos1 = pAnimMesh.pAnim->keyFramePositions[currPosFrameidx];
				const KeyFramePosition& keyFramePos2 = pAnimMesh.pAnim->keyFramePositions[nextPosFrameidx];

				// Look into glm's easing functions
				// 1. Ease-In
				// 2. Ease-Out
				// 3. Ease-InOut
				// glm::sineEaseIn()


				// As the animation time 
				// How would I change this to implement glm::sineEaseIn(x)?
				float posFraction = (pAnimMesh.pAnim->currentTime - keyFramePos1.time) / (keyFramePos2.time - keyFramePos1.time);
				if (posFraction < 0)
					posFraction = 0;

				switch (keyFramePos2.easingType)
				{
				case EaseIn:
					posFraction = glm::sineEaseIn(posFraction);
					pAnimMesh.pMesh->bUseObjectDebugColour = true;
					pAnimMesh.pMesh->objectDebugColourRGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
					break;
				case EaseOut:
					posFraction = glm::sineEaseOut(posFraction);
					pAnimMesh.pMesh->bUseObjectDebugColour = true;
					pAnimMesh.pMesh->objectDebugColourRGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);
					break;
				case EaseInOut:
					posFraction = glm::sineEaseInOut(posFraction);
					pAnimMesh.pMesh->bUseObjectDebugColour = true;
					pAnimMesh.pMesh->objectDebugColourRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
					break;
				default:
					pAnimMesh.pMesh->bUseObjectDebugColour = false;
					break;
				}
				pAnimMesh.pMesh->positionXYZ = keyFramePos1.position + (keyFramePos2.position - keyFramePos1.position) * posFraction;

				//printf("KeyFrame(%d -> %.2f -> %d) position: (%.2f, %.2f)\n",
				//	CurrentKeyFrameIndex, fraction, NextKeyFrameIndex, positionPtr->value.x, positionPtr->value.y);

				//positionPtr->value = (coords1 + coords2) / 2.f * animationPtr->currentTime;
			}
		}
		/* UPDATE SCALES*/

		if (pAnimMesh.pAnim->keyFrameScales.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.
			pAnimMesh.pMesh->scale = pAnimMesh.pAnim->keyFrameScales[0].scale;
		}
		else
		{
			int currScaleFrameidx = FindKeyFrameScaleIndex(pAnimMesh.pAnim, pAnimMesh.pAnim->currentTime);

			// If we are at the last KeyFrame, only use that KeyFrame.
			if (currScaleFrameidx == pAnimMesh.pAnim->keyFrameScales.size() - 1)
			{
				// We only need to set the position of the last KeyFrame.
				pAnimMesh.pMesh->scale = pAnimMesh.pAnim->keyFrameScales[0].scale;
			}
			else
			{
				int nextScaleFrameidx = currScaleFrameidx + 1;

				const KeyFrameScale& keyFrameScale1 = pAnimMesh.pAnim->keyFrameScales[currScaleFrameidx];
				const KeyFrameScale& keyFrameScale2 = pAnimMesh.pAnim->keyFrameScales[nextScaleFrameidx];

				float scaleFraction = (pAnimMesh.pAnim->currentTime - keyFrameScale1.time) / (keyFrameScale2.time - keyFrameScale1.time);
				if (scaleFraction < 0)
					scaleFraction = 0;
				switch (keyFrameScale2.easingType)
				{
				case EaseIn:
					scaleFraction = glm::sineEaseIn(scaleFraction);
					break;
				case EaseOut:
					scaleFraction = glm::sineEaseOut(scaleFraction);
					break;
				case EaseInOut:
					scaleFraction = glm::sineEaseInOut(scaleFraction);
					break;
				default:
					break;
				}
				pAnimMesh.pMesh->scale = (keyFrameScale1.scale + (keyFrameScale2.scale - keyFrameScale1.scale) * scaleFraction);
			}
		}
		/* UPDATE ROTATIONS*/
		if (pAnimMesh.pAnim->keyFrameRotations.size() == 1)
		{
			// We only need to set the position of the only KeyFrame.

			pAnimMesh.pMesh->orientationXYZ = glm::eulerAngles(pAnimMesh.pAnim->keyFrameRotations[0].rotation);
			
		}
		else
		{

			int currRotFrameidx = FindKeyFrameRotIndex(pAnimMesh.pAnim, pAnimMesh.pAnim->currentTime);

			// If we are at the last KeyFrame, only use that KeyFrame.
			if (currRotFrameidx == pAnimMesh.pAnim->keyFrameRotations.size() - 1)
			{
				// We only need to set the position of the last KeyFrame.
				pAnimMesh.pMesh->orientationXYZ = glm::eulerAngles(pAnimMesh.pAnim->keyFrameRotations[currRotFrameidx].rotation);
				
			}
			else
			{

				int nextRotFrameidx = currRotFrameidx + 1;

				const KeyFrameRotation& keyFrameRot1 = pAnimMesh.pAnim->keyFrameRotations[currRotFrameidx];
				const KeyFrameRotation& keyFrameRot2 = pAnimMesh.pAnim->keyFrameRotations[nextRotFrameidx];

				float rotationFraction = (pAnimMesh.pAnim->currentTime - keyFrameRot1.time) / (keyFrameRot2.time - keyFrameRot1.time);
				if (rotationFraction < 0)
					rotationFraction = 0;
				switch (keyFrameRot2.easingType)
				{
				case EaseIn:
					rotationFraction = glm::sineEaseIn(rotationFraction);
					break;
				case EaseOut:
					rotationFraction = glm::sineEaseOut(rotationFraction);
					break;
				case EaseInOut:
					rotationFraction = glm::sineEaseInOut(rotationFraction);
					break;
				default:
					break;
				}

				if (keyFrameRot2.InterpolationType == 1)
				{
					pAnimMesh.pMesh->orientationXYZ = glm::eulerAngles(glm::lerp(keyFrameRot1.rotation, keyFrameRot2.rotation, rotationFraction));
				}
				if (keyFrameRot2.InterpolationType == 2)
				{
					pAnimMesh.pMesh->orientationXYZ = glm::eulerAngles(glm::slerp(keyFrameRot1.rotation, keyFrameRot2.rotation, rotationFraction));
				}
				else
				{
					pAnimMesh.pMesh->orientationXYZ = glm::eulerAngles(keyFrameRot1.rotation + (keyFrameRot2.rotation - keyFrameRot1.rotation) * rotationFraction);
				}
			}
		}

		//  ____                     _    _           _       _       
		// |  _ \                   | |  | |         | |     | |      
		// | |_) | ___  _ __   ___  | |  | |_ __   __| | __ _| |_ ___ 
		// |  _ < / _ \| '_ \ / _ \ | |  | | '_ \ / _` |/ _` | __/ _ \
		// | |_) | (_) | | | |  __/ | |__| | |_) | (_| | (_| | ||  __/
		// |____/ \___/|_| |_|\___|  \____/| .__/ \__,_|\__,_|\__\___|
		//								   | |                        
		//								   |_|       

		if (!(pAnimMesh.pMesh->isRigged))
			continue;

		// Find active keyframes
		if (pAnimMesh.pAnim->keyFrameBonePosition.size() == 0
			|| pAnimMesh.pAnim->keyFrameBoneRotations.size() == 0
			|| pAnimMesh.pAnim->keyFrameBoneScale.size() == 0)
		{
			printf("Contains an empty bone keyframe vector");
			continue;
		}

		// Currently we are using only the current KeyFrame,
		// but we need to smoothly animate between two KeyFrames
		// If the current time is at 1.5, and two KeyFrames are at
		// 1, and 2, then we must find a point between the two 
		// positions
		// 
		//KeyFrame keyframe = FindCurrentKeyFrame(animationPtr, animationPtr->currentTime);
		//positionPtr->value = keyframe.position;

		// If there is only 1 KeyFrame in the animation, only use that

		for (int boneId = 0; boneId < 4; boneId++)
		{
			if (pAnimMesh.pAnim->keyFrameBonePosition.size() == 1)
			{
				// We only need to set the position of the only KeyFrame.
				pAnimMesh.pMesh->v_Bones[boneId]->SetPosition(pAnimMesh.pAnim->keyFrameBonePosition[0].positions[boneId]);
			}
			else
			{


				int currPosFrameidx = FindKeyFramePositionIndex(pAnimMesh.pAnim, pAnimMesh.pAnim->currentTime);

				// If we are at the last KeyFrame, only use that KeyFrame.
				if (currPosFrameidx == pAnimMesh.pAnim->keyFrameBonePosition.size() - 1)
				{
					// We only need to set the position of the last KeyFrame.
					pAnimMesh.pMesh->v_Bones[boneId]->SetPosition(pAnimMesh.pAnim->keyFrameBonePosition[currPosFrameidx].positions[boneId]);
				}
				else
				{
					/* UPDATE POSITIONS */

					// Find a position between the current KeyFrame, and the next KeyFrame
					int nextPosFrameidx = currPosFrameidx + 1;

					const KeyFrameBonePosition& keyFramePos1 = pAnimMesh.pAnim->keyFrameBonePosition[currPosFrameidx];
					const KeyFrameBonePosition& keyFramePos2 = pAnimMesh.pAnim->keyFrameBonePosition[nextPosFrameidx];

					// Look into glm's easing functions
					// 1. Ease-In
					// 2. Ease-Out
					// 3. Ease-InOut
					// glm::sineEaseIn()


					// As the animation time 
					// How would I change this to implement glm::sineEaseIn(x)?
					float posFraction = (pAnimMesh.pAnim->currentTime - keyFramePos1.time) / (keyFramePos2.time - keyFramePos1.time);
					if (posFraction < 0)
						posFraction = 0;

					switch (keyFramePos2.easingType)
					{
					case EaseIn:
						posFraction = glm::sineEaseIn(posFraction);
						pAnimMesh.pMesh->bUseObjectDebugColour = true;
						pAnimMesh.pMesh->objectDebugColourRGBA = glm::vec4(1.f, 0.f, 0.f, 1.f);
						break;
					case EaseOut:
						posFraction = glm::sineEaseOut(posFraction);
						pAnimMesh.pMesh->bUseObjectDebugColour = true;
						pAnimMesh.pMesh->objectDebugColourRGBA = glm::vec4(1.f, 1.f, 0.f, 1.f);
						break;
					case EaseInOut:
						posFraction = glm::sineEaseInOut(posFraction);
						pAnimMesh.pMesh->bUseObjectDebugColour = true;
						pAnimMesh.pMesh->objectDebugColourRGBA = glm::vec4(0.f, 1.f, 0.f, 1.f);
						break;
					default:
						pAnimMesh.pMesh->bUseObjectDebugColour = false;
						break;
					}
					pAnimMesh.pMesh->v_Bones[boneId]->SetPosition(keyFramePos1.positions[boneId] + (keyFramePos2.positions[boneId] - keyFramePos1.positions[boneId]) * posFraction);

					//printf("KeyFrame(%d -> %.2f -> %d) position: (%.2f, %.2f)\n",
					//	CurrentKeyFrameIndex, fraction, NextKeyFrameIndex, positionPtr->value.x, positionPtr->value.y);

					//positionPtr->value = (coords1 + coords2) / 2.f * animationPtr->currentTime;
				}
			}
			/* UPDATE SCALES*/

			if (pAnimMesh.pAnim->keyFrameScales.size() == 1)
			{
				// We only need to set the position of the only KeyFrame.
				pAnimMesh.pMesh->v_Bones[boneId]->SetScale(pAnimMesh.pAnim->keyFrameBoneScale[0].scales[boneId]);
			}
			else
			{
				int currScaleFrameidx = FindKeyFrameScaleIndex(pAnimMesh.pAnim, pAnimMesh.pAnim->currentTime);

				// If we are at the last KeyFrame, only use that KeyFrame.
				if (currScaleFrameidx == pAnimMesh.pAnim->keyFrameBoneScale.size() - 1)
				{
					// We only need to set the position of the last KeyFrame.
					pAnimMesh.pMesh->v_Bones[boneId]->SetScale(pAnimMesh.pAnim->keyFrameBoneScale[currScaleFrameidx].scales[boneId]);
				}
				else
				{
					int nextScaleFrameidx = currScaleFrameidx + 1;

					const KeyFrameBoneScale& keyFrameScale1 = pAnimMesh.pAnim->keyFrameBoneScale[currScaleFrameidx];
					const KeyFrameBoneScale& keyFrameScale2 = pAnimMesh.pAnim->keyFrameBoneScale[nextScaleFrameidx];

					float scaleFraction = (pAnimMesh.pAnim->currentTime - keyFrameScale1.time) / (keyFrameScale2.time - keyFrameScale1.time);
					if (scaleFraction < 0)
						scaleFraction = 0;
					switch (keyFrameScale2.easingType)
					{
					case EaseIn:
						scaleFraction = glm::sineEaseIn(scaleFraction);
						break;
					case EaseOut:
						scaleFraction = glm::sineEaseOut(scaleFraction);
						break;
					case EaseInOut:
						scaleFraction = glm::sineEaseInOut(scaleFraction);
						break;
					default:
						break;
					}
					pAnimMesh.pMesh->v_Bones[boneId]->SetScale((keyFrameScale1.scales[boneId] + (keyFrameScale2.scales[boneId] - keyFrameScale1.scales[boneId]) * scaleFraction));
				}
			}
			/* UPDATE ROTATIONS*/
			if (pAnimMesh.pAnim->keyFrameBoneRotations.size() == 1)
			{
				// We only need to set the position of the only KeyFrame.

				pAnimMesh.pMesh->v_Bones[boneId]->SetRotation(glm::eulerAngles(pAnimMesh.pAnim->keyFrameBoneRotations[0].rotations[boneId]));

			}
			else
			{

				int currRotFrameidx = FindKeyFrameRotIndex(pAnimMesh.pAnim, pAnimMesh.pAnim->currentTime);

				// If we are at the last KeyFrame, only use that KeyFrame.
				if (currRotFrameidx == pAnimMesh.pAnim->keyFrameBoneRotations.size() - 1)
				{
					// We only need to set the position of the last KeyFrame.
					pAnimMesh.pMesh->v_Bones[boneId]->SetRotation(glm::eulerAngles(pAnimMesh.pAnim->keyFrameBoneRotations[currRotFrameidx].rotations[boneId]));

				}
				else
				{

					int nextRotFrameidx = currRotFrameidx + 1;

					const KeyFrameBoneRotation& keyFrameRot1 = pAnimMesh.pAnim->keyFrameBoneRotations[currRotFrameidx];
					const KeyFrameBoneRotation& keyFrameRot2 = pAnimMesh.pAnim->keyFrameBoneRotations[nextRotFrameidx];

					float rotationFraction = (pAnimMesh.pAnim->currentTime - keyFrameRot1.time) / (keyFrameRot2.time - keyFrameRot1.time);
					if (rotationFraction < 0)
						rotationFraction = 0;
					switch (keyFrameRot2.easingType)
					{
					case EaseIn:
						rotationFraction = glm::sineEaseIn(rotationFraction);
						break;
					case EaseOut:
						rotationFraction = glm::sineEaseOut(rotationFraction);
						break;
					case EaseInOut:
						rotationFraction = glm::sineEaseInOut(rotationFraction);
						break;
					default:
						break;
					}

					if (keyFrameRot2.InterpolationType == 1)
					{
						pAnimMesh.pMesh->v_Bones[boneId]->SetRotation(glm::lerp(keyFrameRot1.rotations[boneId], keyFrameRot2.rotations[boneId], rotationFraction));
					}
					if (keyFrameRot2.InterpolationType == 2)
					{
						pAnimMesh.pMesh->v_Bones[boneId]->SetRotation(glm::slerp(keyFrameRot1.rotations[boneId], keyFrameRot2.rotations[boneId], rotationFraction));
					}
					else
					{
						pAnimMesh.pMesh->v_Bones[boneId]->SetRotation(keyFrameRot1.rotations[boneId] + (keyFrameRot2.rotations[boneId] - keyFrameRot1.rotations[boneId]) * rotationFraction);
					}
				}
			}
		}
	}
}

int AnimationSystem::FindKeyFramePositionIndex(Animation* animation, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < animation->keyFramePositions.size(); ++keyFrameIndex)
	{
		if (animation->keyFramePositions[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return animation->keyFramePositions.size() - 1;
}

int AnimationSystem::FindKeyFrameScaleIndex(Animation* animation, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < animation->keyFrameScales.size(); ++keyFrameIndex)
	{
		if (animation->keyFrameScales[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return animation->keyFrameScales.size() - 1;
}

int AnimationSystem::FindKeyFrameRotIndex(Animation* animation, float time)
{
	int keyFrameIndex = 1;
	for (; keyFrameIndex < animation->keyFrameRotations.size(); ++keyFrameIndex)
	{
		if (animation->keyFrameRotations[keyFrameIndex].time > time)
			return keyFrameIndex - 1;
	}

	return animation->keyFrameRotations.size() - 1;
}
//
//const KeyFramePosition& AnimationSystem::FindCurrentKeyFramePosition(Animation* animation, float time)
//{
//	if (animation->keyFramePositions.size() == 1)
//		return animation->keyFramePositions[0];
//
//	for (int i = 1; i < animation->keyFramePositions.size(); ++i)
//	{
//		if (animation->keyFramePositions[i].time > time)
//			return animation->keyFramePositions[i-1];
//	}
//
//	return animation->keyFramePositions[animation->keyFramePositions.size() - 1];
//}
//
//const KeyFrameScale& AnimationSystem::FindCurrentKeyFrameScale(Animation* animation, float time)
//{
//	if (animation->keyFrameScales.size() == 1)
//		return animation->keyFrameScales[0];
//
//	for (int i = 1; i < animation->keyFrameScales.size(); ++i)
//	{
//		if (animation->keyFrameScales[i].time > time)
//			return animation->keyFrameScales[i-1];
//	}
//
//	return animation->keyFrameScales[animation->keyFrameScales.size() - 1];
//}
