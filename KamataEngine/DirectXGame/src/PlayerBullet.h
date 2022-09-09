#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

class Enemy;

class PlayerBullet {

public:
	//�R���X�g���N�^
	PlayerBullet();

	//�f�X�g���N�^
	~PlayerBullet();

	//�����o�֐�
		//������
	void Initialize(Model* model , const Vector3& position);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw(const ViewProjection& viewprojection);

	//�Փ˔���
	void Oncollision();

private:
	void Fire();

	void Homing();

	//�A�N�Z�b�T
public:
	bool GetIsDead();

	Vector3 GetWorldPosition();

	void SetEnemy(Enemy* enemy);

	//�����o�ϐ�
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//���x
	Vector3 velocity_;

	//����
	static const int32_t kLifeTime = 60 * 5;

	//�f�X�^�C�}�[
	int32_t dethTimer_ = kLifeTime;

	//�f�X�t���O
	bool isDead_ = false;

	//�e�̑��x
	const float kBulletSpeed = 1.0f;

	//�z�[�~���O�̊p�x������
	const int homingRotateValue_ = 5;

	int kHomingTimer_ = 0;

	Vector3 angle_;

	Vector3 targetPos_;

	//�z�[�~���O���x
	int homingAccuary_ = 0;

	//�G�̃|�C���^
	Enemy* enemy_ = nullptr;

	enum class Phase {
		Fire,
		Homing,
	};



	Phase phase = Phase::Fire;

	int kFireTimer = 0;

	int kStartHomingTime = 0;
};

