#include "MapChipField.h"

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

void MapChipField::ResetMapChipData() {
	_mapChipData.data.clear();
	_mapChipData.data.resize(kNumBlockVertical);
	for (std::vector<MapChipType>& mapChipDataLine : _mapChipData.data) {
		mapChipDataLine.resize(kNumBlockHorizontal);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {
	// reset
	ResetMapChipData();

	// load csv file
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	// mapChip Csv
	std::stringstream mapChipCsv;

	// file copy
	mapChipCsv << file.rdbuf();

	// close file
	file.close();

	// load MapChipDate from CSV
	for (uint32_t i = 0; i < kNumBlockVertical; ++i) {
		std::string line;
		getline(mapChipCsv, line);

		std::istringstream line_stream(line);

		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			std::string word;
			getline(line_stream, word, ',');

			if (mapChipTable.contains(word)) {
				_mapChipData.data[i][j] = mapChipTable[word];
			}
		}
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex) {
	if (xIndex < 0 || kNumBlockHorizontal - 1 < xIndex) {
		return MapChipType::kBlank;
	}
	if (yIndex < 0 || kNumBlockVertical - 1 < yIndex) {
		return MapChipType::kBlank;
	}
	return _mapChipData.data[yIndex][xIndex];
}

Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex) { return Vector3(kBlockWidth * xIndex, kBlockHeight * (kNumBlockVertical - 1 - yIndex), 0); }

uint32_t MapChipField::GetNumBlockVirtical() { return kNumBlockVertical; }

uint32_t MapChipField::GetNumBlockHorizontal() { return kNumBlockHorizontal; }

MapChipField::IndexSet MapChipField::GetMapChipIndexSetByPosition(const Vector3& position) {
	IndexSet indexSet = {};
	indexSet.xIndex = static_cast<uint32_t>((position.x + kBlockWidth / 2) / kBlockWidth);
	indexSet.yIndex = static_cast<uint32_t>(kNumBlockVertical - ((position.y + kBlockHeight / 2) / kBlockHeight));
	//indexSet.yIndex = static_cast<uint32_t>(kNumBlockVertical - 1 - ((position.y + kBlockHeight / 2) / kBlockHeight));

	return indexSet;
}

MapChipField::Rect MapChipField::GetRectByIndexSet(uint32_t xIndex, uint32_t yIndex) {
	Vector3 center = GetMapChipPositionByIndex(xIndex, yIndex);
	Rect rect = {};
	rect.left = center.x - kBlockWidth / 2.0f;
	rect.top = center.y + kBlockHeight / 2.0f;
	rect.right = center.x + kBlockWidth / 2.0f;
	rect.bottom = center.y - kBlockHeight / 2.0f;
	return rect;
}
