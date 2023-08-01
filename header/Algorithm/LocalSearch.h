#pragma once
#include "OptimAlgoI.h"
#include "NeighbourI.h"
#include "InitialSolutionI.h"
#include "stdafx.h"
template<class Data>
class LocalSearch : public OptimAlgoI<Data>{

public:
	LocalSearch(std::shared_ptr<NeighbourI<Data>> n, std::shared_ptr<DataHolderT<Data>> sol, std::shared_ptr<DataHolderT<Data>> bestSol, std::shared_ptr<InitialSolutionI<Data>> initSol, std::shared_ptr<ObjectiveI<Data>> algoObjective, std::shared_ptr<ObjectiveI<Data>> cmpObjective);
	~LocalSearch();
	//returns optimal box amount
	virtual Metric execute(int steps) override;
	virtual void reset() override;
	void setNeighbourDefinition(std::shared_ptr<NeighbourI<Data>> n);

private:
	std::shared_ptr<NeighbourI<Data>> neighbourDefinition_;
	//Data solution;
	
	
};

template<class Data>
inline LocalSearch<Data>::LocalSearch(std::shared_ptr<NeighbourI<Data>> n, std::shared_ptr<DataHolderT<Data>> sol, std::shared_ptr<DataHolderT<Data>> bestSol, std::shared_ptr<InitialSolutionI<Data>> initSol, std::shared_ptr<ObjectiveI<Data>> algoObjective, std::shared_ptr<ObjectiveI<Data>> cmpObjective)
	: OptimAlgoI<Data>(sol, bestSol, initSol, algoObjective, cmpObjective)
{
	neighbourDefinition_ = n;
	
	OptimAlgoI<Data>::identifier_ = "LocalSearch " + n->getIdentifier();
}

template<class Data>
LocalSearch<Data>::~LocalSearch()
{
	neighbourDefinition_ = nullptr;
	//solution = nullptr;
}


template<class Data>
Metric LocalSearch<Data>::execute(int steps)
{
	auto t1 = std::chrono::high_resolution_clock::now();

	/*
	if (OptimAlgoI<Data>::currentStep_ == -1) {
		OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_, true);
		OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
		neighbourDefinition_->resetData();
		OptimAlgoI<Data>::currentStep_++;
		neighbourDefinition_->initParameters();
	}
	*/
	
	int steps_left = 0;
	while((!UIConstants::useUI_ && steps_left < steps) || (OptimAlgoI<Data>::currentTimeTaken_ < AlgorithmConstants::maxTime_ - AlgorithmConstants::timeOverhead_ && UIConstants::useUI_) && steps_left < steps){
		neighbourDefinition_->optimize();
		float newScore = OptimAlgoI<Data>::algoObjective_->calculateObjectiveScore(OptimAlgoI<Data>::currentSol_);
		float oldBestScore = OptimAlgoI<Data>::currentBestScore_;
		if (newScore < OptimAlgoI<Data>::currentBestScore_) {
			OptimAlgoI<Data>::currentBestScore_ = newScore;
			OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
		}

		neighbourDefinition_->postOptimStep(newScore, oldBestScore);
		steps_left++;

		if(!UIConstants::useUI_ && steps_left % 1000 == 0)
		std::cout << std::to_string(steps_left) + "\n";

		OptimAlgoI<Data>::currentStep_++;
		emit OptimAlgoI<Data>::EmitCurrentStep(OptimAlgoI<Data>::currentStep_);

		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::duration<double, std::milli>(t2 - t1);
		OptimAlgoI<Data>::currentTimeTaken_ += ms.count();
		t1 = std::chrono::high_resolution_clock::now();
		
	}
	neighbourDefinition_->afterOptimization();	

	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration<double, std::milli>(t2 - t1);
	OptimAlgoI<Data>::currentTimeTaken_ += ms.count();

	bool isDone = (OptimAlgoI<Data>::currentTimeTaken_ >= AlgorithmConstants::maxTime_ - AlgorithmConstants::timeOverhead_);

	emit OptimAlgoI<Data>::StepDone();
	emit OptimAlgoI<Data>::EmitTakenTime(OptimAlgoI<Data>::currentTimeTaken_);
	emit OptimAlgoI<Data>::EmitTakenTimeAvg(OptimAlgoI<Data>::currentTimeTaken_ /(double) OptimAlgoI<Data>::currentStep_);
	//emit OptimAlgoI<Data>::DrawSolution();
	return Metric::Metric(OptimAlgoI<Data>::cmpObjective_->calculateObjectiveScore(OptimAlgoI<Data>::bestSol_), OptimAlgoI<Data>::currentTimeTaken_, isDone);
}

template<class Data>
inline void LocalSearch<Data>::reset()
{

	OptimAlgoI<Data>::currentSol_->ResetData();
	OptimAlgoI<Data>::bestSol_->ResetData();
	OptimAlgoI<Data>::currentBestScore_ = AlgorithmConstants::maxScore;;
	OptimAlgoI<Data>::currentStep_ = 0;
	OptimAlgoI<Data>::currentTimeTaken_ = 0.0;

	auto t1 = std::chrono::high_resolution_clock::now();

	OptimAlgoI<Data>::initSol_->CreateInitialSolution(OptimAlgoI<Data>::currentSol_, true);
	OptimAlgoI<Data>::bestSol_->OverwriteData(OptimAlgoI<Data>::currentSol_);
	neighbourDefinition_->resetData();
	neighbourDefinition_->initParameters();


	emit OptimAlgoI<Data>::EmitCurrentStep(OptimAlgoI<Data>::currentStep_);
	emit OptimAlgoI<Data>::StepDone();
	//emit OptimAlgoI<Data>::DrawSolution();

	auto t2 = std::chrono::high_resolution_clock::now();
	auto ms = std::chrono::duration<double, std::milli>(t2 - t1);
	OptimAlgoI<Data>::currentTimeTaken_ += ms.count();
	emit OptimAlgoI<Data>::EmitTakenTime(OptimAlgoI<Data>::currentTimeTaken_);
	emit OptimAlgoI<Data>::EmitTakenTimeAvg(OptimAlgoI<Data>::currentTimeTaken_);
}

template<class Data>
inline void LocalSearch<Data>::setNeighbourDefinition(std::shared_ptr<NeighbourI<Data>> n)
{
	neighbourDefinition_ = n;
	OptimAlgoI<Data>::identifier_ = "LocalSearch " + n->getIdentifier();
}



