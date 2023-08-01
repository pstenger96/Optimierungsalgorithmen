#include "QNeighbourWrapper.h"
#include "NeighbourI.h"
#include "DataHolder.h"
QNeighbourWrapper::QNeighbourWrapper(std::shared_ptr<NeighbourI<std::shared_ptr<class DataHolder>>>  n) : neighbour_(n) {

}

void QNeighbourWrapper::Optim() {
	
	neighbour_->optimize();
}

std::shared_ptr<NeighbourI<std::shared_ptr<class DataHolder>>> QNeighbourWrapper::getNeighbourI()
{
	return neighbour_;
}

void QNeighbourWrapper::setNeighbour(std::shared_ptr<NeighbourI<std::shared_ptr<class DataHolder>>> n)
{
	neighbour_ = n;
}
