#pragma once
#include "stdafx.h"
#include "InitialSolutionI.h"
#include "DataHolderT.h"
#include "ObjectiveI.h"
struct Metric {
public: Metric(float s, float t, bool b) {
	score_ = s;
	time_ = t;
	isDone_ = b;
	
}
	  float score_;
	  float time_;
	  bool isDone_;
};

class SignalHelper : public QObject {
	Q_OBJECT
signals:
	void OptimDone();
	void StepDone();
	void DrawSolution(class BoundingBoxCreator* bBoxCreator, bool isDone);
	void DrawSwappedRects();
	void EmitCurrentStep(int step);
	void EmitTakenTime(double time);
	void EmitTakenTimeAvg(double time);

};
template<class Data>
class OptimAlgoI : public SignalHelper{
public:
	OptimAlgoI(std::shared_ptr<DataHolderT<Data>> dt, std::shared_ptr<DataHolderT<Data>> bestSol, std::shared_ptr<InitialSolutionI<Data>> init, std::shared_ptr<ObjectiveI<Data>> algoObjective, std::shared_ptr<ObjectiveI<Data>> cmpObjective);
	virtual Metric execute(int steps) = 0;
	virtual void reset() = 0;
	void setObjective(std::shared_ptr<ObjectiveI<Data>> objective);
	std::string getIdentifier();
	std::shared_ptr<DataHolderT<Data>> getBestSol();
protected:
	int currentStep_;
	float currentBestScore_;
	std::shared_ptr<DataHolderT<Data>> currentSol_;
	std::shared_ptr<DataHolderT<Data>> bestSol_;
	std::shared_ptr<InitialSolutionI<Data>> initSol_;
	std::shared_ptr<ObjectiveI<Data>> algoObjective_;
	std::shared_ptr<ObjectiveI<Data>> cmpObjective_;
	double currentTimeTaken_;
	std::string identifier_;
};


template<class Data>
inline OptimAlgoI<Data>::OptimAlgoI(std::shared_ptr<DataHolderT<Data>> dt, std::shared_ptr<DataHolderT<Data>> bestSol, std::shared_ptr<InitialSolutionI<Data>> init, std::shared_ptr<ObjectiveI<Data>> algoObjective, std::shared_ptr<ObjectiveI<Data>> cmpObjective)
{
	identifier_ = "";
	currentStep_ = -1;
	currentTimeTaken_ = 0.0;
	currentBestScore_ = AlgorithmConstants::maxScore;
	currentSol_ = dt;
	bestSol_ = bestSol;
	initSol_ = init;
	algoObjective_ = algoObjective;
	cmpObjective_ = cmpObjective;
}

template<class Data>
inline void OptimAlgoI<Data>::setObjective(std::shared_ptr<ObjectiveI<Data>> objective)
{
	algoObjective_ = objective;
}

template<class Data>
inline std::string OptimAlgoI<Data>::getIdentifier()
{
	return identifier_;
}

template<class Data>
inline std::shared_ptr<DataHolderT<Data>> OptimAlgoI<Data>::getBestSol()
{
	return bestSol_;
}
