#pragma once

#include <maya/MFnAnimCurve.h>
#include <maya/MDGModifier.h>

class AnimUtil
{
public:

    void DisplayAnimCurveInfo(MFnAnimCurve& AnimCurveFn);

    static MPlug FindPlug(const MObject& Object, const MString& AttributeName, MStatus& Status);


    static MStatus GetAnimCurveNode(const MObject& Object, const MString& AttributeName, MFnAnimCurve& AnimCurveFn);

    static MStatus CreateAnimCurveNode(const MObject& Object, const MString& AttributeName, MFnAnimCurve& AnimCurveFn, MDGModifier& DGModifier = MDGModifier());


    static MStatus CreateAnimCurveNodes(const MObject& Object, const MStringArray& AttributeNames, MDGModifier& DGModifier = MDGModifier());

    static MStatus DeleteAnimCurveNodes(const MObject& Object, const MStringArray& AttributeNames, MDGModifier& DGModifier);

    static MTime GetCurrentTime();


    static MStatus AddKey(const MObject& Object, const MString& AttributeName);

    static MStatus RemoveKey(const MObject& Object, const MString& AttributeName);


    static double GetAttributeValue(const MObject& Object, const MString& AttributeName, MStatus& Status);

    static MStatus SetKey(const MObject& Object, const MString& AttributeName, MAnimCurveChange& AnimCurveChange);

    static MStatus SetKeys(const MObject& Object, const MStringArray& AttributeNames, MAnimCurveChange& AnimCurveChange);


    static MStatus RemoveKey(const MObject& Object, const MString& AttributeName, MAnimCurveChange& AnimCurveChange);

    static MStatus RemoveKeys(const MObject& Object, const MStringArray& AttributeNames, MAnimCurveChange& AnimCurveChange);


    static MStatus SetValue(MFnAnimCurve& AnimCurveFn, unsigned Index, double Value);


    static MStatus SetTime(MFnAnimCurve& AnimCurveFn, unsigned Index, MTime Time);

};



