#pragma once
#include "stdafx.h"
#include "SortingStrategyI.h"
#include "DataHolder.h"
#include "RectangleCreator.h"
#include "RectangleHolder.h"
template<class Data>
class AreaSortingStrategy : public SortingStrategyI<Data> {
public:
	AreaSortingStrategy();
	virtual void sort(std::shared_ptr<DataHolderT<Data >> ) override;
	
};

template<class Data>
inline AreaSortingStrategy<Data>::AreaSortingStrategy()
{
	SortingStrategyI<Data>::identifier_ = "AreaSort";
}

template<class Data>
inline void AreaSortingStrategy<Data>::sort(std::shared_ptr<DataHolderT<Data >> data)
{
}

template<>
inline void AreaSortingStrategy<std::shared_ptr<DataHolder>>::sort(std::shared_ptr<DataHolderT<std::shared_ptr<DataHolder> >> data)
{
	std::shared_ptr<RectangleCreator> rectCreator = data->getData()->getRectCreator();
	std::vector<RectangleHolder*>* rects = rectCreator->getRectList();
	auto cmp = [](RectangleHolder* r1, RectangleHolder* r2) { return r1->getRect().width() * r1->getRect().height() > r2->getRect().width() * r2->getRect().height(); };
	std::sort(rects->begin(), rects->end(), cmp);
}