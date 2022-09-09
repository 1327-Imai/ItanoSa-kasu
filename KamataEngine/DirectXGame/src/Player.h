#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

#include <memory>
#include <list>

#include "PlayerBullet.h"

class Player {

public:

	//�R���X�g���N�^�̐錾
	Player();

	//�f�X�g���N�^�̐錾
	~Player();

	//�����o�֐��̐錾
	//������
	void Initialize(Model* model , uint32_t textureHandle);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw(ViewProjection viewprojection);

	//�ړ�����
	void Move();

	//��]����
	void Rotate();

	//�e�̔���
	void ShotBullet();

	//�Փ˔���
	void Oncollision();

	//�A�N�Z�b�T
public:
	Vector3 GetWorldPosition();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() {
		return bullets_;
	}

	void SetEnemy(Enemy* enemy);

	//�����o�ϐ�
private:
	//���[���h�ϊ��f�[�^
	WorldTransform worldTransform_;

	//���f��
	Model* model_ = nullptr;

	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;

	//Input
	Input* input_;

	//DebugText
	DebugText* debugText_;

	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//���˃��[�g
	static const int32_t kBulletCT = 1;

	//�f�X�^�C�}�[
	int32_t bulletTimer_ = kBulletCT;

	Enemy* enemy_ = nullptr;

	const int kShotCT = 60;

	int shotTimer_ = 0;

	int fireCount = 0;

	bool isFire = false;

	int shotType = 0;
};

