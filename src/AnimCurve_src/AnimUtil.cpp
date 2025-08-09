#include <AnimCurve_include/AnimUtil.h>

#include <maya/MFnAnimCurve.h>
#include <maya/MSyntax.h>
#include <maya/MArgDatabase.h>
#include <maya/MSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MGlobal.h>
#include <maya/MAnimControl.h>
#include <maya/MDGModifier.h>
#include <maya/MAnimCurveChange.h>
#include <maya/MPlug.h>

void AnimUtil::DisplayAnimCurveInfo(MFnAnimCurve& AnimCurveFn)
{
    MGlobal::displayInfo(MString("num of keys : ") + AnimCurveFn.numKeys());
    MGlobal::displayInfo(MString("is : ") + AnimCurveFn.isStatic());
    MGlobal::displayInfo(MString("pre Infinity Type : ") + AnimCurveFn.preInfinityType());
    MGlobal::displayInfo(MString("post Infinity Type : ") + AnimCurveFn.postInfinityType());

    for (unsigned i = 0; i < AnimCurveFn.numKeys(); i++)
    {
        MGlobal::displayInfo(MString("Time : ") + AnimCurveFn.time(i).value());
        MGlobal::displayInfo(MString("value : ") + AnimCurveFn.value(i));

        MGlobal::displayInfo("");

        MGlobal::displayInfo(MString("is Breakdown : ") + AnimCurveFn.isBreakdown(i));
        MGlobal::displayInfo(MString("tangents Locked : ") + AnimCurveFn.tangentsLocked(i));

        MGlobal::displayInfo("");

        MGlobal::displayInfo(MString("in Tangent Type : ") + AnimCurveFn.inTangentType(i));
        double x, y;
        AnimCurveFn.getTangent(i, x, y, true);
        MGlobal::displayInfo(MString("in Tangent value : [ ") + x + ", " + y + " ]");

        MGlobal::displayInfo("");
        MGlobal::displayInfo(MString("in Tangent Type : ") + AnimCurveFn.outTangentType(i));
        x, y;
        AnimCurveFn.getTangent(i, x, y, false);
        MGlobal::displayInfo(MString("in Tangent value : [ ") + x + ", " + y + " ]");

    }

}

MPlug  AnimUtil::FindPlug(const MObject& Object, const MString& AttributeName, MStatus& Status)
{
    MFnDependencyNode DependNodeFn(Object);
    MPlug AttributePlug = DependNodeFn.findPlug(AttributeName, false, &Status);
    return AttributePlug;
}

MStatus  AnimUtil::GetAnimCurveNode(const MObject& Object, const MString& AttributeName, MFnAnimCurve& AnimCurveFn)
{
    MStatus Status;

    MPlug AttributePlug = FindPlug(Object, AttributeName, Status);
    CHECK_MSTATUS_AND_RETURN_IT(Status);

    MPlug AnimCurveNode = AttributePlug.source(&Status);
    CHECK_MSTATUS_AND_RETURN_IT(Status);

    if (AnimCurveFn.hasObj(AnimCurveNode.node())) // تحدد ما إذا كانت مجموعة الوظائف متوافقة أم لا مع كائن 
        AnimCurveFn.setObject(AnimCurveNode.node());
    else
        return MS::kFailure;

    return Status;
}

MStatus  AnimUtil::CreateAnimCurveNode(const MObject& Object, const MString& AttributeName, MFnAnimCurve& AnimCurveFn, MDGModifier& DGModifier)
{
    MStatus Status;

    MPlug AttributePlug = FindPlug(Object, AttributeName, Status);
    CHECK_MSTATUS_AND_RETURN_IT(Status);

    if (AttributePlug.isConnected())
    {
        MS::kFailure;
    }
    else
    {
        // no need to call MDGModifier.doIt() here, coz the the create() method does that internaly
        AnimCurveFn.create(AttributePlug, MFnAnimCurve::kAnimCurveUnknown, &DGModifier, &Status);
        CHECK_MSTATUS_AND_RETURN_IT(Status);
    }
    return Status;

}

MStatus  AnimUtil::CreateAnimCurveNodes(const MObject& Object, const MStringArray& AttributeNames, MDGModifier& DGModifier)
{
    bool IsCreateCurve = false; // Check If Create At least One Curve for future use if need it
    for (MString AttributeName : AttributeNames)
    {
        if (CreateAnimCurveNode(Object, AttributeName, MFnAnimCurve(), DGModifier))
        {
            IsCreateCurve = true;
        }
    }

    if (IsCreateCurve)
    {
        return MS::kSuccess;
    }
    return MS::kFailure;
}

MStatus  AnimUtil::DeleteAnimCurveNodes(const MObject& Object, const MStringArray& AttributeNames, MDGModifier& DGModifier)
{
    bool IsDeletedCurve = false; // Check If Deleted At least One Curve for future use if need it

    MFnAnimCurve AnimCurveFn;

    for (MString AttributeName : AttributeNames)
    {
        if (GetAnimCurveNode(Object, AttributeName, AnimCurveFn) == MS::kSuccess)
        {
            DGModifier.deleteNode(AnimCurveFn.object());
            DGModifier.doIt();

            IsDeletedCurve = true;
        }
    }

    if (IsDeletedCurve)
    {
        return MS::kSuccess;
    }
    return MS::kFailure;
}

