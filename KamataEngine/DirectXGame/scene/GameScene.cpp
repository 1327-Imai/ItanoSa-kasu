#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>

GameScene::GameScene() {

}

GameScene::~GameScene() {

	delete model_;
	delete debugCamera_;
	delete player_;
	delete enemy_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	//3Dモデルの生成
	model_ = Model::Create();

#pragma region//乱数生成の下準備
	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌ・ツイスターの乱数エンジン
	std::mt19937_64 engine(seed_gen());
	//乱数範囲の設定
	std::uniform_real_distribution<float> rotDist(-PI , PI);
	std::uniform_real_distribution<float> posDist(-10 , 10);
#pragma endregion

#pragma region//worldTransform
	//ワールドトランスフォームの初期化

	//自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	player_->Initialize(model_ , textureHandle_);

	//敵の生成
	enemy_ = new Enemy();
	//敵の初期化
	enemy_->Initialize(model_ , textureHandle_);
	enemy_->SetPlayer(player_);

	player_->SetEnemy(enemy_);

#pragma endregion

#pragma region//viewProjection
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
#pragma endregion 

#pragma region//debugCamera

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280 , 720);

	//軸方向の表示を無効にする
	AxisIndicator::GetInstance()->SetVisible(false);

	//軸方向表示が参考にするビュープロジェクションを設定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());

#pragma endregion
}

void GameScene::Update() {

#pragma region//worldTransform

	//自キャラの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

#pragma endregion

#pragma region//viewProjection



#pragma endregion

#pragma region//debugCamera

#ifdef _DEBUG

	//右シフトを押すとデバッグカメラ有効フラグを切り替え
	if (input_->TriggerKey(DIK_RSHIFT)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		}
		else {
			isDebugCameraActive_ = false;
		}
	}

#endif //_DEBUG

	//カメラの処理
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();

		//デバッグカメラのビュー行列とプロジェクション行列をviewProjectionの同行列に代入
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

		//viewProjectionの転送
		viewProjection_.TransferMatrix();

		//軸方向の表示を有効にする
		AxisIndicator::GetInstance()->SetVisible(true);
	}
	else {
		//viewProjectionの再計算と転送
		viewProjection_.UpdateMatrix();
		viewProjection_.TransferMatrix();

		//軸方向の表示を無効にする
		AxisIndicator::GetInstance()->SetVisible(false);
	}

#pragma endregion

	CheckAllCollisions();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	//3Dモデル描画

	//自キャラの描画
	player_->Draw(viewProjection_);

	//敵の描画
	enemy_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

	//デバッグカメラの更新
	//debugCamera_->Update();

#pragma endregion
}

//衝突判定と応答
void GameScene::CheckAllCollisions() {
	//判定対象AとBの座標
	Vector3 posA , posB;

	//自弾のリストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	//敵弾のリストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();

#pragma region //自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾の座標
		posB = bullet.get()->GetWorldPosition();

		float l = sqrt(
			(posB.x - posA.x) * (posB.x - posA.x) +
			(posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z));

		if (
			sqrt(
			(posB.x - posA.x) * (posB.x - posA.x) +
			(posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z)) <= 2
			) {
			player_->Oncollision();
			bullet.get()->Oncollision();
		}
	}

#pragma endregion

#pragma region //自弾と敵キャラの当たり判定
	//敵キャラの座標
	posA = enemy_->GetWorldPosition();

	//敵キャラと自弾全ての当たり判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾の座標
		posB = bullet.get()->GetWorldPosition();

		float l = sqrt(
			(posB.x - posA.x) * (posB.x - posA.x) +
			(posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z));

		if (
			sqrt(
			(posB.x - posA.x) * (posB.x - posA.x) +
			(posB.y - posA.y) * (posB.y - posA.y) +
			(posB.z - posA.z) * (posB.z - posA.z)) <= 2
			) {
			enemy_->Oncollision();
			bullet.get()->Oncollision();
		}
	}

#pragma endregion

#pragma region //自弾と敵弾の当たり判定

	//自弾の座標
	for (const std::unique_ptr<PlayerBullet>& pBullet : playerBullets) {
		posA = pBullet.get()->GetWorldPosition();

		//自弾と敵弾全ての当たり判定
		for (const std::unique_ptr<EnemyBullet>& eBullet : enemyBullets) {
			//敵弾の座標
			posB = eBullet.get()->GetWorldPosition();

			float l = sqrt(
				(posB.x - posA.x) * (posB.x - posA.x) +
				(posB.y - posA.y) * (posB.y - posA.y) +
				(posB.z - posA.z) * (posB.z - posA.z));

			if (
				sqrt(
				(posB.x - posA.x) * (posB.x - posA.x) +
				(posB.y - posA.y) * (posB.y - posA.y) +
				(posB.z - posA.z) * (posB.z - posA.z)) <= 2
				) {
				pBullet.get()->Oncollision();
				eBullet.get()->Oncollision();
			}
		}
	}

#pragma endregion
}