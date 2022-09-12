#include "Player.h"
#include "Input.h"
#include <cassert>

#include "Myfunc.h"

class Enemy {
public:
	Vector3 GetWorldPosition();

};

//コンストラクタの定義
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

	for (int i = 0; i < 3; i++) {
		if (target_[i]) {
			/*ここにターゲットの描画処理を入れる*/
		}
	}

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
		//スペースキーを押している時
		if (input_->PushKey(DIK_SPACE)) {
			Targetting();
		}
		else {
			//ターゲット[0]に敵の情報が入っていたら弾の発射フラグをオンにする
			if (target_[0]) {
				isFire = true;
				targetTimer_ = 0;
			}
		}

	}

	if (bulletTimer_-- <= 0) {
		if (isFire == true) {
				Vector3 position = worldTransform_.translation_;

				//弾を生成し初期化
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
				newBullet->Initialize(model_ , position);

				newBullet->SetEnemy(target_[fireCount / 2]);

				//弾を登録する
				bullets_.push_back(std::move(newBullet));

				bulletTimer_ = kBulletCT;
				fireCount++;

				//ターゲットした数に合わせて弾の数が変わるならこっちの処理を使う
				//ターゲット[2]まで敵の情報が入っていたら
				if (target_[2]) {
					//6発目まで発射して弾の発射を終える
					if (6 <= fireCount) {
						fireCount = 0;
						shotTimer_ = kShotCT;
						isFire = false;

						target_[2] = nullptr;
						target_[1] = nullptr;
						target_[0] = nullptr;
					}
				}
				//ターゲット[1]まで敵の情報が入っていたら
				else if (target_[1]) {
					//4発目まで発射して弾の発射を終える
					if (4 <= fireCount) {
						fireCount = 0;
						shotTimer_ = kShotCT;
						isFire = false;

						target_[1] = nullptr;
						target_[0] = nullptr;
					}
				}
				//ターゲット[0]まで敵の情報が入っていたら
				else if (target_[0]) {
					//4発目まで発射して弾の発射を終える
					if (2 <= fireCount) {
						fireCount = 0;
						shotTimer_ = kShotCT;
						isFire = false;

						target_[0] = nullptr;
					}
				}

				//ターゲットした数に関係なく6発撃つならこっちの処理を使う
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

	//敵の配列を近い順にソート
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

	//ターゲットタイマーが0ならターゲットの配列に敵の配列を代入していく
	/*
	個人的なこだわりとして同フレームに3つの敵をターゲットするより、
	数フレームごとにターゲットする方がそれっぽいなと思ったのでこういう処理にしている
	またその影響で[1][2]はそれ以前とターゲット先が被らないように条件分岐を入れている
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

void Player::SetEnemy(Enemy* enemy[30]) {

	for (int i = 0; i < enemyNum; i++) {
		enemy_[i] = enemy[i];
	}
}
