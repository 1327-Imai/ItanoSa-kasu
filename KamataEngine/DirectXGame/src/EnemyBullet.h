#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"



class EnemyBullet {

public:
	//�R���X�g���N�^
	EnemyBullet();

	//�f�X�g���N�^
	~EnemyBullet();

	//�����o�֐�
		//������
	void Initialize(Model* model , const Vector3& position , const Vector3& velocity);

	//�X�V����
	void Update();

	//�`�揈��
	void Draw(const ViewProjection& viewprojection);

	bool IsDead() const { return isDead_; }

	//�A�N�Z�b�T
public:
	//�Փ˔���
	Vector3 GetWorldPosition();
	
	void Oncollision();

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

};

