#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Optimierungsalgorithmen.h"
#include "LocalSearch.h"
#include "RuleBasedNeighbour.h"
#include "GeometryBasedNeighbour.h"
#include "GeometryBasedOverlappingNeighbour.h"
#include "OptimAlgoI.h"
#include "DataHolderT.h"
#include "Greedy.h"
#include "AreaSortingStrategy.h"
#include "DiagonalSortingStrategy.h"
#include "ObjectiveI.h"
#include "EmptyBoxObjective.h"
#include "SimpleEmptyBoxObjective.h"
#include "BoxListLengthObjective.h"
class Optimierungsalgorithmen : public QMainWindow
{
    Q_OBJECT

public:
    Optimierungsalgorithmen(QWidget* parent = Q_NULLPTR);
    ~Optimierungsalgorithmen();

public slots:
    void changeAlgorithm(int idx);
private:
    Ui::OptimierungsalgorithmenClass ui;
    QDockWidget* leftDock_;

    class MainScene* mainScene_;
    class MainWindow* mainWindow_;
    class AlgorithmSelectionUI* algoSelectionUI_;

    std::shared_ptr<DataHolderT<std::shared_ptr<class DataHolder>>> dataHolderT_;
    std::shared_ptr<DataHolderT<std::shared_ptr<class DataHolder>>> bestDataHolderT_;

    std::shared_ptr<class DataHolder> dataHolder_;
    std::shared_ptr<class DataHolder> bestDataHolder_;

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
    std::shared_ptr<InitialSolutionI<std::shared_ptr<class DataHolder>>> initSol_;
    class QNeighbourWrapper* neighbourWrapper_;
    class QAlgoWrapper* algoWrapper_;

};
