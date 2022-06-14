/*
 * Imported from vangers-mtool project
 */

#ifndef ONCE_MODEL_CLASS
#define ONCE_MODEL_CLASS

#include <stdint.h>

namespace mtool {
	typedef int8_t Vertex8[3];
	typedef float Vertex[3];
	typedef uint32_t FaceIndices[3];

	typedef double DBV[3];
	typedef double DBM[9];
	typedef int32_t Vector[3];

	// INGAME & INTERNAL groups
	enum ModelTypes
	{
		MODEL_UNKNOWN,
		MODEL_DEBRIS,
		MODEL_SLOT,
		MODEL_WHEEL,
		MODEL_BODY,
	};

	//// Colors for list box
	//const COLORREF ModelColors[5] =
	//{
	//	// RGB(128, 185, 255),
	//	// RGB(128, 220, 255),
	//	RGB(255, 155, 128), // MODEL_UNKNOWN
	//	RGB(128, 255, 192), // MODEL_DEBRIS
	//	RGB(192, 255, 128), // MODEL_SLOT
	//	RGB(255, 238, 128), // MODEL_WHEEL
	//	RGB(255, 210, 128)	// MODEL_BODY
	//};

	// INTERNAL materials IDs
	enum ModelMaterials
	{
		MODEL_MATERIAL_UNKNOWN,
		MODEL_MATERIAL_BODY,
		MODEL_MATERIAL_GLASS,
		MODEL_MATERIAL_METAL,
		MODEL_MATERIAL_TUBE,
		MODEL_MATERIAL_WHEEL,
		MODEL_MATERIAL_DEBRIS,
		MODEL_MATERIAL_SLOT
	};

	// INTERNAL materials
	const float Materials[9][3] =
	{
		0.75f, 0.00f, 0.00f, // MODEL_MATERIAL_UNKNOWN
		0.35f, 0.65f, 0.00f, // MODEL_MATERIAL_BODY
		0.35f, 0.15f, 0.35f, // MODEL_MATERIAL_GLASS
		0.25f, 0.25f, 0.25f, // MODEL_MATERIAL_METAL
		0.10f, 0.10f, 0.10f, // MODEL_MATERIAL_TUBE
		0.15f, 0.15f, 0.15f, // MODEL_MATERIAL_WHEEL
		0.85f, 0.35f, 0.05f, // MODEL_MATERIAL_DEBRIS
		0.65f, 0.65f, 0.65f  // MODEL_MATERIAL_SLOT
	};

	// INGAME colors IDs
	enum ColorsIDs
	{
		COLOR_RESERVED,

		COLOR_BODY,
		COLOR_GLASS,
		COLOR_WHEEL,
		COLOR_METAL,
		COLOR_SLOT,
		COLOR_TUBE,

		COLOR_BODY_RED,
		COLOR_BODY_BLUE,
		COLOR_BODY_YELLOW,
		COLOR_BODY_GRAY,
		COLOR_YELLOW_CHARGED,

		MATERIAL_0,
		MATERIAL_1,
		MATERIAL_2,
		MATERIAL_3,
		MATERIAL_4,
		MATERIAL_5,
		MATERIAL_6,
		MATERIAL_7
	};

	// Face info
	struct Face
	{
		FaceIndices Vertices;
		FaceIndices Normals;

		ColorsIDs ColorID;
		int Reflection;
		float Area;
	};

	// Variable face
	struct Face_v
	{
		int VerticesCount;

		uint32_t *Vertices;
		uint32_t *Normals;

		ColorsIDs ColorID;
		int Reflection;
		float Area;
	};

	/*
	const COLORREF ModelColor[7] =
	{
		RGB(255, 255, 255), // MODEL_UNKNOWN
		RGB(230, 230, 230), // MODEL_SLOT
		RGB(215, 215, 215), // MODEL_DEBRIS
		RGB(200, 200, 200), // MODEL_WHEEL
		RGB(185, 185, 185), // MODEL_WINDOW
		RGB(170, 170, 170), // MODEL_METAL
		RGB(155, 155, 155)  // MODEL_BODY
	};
	*/

	// Simple params
	struct ModelParamsStruct
	{
		ColorsIDs ColorID;	// Body, Wheel, Debris
		uint8_t Reflection;	// Body, Wheel, Debris

		uint8_t WheelSteer;	// Wheel only
		int SlotID;			// Slot only
		int SlotAngle;		// Slot only
	};

	enum CheckPlanes
	{
		PLANE_XY,
		PLANE_YZ,
		PLANE_ZX
	};

	// Results of projection
	struct ProjectionResultsStruct
	{
		double T0;
		double Tx;
		double Ty;
		double Tz;

		double Txx;
		double Tyy;
		double Tzz;

		double Txy;
		double Tyz;
		double Tzx;
	};

