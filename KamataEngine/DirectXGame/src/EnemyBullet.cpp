#include "EnemyBullet.h"
#include <cassert>

#include "Myfunc.h"

//コンストラクタ
EnemyBullet::EnemyBullet() {

}

//デストラクタ
EnemyBullet::~EnemyBullet() {

}

//メンバ関数
//初期化
void EnemyBullet::Initialize(Model* model , const Vector3& position , const Vector3& velocity) {

	//nullポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("red1x1.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

//更新処理
void EnemyBullet::Update() {

	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	Myfunc::UpdateWorldTransform(worldTransform_);

	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}

}

//描画処理
void EnemyBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);

}

//アクセッサ
Vector3 EnemyBullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

//衝突判定
void EnemyBullet::Oncollision() {
	isDead_ = true;
}