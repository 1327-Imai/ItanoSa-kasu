#include "Enemy.h"
#include "Input.h"
#include "DebugText.h"
#include <cassert>

#include "Myfunc.h"

class Player {
public:
	Vector3 GetWorldPosition();
};

//メンバ関数の定義
//初期化
void Enemy::Initialize(Model* model , uint32_t textureHandle , Vector3 position) {

	//nullポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	worldTransform_.translation_ = position;

}

//更新処理
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

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {

		return bullet->IsDead();

					   });

	//worldTransformの更新
	Myfunc::UpdateWorldTransform(worldTransform_);
}

//描画処理
void Enemy::Draw(ViewProjection viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewprojection);
	}
}

//接近フェーズの関数
void Enemy::Approach() {

	//移動(ベクトルを加算)
	move_.z -= 0.1f;
	worldTransform_.translation_ += move_;
	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 5.0f) {
		phase_ = Phase::Leave;
	}
}

//離脱フェーズの関数
void Enemy::Leave() {
	//移動(ベクトルを加算)

	if (worldTransform_.translation_.z < -10 || 10 < worldTransform_.translation_.z) {
		kEnemySpeed *= -1;
	}

	move_.z += kEnemySpeed;

	worldTransform_.translation_ += move_;
}

//弾の発射
void Enemy::ShotBullet() {

	if (bulletTimer_-- <= 0) {
		assert(player_);

		//弾の速度
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

		//敵の位置をコピー
		Vector3 position = worldTransform_.translation_;

		//弾を生成し初期化
		std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
		newBullet->Initialize(model_ , position , velocity);

		//弾を登録する
		bullets_.push_back(std::move(newBullet));

		bulletTimer_ = kBulletCT;
	}
}

//衝突判定
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