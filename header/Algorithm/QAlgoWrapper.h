#pragma once
#include "stdafx.h"
#include "OptimAlgoI.h"
#include "DataHolder.h"
class QAlgoWrapper : public QObject {
	Q_OBJECT
public:
	QAlgoWrapper(std::shared_ptr<OptimAlgoI<std::shared_ptr<DataHolder>>> alg);
	std::shared_ptr<OptimAlgoI<std::shared_ptr<DataHolder>>> getAlgo();
	void setAlgorithm(std::shared_ptr<OptimAlgoI<std::shared_ptr<DataHolder>>> alg);
	void setMod(int mod);
public slots:
	void RunUntilTermination();
	void RunOneStep();
	void Reset();
private:
	class std::shared_ptr<OptimAlgoI<std::shared_ptr<DataHolder>>> algo_;
	int stepIdx_;
	int mod_;
	bool isDone_;
};