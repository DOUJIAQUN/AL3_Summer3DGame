#pragma once
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Skydome.h"


class GameOver {
private:

	bool finished_ = false;

	uint32_t gameOvertextureHandle_ = 0;

	Sprite* gameOverSprite_ = nullptr;

	DirectXCommon* dxCommon_ = nullptr;


	ViewProjection viewProjection_;

		// 天球
	Skydome* _skydome = nullptr;

	Model* _modelSkydemo = nullptr;

	
	Player* player_ = nullptr;

public:
	GameOver();
	~GameOver();
	void Initialize();
	void Update();
	void Draw();

	bool IsFinished() const { return finished_; }
};
