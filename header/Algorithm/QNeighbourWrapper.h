#pragma once
#include "stdafx.h"
#include "NeighbourI.h"
class QNeighbourWrapper : public QObject {
	Q_OBJECT
public:
	QNeighbourWrapper(class std::shared_ptr<NeighbourI<std::shared_ptr<class DataHolder>>> n);
	std::shared_ptr<NeighbourI<std::shared_ptr<class DataHolder>>> getNeighbourI();
	void setNeighbour(std::shared_ptr<NeighbourI<std::shared_ptr<class DataHolder>>> n);
public slots:
	void Optim();
private:
	std::shared_ptr<NeighbourI<std::shared_ptr<class DataHolder>>> neighbour_;
};