#pragma once
#include "stdafx.h"
#include "ObjectiveI.h"
#include "DataHolderT.h"
#include "DataHolder.h"
#include "BoundingBox.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"

template<class Data>
class EmptyBoxObjective : public ObjectiveI<Data> {
public:
	EmptyBoxObjective();
	virtual float calculateObjectiveScore(std::shared_ptr<DataHolderT<Data>> dt) override;
	void setNeighbour(std::shared_ptr<NeighbourI<Data>> neighbour);

private:
	std::shared_ptr<NeighbourI<Data>> neighbour_;
};

template<class Data>
inline EmptyBoxObjective<Data>::EmptyBoxObjective()
{
}

template<class Data>
inline float EmptyBoxObjective<Data>::calculateObjectiveScore(std::shared_ptr<DataHolderT<Data>> dt)
{
	return 0.0f;
}

template<class Data>
inline void EmptyBoxObjective<Data>::setNeighbour(std::shared_ptr<NeighbourI<Data>> neighbour)
{
	neighbour_ = neighbour;
}

template<>
inline float EmptyBoxObjective<std::shared_ptr<DataHolder>>::calculateObjectiveScore(std::shared_ptr<DataHolderT<std::shared_ptr<DataHolder>>> dt)
{
	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	dt->getData()->getBoxCreator()->getBoundingBoxList(bBoxList);
	size_t bBoxListSize = bBoxList.size();
	size_t rectListSize = dt->getData()->getRectCreator()->getRectList()->size();

	float average = static_cast<float>(rectListSize) / static_cast<float>(bBoxListSize);
	float boxScore = 0;

	for (std::shared_ptr<BoundingBox> box : bBoxList) {
		float rectanglesInBox = static_cast<float>((box->getRectangleIndices()).size());

		if (average > rectanglesInBox) {
			boxScore += (average - rectanglesInBox) / average; // the emptier a box is, the better it is
		}

	}

	// upper bound of boxScore : length of rectangleList - 1
	// scoreInformation in Geometry Overlapping: transgressions_ * 1.1 - scoreShift_;
	// explanation for factor 1.1: 4 boxes + 1 transgession must be better than 3 boxes + 2 transgressions
	return ((bBoxListSize + neighbour_->getScoreInformation()) * (rectListSize - 1) - boxScore) * 0.2; // 0.2 to prevent overflow
}