#pragma once
#include "ObjectiveI.h"
#include "DataHolderT.h"
#include "DataHolder.h"
#include "BoundingBox.h"
#include "BoundingBoxCreator.h"
template<class Data>
class BoxListLengthObjective : public ObjectiveI<Data> {
public:
	BoxListLengthObjective();
	virtual float calculateObjectiveScore(std::shared_ptr<DataHolderT<Data>> dt) override;

};

template<class Data>
inline BoxListLengthObjective<Data>::BoxListLengthObjective() : ObjectiveI<Data>()
{
}

template<class Data>
inline float BoxListLengthObjective<Data>::calculateObjectiveScore(std::shared_ptr<DataHolderT<Data>> dt)
{
	return 0.0f;
}

template<>
inline float BoxListLengthObjective<std::shared_ptr<DataHolder>>::calculateObjectiveScore(std::shared_ptr<DataHolderT<std::shared_ptr<DataHolder>>> dt)
{
	std::vector<std::shared_ptr<BoundingBox>> boxList;
	dt->getData()->getBoxCreator()->getBoundingBoxList(boxList);
	return (float) boxList.size();
}