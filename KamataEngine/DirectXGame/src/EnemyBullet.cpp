#include "EnemyBullet.h"
#include <cassert>

#include "Myfunc.h"

//�R���X�g���N�^
EnemyBullet::EnemyBullet() {

}

//�f�X�g���N�^
EnemyBullet::~EnemyBullet() {

}

//�����o�֐�
//������
void EnemyBullet::Initialize(Model* model , const Vector3& position , const Vector3& velocity) {

	//null�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("red1x1.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//�����Ŏ󂯎�������x�������o�ϐ��ɑ��
	velocity_ = velocity;
}

//�X�V����
void EnemyBullet::Update() {

	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

	Myfunc::UpdateWorldTransform(worldTransform_);

	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}

}

//�`�揈��
void EnemyBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);

}

//�A�N�Z�b�T
Vector3 EnemyBullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

//�Փ˔���
void EnemyBullet::Oncollision() {
	isDead_ = true;
}