#include "TitleScene.h"
#include "TextureManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	//delete _modelSkydemo;
	delete gametitleSprite_;
}

void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();

	viewProjection_.Initialize();

	gametitletextureHandle_ = TextureManager::Load("title.png");

	gametitleSprite_ = Sprite::Create(gametitletextureHandle_, {0, 0});


	

	//====================天球==========================
	_modelSkydemo = Model::CreateFromOBJ("skydome", true); // 天球モデル

	_skydome = new Skydome();

	_skydome->Initialize(_modelSkydemo, &viewProjection_);

	
}

void TitleScene::Update() {
	_skydome->Update();
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {
	// コマンドリストの取得	获取命令列表
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();
	Model::PreDraw(commandList);
	_skydome->Draw();
	Model::PostDraw();

	Sprite::PreDraw(commandList);
	gametitleSprite_->Draw();
	Sprite::PostDraw();

	
	
}
