#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <cassert>
#include <numbers>

class Player;
class Enemy {

private:
	//=====================碰撞=====================

	static inline const float kWidth = 1.99f;
	static inline const float kHeight = 1.99f;

	//===================Move===================

	static inline const float kWalkSpeed = 0.05f;
	Vector3 velocity_ = {-kWalkSpeed, 0.0f, 0.0f};

	//===================Animation===============

	// static inline const float kWalkMotionAngleStart = -1 * std::numbers::pi_v<float>; // 最初的角度
	// static inline const float kWalkMotionAngleEnd = 2 * std::numbers::pi_v<float>;    // 最后的角度
	// static inline const float kWalkMotionTime = 2.0f;                                 // 动画周期时间（秒）

	static inline const float kWalkMotionAngleStart = 0;                           // 最初的角度
	static inline const float kWalkMotionAngleEnd = 1 * std::numbers::pi_v<float>; // 最后的角度
	static inline const float kWalkMotionTime = 2.0f;                              // 动画周期时间（秒）

	float walkTimer_ = 0.0f; // 动画计时器经过时间

	//===================Others===================

	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;
	Model* model_ = nullptr;

public:
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	//===================冲突判定===================

	/// <summary>
	/// 获得世界位置坐标
	/// </summary>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 获得AABB
	/// </summary>
	AABB GetAABB();

	/// <summary>
	/// 冲突结果
	/// </summary>
	void OnCollision(const Player* player);

	//==============================================
};