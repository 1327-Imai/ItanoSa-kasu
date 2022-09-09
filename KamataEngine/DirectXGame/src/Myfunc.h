#pragma once
#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "PrimitiveDrawer.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

namespace Myfunc {

	namespace Affine {
		//���[���h�ϊ��s���ݒ肷��֐�
		void SetMatScale(Matrix4& affineMat , Vector3 scale);				//�X�P�[�����O�s���ݒ肷��֐�
		void SetMatRotation(Matrix4& affineMat , Vector3 rotation);			//��]�s���ݒ肷��֐�
		void SetMatTranslation(Matrix4& affineMat , Vector3 translation);	//���s�ړ��s��̐ݒ������֐�

		//���[���h�ϊ��s��𐶐�����֐�
		Matrix4 CreateMatScale(Matrix4 affineMat , Vector3 scale);				//�X�P�[�����O�s��𐶐�����֐�
		Matrix4 CreateMatRotation(Matrix4 affineMat , Vector3 rotation);			//��]�s��𐶐�����֐�
		Matrix4 CreateMatTranslation(Matrix4 affineMat , Vector3 translation);	//���s�ړ��s��̐���������֐�
	}

	namespace MyMathUtility {
		//�x���@�̌ʓx�@�̊֐�
		float Deg2Rad(float Deg);	//�x���@���ʓx�@�ɕϊ�����֐�
		float Rad2Deg(float rad);	//�ʓx�@��x���@�ɕϊ�����֐�

		//�x�N�g���ƍs��̊|���Z������֐�
		Vector3 MulVector3AndMatrix4(Vector3 vec , Matrix4 mat);

		Vector3 CrossProduction(Vector3 v1 , Vector3 v2);

		Vector3 SubVector3(Vector3 v1 , Vector3 v2);
	}

	//WorldTransform���X�V����֐�
	void UpdateWorldTransform(WorldTransform& worldTransform);

}