#pragma once
#include <Vector3.h>
#include <cassert>
#include <cstdint>
#include <fstream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
/// <summary>
/// Map Type
/// </summary>
enum MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

typedef struct {
	std::vector<std::vector<MapChipType>> data;
} MapChipData;

class MapChipField {
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	// blockのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// blockの個数
	static inline const uint32_t kNumBlockVertical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;


	MapChipData _mapChipData;

	/// <summary>
	/// MapChip Reset
	/// </summary>
	void ResetMapChipData();
	/// <summary>
	/// Load CSV file
	/// </summary>
	/// <param name="filePath">file path</param>
	void LoadMapChipCsv(const std::string& filePath);
	/// <summary>
	/// Get MapChip Type by index
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);
	/// <summary>
	/// Get MapChip Position by index
	/// </summary>
	/// <param name="xIndex"></param>
	/// <param name="yIndex"></param>
	/// <returns></returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);
	/// <summary>
	/// Get NumBlock Virtical
	/// </summary>
	/// <returns></returns>
	uint32_t GetNumBlockVirtical();
	/// <summary>
	/// Get NumBlock Horizontal
	/// </summary>
	/// <returns></returns>
	uint32_t GetNumBlockHorizontal();

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	Rect GetRectByIndexSet(uint32_t xIndex, uint32_t yIndex);
};

