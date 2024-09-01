#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "DebugCamera.h" //DebugCameraを利用するため	为了使用DebugCamera
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <cassert>
#include <vector>

/// <summary>
/// ゲームシーン	GameScene
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ	构造函数
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ		析构函数
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化	初始化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理	每帧处理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画		描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 检查场景所有碰撞
	/// </summary>
	void CheckAllCollisions();
	void CheckGameClear();
	/// <summary>
	/// 切换阶段
	/// </summary>
	void ChangePhase();

	/// <summary>
	/// flag getter
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	bool SetIsDead(bool value) {
		isDead_ = value;
		return isDead_;
	}

	bool IsFinished() const { return finished_; }
	bool IsVictory() const { return victory_; }

	AABB GetAABB();

private: // メンバ変数	成员变量
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr; // 输入
	Audio* audio_ = nullptr; // 音频Audio
	//========================scene切换参数============================
	bool finished_ = false;
	bool victory_ = false;
	//========================phase切换参数============================
	// flag
	bool isDead_ = false;
	bool isClear_ = false;
	//===================================================================

	// character 当たり判定 size （可能要调整）
	static inline const float kWidth = 1.99f;
	static inline const float kHeight = 1.99f;

	bool isDebugCameraActive = false;

	DebugCamera* debugCamera_ = nullptr;

	Model* model_ = nullptr;

	Model* _modelSkydemo = nullptr;

	Model* _modelPlayerOBJ = nullptr;

	Model* _modelEnemyOBJ = nullptr;

	Model* _modelParticleOBJ = nullptr;

	Model* _modelGoal = nullptr;

	WorldTransform goalWorldTransform_;
	ViewProjection viewProjection_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	
	// 天球
	Skydome* _skydome = nullptr;

	// MapChip
	MapChipField* _mapChipField = nullptr;
	Vector3 goalPosition_;     // 通关目标点
	float goalRadius_ = 1.0f;  // 目标区域的半径，允许玩家接近该区域
	

	// player
	Player* _player = nullptr;

	// enemy
	// Enemy* _enemy = nullptr;
	std::list<Enemy*> _enemies; // 多个敌人
	static inline const uint32_t enemyCount = 9;

	// 粒子
	DeathParticles* deathParticles_ = nullptr;

	// 追踪camera
	CameraController* _cameraController = nullptr;

	/// <summary>
	/// create blocks
	/// </summary>
	void GenerateBlocks();

	//===============================phase=============================

	enum class Phase {
		kPlay,
		kDeath,
	};

	Phase phase_;

	//===================================================================

	/// <summary>
	/// ゲームシーン用	GameScene用
	/// </summary>
};