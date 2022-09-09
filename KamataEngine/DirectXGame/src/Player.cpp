#include "Player.h"
#include "Input.h"
#include <cassert>

#include "Myfunc.h"

//コンストラクタの定義
Player::Player() {
	input_ = nullptr;
	debugText_ = nullptr;
}

//デストラクタの定義
Player::~Player() {

}

//メンバ関数の定義
//初期化
void Player::Initialize(Model* model , uint32_t textureHandle) {

	//nullポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

}

//更新処理
void Player::Update() {

	Move();

	//worldTransformの更新
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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {

		return bullet->GetIsDead();

					   });

}

//描画処理
void Player::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);

	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewprojection);
	}

	//デバッグ表示
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

//移動処理
void Player::Move() {
	//プレイヤーの移動速度を設定
	const float speed = 0.5f;

	//プレイヤーの移動ベクトル
	Vector3 move = {0 , 0 , 0};

	//移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;

	//キー入力による移動処理
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

	//座標移動
	worldTransform_.translation_ += move;

	//移動限界を超えないようにする処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x , -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x , +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y , -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y , +kMoveLimitY);

}

//回転処理
void Player::Rotate() {
	//プレイヤーの回転速度を設定
	const float rotationSpeed = Myfunc::MyMathUtility::Deg2Rad(5.0f);

	//プレイヤーの回転ベクトル
	Vector3 rotation = {0 , 0 , 0};

	//回転限界
	const float kRotationLimitX = Myfunc::MyMathUtility::Deg2Rad(45.0f);
	const float kRotationLimitY = Myfunc::MyMathUtility::Deg2Rad(45.0f);

	//キー入力による回転処理
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

	//回転
	worldTransform_.rotation_ += rotation;

	//回転限界を超えないようにする処理
	worldTransform_.rotation_.x = max(worldTransform_.rotation_.x , -kRotationLimitX);
	worldTransform_.rotation_.x = min(worldTransform_.rotation_.x , +kRotationLimitX);
	worldTransform_.rotation_.y = max(worldTransform_.rotation_.y , -kRotationLimitY);
	worldTransform_.rotation_.y = min(worldTransform_.rotation_.y , +kRotationLimitY);

}

//弾の発射
void Player::ShotBullet() {

		if (shotTimer_-- < 0) {
			if (input_->PushKey(DIK_SPACE)) {

				isFire = true;
			}
		}

		if (bulletTimer_-- <= 0) {
			if (isFire == true) {
				Vector3 position = worldTransform_.translation_;

				//弾を生成し初期化
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
				newBullet->Initialize(model_ , position);

				newBullet->SetEnemy(enemy_);

				//弾を登録する
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
//衝突判定
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