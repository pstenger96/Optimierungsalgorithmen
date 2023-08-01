#pragma once
#include "stdafx.h"
#include "DataHolder.h"

template<class Data>
class DataHolderT {
public:
	DataHolderT(Data dt);
	~DataHolderT();
	Data getData();
	void OverwriteData(std::shared_ptr<DataHolderT<Data>> other);
	void ResetData();
	void SoftResetData();
private:
	Data data_;
};

template<class Data>
inline DataHolderT<Data>::DataHolderT(Data dt)
{
	data_ = dt;
}

template<class Data>
inline DataHolderT<Data>::~DataHolderT()
{
}

template<class Data>
inline Data DataHolderT<Data>::getData()
{
	return data_;
}

template<class Data>
inline void DataHolderT<Data>::OverwriteData(std::shared_ptr<DataHolderT<Data>> other)
{
}

template<class Data>
inline void DataHolderT<Data>::ResetData()
{
}




template<>
inline void DataHolderT<std::shared_ptr<DataHolder>>::SoftResetData()
{
	data_->ResetBoundingBoxContent();
}


template<>
inline void DataHolderT<std::shared_ptr<DataHolder>>::OverwriteData(std::shared_ptr<DataHolderT<std::shared_ptr<DataHolder>>> other) {
	data_->OverwriteData(other->getData());
}




template<>
inline void DataHolderT<std::shared_ptr<DataHolder>>::ResetData() {
	if (UIConstants::useUI_)
		data_->ResetData();
	else data_->ResetBoundingBoxForTestEnv();
}