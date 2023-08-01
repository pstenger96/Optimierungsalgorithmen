#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"
#include "DataHolderT.h"

template<class Data>
class NeighbourI {

public:
	NeighbourI(std::shared_ptr<DataHolderT<Data>> d, std::shared_ptr<DataHolderT<Data>> currentBest,  std::shared_ptr<InitialSolutionI<Data>> initSol);
	
	virtual ~NeighbourI();
	virtual float optimize() = 0;
	virtual void resetData() = 0;
	virtual void initParameters() = 0;
	virtual void afterOptimization() = 0;
	virtual void postOptimStep(float newScore, float oldScore) = 0;
	virtual float getScoreInformation() = 0;

	std::string getIdentifier();
	
protected:
	std::shared_ptr<DataHolderT<Data>> data_;
	std::shared_ptr<DataHolderT<Data>> bestData_;
	std::shared_ptr<InitialSolutionI<Data>> initSol_;
	std::string identifier_;
};

template<class Data>
inline NeighbourI<Data>::NeighbourI(std::shared_ptr<DataHolderT<Data>> d, std::shared_ptr<DataHolderT<Data>> currentBest, std::shared_ptr<InitialSolutionI<Data>> initSol) : data_(d), bestData_(currentBest), initSol_(initSol)
{
	identifier_ = "";
}


template<class Data>
inline NeighbourI<Data>::~NeighbourI()
{
}

template<class Data>
inline std::string NeighbourI<Data>::getIdentifier()
{
	return identifier_;
}
