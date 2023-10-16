#pragma once

#include <vector>

#include "Log.h"
#include "item/Item.h"

// This is a custom class that inherits from the C++ vector class and as a vector with a maximum size number of items
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

// This is used for as an interface for the container, it is used because the program needs to be able have a weapon container and an item container which means there has to be an interface that allows them to be passed around and get items from it, however it then can be converted to the Weapon or Item container when items need to be put in them
class IContainer
{
  public:
	// Stores teh type
	enum Type
	{
		item,
		weapon
	};

  public:
	IContainer() {}
	virtual ~IContainer() {}

	// All these are pure virtual functions as they only act as an interface to the actual class
	virtual Type     getType() const          = 0;
	virtual Item *   getItem(int index) const = 0;   // This function returns an item pointer version of the item wanted (as weapon pointers can be converted into item pointers)
	virtual uint16_t size()                   = 0;
};