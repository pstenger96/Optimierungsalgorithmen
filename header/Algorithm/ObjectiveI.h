#pragma once
#include "stdafx.h"
#include "DataHolderT.h"
template<class Data>
class ObjectiveI {
public:
	ObjectiveI();
	virtual float calculateObjectiveScore(std::shared_ptr<DataHolderT<Data>> dt) = 0;
};

template<class Data>
inline ObjectiveI<Data>::ObjectiveI()
{
}

