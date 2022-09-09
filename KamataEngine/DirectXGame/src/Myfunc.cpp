#include "Myfunc.h"

#pragma region//���[���h�ϊ��s���ݒ肷��֐�

//�X�P�[�����O�s���ݒ肷��֐�
void Myfunc::Affine::SetMatScale(Matrix4& affineMat , Vector3 scale) {
	//�X�P�[�����O�s���錾
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matScale;

}

//��]�s���ݒ肷��֐�
void Myfunc::Affine::SetMatRotation(Matrix4& affineMat , Vector3 rotation) {

	//��]�s���錾
	//Z����]
	Matrix4 matRotZ = {
		cos(rotation.z) , sin(rotation.z) , 0 , 0 ,
		-sin(rotation.z) , cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X����]
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , cos(rotation.x) , sin(rotation.x) , 0 ,
		0 , -sin(rotation.x) , cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y����]
	Matrix4 matRotY = {
		cos(rotation.y) , 0 , -sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		sin(rotation.y) , 0 , cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matRotZ;
	affineMat *= matRotX;
	affineMat *= matRotY;

}

//���s�ړ��s��̐ݒ������֐�
void Myfunc::Affine::SetMatTranslation(Matrix4& affineMat , Vector3 translation) {

	//���s�ړ��s��̐錾
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//�s��̌v�Z
	affineMat *= matTranslation;
}
#pragma endregion

#pragma region//���[���h�ϊ��s��𐶐�����֐�
//�X�P�[�����O�s��𐶐�����֐�
Matrix4 Myfunc::Affine::CreateMatScale(Matrix4 affineMat , Vector3 scale) {

	//�X�P�[�����O�s���錾
	Matrix4 matScale = {
		scale.x , 0 , 0 , 0 ,
		0 , scale.y , 0 , 0 ,
		0 , 0 , scale.z , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	return matScale;

}

//��]�s��𐶐�����֐�
Matrix4 Myfunc::Affine::CreateMatRotation(Matrix4 affineMat , Vector3 rotation) {

	//��]�s���錾
	Matrix4 matRot = MathUtility::Matrix4Identity();

	//Z����]
	Matrix4 matRotZ = {
		cos(rotation.z) , sin(rotation.z) , 0 , 0 ,
		-sin(rotation.z) , cos(rotation.z) , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//X����]
	Matrix4 matRotX = {
		1 , 0 , 0 , 0 ,
		0 , cos(rotation.x) , sin(rotation.x) , 0 ,
		0 , -sin(rotation.x) , cos(rotation.x) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//Y����]
	Matrix4 matRotY = {
		cos(rotation.y) , 0 , -sin(rotation.y) , 0 ,
		0 , 1 , 0 , 0 ,
		sin(rotation.y) , 0 , cos(rotation.y) , 0 ,
		0 , 0 , 0 , 1 ,
	};

	//�s��̌v�Z
	matRot *= matRotZ;
	matRot *= matRotX;
	matRot *= matRotY;

	return matRot;

}

//���s�ړ��s��̐���������֐�
Matrix4 Myfunc::Affine::CreateMatTranslation(Matrix4 affineMat , Vector3 translation) {

	//���s�ړ��s��̐錾
	Matrix4 matTranslation = {
		1 , 0 , 0 , 0 ,
		0 , 1 , 0 , 0 ,
		0 , 0 , 1 , 0 ,
		translation.x , translation.y , translation.z , 1 ,
	};

	//�s��̌v�Z
	return matTranslation;
}

#pragma endregion

#pragma region//�x���@�̌ʓx�@�̊֐�

//�x���@���ʓx�@�ɕϊ�����֐�
float Myfunc::MyMathUtility::Deg2Rad(float Deg) {

	const float PI = 3.141592f;

	return Deg * PI / 180;

}

//�ʓx�@��x���@�ɕϊ�����֐�
float Myfunc::MyMathUtility::Rad2Deg(float rad) {

	const float PI = 3.141592;

	return rad / PI * 180;

}

#pragma endregion

//�x�N�g���ƍs��̊|���Z������֐�
Vector3  Myfunc::MyMathUtility::MulVector3AndMatrix4(Vector3 vec , Matrix4 mat) {

	Vector3 ans;

	ans.x =
		mat.m[0][0] * vec.x +
		mat.m[1][0] * vec.y +
		mat.m[2][0] * vec.z +
		mat.m[3][0] * 0;

	ans.y =
		mat.m[0][1] * vec.x +
		mat.m[1][1] * vec.y +
		mat.m[2][1] * vec.z +
		mat.m[3][1] * 0;

	ans.z =
		mat.m[0][2] * vec.x +
		mat.m[1][2] * vec.y +
		mat.m[2][2] * vec.z +
		mat.m[3][2] * 0;

	return ans;

}

Vector3 Myfunc::MyMathUtility::CrossProduction(Vector3 v1 , Vector3 v2) {
	Vector3 ans;

	ans.x = v1.y * v2.z - v1.z * v2.y;
	ans.y = v1.z * v2.x - v1.x * v2.z;
	ans.z = v1.x * v2.y - v1.y * v2.x;

	return ans;

}

Vector3 Myfunc::MyMathUtility::SubVector3(Vector3 v1 , Vector3 v2) {
	Vector3 ans;

	ans.x = v1.x - v2.x;
	ans.y = v1.y - v2.y;
	ans.z = v1.z - v2.z;

	return ans;

}

//WorldTransform���X�V����֐�
void Myfunc::UpdateWorldTransform(WorldTransform& worldTransform) {
	//���[���h�ϊ��s���p��
	Matrix4 affineMat = MathUtility::Matrix4Identity();

	//���[���h�ϊ��s��ɃX�P�[�����O,��],���s�ړ��̍s�������
	Myfunc::Affine::SetMatScale(affineMat , worldTransform.scale_);
	Myfunc::Affine::SetMatRotation(affineMat , worldTransform.rotation_);
	Myfunc::Affine::SetMatTranslation(affineMat , worldTransform.translation_);

	//���[���h�s��ɒP�ʍs�����
	worldTransform.matWorld_ = MathUtility::Matrix4Identity();

	//�s��̌v�Z
	worldTransform.matWorld_ *= affineMat;

	//�����e������ꍇ
	if (worldTransform.parent_) {
		//�e�̃��[���h�s��Ƃ̌v�Z���s��
		worldTransform.matWorld_ *= worldTransform.parent_->matWorld_;
	}

	//�s��̓]��
	worldTransform.TransferMatrix();
}