#include "Optimierungsalgorithmen.h"
#include "MainWindow.h"
#include "AlgorithmSelectionUI.h"
#include "MainScene.h"
#include "RectangleDrawer.h"
#include "BoundingBoxDrawer.h"
#include "DataHolder.h"
#include "RectangleCreator.h"
#include "QNeighbourWrapper.h"
#include "InitialSolutionI.h"
#include "SimpleInitialSolution.h"
#include "OptimAlgoI.h"
#include "QAlgoWrapper.h"
#include "Drawer.h"
#include "stdafx.h"

//#include "Memory.h"
Optimierungsalgorithmen::Optimierungsalgorithmen(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    mainScene_ = new MainScene(this);


    mainWindow_ = new MainWindow(this, mainScene_);
    this->setCentralWidget(mainWindow_);

    algoSelectionUI_ = new AlgorithmSelectionUI();
    algoSelectionUI_->setParent(this);

    dataHolder_ = std::make_shared< DataHolder>();
    bestDataHolder_ = std::make_shared< DataHolder>();
    dataHolderT_ = std::make_shared< DataHolderT<std::shared_ptr<DataHolder>>>(dataHolder_);
    bestDataHolderT_ = std::make_shared< DataHolderT<std::shared_ptr<DataHolder>>>(bestDataHolder_);


    initSol_ = std::make_shared<SimpleInitialSolution<std::shared_ptr<DataHolder>>>();
    emptyBoxObjective_ = std::make_shared< EmptyBoxObjective<std::shared_ptr<DataHolder>>>();
    simpleEmptyBoxObjective_ = std::make_shared< SimpleEmptyBoxObjective<std::shared_ptr<DataHolder>>>();
    boxListLengthObjective_ = std::make_shared< BoxListLengthObjective<std::shared_ptr<DataHolder>>>();

    ruleBasedNeighbour_ = std::make_shared< RuleBasedNeighbour<std::shared_ptr<DataHolder>>>(dataHolderT_, bestDataHolderT_, initSol_);
    geometryBasedNeighbour_ = std::make_shared< GeometryBasedNeighbour<std::shared_ptr<DataHolder>>>(dataHolderT_, bestDataHolderT_, initSol_);
    geometryBasedOverlappingNeighbour_ = std::make_shared< GeometryBasedOverlappingNeighbour<std::shared_ptr<DataHolder>>>(dataHolderT_, bestDataHolderT_, initSol_);
    emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);

     neighbourWrapper_ = new QNeighbourWrapper(ruleBasedNeighbour_);
     neighbourWrapper_->setParent(this);
    localSearch_ = std::make_shared< LocalSearch<std::shared_ptr<DataHolder>>>(neighbourWrapper_->getNeighbourI(), dataHolderT_, bestDataHolderT_, initSol_, simpleEmptyBoxObjective_, boxListLengthObjective_);

    areaSortStrategy_ = std::make_shared< AreaSortingStrategy<std::shared_ptr<DataHolder>>>();
    diagonalSortStrategy_ = std::make_shared< DiagonalSortingStrategy<std::shared_ptr<DataHolder>>>();
    greedy_ = std::make_shared< Greedy<std::shared_ptr<DataHolder>>>(dataHolderT_, bestDataHolderT_, initSol_, areaSortStrategy_, simpleEmptyBoxObjective_, boxListLengthObjective_);

   

    selectedAlgorithm_ = localSearch_;

     algoWrapper_ = new QAlgoWrapper(selectedAlgorithm_);
     algoWrapper_->setParent(this);

    connect(algoSelectionUI_->getRecAmountSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedI);
    connect(algoSelectionUI_->getRecAmountLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectAmountChangedS);

    connect(algoSelectionUI_->getRecMinWidthSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinWidthChangedI);
    connect(algoSelectionUI_->getRecMinWidthLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinWidthChangedS);

    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxWidthChangedI, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxWidthChangedI);
    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxWidthChangedS, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxWidthChangedS);

    connect(algoSelectionUI_->getRecMinHeightSlider(), &QSlider::valueChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinHeightChangedI);
    connect(algoSelectionUI_->getRecMinHeightLineEdit(), &QLineEdit::textChanged, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMinHeightChangedS);

    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxHeightChangedI, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxHeightChangedI);
    connect(algoSelectionUI_, &AlgorithmSelectionUI::EmitMaxHeightChangedS, mainScene_->getRecDrawer(), &RectangleDrawer::DrawRectMaxHeightChangedS);

    connect(algoSelectionUI_->getBoxEdgeSlider(), &QSlider::valueChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedI);
    connect(algoSelectionUI_->getBoxEdgeLineEdit(), &QLineEdit::textChanged, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChangedS);

    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EdgeLengthChanged);
    connect(mainScene_->getBoxDrawer(), &BoundingBoxDrawer::BoundingBoxSizeChanged, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EdgeLengthChanged);

    //This sets boundingboxes after optimization is done
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitRectList, mainScene_->getBoxDrawer(), &BoundingBoxDrawer::SetBoundingBoxes);
    connect(bestDataHolder_->getRectCreator().get(), &RectangleCreator::EmitRectList, mainScene_->getRecDrawer(), &RectangleDrawer::SetRects);

    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::OptimDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::StepDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::OptimDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnOptimDone);
    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::StepDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnStepDone);
    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::OptimDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnOptimDone);
    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::StepDone, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::OnStepDone);
    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::OptimDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);
    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::StepDone, bestDataHolder_->getRectCreator().get(), &RectangleCreator::OnOptimDone);


    //Set FitScore after optim is done
    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::EmitSquareSize, bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::RecSquareSizeChanged);
    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::EmitSquareSize, dataHolder_->getBoxCreator().get(), &BoundingBoxCreator::RecSquareSizeChanged);
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitNumberOfBoundingBoxes, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentNumberLabel);
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitOptimalAmountRect, algoSelectionUI_, &AlgorithmSelectionUI::setOptimNumberLabel);
    connect(bestDataHolder_->getBoxCreator().get(), &BoundingBoxCreator::EmitCurrentFitScore, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentOptimPctLabel);

    //Upon changing parameters this updates rectList
    connect(mainScene_->getRecDrawer(), &RectangleDrawer::EmitRectInformation, dataHolder_->getRectCreator().get(), &RectangleCreator::CreateRects);
    connect(dataHolder_->getRectCreator().get(), &RectangleCreator::RectListUpdated, mainScene_->getRecDrawer(), &RectangleDrawer::SetRects);

    //AlgorithmChange/Start Logic
    connect(algoSelectionUI_->getAlgoSelectionBox(), &QComboBox::currentIndexChanged, this, &Optimierungsalgorithmen::changeAlgorithm);
    connect(algoSelectionUI_->getStartButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::RunUntilTermination);

    //Draw Solution
    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::DrawSolution, mainScene_->getDrawer(), &Drawer::DrawScene);
    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::DrawSolution, mainScene_->getDrawer(), &Drawer::DrawScene);

    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::DrawSwappedRects, mainScene_->getDrawer(), &Drawer::DrawSwappedRects);

    //Step Logic
    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::EmitCurrentStep, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentStepNumberLabel);
    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::EmitCurrentStep, algoSelectionUI_, &AlgorithmSelectionUI::setCurrentStepNumberLabel);
    connect(algoSelectionUI_->getNextStepButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::RunOneStep);

    //Reset Button
    connect(algoSelectionUI_->getResetButton(), &QPushButton::clicked, algoWrapper_, &QAlgoWrapper::Reset);

    //Timer Logic
    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::EmitTakenTime, algoSelectionUI_, &AlgorithmSelectionUI::setTimeDisplayLabel);
    connect(localSearch_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::EmitTakenTimeAvg, algoSelectionUI_, &AlgorithmSelectionUI::setAvgTimeDisplayLabel);

    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::EmitTakenTime, algoSelectionUI_, &AlgorithmSelectionUI::setTimeDisplayLabel);
    connect(greedy_.get(), &OptimAlgoI<std::shared_ptr<DataHolder>>::EmitTakenTimeAvg, algoSelectionUI_, &AlgorithmSelectionUI::setAvgTimeDisplayLabel);

    leftDock_ = new QDockWidget(this);
    leftDock_->setTitleBarWidget(new QWidget());
    leftDock_->setFeatures(QDockWidget::NoDockWidgetFeatures);
    leftDock_->setFixedSize(UIConstants::leftDockWidth_, UIConstants::leftDockHeight_);
    leftDock_->setWidget(algoSelectionUI_);
    this->addDockWidget(Qt::LeftDockWidgetArea, leftDock_);

    dataHolder_->getRectCreator()->CreateRects(AlgorithmConstants::initialAmount_, AlgorithmConstants::initialEdgeSize_, AlgorithmConstants::initialEdgeSize_, AlgorithmConstants::initialEdgeSize_, AlgorithmConstants::initialEdgeSize_);



}

