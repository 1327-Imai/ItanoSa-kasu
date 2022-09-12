#include "Enemy.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>

#include "Myfunc.h"

class Player {
public:
	Vector3 GetWorldPosition();
};

//�����o�֐��̒�`
//������
void Enemy::Initialize(Model* model , uint32_t textureHandle , Vector3 position) {

	//null�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

}

//�X�V����
void Enemy::Update() {
	move_ = {0.0f , 0.0f , 0.0f};

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:
	Approach();
	break;

	case Enemy::Phase::Leave:
	Leave();
	break;
	}

	ShotBullet();
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {

		return bullet->IsDead();

					   });

	//worldTransform�̍X�V
	Myfunc::UpdateWorldTransform(worldTransform_);
}

//�`�揈��
void Enemy::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewprojection);
	}
}

//�ڋ߃t�F�[�Y�̊֐�
void Enemy::Approach() {

	//�ړ�(�x�N�g�������Z)
	move_.z -= 0.1f;
	worldTransform_.translation_ += move_;
	//����̈ʒu�ɓ��B�����痣�E
	if (worldTransform_.translation_.z < 5.0f) {
		phase_ = Phase::Leave;
	}
}

//���E�t�F�[�Y�̊֐�
void Enemy::Leave() {
	//�ړ�(�x�N�g�������Z)

	if (worldTransform_.translation_.z < -10 || 10 < worldTransform_.translation_.z) {
		kEnemySpeed *= -1;
	}

	move_.z += kEnemySpeed;

	worldTransform_.translation_ += move_;
}

//�e�̔���
void Enemy::ShotBullet() {

	if (bulletTimer_-- <= 0) {
		assert(player_);

		//�e�̑��x
		const float kBulletSpeed = 0.5f;

		Vector3 playerWorldPos = player_->GetWorldPosition();
		Vector3 enemyWorldPos = GetWorldPosition();

		Vector3 angle = playerWorldPos;
		angle -= enemyWorldPos;

		Vector3 velocity = {
			angle.x / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
			angle.y / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
			angle.z / sqrt(angle.x * angle.x + angle.y * angle.y + angle.z * angle.z) * kBulletSpeed ,
		};

		//�G�̈ʒu���R�s�[
		Vector3 position = worldTransform_.translation_;

		//�e�𐶐���������
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_ , position , velocity);

		//�e��o�^����
		bullets_.push_back(std::move(newBullet));

		bulletTimer_ = kBulletCT;
	}
}

//�Փ˔���
void Enemy::Oncollision() {
}

void Enemy::SetPlayer(Player* player) {
	player_ = player;
}

Vector3 Enemy::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}