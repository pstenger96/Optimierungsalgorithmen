#include "TestEnvironment.h"
#include "RectangleCreator.h"
#include "BoundingBoxCreator.h"
#include "DataHolder.h"
#include "DataHolderT.h"
#include "SimpleInitialSolution.h"
TestEnvironment::TestEnvironment(int rectAmount, int boxLength)
{
    
    min_rect_width_vector_ = { 20, 10,  10, 30, 10, 20 };
    max_rect_width_vector_ = { 20, 20,  15, 50, 50, 30 };
    min_rect_height_vector_ = { 20, 10,  30, 10, 10, 20 };
    max_rect_height_vector_ = { 20, 20, 50, 15, 50, 30 };
   
    /*
    std::random_device rd;
    std::mt19937 engine(rd());
    std::uniform_int_distribution<> dist_width(5, 50);
    std::uniform_int_distribution<> dist_height(5, 50);
    for (int s = 0; s < 5; ++s) {
        int w1 = dist_width(engine);
        int w2 = dist_width(engine);
        int h1 = dist_height(engine);
        int h2 = dist_height(engine);

        min_rect_width_vector_.emplace_back(std::min(w1, w2));
        max_rect_width_vector_.emplace_back(std::max(w1, w2));
        min_rect_height_vector_.emplace_back(std::min(h1, h2));
        max_rect_height_vector_.emplace_back(std::max(h1, h2));
    }

    for (int q = 0; q < min_rect_height_vector_.size(); ++q) {
        std::cout << min_rect_width_vector_[q] << " " << max_rect_width_vector_[q] << "\n";
        std::cout << min_rect_height_vector_[q] << " " << max_rect_height_vector_[q] << "\n";
        std::cout << "\n";
    }
    */

    rectAmount_ = rectAmount;

    instances_ = min_rect_height_vector_.size();
    int min_rect_width = min_rect_width_vector_[0];
    int min_rect_height = min_rect_height_vector_[0];
    int max_rect_width = max_rect_width_vector_[0];
    int max_rect_height = max_rect_height_vector_[0];
    bestData_ = std::make_shared< DataHolder>(rectAmount_, min_rect_width, min_rect_height, max_rect_width, max_rect_height, boxLength);
    data_ = std::make_shared< DataHolder>(rectAmount_, min_rect_width, min_rect_height, max_rect_width, max_rect_height, boxLength);
    bestDataT_ = std::make_shared <DataHolderT<std::shared_ptr<DataHolder>>>(bestData_);
    dataT_ = std::make_shared< DataHolderT<std::shared_ptr<DataHolder>>>(data_);

    initSol_ = std::make_shared< SimpleInitialSolution<std::shared_ptr<DataHolder>>>();
    emptyBoxObjective_ = std::make_shared< EmptyBoxObjective<std::shared_ptr<DataHolder>>>();
    simpleEmptyBoxObjective_ = std::make_shared< SimpleEmptyBoxObjective<std::shared_ptr<DataHolder>>>();
    boxListLengthObjective_ = std::make_shared< BoxListLengthObjective<std::shared_ptr<DataHolder>>>();

    ruleBasedNeighbour_ = std::make_shared< RuleBasedNeighbour<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_);
    geometryBasedNeighbour_ = std::make_shared< GeometryBasedNeighbour<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_);
    geometryBasedOverlappingNeighbour_ = std::make_shared< GeometryBasedOverlappingNeighbour<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_);
    emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);


    localSearch_ = std::make_shared <LocalSearch<std::shared_ptr<DataHolder>>>(ruleBasedNeighbour_, dataT_, bestDataT_, initSol_, simpleEmptyBoxObjective_, boxListLengthObjective_);

    areaSortStrategy_ = std::make_shared< AreaSortingStrategy<std::shared_ptr<DataHolder>>>();
    diagonalSortStrategy_ = std::make_shared< DiagonalSortingStrategy<std::shared_ptr<DataHolder>>>();
    greedy_ = std::make_shared< Greedy<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_, areaSortStrategy_, simpleEmptyBoxObjective_, boxListLengthObjective_);



    selectedAlgorithm_ = localSearch_;
}

TestEnvironment::TestEnvironment(int instances, int rect_amount, int min_rect_width, int min_rect_height, int max_rect_width, int max_rect_height, int boxLength)
{
    bestData_ = std::make_shared< DataHolder>(rectAmount_, min_rect_width, min_rect_height, max_rect_width, max_rect_height, boxLength);
    data_ = std::make_shared< DataHolder>(rectAmount_, min_rect_width, min_rect_height, max_rect_width, max_rect_height, boxLength);
    bestDataT_ = std::make_shared <DataHolderT<std::shared_ptr<DataHolder>>>(bestData_);
    dataT_ = std::make_shared< DataHolderT<std::shared_ptr<DataHolder>>>(data_);

    initSol_ = std::make_shared< SimpleInitialSolution<std::shared_ptr<DataHolder>>>();
    emptyBoxObjective_ = std::make_shared< EmptyBoxObjective<std::shared_ptr<DataHolder>>>();
    simpleEmptyBoxObjective_ = std::make_shared< SimpleEmptyBoxObjective<std::shared_ptr<DataHolder>>>();
    boxListLengthObjective_ = std::make_shared< BoxListLengthObjective<std::shared_ptr<DataHolder>>>();

    ruleBasedNeighbour_ = std::make_shared< RuleBasedNeighbour<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_);
    geometryBasedNeighbour_ = std::make_shared< GeometryBasedNeighbour<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_);
    geometryBasedOverlappingNeighbour_ = std::make_shared< GeometryBasedOverlappingNeighbour<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_);
    emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);


    localSearch_ = std::make_shared <LocalSearch<std::shared_ptr<DataHolder>>>(ruleBasedNeighbour_, dataT_, bestDataT_, initSol_, simpleEmptyBoxObjective_, boxListLengthObjective_);

    areaSortStrategy_ = std::make_shared< AreaSortingStrategy<std::shared_ptr<DataHolder>>>();
    diagonalSortStrategy_ = std::make_shared< DiagonalSortingStrategy<std::shared_ptr<DataHolder>>>();
    greedy_ = std::make_shared< Greedy<std::shared_ptr<DataHolder>>>(dataT_, bestDataT_, initSol_, areaSortStrategy_, simpleEmptyBoxObjective_, boxListLengthObjective_);


    selectedAlgorithm_ = localSearch_;



    instances_ = min_rect_height_vector_.size();
}