Optimierungsalgorithmen::~Optimierungsalgorithmen()
{
    
}

void Optimierungsalgorithmen::changeAlgorithm(int idx)
{
    switch (idx) {
    case 0:
        neighbourWrapper_->setNeighbour(ruleBasedNeighbour_);
        localSearch_->setNeighbourDefinition(ruleBasedNeighbour_);
        emptyBoxObjective_->setNeighbour(ruleBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        algoWrapper_->setMod(2);
        break;
    case 1:
        neighbourWrapper_->setNeighbour(geometryBasedNeighbour_);
        localSearch_->setNeighbourDefinition(geometryBasedNeighbour_);
        emptyBoxObjective_->setNeighbour(geometryBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        algoWrapper_->setMod(2);
        break;
    case 2:
        neighbourWrapper_->setNeighbour(geometryBasedOverlappingNeighbour_);
        localSearch_->setNeighbourDefinition(geometryBasedOverlappingNeighbour_);
        emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);
        localSearch_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        algoWrapper_->setMod(2);
        break;
    case 3:
        greedy_->setSortStrat(areaSortStrategy_);
        greedy_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = greedy_;
        algoWrapper_->setMod(1);
        break;
    case 4:
        greedy_->setSortStrat(diagonalSortStrategy_);
        greedy_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = greedy_;
        algoWrapper_->setMod(1);
        break;
    }
    algoWrapper_->setAlgorithm(selectedAlgorithm_);
    algoWrapper_->Reset();
}

