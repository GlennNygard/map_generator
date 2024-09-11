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
		m_remainingPossibleTypes = {};
	}

	const std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT>& get_remaining() {
		m_dirty = true;
		return m_remainingPossibleTypes;
	};

	template<size_t foliageCount>
	void set_remaining(std::array<int, foliageCount> dataArr) {
		m_remainingPossibleTypes = dataArr;
		m_dirty = true;
		hasData = true;
	}

	size_t get_remaining_length() {
		if(m_dirty) {
			if(!hasData) {
				m_remainingLength = m_remainingPossibleTypes.size();
			}
			else {
				int remCount = 0;
				for(int i = 0; i < m_remainingPossibleTypes.size(); i++) {
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

	private:

	std::array<int, FoliageHelpers::MAX_FOLIAGE_COUNT> m_remainingPossibleTypes;
	int m_remainingLength;
	bool m_dirty;
};