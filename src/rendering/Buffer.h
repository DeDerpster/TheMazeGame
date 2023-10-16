#pragma once

#include "Log.h"

template <typename T>
class Buffer : public std::vector<T *>
{
  private:
	std::array<uint32_t, 11> layersLoc;   // last layer is there for safety reasons and just stores the size of the vector

  public:
	Buffer()
	{
		memset(&layersLoc, 0, sizeof(layersLoc));
	}
	~Buffer()
	{
		for(T *obj : *this)
			delete obj;
	}

	void addElement(T *obj, uint8_t layer, bool orderByYAxis)
	{
		if(layer >= layersLoc.size() - 1)   // NOTE: this skips the last layer because it for safety
		{
			Log::warning("Buffer layer given is to high!");
			layer = layersLoc.size() - 2;
		}

		int layerSize = layersLoc[layer + 1] - layersLoc[layer];

		if(orderByYAxis && layerSize > 0)
		{
			float objY = obj->position.y;
			// If the size is one it does a quick evaluation to see where to place it
			if(layerSize == 1)
			{
				auto it = begin() + layersLoc[layer];
				if((*it)->position.y > objY)
					insert(it + layerSize, obj);
				else
					insert(it, obj);
			}
			else
			{
				// This is a modified binomial search to find the range of where to place the new element
				// These variables keeps track of the range of elements of the set
				int  startSub   = 0;
				int  endSub     = layerSize;
				int  index      = (startSub + endSub) / 2;
				auto layerBegin = begin() + layersLoc[layer];

				auto getYOf = [layerBegin](int index) -> float {
					return (*(layerBegin + index))->position.y;
				};

				while(startSub != endSub && startSub < endSub)   // This continues going until the range is 0
				{
					if(index + 1 >= layerSize)
						index = layerSize - 2;   // If it is looking at the end of the list, it minuses 1, so that the algorithm can work without any errors
					else if(index < 0)
						index = 0;

					// Gets the y positions of the elements at the current index and above it
					float thisY = getYOf(index);
					float nextY = getYOf(index + 1);

					if(nextY > thisY)   // Checks to see if the vector is sorted incorrectly
					{
						Log::critical("Buffer is not sorted correctly!", LOGINFO);
						index = size();
						break;
					}

					// Checks to see if it has found a location to place the new element
					if(objY == thisY || (thisY > objY && objY > nextY))
					{
						index++;
						break;
					}
					else if(objY == nextY)
					{
						index += 2;
						break;
					}
					else if(objY < nextY)   // Otherwise it modifies the range for the binomial search to work
						startSub = index + 2;
					else if(objY > thisY)
						endSub = index;

					index = (startSub + endSub) / 2;   // Sets the index to be inbetween the new ranges
				}

				if(index > layerSize)
				{
					Log::warning("The index is over its range!");
					insert(begin() + layersLoc[layer + 1], obj);
				}
				else
				{
					if(index > 0 && index < layerSize)
					{
						if(getYOf(index - 1) < objY || objY < getYOf(index))
						{
							Log::critical("Index is incorrectly calculated", LOGINFO);
						}
					}
					insert(layerBegin + index, obj);   // Inserts the object at the correct position
				}
			}
		}
		else
		{
			// Inserts it at the end of the layer wanted
			if(layersLoc[layer + 1] == size())
				push_back(obj);
			else
				insert(begin() + layersLoc[layer + 1], obj);
		}

		// updates locations of the layers
		for(int i = layer + 1; i < layersLoc.size(); i++)
			layersLoc[i]++;
	}

	uint32_t getLayerPos(uint8_t layer)
	{
		if(layer >= layersLoc.size())   // NOTE: this skips the last layer because it for safety
		{
			Log::warning("Buffer layer given is to high!");
			layer = layersLoc.size() - 1;
		}
		return layersLoc[layer];
	}
	uint32_t getLayerSize(uint8_t layer)
	{
		if(layer >= layersLoc.size())   // NOTE: this skips the last layer because it for safety
		{
			Log::warning("Buffer layer given is to high!");
			layer = layersLoc.size() - 1;
		}
		return layersLoc[layer + 1] - layersLoc[layer];
	}

	void clear()
	{
		// Clears the layers information
		memset(&layersLoc, 0, sizeof(layersLoc));
		std::vector<T *>::clear();
	}
};