MTime  AnimUtil::GetCurrentTime()
{
    return MAnimControl::currentTime();
}

MStatus AnimUtil::AddKey(const MObject& Object, const MString& AttributeName)
{
    MStatus Status;
    MFnAnimCurve AnimCurveFn;

    Status = GetAnimCurveNode(Object, AttributeName, AnimCurveFn);
    if (!Status)
    {
        Status = CreateAnimCurveNode(Object, AttributeName, AnimCurveFn);
        CHECK_MSTATUS_AND_RETURN_IT(Status);
    }

    MTime CurrentTime = GetCurrentTime();
    unsigned Index = 0;
    bool isExist = AnimCurveFn.find(CurrentTime, Index);

    if (isExist == false)
    {
        AnimCurveFn.insertKey(CurrentTime, false, nullptr, &Status);
        return Status;
    }

    return MS::kFailure;
}
MStatus AnimUtil::RemoveKey(const MObject& Object, const MString& AttributeName)
{
    MStatus Status;
    MFnAnimCurve AnimCurveFn;

    Status = GetAnimCurveNode(Object, AttributeName, AnimCurveFn);
    if (!Status)
    {
        Status = CreateAnimCurveNode(Object, AttributeName, AnimCurveFn);
        CHECK_MSTATUS_AND_RETURN_IT(Status);
    }

    MTime CurrentTime = GetCurrentTime();
    unsigned Index = 0;
    bool isExist = AnimCurveFn.find(CurrentTime, Index);

    if (isExist)
    {
        Status = AnimCurveFn.remove(Index);

        if (AnimCurveFn.numKeys() == 0)
        {
            MDGModifier mod;
            mod.deleteNode(AnimCurveFn.object());
            mod.doIt();

        }
        return Status;
    }

    return MS::kFailure;

}

double AnimUtil::GetAttributeValue(const MObject& Object, const MString& AttributeName, MStatus& Status)
{
    MPlug AttributePlug = FindPlug(Object, AttributeName, Status);

    if (!Status || AttributePlug.isNull())
    {
        Status = MS::kFailure;
        return 0;
    }

    return AttributePlug.asDouble();
}

MStatus AnimUtil::SetKey(const MObject& Object, const MString& AttributeName, MAnimCurveChange& AnimCurveChange)
{
    MStatus Status;

    MFnAnimCurve AnimCurveFn;
    Status = GetAnimCurveNode(Object, AttributeName, AnimCurveFn);
    CHECK_MSTATUS_AND_RETURN_IT(Status);

    double AttributeValue = GetAttributeValue(Object, AttributeName, Status);
    CHECK_MSTATUS_AND_RETURN_IT(Status);

    MTime CurrentTime = GetCurrentTime();

    unsigned index;
    if (AnimCurveFn.find(CurrentTime, index) == false)
    {
        index = AnimCurveFn.insertKey(CurrentTime, false, &AnimCurveChange, &Status);
        CHECK_MSTATUS_AND_RETURN_IT(Status);
    }

    Status = AnimCurveFn.setValue(index, AttributeValue, &AnimCurveChange);

    return Status;
}
MStatus AnimUtil::SetKeys(const MObject& Object, const MStringArray& AttributeNames, MAnimCurveChange& AnimCurveChange)
{
    bool IsAnimCurveChange = false;

    for (MString AttributeName : AttributeNames)
    {
        if (SetKey(Object, AttributeName, AnimCurveChange) == MS::kSuccess)
        {
            IsAnimCurveChange = true;
        }
    }

    if (IsAnimCurveChange)
        return MS::kSuccess;


    return MS::kFailure;

}

MStatus AnimUtil::RemoveKey(const MObject& Object, const MString& AttributeName, MAnimCurveChange& AnimCurveChange)
{
    MStatus Status;

    MFnAnimCurve AnimCurveFn;
    Status = GetAnimCurveNode(Object, AttributeName, AnimCurveFn);
    CHECK_MSTATUS_AND_RETURN_IT(Status);

    MTime CurrentTime = GetCurrentTime();

    unsigned index;
    if (AnimCurveFn.find(CurrentTime, index) == true)
    {
        return AnimCurveFn.remove(index, &AnimCurveChange);
    }

    return MS::kFailure;
}
MStatus AnimUtil::RemoveKeys(const MObject& Object, const MStringArray& AttributeNames, MAnimCurveChange& AnimCurveChange)
{
    bool IsAnimCurveChange = false;

    for (MString AttributeName : AttributeNames)
    {
        if (RemoveKey(Object, AttributeName, AnimCurveChange) == MS::kSuccess)
        {
            IsAnimCurveChange = true;
        }
    }

    if (IsAnimCurveChange)
        return MS::kSuccess;

    return MS::kFailure;
}

MStatus AnimUtil::SetValue(MFnAnimCurve& AnimCurveFn, unsigned Index, double Value)
{
    return AnimCurveFn.setValue(Index, Value);
}

MStatus AnimUtil::SetTime(MFnAnimCurve& AnimCurveFn, unsigned Index, MTime Time)
{
    return AnimCurveFn.setTime(Index, Time);
}



