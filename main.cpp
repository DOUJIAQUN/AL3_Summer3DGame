#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "GameOver.h"
#include "GameVictory.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "TitleScene.h"
#include "WinApp.h"

enum class Scene {
	kUnkonwn = 0,
	kTitle,
	kGame,
	kGameOver,
	kGameVictory,
};
Scene scene = Scene::kUnkonwn;

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
GameOver* gameOver = nullptr;
GameVictory* gameVictor = nullptr;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			scene = Scene::kGameOver;
			delete gameScene;
			gameScene = nullptr;
			gameOver = new GameOver();
			gameOver->Initialize();
		} 
		else if (gameScene->IsVictory()) {
			scene = Scene::kGameVictory;
			delete gameScene;
			gameScene = nullptr;
			gameVictor = new GameVictory;
			gameVictor->Initialize();
		}
		break;
	case Scene::kGameOver:
		if (gameOver->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOver;
			gameOver = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	case Scene::kGameVictory:
		if (gameVictor->IsFinished()) {
			scene = Scene::kTitle;
			delete gameVictor;
			gameVictor = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	}
	
	
   
}
void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameOver:
		gameOver->Update();
		break;
	case Scene::kGameVictory:
		gameVictor->Update();
		break;
	}

}
void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOver->Draw();
		break;
	case Scene::kGameVictory:
		gameVictor->Draw();
		break;
	}
}
// Windowsアプリでのエントリーポイント(main関数)	主函数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能		泛用功能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;

	// ゲームウィンドウの作成		GameWindow制作
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"Veil_Runner"); // User Name

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化		输入（Input）初始化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化		Audio初始化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化	TextureManager初始化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化	Sprite静态初始化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化		3D模型静态初始化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion
	// 最初のシーンの初期化
	scene = Scene::kTitle;

	// タイトルシーンの初期化	TitleScene初始化
	titleScene = new TitleScene();
	titleScene->Initialize();

	// ゲームシーンの初期化	GameScene初始化
	gameScene = new GameScene();
	gameScene->Initialize();

	// メインループ	主循环
	while (true) {
		// メッセージ処理		信息处理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始		开始受理ImGui
		imguiManager->Begin();
		// 入力関連の毎フレーム処理	输入关联的每帧处理
		input->Update();

		// タイトルシーンの毎フレーム処理	TitleScene的每帧处理
		// titleScene->Update();
		// ゲームシーンの毎フレーム処理	GameScene的每帧处理
		// gameScene->Update();

		ChangeScene();
		UpdateScene();

		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始		描画开始
		dxCommon->PreDraw();

		// ゲームシーンの描画		GameScene描画
		// gameScene->Draw();
		// タイトルシーンの描画		TitleScene描画
		// titleScene->Draw();

		DrawScene();

		// 軸表示の描画
		axisIndicator->Draw();
		// プリミティブ描画のリセット		重置原始绘制
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete titleScene;
	delete gameScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}