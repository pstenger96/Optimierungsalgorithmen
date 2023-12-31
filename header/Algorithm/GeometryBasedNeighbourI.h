#pragma once
#include <BoundingBox.h>
#include "NeighbourI.h"
#include "DataHolder.h"
#include "BoundingBoxCreator.h"
#include "RectangleCreator.h"
#include "InitialSolutionI.h"
#include "BoundingBox.h"
#include "RectangleHolder.h"


template<class Data>
class GeometryBasedNeighbourI : public NeighbourI<Data>
{
public:
	GeometryBasedNeighbourI(std::shared_ptr<DataHolderT<Data>> data, std::shared_ptr<DataHolderT<Data>> currentBest, std::shared_ptr<InitialSolutionI<Data>> initSol);
	virtual ~GeometryBasedNeighbourI();
	virtual float optimize() = 0;
	virtual void afterOptimization() override;
	virtual void postOptimStep(float newScore, float oldScore) override;
	virtual void resetData() override;

	virtual void initParameters() = 0;
	virtual void beforeScoreCalculation(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isTransgressionRect) = 0;
	virtual bool tryFitWrapper(std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIdx, std::vector<class RectangleHolder*>* rectangles, int rectIdx, bool multipleRects) = 0;
	virtual int getRectPos(const int rectListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList, bool& isTransgressionRect) = 0;
	virtual int getBoxPos(const int boxListSize, std::vector<std::shared_ptr<BoundingBox>>& boxList) = 0;
	virtual void resetBoxPos() = 0;
	
	// for overlapping
	virtual void decreaseT(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, bool isBetter) = 0;
	virtual int calculateOverlappingWrapper(std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& bBoxList) = 0;

protected:
	int iteration_;
	int initalNumberOfBoxes_;
	bool fitBoundingBox(std::vector<int> indices, std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex);
	bool removeRectFromBox(std::shared_ptr<BoundingBox>& oldBox, int oldBoxIdx, int rectIdx, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, std::vector<class RectangleHolder*>* rectList, std::shared_ptr<BoundingBox>* newBox = NULL);
	void handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex);
	bool resetData_;
	float findNeighbour(bool withoutOverlapping);
	void setAllToDefaultColors(std::vector<class RectangleHolder*>* rectList);

	bool overlapping_;
	int numberOfAddedBoxes_;
	
	int noBetterNeighbourFound_; // number of iterations
};


template<class Data>
inline GeometryBasedNeighbourI<Data>::GeometryBasedNeighbourI(std::shared_ptr<DataHolderT<Data>> data, std::shared_ptr<DataHolderT<Data>> currentBest, std::shared_ptr<InitialSolutionI<Data>> initSol) : NeighbourI<Data>(data, currentBest, initSol) {
	resetData_ = false;
	overlapping_ = false;
	numberOfAddedBoxes_ = 0;
	iteration_ = 0;
	noBetterNeighbourFound_ = 0;
}

template<class Data>
inline GeometryBasedNeighbourI<Data>::~GeometryBasedNeighbourI()
{
}


template<>
inline void GeometryBasedNeighbourI<std::shared_ptr<DataHolder>>::afterOptimization()
{
}


// note: this method doesn't update index vector of the bounding box
template<>
inline bool GeometryBasedNeighbourI<std::shared_ptr<DataHolder>>::fitBoundingBox(std::vector<int> indices, std::vector<class RectangleHolder*>* rectangles, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex) {
	boxList[boxIndex]->removeLowerLevelBoundingBoxes();

	// place all rectangles specified in indices at the given bounding box
	for (int i : indices) {
		if (!this->tryFitWrapper(boxList, boxIndex, rectangles, i, true)) {
			return false;
		}
	}
	return true;
}

template<>
inline bool GeometryBasedNeighbourI<std::shared_ptr<DataHolder>>::removeRectFromBox(std::shared_ptr<BoundingBox>& oldBox, int oldBoxIdx, int rectIdx, std::vector<std::shared_ptr<BoundingBox>>& bBoxList, std::vector<class RectangleHolder*>* rectList, std::shared_ptr<BoundingBox>* newBox) {
	oldBox->removeRectangleIndex(rectIdx);

	// replace all rectangles previous bounding box
	if (this->fitBoundingBox(oldBox->getRectangleIndices(), rectList, bBoxList, oldBoxIdx)) {
		if (newBox != nullptr) (*newBox)->addRectangleIndex(rectIdx);

		// delete bounding box in case it is empty
		if (oldBox->getRectangleIndices().size() == 0) {
			this->handleEmptyBoundingBox(data_->getData()->getBoxCreator(), bBoxList, oldBoxIdx);
		}

		return true;
	}

	else {
		oldBox->addRectangleIndex(rectIdx);
		return false;
	}
}

