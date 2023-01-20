#include "ReplicationNode.h"
#include <maya/MGlobal.h>

MObject ReplicationNode::inputGeometry;
MObject ReplicationNode::outputGeometry;
MObject ReplicationNode::size;
MTypeId ReplicationNode::id(0x80000);

void* ReplicationNode::creator()
{
	return new ReplicationNode();
}

MStatus ReplicationNode::compute(const MPlug& plug, MDataBlock& data)

{
	MStatus returnStatus;

	if (plug == outputGeometry) {

		// Input handles
		MDataHandle inputGeometryHandle = data.inputValue(inputGeometry, &returnStatus);
		McheckErr(returnStatus, "Error getting angle data handle\n");

		MDataHandle sizeHandle = data.inputValue(size, &returnStatus);
		McheckErr(returnStatus, "Error getting step size data handle\n");

		// Output handle
		MDataHandle outputGeometryHandle = data.outputValue(outputGeometry, &returnStatus);
		McheckErr(returnStatus, "ERROR getting geometry data handle\n");

		// Mesh manipulation
		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		// IMPLEMENTATION HERE
		/*// Initialize LSystem 
		lsys = LSystem();
		double angle = angleData.asDouble();
		lsys.setDefaultAngle(angle);

		double stepsize = stepSizeData.asDouble();
		lsys.setDefaultStep(stepsize);

		// Load grammar program
		if (grammarData.asString() == "") {
			return MStatus::kSuccess;
		}
		lsys.loadProgram(grammarData.asString().asChar());

		// Recreate mesh
		std::vector<LSystem::Branch> branches;
		// const int frame = (int)time.as(MTime::kFilm);
		lsys.process(time.value(), branches);

		// Get base of LSystem tree
		MPoint initialStart = MPoint(branches[0].first[0], branches[0].first[2], branches[0].first[1]);
		MPoint initialEnd = MPoint(branches[0].second[0], branches[0].second[2], branches[0].second[1]);
		CylinderMesh baseCylinder = CylinderMesh(initialStart, initialEnd, 0.25);

		MPointArray points;
		MIntArray faceCounts;
		MIntArray faceConnects;
		baseCylinder.getMesh(points, faceCounts, faceConnects);

		// Add other branches
		for (int i = 1; i < branches.size(); i++) {
			LSystem::Branch currBranch = branches.at(i);
			vec3 startPoint = currBranch.first;
			vec3 endPoint = currBranch.second;

			addCylinder(points, faceCounts, faceConnects, startPoint, endPoint);
		}

		MFnMesh	meshFS;
		meshFS.create(points.length(), faceCounts.length(),
			points, faceCounts, faceConnects, newOutputData, &returnStatus);
		McheckErr(returnStatus, "ERROR creating new geometry");*/

		outputGeometryHandle.set(newOutputData);
		data.setClean(plug);
	}
	else
		return MS::kUnknownParameter;

	return MS::kSuccess;
}

MStatus ReplicationNode::initialize()
{
	MFnTypedAttribute geomAttr;
	MFnNumericAttribute sizeAttr;
	MStatus returnStatus;

	// Create attributes (initialization)
	ReplicationNode::inputGeometry = geomAttr.create("input_geometry", "i_geom",
		MFnData::kMesh,
		MObject::kNullObj, // initial data
		&returnStatus);
	McheckErr(returnStatus, "ERROR creating ReplicationNode input geometry attribute\n");

	ReplicationNode::size = sizeAttr.create("size", "s",
		MFnNumericData::kDouble,
		1.0,
		&returnStatus);
	McheckErr(returnStatus, "ERROR creating ReplicationNode size attribute\n");

	ReplicationNode::outputGeometry = geomAttr.create("output_geometry", "o_geom",
		MFnData::kMesh,
		MObject::kNullObj,
		&returnStatus);
	McheckErr(returnStatus, "ERROR creating ReplicationNode output geometry attribute\n");

	// Add attributes to node
	returnStatus = addAttribute(ReplicationNode::inputGeometry);
	McheckErr(returnStatus, "ERROR adding input geometry attribute\n");

	returnStatus = addAttribute(ReplicationNode::size);
	McheckErr(returnStatus, "ERROR adding size attribute\n");

	returnStatus = addAttribute(ReplicationNode::outputGeometry);
	McheckErr(returnStatus, "ERROR adding output geometry attribute\n");

	// Attribute affects (changing attributes should change output)
	returnStatus = attributeAffects(ReplicationNode::inputGeometry,
		ReplicationNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in input geometry attributeAffects\n");

	returnStatus = attributeAffects(ReplicationNode::size,
		ReplicationNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in sSize attributeAffects\n");

	return MS::kSuccess;
}