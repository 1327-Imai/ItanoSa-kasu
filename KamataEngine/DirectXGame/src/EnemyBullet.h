#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"



class EnemyBullet {

public:
	//コンストラクタ
	EnemyBullet();

	//デストラクタ
	~EnemyBullet();

	//メンバ関数
		//初期化
	void Initialize(Model* model , const Vector3& position , const Vector3& velocity);

	//更新処理
	void Update();

	//描画処理
	void Draw(const ViewProjection& viewprojection);

	bool IsDead() const { return isDead_; }

	//アクセッサ
public:
	//衝突判定
	Vector3 GetWorldPosition();
	
	void Oncollision();

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

};

