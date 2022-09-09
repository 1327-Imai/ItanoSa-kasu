#include "PlayerBullet.h"
#include <cassert>
#include <random>

#include "Myfunc.h"

class Enemy {
public:
	Vector3 GetWorldPosition();

};

//�R���X�g���N�^
PlayerBullet::PlayerBullet() {

}

//�f�X�g���N�^
PlayerBullet::~PlayerBullet() {

}

//�����o�֐�
//������
void PlayerBullet::Initialize(Model* model , const Vector3& position ) {

#pragma region//���������̉�����
	//�����V�[�h������
	std::random_device seed_gen;
	//�����Z���k�E�c�C�X�^�[�̗����G���W��
	std::mt19937_64 engine(seed_gen());
	//�����͈͂̐ݒ�
	std::uniform_real_distribution<float> accuaryDist(0.0 , 2.0);
	std::uniform_real_distribution<float> wayDist(0 , 4.0);
	std::uniform_real_distribution<float> timeDist(0 , 30.0);

#pragma endregion

	//null�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	//�����Ŏ󂯎�����������W���Z�b�g
	worldTransform_.translation_ = position;

	//���x�̌���
	homingAccuary_ = accuaryDist(engine);
	kHomingTimer_ = 0;

	angle_ = {0 , 0 , 1};

	worldTransform_.scale_ = {0.5 , 0.5 , 0.5};

	//��]�s���p��
	Matrix4 matRot = MathUtility::Matrix4Identity();

	//��]�x�N�g����p��
	Vector3 rotation = {
		Myfunc::MyMathUtility::Deg2Rad((wayDist(engine) - 2) * 20) ,
		Myfunc::MyMathUtility::Deg2Rad((wayDist(engine) - 2) * 20) ,
		0};

	//��]�s��ɉ�]�x�N�g���𔽉f
	Myfunc::Affine::SetMatRotation(matRot , rotation);

	//�A���O���Ɖ�]�s��̐Ȃ����߂�
	angle_ = Myfunc::MyMathUtility::MulVector3AndMatrix4(angle_ , matRot);

	kFireTimer = 0; kStartHomingTime = timeDist(engine);


}

//�X�V����
void PlayerBullet::Update() {

	switch (phase) {
	case PlayerBullet::Phase::Fire:
	default:
	Fire();
	break;

	case PlayerBullet::Phase::Homing:
	Homing();
	break;
	}

	Vector3 direction = {
		angle_.x / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) ,
		angle_.y / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) ,
		angle_.z / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) ,
	};

	worldTransform_.rotation_ = direction;

	//���W���ړ�������
	worldTransform_.translation_ += velocity_;

	Myfunc::UpdateWorldTransform(worldTransform_);

	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}

}

//�`�揈��
void PlayerBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);

}

void PlayerBullet::Fire() {
	kFireTimer++;

	if (kStartHomingTime <= kFireTimer) {
		phase = Phase::Homing;
	}

	//�A���O������e���i�ރx�N�g�����Z�o
	velocity_ = {
		angle_.x / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.y / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.z / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
	};
}

void PlayerBullet::Homing() {
	//�^�C�}�[�̌���
	kHomingTimer_--;

	//�^�C�}�[��0�ȉ��̎�
	if (kHomingTimer_ <= 0) {

		//�z�[�~���O�ʒu���X�V
		targetPos_ = enemy_->GetWorldPosition();

		//�z�[�~���O���x�ɍ��킹�ă^�C�}�[�����Z�b�g
		kHomingTimer_ = homingAccuary_ * 5;
	}

	//�e�̃A���O���ƒe����G�܂ł̃x�N�g���̊O�ς��o��
	Vector3 cross = MathUtility::Vector3Cross(
		angle_ ,
		Myfunc::MyMathUtility::SubVector3(targetPos_ , worldTransform_.translation_)
	);

	//��]�s���p��
	Matrix4 matRot = MathUtility::Matrix4Identity();

	//��]�x�N�g����p��
	Vector3 rotation = {0 , 0 , 0};

	//���܂����O�ς̐����ɂ���ĉ�]�x�N�g���𑝌�������
	if (cross.x < 0) {
		rotation.x -= Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}
	else if (0 < cross.x) {
		rotation.x += Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}

	if (cross.y < 0) {
		rotation.y -= Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}
	else if (0 < cross.y) {
		rotation.y += Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}

	if (cross.z < 0) {
		rotation.z -= Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}
	else if (0 < cross.z) {
		rotation.z += Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}

	//��]�s��ɉ�]�x�N�g���𔽉f
	Myfunc::Affine::SetMatRotation(matRot , rotation);

	//�A���O���Ɖ�]�s��̐Ȃ����߂�
	angle_ = Myfunc::MyMathUtility::MulVector3AndMatrix4(angle_ , matRot);

	//�A���O������e���i�ރx�N�g�����Z�o
	velocity_ = {
		angle_.x / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.y / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.z / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
	};

}

//�Փ˔���
void PlayerBullet::Oncollision() {
	isDead_ = true;
}

//�A�N�Z�b�T

bool PlayerBullet::GetIsDead() {
	return isDead_;
}

Vector3 PlayerBullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::SetEnemy(Enemy* enemy) {
	enemy_ = enemy;
}