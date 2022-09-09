#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

class Enemy;

class PlayerBullet {

public:
	//コンストラクタ
	PlayerBullet();

	//デストラクタ
	~PlayerBullet();

	//メンバ関数
		//初期化
	void Initialize(Model* model , const Vector3& position);

	//更新処理
	void Update();

	//描画処理
	void Draw(const ViewProjection& viewprojection);

	//衝突判定
	void Oncollision();

private:
	void Fire();

	void Homing();

	//アクセッサ
public:
	bool GetIsDead();

	Vector3 GetWorldPosition();

	void SetEnemy(Enemy* enemy);

	//メンバ変数
private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;

	//デスタイマー
	int32_t dethTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;

	//弾の速度
	const float kBulletSpeed = 1.0f;

	//ホーミングの角度調整量
	const int homingRotateValue_ = 5;

	int kHomingTimer_ = 0;

	Vector3 angle_;

	Vector3 targetPos_;

	//ホーミング精度
	int homingAccuary_ = 0;

	//敵のポインタ
	Enemy* enemy_ = nullptr;

	enum class Phase {
		Fire,
		Homing,
	};



	Phase phase = Phase::Fire;

	int kFireTimer = 0;

	int kStartHomingTime = 0;
};

