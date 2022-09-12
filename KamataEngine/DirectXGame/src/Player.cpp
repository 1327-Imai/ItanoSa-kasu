#include "Player.h"
#include "Input.h"
#include <cassert>

#include "Myfunc.h"

class Enemy {
public:
	Vector3 GetWorldPosition();

};

//�R���X�g���N�^�̒�`
Player::Player() {
	input_ = nullptr;
	debugText_ = nullptr;
	for (int i = 0; i < enemyNum; i++) {
		delete enemy_[i];
	}
	for (int i = 0; i < 3; i++) {
		delete target_[i];
	}
}

//�f�X�g���N�^�̒�`
Player::~Player() {

}

//�����o�֐��̒�`
//������
void Player::Initialize(Model* model , uint32_t textureHandle) {

	//null�|�C���^�`�F�b�N
	assert(model);

	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���g���C���X�^���X���擾����
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//���[���h�ϊ��̏�����
	worldTransform_.Initialize();

}

//�X�V����
void Player::Update() {

	Move();

	//worldTransform�̍X�V
	Myfunc::UpdateWorldTransform(worldTransform_);

	ShotBullet();

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {

		return bullet->GetIsDead();

					   });

}

//�`�揈��
void Player::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewprojection);
	}

	//�f�o�b�O�\��
	debugText_->SetPos(50 , 150);
	debugText_->Printf(
		"worldTransform:(%f,%f,%f)" ,
		worldTransform_.translation_.x ,
		worldTransform_.translation_.y ,
		worldTransform_.translation_.z
	);
	debugText_->SetPos(50 , 165);
	debugText_->Printf(
		"viewprojection:(%f,%f,%f)" ,
		viewprojection.eye.x ,
		viewprojection.eye.y ,
		viewprojection.eye.z
	);

	for (int i = 0; i < 3; i++) {
		if (target_[i]) {
			/*�����Ƀ^�[�Q�b�g�̕`�揈��������*/
		}
	}

}

//�ړ�����
void Player::Move() {
	//�v���C���[�̈ړ����x��ݒ�
	const float speed = 0.5f;

	//�v���C���[�̈ړ��x�N�g��
	Vector3 move = {0 , 0 , 0};

	//�ړ����E���W
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//�L�[���͂ɂ��ړ�����
	if (input_->PushKey(DIK_A)) {
		move.x -= speed;
	}
	if (input_->PushKey(DIK_D)) {
		move.x += speed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += speed;
	}
	if (input_->PushKey(DIK_S)) {
		move.y -= speed;
	}

	//���W�ړ�
	worldTransform_.translation_ += move;

	//�ړ����E�𒴂��Ȃ��悤�ɂ��鏈��
	worldTransform_.translation_.x = max(worldTransform_.translation_.x , -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x , +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y , -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y , +kMoveLimitY);

}

//��]����
void Player::Rotate() {
	//�v���C���[�̉�]���x��ݒ�
	const float rotationSpeed = Myfunc::MyMathUtility::Deg2Rad(5.0f);

	//�v���C���[�̉�]�x�N�g��
	Vector3 rotation = {0 , 0 , 0};

	//��]���E
	const float kRotationLimitX = Myfunc::MyMathUtility::Deg2Rad(45.0f);
	const float kRotationLimitY = Myfunc::MyMathUtility::Deg2Rad(45.0f);

	//�L�[���͂ɂ���]����
	if (input_->PushKey(DIK_LEFT) || input_->PushKey(DIK_RIGHT)) {
		if (input_->PushKey(DIK_LEFT)) {
			rotation.y -= rotationSpeed;
		}

		if (input_->PushKey(DIK_RIGHT)) {
			rotation.y += rotationSpeed;
		}
	}
	else {
		if (worldTransform_.rotation_.y < 0) {
			rotation.y += rotationSpeed;
			if (0 < worldTransform_.rotation_.y) {
				rotation.y = 0;
			}
		}

		else if (0 < worldTransform_.rotation_.y) {
			rotation.y -= rotationSpeed;
			if (worldTransform_.rotation_.y < 0) {
				rotation.y = 0;
			}
		}
	}

	if (input_->PushKey(DIK_UP) || input_->PushKey(DIK_DOWN)) {
		if (input_->PushKey(DIK_UP)) {
			rotation.x -= rotationSpeed;
		}
		if (input_->PushKey(DIK_DOWN)) {
			rotation.x += rotationSpeed;
		}
	}
	else {
		if (worldTransform_.rotation_.x < 0) {
			rotation.x += rotationSpeed;
			if (0 < worldTransform_.rotation_.x) {
				rotation.x = 0;
			}
		}

		else if (0 < worldTransform_.rotation_.x) {
			rotation.x -= rotationSpeed;
			if (worldTransform_.rotation_.x < 0) {
				rotation.x = 0;
			}
		}
	}

	//��]
	worldTransform_.rotation_ += rotation;

	//��]���E�𒴂��Ȃ��悤�ɂ��鏈��
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x , -kRotationLimitX);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x , +kRotationLimitX);
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y , -kRotationLimitY);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y , +kRotationLimitY);

}

