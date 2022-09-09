#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "DebugText.h"

#include <memory>
#include <list>

#include "PlayerBullet.h"

class Player {

public:

	//コンストラクタの宣言
	Player();

	//デストラクタの宣言
	~Player();

	//メンバ関数の宣言
	//初期化
	void Initialize(Model* model , uint32_t textureHandle);

	//更新処理
	void Update();

	//描画処理
	void Draw(ViewProjection viewprojection);

	//移動処理
	void Move();

	//回転処理
	void Rotate();

	//弾の発射
	void ShotBullet();

	//衝突判定
	void Oncollision();

	//アクセッサ
public:
	Vector3 GetWorldPosition();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() {
		return bullets_;
	}

	void SetEnemy(Enemy* enemy);

	//メンバ変数
private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//Input
	Input* input_;

	//DebugText
	DebugText* debugText_;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;

	//発射レート
	static const int32_t kBulletCT = 1;

	//デスタイマー
	int32_t bulletTimer_ = kBulletCT;

	Enemy* enemy_ = nullptr;

	const int kShotCT = 60;

	int shotTimer_ = 0;

	int fireCount = 0;

	bool isFire = false;

	int shotType = 0;
};

