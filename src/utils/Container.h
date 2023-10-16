#pragma once

#include <vector>

#include "Item.h"

template <typename T>
class Container : public std::vector<T>
{
	uint16_t maxSize;

  public:
	Container(uint16_t max)
		: maxSize(max)
	{
		std::vector<T>::reserve(maxSize);
	}
	~Container()
	{
	}

	bool     isFull() { return std::vector<T>::size() == maxSize; }
	void     setMaxSize(int maxSize) { maxSize = maxSize; }
	uint16_t getMaxSize() { return maxSize; }

	void push_back(const T &element)
	{
		if(std::vector<T>::size() < maxSize)
			std::vector<T>::push_back(element);
		else
		{
			Log::warning("Cannot store anymore!");
		}
	}

	void insert(typename std::vector<T>::iterator it, const T &element)
	{
		if(std::vector<T>::size() < maxSize)
			std::vector<T>::insert(it, element);
		else
		{
			Log::warning("Cannot store anymore!");
		}
	}
};

class IContainer
{
  public:
	enum Type
	{
		item,
		weapon
	};

  public:
	IContainer() {}
	virtual ~IContainer() {}

	virtual Type     getType() const          = 0;
	virtual Item *   getItem(int index) const = 0;
	virtual uint16_t size()                   = 0;
};