//�e�̔���
void Player::ShotBullet() {

	if (shotTimer_-- < 0) {
		//�X�y�[�X�L�[�������Ă��鎞
		if (input_->PushKey(DIK_SPACE)) {
			Targetting();
		}
		else {
			//�^�[�Q�b�g[0]�ɓG�̏�񂪓����Ă�����e�̔��˃t���O���I���ɂ���
			if (target_[0]) {
				isFire = true;
				targetTimer_ = 0;
			}
		}

	}

	if (bulletTimer_-- <= 0) {
		if (isFire == true) {
				Vector3 position = worldTransform_.translation_;

				//�e�𐶐���������
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
				newBullet->Initialize(model_ , position);

				newBullet->SetEnemy(target_[fireCount / 2]);

				//�e��o�^����
				bullets_.push_back(std::move(newBullet));

				bulletTimer_ = kBulletCT;
				fireCount++;

				//�^�[�Q�b�g�������ɍ��킹�Ēe�̐����ς��Ȃ炱�����̏������g��
				//�^�[�Q�b�g[2]�܂œG�̏�񂪓����Ă�����
				if (target_[2]) {
					//6���ڂ܂Ŕ��˂��Ēe�̔��˂��I����
					if (6 <= fireCount) {
						fireCount = 0;
						shotTimer_ = kShotCT;
						isFire = false;

						target_[2] = nullptr;
						target_[1] = nullptr;
						target_[0] = nullptr;
					}
				}
				//�^�[�Q�b�g[1]�܂œG�̏�񂪓����Ă�����
				else if (target_[1]) {
					//4���ڂ܂Ŕ��˂��Ēe�̔��˂��I����
					if (4 <= fireCount) {
						fireCount = 0;
						shotTimer_ = kShotCT;
						isFire = false;

						target_[1] = nullptr;
						target_[0] = nullptr;
					}
				}
				//�^�[�Q�b�g[0]�܂œG�̏�񂪓����Ă�����
				else if (target_[0]) {
					//4���ڂ܂Ŕ��˂��Ēe�̔��˂��I����
					if (2 <= fireCount) {
						fireCount = 0;
						shotTimer_ = kShotCT;
						isFire = false;

						target_[0] = nullptr;
					}
				}

				//�^�[�Q�b�g�������Ɋ֌W�Ȃ�6�����Ȃ炱�����̏������g��
				//if (6 <= fireCount) {
				//	fireCount = 0;
				//	shotTimer_ = kShotCT;
				//	isFire = false;
				//	
				//	target_[2] = nullptr;
				//	target_[1] = nullptr;
				//	target_[0] = nullptr;
				//}
		}
	}

	debugText_->SetPos(100 , 100);
	debugText_->Printf("%d" , bullets_.size());

}

void Player::Targetting() {

	//�G�̔z����߂����Ƀ\�[�g
	for (int i = 0; i < enemyNum; i++) {
		for (int j = 0; j < enemyNum - 1; j++) {

			Enemy* tmp = nullptr;

			if (enemy_[j + 1]->GetWorldPosition().z - worldTransform_.translation_.z <
				enemy_[j]->GetWorldPosition().z - worldTransform_.translation_.z) {

				tmp = enemy_[j];
				enemy_[j] = enemy_[j + 1];
				enemy_[j + 1] = tmp;


			}
		}
	}

	//�^�[�Q�b�g�^�C�}�[��0�Ȃ�^�[�Q�b�g�̔z��ɓG�̔z��������Ă���
	/*
	�l�I�Ȃ������Ƃ��ē��t���[����3�̓G���^�[�Q�b�g������A
	���t���[�����ƂɃ^�[�Q�b�g�������������ۂ��ȂƎv�����̂ł������������ɂ��Ă���
	�܂����̉e����[1][2]�͂���ȑO�ƃ^�[�Q�b�g�悪���Ȃ��悤�ɏ�����������Ă���
	*/
	if (targetTimer_-- < 0) {
		if (target_[0] == nullptr) {
			target_[0] = enemy_[0];
		}
		else if (target_[1] == nullptr) {
			if (target_[0] != enemy_[0]) {
				target_[1] = enemy_[0];
			}
			else {
				target_[1] = enemy_[1];
			}
		}
		else if (target_[2] == nullptr) {
			if (target_[0] != enemy_[0] && target_[1] != enemy_[0]) {
				target_[2] = enemy_[0];
			}
			else if (target_[0] != enemy_[1] && target_[1] != enemy_[1]) {
				target_[2] = enemy_[1];
			}
			else {
				target_[2] = enemy_[2];
			}
		}
		targetTimer_ = kTargetCT;
	}

}

//�Փ˔���
void Player::Oncollision() {
}

Vector3 Player::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Player::SetEnemy(Enemy* enemy[30]) {

	for (int i = 0; i < enemyNum; i++) {
		enemy_[i] = enemy[i];
	}
}
