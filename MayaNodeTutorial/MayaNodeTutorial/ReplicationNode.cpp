#include <fstream>
#include <sstream>
#include <vector>

#include "cylinder.h"
#include "ReplicationNode.h"
#include <maya/MGlobal.h>

MObject ReplicationNode::inputPositions;
MObject ReplicationNode::outputGeometry;
MObject ReplicationNode::radius;
MTypeId ReplicationNode::id(0x80000);

void* ReplicationNode::creator()
{
	MGlobal::displayInfo("In creator");
	return new ReplicationNode();
}

MStatus ReplicationNode::compute(const MPlug& plug, MDataBlock& data)

{
	MStatus returnStatus;

	if (plug == outputGeometry) {
		MGlobal::displayInfo("In conditional");
		// Input handles
		MDataHandle positionsHandle = data.inputValue(inputPositions, &returnStatus);
		McheckErr(returnStatus, "Error getting positions data handle\n");
		MString positionsString = positionsHandle.asString();
		if (positionsString == "") {
			return MS::kSuccess;
		}

		MDataHandle sizeHandle = data.inputValue(radius, &returnStatus);
		McheckErr(returnStatus, "Error getting size data handle\n");
		double r = sizeHandle.asDouble();

		// Output handle
		MDataHandle outputGeometryHandle = data.outputValue(outputGeometry, &returnStatus);
		McheckErr(returnStatus, "ERROR getting geometry data handle\n");

		// Mesh manipulation
		MFnMeshData dataCreator;
		MObject newOutputData = dataCreator.create(&returnStatus);
		McheckErr(returnStatus, "ERROR creating outputData");

		// Read input positions from text file
		std::vector<MPoint> worldPositions;

		std::string filePath = positionsString.asChar();
		MGlobal::displayInfo(filePath.c_str());

		std::fstream newFile;
		newFile.open(filePath, ios::in);

		if (newFile.is_open())
		{
			std::string line;
			while (getline(newFile, line))
			{
				MGlobal::displayInfo("In getline");
				std::vector<float> tokens;

				std::string token;
				std::istringstream s(line);
				while (s >> token) {
					tokens.push_back(std::stof(token));
				}

				MGlobal::displayInfo(std::to_string(tokens.size()).c_str());
				MPoint worldPos = MPoint(tokens[0], tokens[1], tokens[2]);
				MString worldPosStr = (std::to_string(worldPos.x) + " " + std::to_string(worldPos.y) + " " + std::to_string(worldPos.z)).c_str();
				MGlobal::displayInfo(worldPosStr);

				worldPositions.push_back(worldPos);
			}

			newFile.close();
		}

		if (worldPositions.size() > 0)
		{
			MGlobal::displayInfo(std::to_string(r).c_str());

			MPoint initialStart = MPoint(worldPositions[0].x, worldPositions[0].y, worldPositions[0].z);
			MPoint initialEnd = MPoint(worldPositions[1].x, worldPositions[1].y, worldPositions[1].z);
			CylinderMesh baseCylinder = CylinderMesh(initialStart, initialEnd, r);

			MPointArray points;
			MIntArray faceCounts;
			MIntArray faceConnects;
			baseCylinder.getMesh(points, faceCounts, faceConnects);

			for (int i = 1; i < worldPositions.size() - 1; i++) {
				MPoint startPoint = worldPositions[i];
				MPoint endPoint = worldPositions[i + 1];
				CylinderMesh cylinder = CylinderMesh(startPoint, endPoint, r);

				cylinder.appendToMesh(points, faceCounts, faceConnects);
			}

			MFnMesh	meshFS;
			meshFS.create(points.length(), faceCounts.length(),
				points, faceCounts, faceConnects, newOutputData, &returnStatus);
			McheckErr(returnStatus, "ERROR creating new geometry");
		}

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
	ReplicationNode::inputPositions = geomAttr.create("input_positions", "pos",
		MFnData::kString,
		MObject::kNullObj, // initial data
		&returnStatus);
	McheckErr(returnStatus, "ERROR creating ReplicationNode input positions attribute\n");

	ReplicationNode::radius = sizeAttr.create("radius", "r",
		MFnNumericData::kDouble,
		0.25,
		&returnStatus);
	McheckErr(returnStatus, "ERROR creating ReplicationNode size attribute\n");

	ReplicationNode::outputGeometry = geomAttr.create("output_geometry", "o_geom",
		MFnData::kMesh,
		MObject::kNullObj,
		&returnStatus);
	McheckErr(returnStatus, "ERROR creating ReplicationNode output geometry attribute\n");

	// Add attributes to node
	returnStatus = addAttribute(ReplicationNode::inputPositions);
	McheckErr(returnStatus, "ERROR adding input geometry attribute\n");

	returnStatus = addAttribute(ReplicationNode::radius);
	McheckErr(returnStatus, "ERROR adding size attribute\n");

	returnStatus = addAttribute(ReplicationNode::outputGeometry);
	McheckErr(returnStatus, "ERROR adding output geometry attribute\n");

	// Attribute affects (changing attributes should change output)
	returnStatus = attributeAffects(ReplicationNode::inputPositions,
		ReplicationNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in input positions attributeAffects\n");

	returnStatus = attributeAffects(ReplicationNode::radius,
		ReplicationNode::outputGeometry);
	McheckErr(returnStatus, "ERROR in size attributeAffects\n");

	return MS::kSuccess;
}