TestEnvironment::~TestEnvironment() {

    min_rect_width_vector_.clear();
    min_rect_width_vector_.shrink_to_fit();

    max_rect_width_vector_.clear();
    max_rect_width_vector_.shrink_to_fit();

    min_rect_height_vector_.clear();
    min_rect_height_vector_.shrink_to_fit();

    max_rect_height_vector_.clear();
    max_rect_width_vector_.shrink_to_fit();

  
}

void TestEnvironment::Run(std::string path)
{
    std::ofstream file;
    file.open(path);


    Metric metric = { 0.0, 0.0 , false };
    for (int i = 0; i < instances_; i++) {
        std::cout << "Instance " << i << "\n";
        data_->ResetRectanglesForTestEnv(rectAmount_, min_rect_width_vector_[i], max_rect_width_vector_[i], min_rect_height_vector_[i], max_rect_height_vector_[i]);
        std::cout << min_rect_width_vector_[i] << " " << max_rect_width_vector_[i] << "\n";
        std::cout << min_rect_height_vector_[i] << " " << max_rect_height_vector_[i] << "\n";
        std::cout << "\n";
        for (int k = 0; k < AlgorithmConstants::amountAlgorithms_; ++k) {

            //if (k != 2) continue;
            std::cout << "Algo " << k << "\n";
            setAlgorithm(k);
            ProtocollNewLine(file, k);
            file << "[" + std::to_string(min_rect_width_vector_[i]) + "-" + std::to_string(max_rect_width_vector_[i]) + "] [" + std::to_string(min_rect_height_vector_[i]) + "-" + std::to_string(max_rect_height_vector_[i]) + "]" + ",";
            for (int j = 0; j < 900; j++) {
                // std::cout << std::to_string(j) + "\n";
                if (((k == 3 || k == 4) && j == 0) || k < 3) {
                    metric = selectedAlgorithm_->execute(1);
                }
                Protocoll(file, metric.score_, metric.time_);
            }
            if (i == 5 && k == 0) {
                int dbg = 0;
            }
            //std::cout << "First phase over \n";
            if (k < 3) {
                selectedAlgorithm_->execute(AlgorithmConstants::maxIterations - 990);
            }
            for (int u = 0; u < 90; u++) {
                if (k < 3) {
                    metric = selectedAlgorithm_->execute(1);
                }
                Protocoll(file, metric.score_, metric.time_);
            }
        
            file << "," << "[" + std::to_string(min_rect_width_vector_[i]) + "-" + std::to_string(max_rect_width_vector_[i]) + "] [" + std::to_string(min_rect_height_vector_[i]) + "-" + std::to_string(max_rect_height_vector_[i]) + "]" + ",";
            file << std::to_string(k) + ". " + selectedAlgorithm_->getIdentifier() + ",";
        }
       
        file << "\n";
    }
    file.close();
    std::cout << "done \n";
}

void TestEnvironment::Protocoll(std::ofstream& file, float score, float time)
{
    std::stringstream stream;
    stream << std::fixed << std::setprecision(3) << score;
    stream << " after " << std::fixed << std::setprecision(2) << time << "ms,";

    std::string s = stream.str();
    const char* chars = s.c_str();

    file << chars;

}

void TestEnvironment::ProtocollNewLine(std::ofstream& file, int prefix)
{
    file << "\n" + std::to_string(prefix) + ". " + selectedAlgorithm_->getIdentifier() + ",";
}

void TestEnvironment::setAlgorithm(int index)
{

    switch (index) {
    case 0:
        localSearch_->setNeighbourDefinition(ruleBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        break;
    case 1:

        localSearch_->setNeighbourDefinition(geometryBasedNeighbour_);
        localSearch_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        break;
    case 2:
        localSearch_->setNeighbourDefinition(geometryBasedOverlappingNeighbour_);
        emptyBoxObjective_->setNeighbour(geometryBasedOverlappingNeighbour_);
        localSearch_->setObjective(emptyBoxObjective_);
        selectedAlgorithm_ = localSearch_;
        break;
    case 3:
        greedy_->setSortStrat(areaSortStrategy_);
        greedy_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = greedy_;
        break;
    case 4:
        greedy_->setSortStrat(diagonalSortStrategy_);
        greedy_->setObjective(simpleEmptyBoxObjective_);
        selectedAlgorithm_ = greedy_;
        break;
    }
    selectedAlgorithm_->reset();

}
