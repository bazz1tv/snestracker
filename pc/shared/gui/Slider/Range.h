//
//  Range.h
//  MeatBall
//
//  Created by bazz on 2/11/13.
//  Copyright (c) 2013 Student. All rights reserved.
//

#pragma once

template <class T>
class Range
{
public:
	Range(T min, T max) { this->min = min; this->max = max; }
	~Range()
	{
		//min = (T)0.0;
		//max = (T)0.0;
	}
	void setMin(T min)
	{
		this->min = min;
	}
	void setMax(T max)
	{
		this->max = max;
	}
	
	
	T max, min;
};