template<class Data>
inline void GeometryBasedNeighbourI<Data>::handleEmptyBoundingBox(std::shared_ptr<BoundingBoxCreator> boxCreator, std::vector<std::shared_ptr<BoundingBox>>& boxList, int boxIndex)
{
	boxCreator->resetOneBoundingBox(boxIndex);
	boxCreator->getBoundingBoxList(boxList);
}




template<>
inline float GeometryBasedNeighbourI<std::shared_ptr<DataHolder>>::findNeighbour(bool withoutOverlapping) {
	++iteration_;
	bool foundNeighbour = false;
	int changedBox1, changedBox2, changedBox1newIndex;
	bool deleteBox1 = false;
	overlapping_ = !withoutOverlapping;

	// get bounding box list
	std::shared_ptr<BoundingBoxCreator> boxCreator = data_->getData()->getBoxCreator();
	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	boxCreator->getBoundingBoxList(bBoxList);

	// get rectangle list
	std::shared_ptr<RectangleCreator> rectCreator = data_->getData()->getRectCreator();
	std::vector<class RectangleHolder*>* rectList = rectCreator->getRectList();
	size_t rectListSize = rectList->size();

	if (rectListSize <= 1) {
		return 0;
	}




	/******* NEIGHBOUR: Place a rectangle at another bounding box and in case it doesn't fit, rotate the rectangle and try to place again *******/

	size_t iteration = 0;

	// iterate through rectangles until a rectangle is found which can be placed in a rotated way
	while (!foundNeighbour && iteration < rectListSize) {

		// 1) select a rectangle
		bool isTransgressionRect = false;
		int rectIdx = this->getRectPos(rectListSize, bBoxList, isTransgressionRect);
		RectangleHolder* rectHolder = (*rectList)[rectIdx];
		QRectF& rect = rectHolder->getRectRef();

		// skip rectangle with a probability of 70 % in case its area is greater than 75 % of the bounding box
		while (!isTransgressionRect && rect.width() * rect.height() > 0.75 * bBoxList[0]->getBoxWidth() * bBoxList[0]->getBoxWidth()) {
			if (rand() % 100 < 30) break;
			// std::cout << "skipped rectangle with idx " << rectIdx << std::endl;
			rectIdx = this->getRectPos(rectListSize, bBoxList, isTransgressionRect);
			RectangleHolder* rectHolder = (*rectList)[rectIdx];
			QRectF& rect = rectHolder->getRectRef(); // get a rectangle from list, start with the last one
		}
		
		bool hasRotated = false;

		// 2) get index of rectangle's current bounding box
		int oldBoxIndex = -1;
		// oldBoxIndex = (*rectList)[rectIdx]->getBoundingBoxIndex(); TODO: doesn't work properly!
		for (int boxIdx = 0; boxIdx < bBoxList.size(); ++boxIdx) {
			std::vector<int> vec = bBoxList[boxIdx]->getRectangleIndices();
			if (std::find(vec.begin(), vec.end(), rectIdx) != vec.end()) {
				oldBoxIndex = boxIdx;
			}
		}
		std::shared_ptr<BoundingBox>& oldBox = bBoxList[oldBoxIndex];

		if (oldBoxIndex < 0) {
			std::cout << "**** ERROR: Rectangle in no bounding box ****" << std::endl;
			return AlgorithmConstants::maxScore;
		}

		// std::cout << "CHOOSE RECT " << rectIdx << " from box " << (*rectList)[rectIdx]->getBoundingBoxIndex() << std::endl;
		// std::cout << "CHOOSE RECT " << rectIdx << " from box " << oldBoxIndex << std::endl;

		// 3) try to move to another bounding box
		int boxIteration = 0;
		int newBoxIdx = -1;
		this->resetBoxPos();
		while (boxIteration++ < bBoxList.size()) {
			newBoxIdx = this->getBoxPos(bBoxList.size(), bBoxList);

			// std::cout << "try box  " << newBoxIdx << std::endl;

			if (newBoxIdx == oldBoxIndex) {
				continue; // must be handled separatly
			}

			std::shared_ptr<BoundingBox>& newBox = bBoxList[newBoxIdx];

			bool rectFitsInBox = this->tryFitWrapper(bBoxList, newBoxIdx, rectList, rectIdx, false);
			if (!rectFitsInBox) {
				// rotate rectangle and try to place again
				rectHolder->rotateRect();
				hasRotated = true;
				rectFitsInBox = this->tryFitWrapper(bBoxList, newBoxIdx, rectList, rectIdx, false);
			}
			
			if (rectFitsInBox) {
				int oldBoxListSize = bBoxList.size();
				// std::cout << "rect fits in box " << rectFitsInBox << std::endl;
				foundNeighbour = this->removeRectFromBox(oldBox, oldBoxIndex, rectIdx, bBoxList, rectList, &newBox);
				if (foundNeighbour) {
					// std::cout << "place at box " << newBoxIdx << std::endl;
					this->setAllToDefaultColors(rectList);
					(*rectList)[rectIdx]->setToSwappedColor();
					this->beforeScoreCalculation(rectList, bBoxList, isTransgressionRect);
					changedBox1 = oldBoxIndex;
					changedBox2 = newBoxIdx;
					deleteBox1 = bBoxList.size() < oldBoxListSize;
				}
				else { // this rectangle can not be replaced since old bounding box cannot be resorted
					data_->getData()->OverwriteData(bestData_->getData());
					boxCreator->getBoundingBoxList(bBoxList);
					// std::cout << "old box cannot be reordered" << std::endl;
				}
				break;

			}
			else {
				if (hasRotated) { // reset rotation
					hasRotated = false;
					rectHolder->rotateRect();
				}
			}
		}

		// add bounding box
		if (!foundNeighbour && overlapping_ && isTransgressionRect && rand() % 100 < 80 && oldBox->getRectangleIndices().size() > 1) {
			if (this->removeRectFromBox(oldBox, oldBoxIndex, rectIdx, bBoxList, rectList)) {
				//std::cout << "added box" << std::endl;
				int amount = rectListSize;
				int recsPerLine = std::min(UIConstants::maxBoxesPerLine, (int)std::ceil(std::sqrt(amount)));
				int x_pos = ((rectListSize+numberOfAddedBoxes_) % recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
				int y_pos = (int)((rectListSize + numberOfAddedBoxes_) / (float)recsPerLine) * (AlgorithmConstants::maxBoxEdgeSize_ + UIConstants::rectangleSpace_);
				++numberOfAddedBoxes_;

				boxCreator->addBoundingBox(x_pos, y_pos, (*rectList)[rectIdx], rectIdx, bBoxList.size());
				boxCreator->getBoundingBoxList(bBoxList);
				foundNeighbour = true;
				changedBox1 = oldBoxIndex;
				changedBox2 = int(bBoxList.size()) - 1;
				this->beforeScoreCalculation(rectList, bBoxList, isTransgressionRect);
				break;
			 }
			else { // this rectangle can not be replaced since old bounding box cannot be resorted
				// std::cout << "old Box cannot be reordered" << std::endl;

				data_->getData()->OverwriteData(bestData_->getData());
				boxCreator->getBoundingBoxList(bBoxList);
			}
		}

		++iteration;
	}

	//if (foundNeighbour == false) //std::cout << "No neighbour" << std::endl;

	return 0;
}

template<>
inline void GeometryBasedNeighbourI<std::shared_ptr<DataHolder>>::postOptimStep(float newScore, float oldScore)
{
	if (newScore >= oldScore) {
		data_->OverwriteData(bestData_);
		++noBetterNeighbourFound_;
	}
	else {
		noBetterNeighbourFound_ = 0;
	}

	std::vector<std::shared_ptr<BoundingBox>> bBoxList;
	data_->getData()->getBoxCreator()->getBoundingBoxList(bBoxList);
	std::vector<class RectangleHolder*>* rectList = data_->getData()->getRectCreator()->getRectList();

	this->decreaseT(rectList, bBoxList, newScore < oldScore);
}

template<class Data>
inline void GeometryBasedNeighbourI<Data>::resetData()
{
	noBetterNeighbourFound_ = 0;
}

template<class Data>
inline void GeometryBasedNeighbourI<Data>::setAllToDefaultColors(std::vector<class RectangleHolder*>* rectList)
{
	for (RectangleHolder* rect : *rectList) {
		rect->setToDefaultColor();
	}
}
