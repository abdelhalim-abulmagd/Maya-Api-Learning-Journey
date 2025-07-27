#pragma once

#include <maya/MUserData.h>
#include <maya/MPoint.h>
#include <maya/MString.h>
#include <maya/MColor.h>

class DistanceBetweenLocatorUserData : public MUserData
{
public:
	DistanceBetweenLocatorUserData();

	virtual ~DistanceBetweenLocatorUserData() override;

	MPoint Point1, Point2;
	MPoint TextPosition;
	double Distance;

	MColor Color;

};