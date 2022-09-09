#include "Player.h"
#include "Input.h"
#include <cassert>

#include "Myfunc.h"

//�R���X�g���N�^�̒�`
Player::Player() {
	input_ = nullptr;
	debugText_ = nullptr;
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

	if (input_->TriggerKey(DIK_LSHIFT)) {
		if (shotType == 0) {
			shotType = 1;
		}
		else {
			shotType = 0;
		}
	}

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
			if (input_->PushKey(DIK_SPACE)) {

				isFire = true;
			}
		}

		if (bulletTimer_-- <= 0) {
			if (isFire == true) {
				Vector3 position = worldTransform_.translation_;

				//�e�𐶐���������
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
				newBullet->Initialize(model_ , position);

				newBullet->SetEnemy(enemy_);

				//�e��o�^����
				bullets_.push_back(std::move(newBullet));

				bulletTimer_ = kBulletCT;
				fireCount++;
				if (5 < fireCount) {
					fireCount = 0;
					shotTimer_ = kShotCT;
					isFire = false;
				}
			}
		}

		debugText_->SetPos(100 , 100);
		debugText_->Printf("%d" , bullets_.size());

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

void Player::SetEnemy(Enemy* enemy) {
	enemy_ = enemy;
}