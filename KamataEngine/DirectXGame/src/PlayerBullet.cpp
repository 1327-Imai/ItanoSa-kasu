#include "PlayerBullet.h"
#include <cassert>
#include <random>

#include "Myfunc.h"

class Enemy {
public:
	Vector3 GetWorldPosition();

};

//コンストラクタ
PlayerBullet::PlayerBullet() {

}

//デストラクタ
PlayerBullet::~PlayerBullet() {

}

//メンバ関数
//初期化
void PlayerBullet::Initialize(Model* model , const Vector3& position ) {

#pragma region//乱数生成の下準備
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の設定
	std::uniform_real_distribution<float> accuaryDist(0.0 , 2.0);
	std::uniform_real_distribution<float> wayDist(0 , 4.0);
	std::uniform_real_distribution<float> timeDist(0 , 30.0);

#pragma endregion

	//nullポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = TextureManager::Load("white1x1.png");

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	//精度の決定
	homingAccuary_ = accuaryDist(engine);
	kHomingTimer_ = 0;

	angle_ = {0 , 0 , 1};

	worldTransform_.scale_ = {0.5 , 0.5 , 0.5};

	//回転行列を用意
	Matrix4 matRot = MathUtility::Matrix4Identity();

	//回転ベクトルを用意
	Vector3 rotation = {
		Myfunc::MyMathUtility::Deg2Rad((wayDist(engine) - 2) * 20) ,
		Myfunc::MyMathUtility::Deg2Rad((wayDist(engine) - 2) * 20) ,
		0};

	//回転行列に回転ベクトルを反映
	Myfunc::Affine::SetMatRotation(matRot , rotation);

	//アングルと回転行列の席を求める
	angle_ = Myfunc::MyMathUtility::MulVector3AndMatrix4(angle_ , matRot);

	kFireTimer = 0; kStartHomingTime = timeDist(engine);


}

//更新処理
void PlayerBullet::Update() {

	switch (phase) {
	case PlayerBullet::Phase::Fire:
	default:
	Fire();
	break;

	case PlayerBullet::Phase::Homing:
	Homing();
	break;
	}

	Vector3 direction = {
		angle_.x / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) ,
		angle_.y / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) ,
		angle_.z / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) ,
	};

	worldTransform_.rotation_ = direction;

	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	Myfunc::UpdateWorldTransform(worldTransform_);

	if (--dethTimer_ <= 0) {
		isDead_ = true;
	}

}

//描画処理
void PlayerBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_ , viewprojection , textureHandle_);

}

void PlayerBullet::Fire() {
	kFireTimer++;

	if (kStartHomingTime <= kFireTimer) {
		phase = Phase::Homing;
	}

	//アングルから弾が進むベクトルを算出
	velocity_ = {
		angle_.x / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.y / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.z / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
	};
}

void PlayerBullet::Homing() {
	//タイマーの減少
	kHomingTimer_--;

	//タイマーが0以下の時
	if (kHomingTimer_ <= 0) {

		//ホーミング位置を更新
		targetPos_ = enemy_->GetWorldPosition();

		//ホーミング精度に合わせてタイマーをリセット
		kHomingTimer_ = homingAccuary_ * 5;
	}

	//弾のアングルと弾から敵までのベクトルの外積を出す
	Vector3 cross = MathUtility::Vector3Cross(
		angle_ ,
		Myfunc::MyMathUtility::SubVector3(targetPos_ , worldTransform_.translation_)
	);

	//回転行列を用意
	Matrix4 matRot = MathUtility::Matrix4Identity();

	//回転ベクトルを用意
	Vector3 rotation = {0 , 0 , 0};

	//求まった外積の正負によって回転ベクトルを増減させる
	if (cross.x < 0) {
		rotation.x -= Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}
	else if (0 < cross.x) {
		rotation.x += Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}

	if (cross.y < 0) {
		rotation.y -= Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}
	else if (0 < cross.y) {
		rotation.y += Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}

	if (cross.z < 0) {
		rotation.z -= Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}
	else if (0 < cross.z) {
		rotation.z += Myfunc::MyMathUtility::Deg2Rad(homingRotateValue_);
	}

	//回転行列に回転ベクトルを反映
	Myfunc::Affine::SetMatRotation(matRot , rotation);

	//アングルと回転行列の席を求める
	angle_ = Myfunc::MyMathUtility::MulVector3AndMatrix4(angle_ , matRot);

	//アングルから弾が進むベクトルを算出
	velocity_ = {
		angle_.x / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.y / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
		angle_.z / sqrt(angle_.x * angle_.x + angle_.y * angle_.y + angle_.z * angle_.z) * kBulletSpeed ,
	};

}

//衝突判定
void PlayerBullet::Oncollision() {
	isDead_ = true;
}

//アクセッサ

bool PlayerBullet::GetIsDead() {
	return isDead_;
}

Vector3 PlayerBullet::GetWorldPosition() {

	Vector3 worldPos;

	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void PlayerBullet::SetEnemy(Enemy* enemy) {
	enemy_ = enemy;
}