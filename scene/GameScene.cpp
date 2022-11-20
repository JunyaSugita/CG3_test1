#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	view_.Initialize();
	view_.eye = { 0,10,-10 };
	view_.UpdateMatrix();
	
	worldTransform_.Initialize();
	worldTransform_.scale_ = { 10,1,10 };
	MatCalc(worldTransform_);

	model_ = Model::Create();

	particleM_ = ParticleManager::Create();
	particleM_->Initialize();
	particleM_->Update();

	grassM_ = GrassManager::Create();
	grassM_->Update();
}

void GameScene::Update() {

	//カメラ
	view_.eye = { 
		cos(angleX_ * PI / 180) * cos(angleY_ * PI / 180) * 10,
		sin(angleY_ * PI / 180) * 10,
		sin(angleX_ * PI / 180) * cos(angleY_ * PI / 180) * 10};
	view_.UpdateMatrix();
	particleM_->CameraMoveEyeVector(view_);
	grassM_->CameraMoveEyeVector(view_);

	if (input_->PushKey(DIK_LEFT)) {
		angleX_--;
	}
	if (input_->PushKey(DIK_RIGHT)) {
		angleX_++;
	}
	if (input_->PushKey(DIK_UP)) {
		if (angleY_ < 89) {
			angleY_++;
		}
	}
	if (input_->PushKey(DIK_DOWN)) {
		if (angleY_ > -89) {
			angleY_--;
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		scene++;
		if (scene > 2) {
			scene = 0;
		}
	}

	//パーティクル
	if (scene == 2) {
		particleM_->ParticleGenerate();
	}
	particleM_->Update();
	
	//草
	grassM_->Update();

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
	//model_->Draw(worldTransform_,view_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion



#pragma region パーティクルの描画
	// パーティクル描画前処理
	ParticleManager::PreDraw(commandList);

	/// <summary>
	/// ここにパーティクルの描画処理を追加できる
	/// </summary>
	if (scene == 2) {
		particleM_->Draw();
	}
	// パーティクル描画後処理
	ParticleManager::PostDraw();
#pragma endregion

#pragma 草の描画
	// パーティクル描画前処理
	GrassManager::PreDraw(commandList);

	/// <summary>
	/// ここにパーティクルの描画処理を追加できる
	/// </summary>
	if (scene == 1) {
		grassM_->Draw();
	}
	// パーティクル描画後処理
	GrassManager::PostDraw();
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

#pragma endregion
}
