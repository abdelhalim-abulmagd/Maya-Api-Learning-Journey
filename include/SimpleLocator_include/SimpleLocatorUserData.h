#pragma once

#include <maya/MUserData.h>
#include <maya/MColor.h>

class SimpleLocatorUserData : public MUserData
{
public:
	//SimpleLocatorUserData(bool deleteAfterUse = false);
	SimpleLocatorUserData();

	enum ShapeType
	{
		Circle = 0, Square = 1, Traingle = 2
	};
	virtual ~SimpleLocatorUserData() override;

	int ShapeIndex;
	MColor Color;
};
