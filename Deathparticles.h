#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <algorithm>
#include <array>
#include <cassert>
#include <numbers>

class DeathParticles {
private:
	//==================Default==================
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

	//===================个数====================
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//===================粒子移动====================
	// 粒子运动持续时间
	static inline const float kDuration = 1.0f;
	// 粒子速度
	static inline const float kSpeed = 0.1f;
	// 粒子分割数
	static inline const float kAngleUnit = std::numbers::pi_v<float> * 2 / kNumParticles;

	//===================粒子消失====================
	// flag
	bool isFinished_ = false;
	// 倒计时
	float counter_ = 0.0f;

	//===================粒子变浅====================
	ObjectColor objectColor_;
	Vector4 color_;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsFinished() { return isFinished_; }
};