	// Params from projection
	struct ProjectionParamsStruct
	{
		// Input
		int Size_X;
		int Size_Y;
		int Size_Z;

		float Scale;
		uint8_t OnlyUpperBuffer;
		Vertex *LocalVertices;

		// Output
		struct ProjectionResultsStruct prs;
	};

	// Physical params
	struct PhysicalParamsStruct
	{
		double Volume;
		DBV MassCenter;
		DBM IntertiaTensor;
	};

	// XYZ sorting...
	struct SortData
	{
		float FaceCenter;
		int FaceIndex;

		SortData *Next;
	};

	// This struct is used to understand
	// which faces contains particular vertex
	// Indices is the indices of the vertex in faces
	struct VertexLink
	{
		int Reflection;		// Reflection of the vertex
		ColorsIDs ColorID;	// Color of the vertex
		float TotalArea;	// Total area of all faces

		int FacesCount;		// Count of the faces
		uint32_t *Indices;		// Indices of the vertex in faces
		uint32_t *Faces;		// Indices of the faces
	};

	// Model definition
	class Model
	{
	public:
		// Information section
		char *ModelName;
		ModelTypes ModelType;
		ModelMaterials ModelMaterial;

		// Index in the models list
		int ModelIndex;
		uint8_t ModelEnabled;
		uint8_t ModelBoxed;

		ModelParamsStruct ModelParams;

		// Geometrics section
		int VerticesCount;
		Vertex *Vertices;

		int FacesNormalsCount;
		Vertex *FacesNormals;

		int VerticesNormalsCount;
		Vertex *VerticesNormals;

		int FacesCount;
		Face *Faces;

		// Additional section
		Vertex Max, Min;
		Vertex Offset;

		// ---------------
		// MODEL'S METHODS
		// ---------------

		Model();
		~Model();

		void SetParamsUsingName(char *Name);
		void SetModelName(const char *Name, bool AutoParams);

		void SetOffset(Vertex &MinusOffset);
		void SetScale(float Scale);

		float CalculateRadius(bool ModelBasis);
		bool CalculatePhysicalParams(PhysicalParamsStruct &pps, bool OnlyUpperBuffer);
		bool CalculateProjection(ProjectionParamsStruct &pps);
		void CalculateFaceMiddle8(FaceIndices &f, Vertex8 &FaceMiddle);

		void CalculateVertex8(Vertex &v, Vertex8 &v8);
		void CalculateNormal8(Vertex &n, Vertex8 &n8);

		void CalculateDims();
		bool MakeFacesNormals();
		bool MakeVerticesNormals();
		bool MakeModelBound(Model *m, int Segments);

		bool RemoveSingularFaces();
		bool MakeVerticesNormalsEdged(VertexLink *VertexLinks, int Angle);
		bool MakeVerticesWeld(VertexLink *VertexLinks, int WeldDelta, bool IgnoreColors);
		bool MakeVertexLinks(VertexLink **VertexLinks);
		void FreeVertexLinks(VertexLink **VertexLinks, int Length);
		bool OptimizeGeometry(bool IgnoreColors);

		void InvertAxis(bool xAxis, bool yAxis, bool zAxis);

		void DrawModelBox();
		void DrawModel(uint8_t FacesColors);
		void DrawModelFacesNormals(float Scale);
		void DrawModelVerticesNormals(float Scale);
		void DrawModelLattice();

	//	uint8_t SaveBinaryVerticesNormals(HANDLE hFile);
	//	uint8_t SaveBinaryVertices(HANDLE hFile);
	//	uint8_t SaveBinarySortings(HANDLE hFile);
	//	uint8_t SaveBinaryFaces(HANDLE hFile);
	//	uint8_t SaveBinaryParams(HANDLE hFile, uint8_t Bound);
	//	uint8_t SaveBinary(HANDLE hFile, uint8_t Bound);

	//	uint8_t SaveAsciiFaces(HANDLE hFile, DWORD StartVertexIndex);
	//	uint8_t SaveAsciiVertices(HANDLE hFile);
	//	uint8_t SaveAscii(HANDLE hFile, DWORD StartVertexIndex);

	//	uint8_t LoadBinaryVerticesNormals(HANDLE hFile);
	//	uint8_t LoadBinaryVertices(HANDLE hFile);
	//	uint8_t LoadBinarySortings(HANDLE hFile, int SavedFacesCount);
	//	uint8_t LoadBinaryFaces(HANDLE hFile);
	//	uint8_t LoadBinaryParams(HANDLE hFile);
	//	uint8_t LoadBinary(HANDLE hFile);

		void EmptyModel();

		bool MorphModelBound(Model *m, bool SetColors);
	//	bool CloneModel(Model *m);
	//	bool JoinModels(Model **List, int Count);
	};

}

#endif
