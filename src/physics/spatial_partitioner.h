#pragma once
#include <vector>

namespace sim {
class Particle;

template <typename InfoType, typename ValueType>
class SpatialPartitioner
{
public:
    virtual void add(InfoType& entity) = 0;
    virtual void remove(InfoType& entity) = 0;
    virtual std::vector<ValueType> getNearby(InfoType& entity) = 0;
};

}