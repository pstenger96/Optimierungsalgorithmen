#pragma once 
#include "stdafx.h"
#include "DataHolderT.h"
#include "InitialSolutionI.h"
#include "OptimAlgoI.h"
#include "LocalSearch.h"
#include "RuleBasedNeighbour.h"
#include "GeometryBasedNeighbour.h"
#include "ObjectiveI.h"
#include "EmptyBoxObjective.h"
#include "SimpleEmptyBoxObjective.h"
#include "BoxListLengthObjective.h"
#include "Greedy.h"
#include "AreaSortingStrategy.h"
#include "DiagonalSortingStrategy.h"
#include "GeometryBasedOverlappingNeighbour.h"
class TestEnvironment {
public:
	TestEnvironment(int rectAmount, int boxLength);
	TestEnvironment(int instances, int rect_amount, int min_rect_width, int min_rect_height, int max_rect_width, int max_rect_height, int boxLenght);
	~TestEnvironment();
	void Run(std::string path);
private:
	void Protocoll(std::ofstream& file, float score, float time);
	void ProtocollNewLine(std::ofstream& file, int prefix);
	void setAlgorithm(int index);
	int instances_;
	

	std::shared_ptr<class RectangleCreator> rectCreator_;
	std::shared_ptr<class BoundingBoxCreator> boxCreator_;
	std::shared_ptr<DataHolderT<std::shared_ptr<class DataHolder>>> bestDataT_;
	std::shared_ptr<DataHolderT<std::shared_ptr<class DataHolder>>> dataT_;
	std::shared_ptr<class DataHolder> bestData_;
	std::shared_ptr<class DataHolder> data_;

	std::shared_ptr<InitialSolutionI<std::shared_ptr<DataHolder>>> initSol_;
	

	std::shared_ptr<OptimAlgoI<std::shared_ptr<class DataHolder>>> selectedAlgorithm_;
	std::shared_ptr<LocalSearch<std::shared_ptr<class DataHolder>>> localSearch_;
	std::shared_ptr<Greedy<std::shared_ptr<class DataHolder>>> greedy_;


	std::shared_ptr<EmptyBoxObjective<std::shared_ptr<class DataHolder>>> emptyBoxObjective_;
	std::shared_ptr<SimpleEmptyBoxObjective<std::shared_ptr<class DataHolder>>> simpleEmptyBoxObjective_;
	std::shared_ptr<BoxListLengthObjective<std::shared_ptr<class DataHolder>>> boxListLengthObjective_;

	std::shared_ptr<AreaSortingStrategy<std::shared_ptr<class DataHolder>>> areaSortStrategy_;
	std::shared_ptr<DiagonalSortingStrategy<std::shared_ptr<class DataHolder>>> diagonalSortStrategy_;
	std::shared_ptr<RuleBasedNeighbour<std::shared_ptr<class DataHolder>>> ruleBasedNeighbour_;
	std::shared_ptr<GeometryBasedNeighbour<std::shared_ptr<class DataHolder>>> geometryBasedNeighbour_;
	std::shared_ptr<GeometryBasedOverlappingNeighbour<std::shared_ptr<class DataHolder>>> geometryBasedOverlappingNeighbour_;


	std::vector<int> min_rect_width_vector_;
	std::vector<int> max_rect_width_vector_;
	std::vector<int> min_rect_height_vector_;
	std::vector<int> max_rect_height_vector_;
	int rectAmount_;
	
};