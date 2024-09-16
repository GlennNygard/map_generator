#pragma once

#include <array>

#include "foliage.h"
#include "logger.h"


/**
 * 
 * This simple container just aims to nest 2 vectors of (moderately) known sizes
 * into a single array for quick memory access.
 * 
 */
template<size_t MaxOuterSize, size_t MaxInnerSize>
class FoliageSquashedList {

public:
    using Element = std::pair<FoliageType, int>;

    static const int INNER_OFFSET = 1;

    FoliageSquashedList() : m_sizeA(0), m_squashedList({}) {}

    FoliageSquashedList(const std::vector<std::vector<Element>>& nestedList) {

        m_sizeA = nestedList.size();

        if(m_sizeA > MaxOuterSize) {
            throw std::out_of_range(std::format("Vector to be squashed is too large. {} >= {}",
                m_sizeA, static_cast<int>(MaxOuterSize)));
        }

        bool nestedListTooLarge = false;
        for(size_t vecInd = 0; vecInd < m_sizeA; vecInd++) {
            auto innerList = nestedList[vecInd];

            int innerSize = innerList.size();
            if(innerSize > MaxInnerSize) {
                throw std::out_of_range(std::format("Vector to be squashed is too large. {} >= {}",
                    m_sizeA, static_cast<int>(MaxOuterSize)));
            }

            int offsetVecInd = vecInd * (MaxInnerSize * INNER_OFFSET);
            m_squashedList[offsetVecInd] = std::make_pair<FoliageType, int> (innerSize, 0);
            for(size_t nestInd = 0; nestInd < innerSize; nestInd++) {

                m_squashedList[offsetVecInd + nestInd + INNER_OFFSET] = innerList[nestInd];
            }
        }
    }

    size_t SizeA() const {
        return m_sizeA;
    }

    size_t SizeB(int a) const {
        return m_squashedList.at(a).first;
    }

    Element At(int a, int b) const {
        return m_squashedList.at(a*(MaxInnerSize + INNER_OFFSET) + b + 1);
    }

private:
    int m_sizeA;

    std::array<Element, MaxOuterSize * (MaxInnerSize + INNER_OFFSET)> m_squashedList;
};
