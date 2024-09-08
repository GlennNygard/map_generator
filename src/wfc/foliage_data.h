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
		m_dirty = false;
		m_remainingLength = 0;
	}

	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> get_remaining_possible_types() {
		m_dirty = true;
		return m_remainingPossibleTypes;
	};

	template<size_t size>
	void set_remaining_possible_types(std::array<int, size> dataArr) {
		m_remainingPossibleTypes = dataArr;
		m_dirty = true;
		hasData = true;
	};

	int get_remaining_length() {
		if(m_dirty) {
			if(!hasData) {
				m_remainingLength = static_cast<int>(FoliageType::Foliage_MAX);
			}
			else {
				int remCount = 0;
				for(int i = 0; i < static_cast<int>(FoliageType::Foliage_MAX); i++) {
					if(m_remainingPossibleTypes[i] <= 0) {
						continue;
					}
					remCount++;
				}
				m_remainingLength = remCount;
			}
			m_dirty = false;
		}
		return m_remainingLength;
	}

	template<size_t size>
	void set_remaining_from_array(std::array<int, size> dataArr) {
		// if(!m_remainingPossibleTypes) {
		// 	m_remainingPossibleTypes = std::array<int, size>();
		// }

		int dataArrLength = size;
		for(int i = 0; i < dataArrLength; i++) {
			m_remainingPossibleTypes[i] = dataArr[i];
		}
		m_dirty = true;
		hasData = true;
	}

	private:

	std::array<int, static_cast<size_t>(FoliageType::Foliage_MAX)> m_remainingPossibleTypes;
	int m_remainingLength;
	bool m_dirty;
};