#pragma once

#include <optional>

#include "foliage.h"


struct FoliageData {

	public:

	bool hasData;
	bool isWalkableOnly;

	FoliageData() {
		hasData = false;
		isWalkableOnly = false;
		_dirty = false;
		_remainingLength = 0;
	}

	std::optional<std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)>> get_remaining_possible_types() {
		_dirty = true;
		return _remainingPossibleTypes;
	};

	template<size_t size>
	void set_remaining_possible_types(std::array<int, size> dataArr) {
		_remainingPossibleTypes = dataArr;
		_dirty = true;
		hasData = true;
	};

	int get_remaining_length() {
		if(_dirty) {
			if(!hasData) {
				_remainingLength = static_cast<int>(FoliageType::Foliage_MAX);
			}
			else {
				int remCount = 0;
				for(int i = 0; i < static_cast<int>(FoliageType::Foliage_MAX); i++) {
					if((*_remainingPossibleTypes)[i] <= 0) {
						continue;
					}
					remCount++;
				}
				_remainingLength = remCount;
			}
			_dirty = false;
		}
		return _remainingLength;
	}

	template<size_t size>
	void set_remaining_from_array(std::array<int, size> dataArr) {
		if(!_remainingPossibleTypes) {
			_remainingPossibleTypes = std::optional<std::array<int, size>>();
		}

		int dataArrLength = size;
		for(int i = 0; i < dataArrLength; i++) {
			(*_remainingPossibleTypes)[i] = dataArr[i];
		}
		_dirty = true;
		hasData = true;
	}

	private:

	std::optional<std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)>> _remainingPossibleTypes;
	int _remainingLength;
	bool _dirty;
};