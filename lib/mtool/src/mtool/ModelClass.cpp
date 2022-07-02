#include <float.h>
#include <math.h>

#include <stdio.h>
#include <cstring>

//#include "Globals.h"
#include "ModelClass.h"
//#include "MechousSettings.h"

#define Pi_10		3.1415926536

// Weight of the point 
// of plane with null thickness
#define NULL_THICKNESS_WEIGHT	1.0

// Debug defines
// #define WRITE_PROJECTION_BUFFERS
// #define SHOW_OPTIMIZATION_STAGES

// M_TOOL features
#define USE_M_TOOL_FEATURE_GEOMETRY
#define USE_M_TOOL_FEATURE_BOUND

// M_TOOL defines...
#define UPPER_BODY_POLYGON	4
#define SIDE_BODY_POLYGON	1
#define LOWER_BODY_POLYGON	0
#define BOUND_REFLECTION	8

// MODEL_BODY
const char STR_BODY[]	 = "BODY";
const char STR_METAL[]	 = "METAL";
const char STR_GLASS[]	 = "GLASS";
const char STR_WINDOW[]	 = "WINDOW";
const char STR_TUBE[]	 = "TUBE";

// MODEL_DEBRIS
const char STR_DEBRIS[]	 = "DEBRIS";

// MODEL_WHEEL
const char STR_WHEEL[]	 = "WHEEL";

// MODEL_SLOT
const char STR_SLOT[]	 = "SLOT";

using namespace mtool;

//// Set structs align
//#pragma pack(push, 1)

// Static saving params.
// Used while saving process only

// Model physical params
struct ModelSavingParamsStruct 
{
	// Defined - fully
	int C3D_VERSION_3;
	int VerticesCount;
	int VerticesNormalsCount;
	int FacesCount;
	int FacesVerticesCount;

	int xMax, yMax, zMax;
	int xMin, yMin, zMin;
	int xOff, yOff, zOff;
	int rMax;

	int Phi, Psi, Theta;

	double Volume;
	DBV MassCenter;
	DBM InertiaTensor;
};

// Geomerty saving structs
struct VertexSavingStruct
{
	Vertex v;
	Vertex8 v8;
	int Sort;
};

struct NormalSavingStruct
{
	Vertex8 n8;
	uint8_t Reflection;
	int Sort;
};

struct FaceVertexStruct
{	
	int VertexIndex;
	int NormalIndex;
};

struct FaceSavingInfo
{
	int VerticesCount;
	int Sort;
	uint32_t ColorID;
	uint32_t ColorShift;
	Vertex8 Normal;
	uint8_t Reflection;
	Vertex8 Middle;
};

struct FaceSavingStruct
{
	// Face info
	struct FaceSavingInfo FaceInfo;

	// Vertex and normal indices 
	// of the one face vertex
	FaceVertexStruct Indices[3];
};

// Restore align
#pragma pack(pop)

// M_TOOL defines
const int NORMAL_LEN = 7;
const int NORMAL = ((1 << NORMAL_LEN) - 5);

// Min, max and 
// size of the face
struct FaceBox
{
	Vertex Min;
	Vertex Max;
	Vertex Size;
};

template <class T>
int Round (const T &t)
{
	if (t >= 0.0)
		return (int) (t + 0.5);
	else
		return (int) (t - 0.5);
};

void ZeroMemory(void* ptr, size_t size){
	::memset(ptr, 0, size);
}

void CopyMemory(void* dst, void* src, size_t size){
	::memcpy(dst, src, size);
}

// ------- CODE START HERE -------

Model::Model()
{
	// Clear params...
	ZeroMemory(&ModelParams, sizeof(ModelParamsStruct));
	
	FacesCount = 0;
	FacesNormalsCount = 0;
	
	VerticesCount = 0;
	VerticesNormalsCount = 0;

	Vertices = NULL;
	FacesNormals = NULL;
	VerticesNormals = NULL;
	Faces = NULL;

	ModelName = NULL;
	ModelType = MODEL_UNKNOWN;
	ModelMaterial = MODEL_MATERIAL_UNKNOWN;

	ModelEnabled = true;
	ModelBoxed = false;
	ModelIndex = 0;

	Max[0] = Max[1] = Max[2] = 0.0;
	Min[0] = Min[1] = Min[2] = 0.0;
	Offset[0] = Offset[1] = Offset[2] = 0.0;

	// Default model params
	ModelParams.ColorID = COLOR_RESERVED;
	ModelParams.SlotAngle = 2;		// None
	ModelParams.Reflection = 0;		// Diffuse
	ModelParams.SlotID = 2;			// Middle
	ModelParams.WheelSteer = 0;		// Not steer
};

void Model::EmptyModel()
{
	if (Vertices) 
	{ 
		delete [] Vertices;
		Vertices = NULL;
	}

	if (FacesNormals)
	{
		delete [] FacesNormals;
		FacesNormals = NULL;
	}

	if (VerticesNormals)
	{
		delete [] VerticesNormals;
		VerticesNormals = NULL;
	}

	if (Faces) 
	{
		delete [] Faces;
		Faces = NULL;
	}
};

Model::~Model()
{
	EmptyModel();
	
	if (ModelName)
	{
		delete [] ModelName;
		ModelName = NULL;
	}
};

//// Copy contents of Model to m
//bool Model::CloneModel(Model *m)
//{
//	  Face *NewFaces = new Face [FacesCount];
//	Vertex *NewVertices = new Vertex [VerticesCount];
//	Vertex *NewFacesNormals = new Vertex [FacesNormalsCount];
//	Vertex *NewVerticesNormals = new Vertex [VerticesNormalsCount];

//	char *NewModelName = NULL;

//	if (ModelName)
//	{
//		size_t ModelNameLen = strlen(ModelName) + 1;
//		NewModelName = new char [ModelNameLen];
//	}

//	// Memory check...
//	if (!NewFaces || !NewFacesNormals ||
//		!NewVertices || !NewVerticesNormals)
//	{
//		if (NewFaces) delete [] NewFaces;
//		if (NewVertices) delete [] NewVertices;
//		if (NewFacesNormals) delete [] NewFacesNormals;
//		if (NewVerticesNormals) delete [] NewVerticesNormals;

//		if (NewModelName) delete [] NewModelName;

//		return false;
//	}
	
//	// Clear existing name
//	if (m -> ModelName)
//	{
//		delete [] m -> ModelName;
//		m -> ModelName = NULL;
//	}

//	// Copy name of the model
//	if (ModelName)
//	{
//		lstrcpyA(NewModelName, ModelName);
//		m -> ModelName = NewModelName;
//	}

//	// Copy existing geometry
//	CopyMemory(NewFaces, Faces, FacesCount * sizeof(Face));
//	CopyMemory(NewVertices, Vertices, VerticesCount * sizeof(Vertex));
//	CopyMemory(NewFacesNormals, FacesNormals, FacesNormalsCount * sizeof(Vertex));
//	CopyMemory(NewVerticesNormals, VerticesNormals, VerticesNormalsCount * sizeof(Vertex));
	
//	// Clear existing geometry
//	m -> EmptyModel();
	
//	// Assing new geomerty
//	m -> Faces = NewFaces;
//	m -> Vertices = NewVertices;
//	m -> FacesNormals = NewFacesNormals;
//	m -> VerticesNormals = NewVerticesNormals;

//	// Set new counts
//	m -> FacesCount = FacesCount;
//	m -> VerticesCount = VerticesCount;
//	m -> FacesNormalsCount = FacesNormalsCount;
//	m -> VerticesNormalsCount = VerticesNormalsCount;

//	for (int i = 0; i < 3; i++)
//	{
//		m -> Min[i] = Min[i];
//		m -> Max[i] = Max[i];
//		m -> Offset[i] = Offset[i];
//	}

//	m -> ModelType = ModelType;
//	m -> ModelMaterial = ModelMaterial;
//	m -> ModelBoxed = ModelBoxed;
//	m -> ModelEnabled = ModelEnabled;
//	m -> ModelParams = ModelParams;

//	return true;
//};

//bool Model::JoinModels(Model **List, int Count)
//{
//	// Empty list
//	if (!Count) return false;

//	int NewFacesCount = FacesCount;
//	int NewVerticesCount = VerticesCount;
//	int NewFacesNormalsCount = FacesNormalsCount;
//	int NewVerticesNormalsCount = VerticesNormalsCount;

//	for (int i = 0; i < Count; i++)
//	{
//		Model *m = List[i];

//		NewFacesCount += m -> FacesCount;
//		NewVerticesCount += m -> VerticesCount;
//		NewFacesNormalsCount += m -> FacesNormalsCount;
//		NewVerticesNormalsCount += m -> VerticesNormalsCount;
//	}

//	  Face *NewFaces = new Face [NewFacesCount];
//	Vertex *NewVertices = new Vertex [NewVerticesCount];
//	Vertex *NewFacesNormals = new Vertex [NewFacesNormalsCount];
//	Vertex *NewVerticesNormals = new Vertex [NewVerticesNormalsCount];

//	// Memory check...
//	if (!NewFaces || !NewFacesNormals ||
//		!NewVertices || !NewVerticesNormals)
//	{
//		if (NewFaces) delete [] NewFaces;
//		if (NewVertices) delete [] NewVertices;
//		if (NewFacesNormals) delete [] NewFacesNormals;
//		if (NewVerticesNormals) delete [] NewVerticesNormals;

//		return false;
//	}
	
//	// Copy existing geometry
//	CopyMemory(NewFaces, Faces, FacesCount * sizeof(Face));
//	CopyMemory(NewVertices, Vertices, VerticesCount * sizeof(Vertex));
//	CopyMemory(NewFacesNormals, FacesNormals, FacesNormalsCount * sizeof(Vertex));
//	CopyMemory(NewVerticesNormals, VerticesNormals, VerticesNormalsCount * sizeof(Vertex));
	
//	// Set arrays offsets
//	int FacesOffset = FacesCount;
//	int VerticesOffset = VerticesCount;
//	int VerticesNormalsOffset = VerticesNormalsCount;

//	// Clear existing geometry
//	EmptyModel();

//	// Assing new geomerty
//	Faces = NewFaces;
//	Vertices = NewVertices;
//	FacesNormals = NewFacesNormals;
//	VerticesNormals = NewVerticesNormals;

//	// Set new counts
//	FacesCount = NewFacesCount;
//	VerticesCount = NewVerticesCount;
//	FacesNormalsCount = NewFacesNormalsCount;
//	VerticesNormalsCount = NewVerticesNormalsCount;
	
//	// Finally... Go!
//	for (int i = 0; i < Count; i++)
//	{
//		Model *m = List[i];

//		// Copy faces
//		void *Destination = &Faces[FacesOffset];
//		SIZE_T Size = (m -> FacesCount) * sizeof(Face);
//		CopyMemory(Destination, m -> Faces, Size);

//		// Correct indices of vertices and normals
//		for (int j = 0; j < m -> FacesCount; j++)
//		{
//			Face &f = Faces[FacesOffset + j];
			
//			f.Vertices[0] += VerticesOffset;
//			f.Vertices[1] += VerticesOffset;
//			f.Vertices[2] += VerticesOffset;

//			f.Normals[0] += VerticesNormalsOffset;
//			f.Normals[1] += VerticesNormalsOffset;
//			f.Normals[2] += VerticesNormalsOffset;
//		};
		
//		// Copy vertices
//		Destination = &Vertices[VerticesOffset];
//		Size = (m -> VerticesCount) * sizeof(Vertex);
//		CopyMemory(Destination, m -> Vertices, Size);
		
//		// Copy faces normals
//		Destination = &FacesNormals[FacesOffset];
//		Size = (m -> FacesNormalsCount) * sizeof(Vertex);
//		CopyMemory(Destination, m -> FacesNormals, Size);

//		// Copy vertices normals
//		Destination = &VerticesNormals[VerticesNormalsOffset];
//		Size = (m -> VerticesNormalsCount) * sizeof(Vertex);
//		CopyMemory(Destination, m -> VerticesNormals, Size);
		
//		FacesOffset += m -> FacesCount;
//		VerticesOffset += m -> VerticesCount;
//		VerticesNormalsOffset += m -> VerticesNormalsCount;
//	}

//	CalculateDims();

//	return true;
//};

//// Sets model params based on model's name
//void Model::SetParamsUsingName(char *Name)
//{
//	SIZE_T UpperNameLen = strlen(Name) + 1;
//	char *UpperName = new char [UpperNameLen];
	
//	if (!UpperName) return ;
//	ZeroMemory(UpperName, UpperNameLen);

//	lstrcpyA(UpperName, Name);
//	CharUpperA(UpperName);
	
//	// Stupid code...
//	// Set preview material
//	// Use CRC or Hash for comparing strings?..
//	if (strstr(UpperName, STR_BODY))
//		ModelMaterial = MODEL_MATERIAL_BODY;
//	else
//	if (strstr(UpperName, STR_DEBRIS))
//		ModelMaterial = MODEL_MATERIAL_DEBRIS;
//	else
//	if (strstr(UpperName, STR_WHEEL))
//		ModelMaterial = MODEL_MATERIAL_WHEEL;
//	else
//	if (strstr(UpperName, STR_METAL))
//		ModelMaterial = MODEL_MATERIAL_METAL;
//	else
//	if (strstr(UpperName, STR_SLOT))
//		ModelMaterial = MODEL_MATERIAL_SLOT;
//	else
//	if (strstr(UpperName, STR_GLASS) || strstr(UpperName, STR_WINDOW))
//		ModelMaterial = MODEL_MATERIAL_GLASS;
//	else
//	if (strstr(UpperName, STR_TUBE))
//		ModelMaterial = MODEL_MATERIAL_TUBE;
//	else ModelMaterial = MODEL_MATERIAL_UNKNOWN;

//	delete [] UpperName;

//	// Set model type
//	// and INGAME ColorID
//	switch (ModelMaterial)
//	{
//		case MODEL_MATERIAL_BODY :
//		{
//			ModelType = MODEL_BODY;
//			ModelParams.ColorID = COLOR_BODY;
//			break;
//		}

//		case MODEL_MATERIAL_TUBE :
//		{
//			ModelType = MODEL_BODY;
//			ModelParams.ColorID = COLOR_TUBE;
//			ModelParams.Reflection = 2;
//			break;
//		}

//		case MODEL_MATERIAL_GLASS :
//		{
//			ModelType = MODEL_BODY;
//			ModelParams.ColorID = COLOR_GLASS;
//			ModelParams.Reflection = 8;
//			break;
//		}

//		case MODEL_MATERIAL_METAL :
//		{
//			ModelType = MODEL_BODY;
//			ModelParams.ColorID = COLOR_METAL;
//			ModelParams.Reflection = 4;
//			break;
//		}

//		case MODEL_MATERIAL_DEBRIS :
//		{
//			ModelType = MODEL_DEBRIS;
//			ModelParams.ColorID = COLOR_BODY;
//			break;
//		}

//		case MODEL_MATERIAL_WHEEL :
//		{
//			ModelType = MODEL_WHEEL;
//			ModelParams.ColorID = COLOR_WHEEL;
//			break;
//		}

//		case MODEL_MATERIAL_SLOT :
//		{
//			ModelType = MODEL_SLOT;
//			ModelParams.ColorID = COLOR_SLOT;
//			break;
//		}

//		case MODEL_MATERIAL_UNKNOWN :
//		{
//			ModelType = MODEL_UNKNOWN;
//			break;
//		}
//	}
//};

//void Model::SetModelName(const char *Name, bool AutoParams)
//{
//	// Delete existing...
//	if (ModelName)
//	{
//		delete [] ModelName;
//		ModelName = NULL;
//	}

//	// Set new name
//	int NameLen = (int) strlen(Name);
//    ModelName = new char[NameLen + 1];

//	if (ModelName)
//	{
//		CopyMemory(ModelName, Name, NameLen + 1);
//		if (AutoParams)	SetParamsUsingName(ModelName);
//	}
//};

//void Model::DrawModel(bool FacesColors)
//{
//	// Set material
//	const float *Material = Materials[ModelMaterial];
//	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Material);
	
//	// If faces count used
//	int MaterialIndex = MODEL_MATERIAL_UNKNOWN;

//	// Avoid flickering lines
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glPolygonOffset(1.0f, 1.0f);

//	glBegin(GL_TRIANGLES);
	
//	// Draw model
//	for (int i = 0; i < FacesCount; i++)
//	{
//		Face &f = Faces[i];

//		if (FacesColors)
//		{
//			switch (f.ColorID)
//			{
//				case COLOR_RESERVED	 : { MaterialIndex = MODEL_MATERIAL_UNKNOWN; break; }

//				case COLOR_BODY	 : { MaterialIndex = MODEL_MATERIAL_BODY;	 break; }
//				case COLOR_GLASS : { MaterialIndex = MODEL_MATERIAL_GLASS;	 break; }
//				case COLOR_WHEEL : { MaterialIndex = MODEL_MATERIAL_WHEEL;	 break; }
//				case COLOR_METAL : { MaterialIndex = MODEL_MATERIAL_METAL;	 break; }
//				case COLOR_SLOT	 : { MaterialIndex = MODEL_MATERIAL_SLOT;	 break; }
//				case COLOR_TUBE	 : { MaterialIndex = MODEL_MATERIAL_TUBE;	 break; }
//  			}

//			Material = Materials[MaterialIndex];
//			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, Material);
//		}

//		glNormal3fv(FacesNormals[i]);
		
//		glVertex3fv(Vertices[f.Vertices[0]]);
//		glVertex3fv(Vertices[f.Vertices[1]]);
//		glVertex3fv(Vertices[f.Vertices[2]]);
//	}
	
//	glEnd();

//	glDisable(GL_POLYGON_OFFSET_FILL);
//};

//// Draw faces normals
//void Model::DrawModelFacesNormals(float Scale)
//{
//	for (int i = 0; i < FacesCount; i++)
//	{
//		FaceIndices &f = Faces[i].Vertices;
//		Vertex &n = FacesNormals[i];

//		Vertex FaceCenter;
		
//		// Zero vector
//		for (int j = 0; j < 3; j++)
//			FaceCenter[j] = 0.0;

//		for (int j = 0; j < 3; j++)
//		{
//			Vertex &v = Vertices[f[j]];
	
//			// Accumulate vertices
//			for (int k = 0; k < 3; k++)
//				FaceCenter[k] += v[k];
//		}

//		// Find center
//		for (int j = 0; j < 3; j++)
//			FaceCenter[j] /= 3.0f;

//		glBegin(GL_POINTS);
//			glVertex3fv(FaceCenter);
//		glEnd();

//		glBegin(GL_LINES);
//			glVertex3fv(FaceCenter);

//			// Endpoint of normal
//			for (int j = 0; j < 3; j++)
//				FaceCenter[j] += n[j] * 10.0f / Scale;

//			glVertex3fv(FaceCenter);
//		glEnd();
//	}
//};

//// Draw vertices normals
//void Model::DrawModelVerticesNormals(float Scale)
//{
//	// This will draw some normals two or
//	// more times. But what if I add some
//	// new normals to the list? What will
//	// be the base-point for it? Commonly
//	// number of vertices not equal to the
//	// number of vertices-normals because
//	// of existence of sharp edges. That's
//	// the reason...

//	for (int i = 0; i < FacesCount; i++)
//	{
//		Face &f = Faces[i];
		
//		for (int j = 0; j < 3; j++)
//		{
//			Vertex &v = Vertices[f.Vertices[j]];
//			Vertex &vn = VerticesNormals[f.Normals[j]];

//			glBegin(GL_POINTS);
//				glVertex3fv(v);
//			glEnd();

//			glBegin(GL_LINES);
//				glVertex3fv(v);

//				Vertex NormalEnd;
//				for (int k = 0; k < 3; k++)
//					NormalEnd[k] = v[k] + vn[k] * 10.0f / Scale;

//				glVertex3fv(NormalEnd);
//			glEnd();
//		}
//	}
//};

//void Model::DrawModelLattice()
//{
//	for (int i = 0; i < FacesCount; i++)
//	{
//		FaceIndices &f = Faces[i].Vertices;
	
//		glBegin(GL_LINE_LOOP);
		
//		glVertex3fv(Vertices[f[0]]);
//		glVertex3fv(Vertices[f[1]]);
//		glVertex3fv(Vertices[f[2]]);

//		glEnd();
//	}
//};

//void Model::DrawModelBox()
//{
//	glBegin(GL_LINE_STRIP);
//		// Face 1
//		glVertex3f(Min[0], Max[1], Max[2]);
//		glVertex3f(Max[0], Max[1], Max[2]);
//		glVertex3f(Max[0], Min[1], Max[2]);
//		glVertex3f(Min[0], Min[1], Max[2]);
//		glVertex3f(Min[0], Max[1], Max[2]);

//		// Face 2
//		glVertex3f(Min[0], Max[1], Min[2]);
//		glVertex3f(Max[0], Max[1], Min[2]);
//		glVertex3f(Max[0], Min[1], Min[2]);
//		glVertex3f(Min[0], Min[1], Min[2]);
//		glVertex3f(Min[0], Max[1], Min[2]);
//	glEnd();

//	glBegin(GL_LINES);
//		glVertex3f(Max[0], Max[1], Max[2]);
//		glVertex3f(Max[0], Max[1], Min[2]);

//		glVertex3f(Max[0], Min[1], Max[2]);
//		glVertex3f(Max[0], Min[1], Min[2]);

//		glVertex3f(Min[0], Min[1], Max[2]);
//		glVertex3f(Min[0], Min[1], Min[2]);
//	glEnd();
//};

float VectorNorm(Vertex &v)
{
	return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
};

void VectorZero(Vertex &v)
{
	v[2] = v[1] = v[0] = 0.0f;
};

void VectorCopyVector(Vertex &Dst, Vertex &Src)
{
	Dst[0] = Src[0];
	Dst[1] = Src[1];
	Dst[2] = Src[2];
};

void VectorMinusVector(Vertex &v1, Vertex &v2)
{
	v1[0] -= v2[0];
	v1[1] -= v2[1];
	v1[2] -= v2[2];
};

void VectorMinusVector2(Vertex &v1, Vertex &v2, Vertex &v)
{
	v[0] = v1[0] - v2[0];
	v[1] = v1[1] - v2[1];
	v[2] = v1[2] - v2[2];
};

void VectorPlusVector(Vertex &v1, Vertex &v2)
{
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
};

void VectorMultiplyNumber(Vertex &v, float n)
{
	v[0] *= n;
	v[1] *= n;
	v[2] *= n;
};

void VectorMultiplyNumber2(Vertex &v1, float n, Vertex &v)
{
	v[0] = v1[0] * n;
	v[1] = v1[1] * n;
	v[2] = v1[2] * n;
};

float VectorMultiplyVector(Vertex &v1, Vertex &v2)
{
	float Product = v1[0] * v2[0] + 
					v1[1] * v2[1] + 
					v1[2] * v2[2]; 

	return Product;
};

void VectorDivideNumber(Vertex &v, float n)
{
	v[0] /= n;
	v[1] /= n;
	v[2] /= n;
};

void VectorDivideNumber2(Vertex &v1, float n, Vertex &v)
{
	v[0] = v1[0] / n;
	v[1] = v1[1] / n;
	v[2] = v1[2] / n;
};

void VectorNormalize(Vertex &v)
{
	float n = VectorNorm(v);
	if (n) VectorDivideNumber(v, n);
};

void VectorGetNormal(Vertex &v1, Vertex &v2, Vertex &v)
{
	v[0] = v1[1] * v2[2] - v2[1] * v1[2];
	v[1] = v1[2] * v2[0] - v2[2] * v1[0];
	v[2] = v1[0] * v2[1] - v2[0] * v1[1];

	VectorNormalize(v);
};

void VectorGetNormal2(Vertex &v1, Vertex &v2, Vertex &v)
{
	v[0] = v1[1] * v2[2] - v2[1] * v1[2];
	v[1] = v1[2] * v2[0] - v2[2] * v1[0];
	v[2] = v1[0] * v2[1] - v2[0] * v1[1];
};

void MakeVector(Vertex &b, Vertex &e, Vertex &v)
{
	v[0] = e[0] - b[0];
	v[1] = e[1] - b[1];
	v[2] = e[2] - b[2];
};

bool VectorSameVector(Vertex &v1, Vertex &v2)
{
	return      (v1[0] == v2[0] &&
				 v1[1] == v2[1] &&
				 v1[2] == v2[2]);
}; 

float VectorDistanceVector(Vertex &v1, Vertex &v2)
{
	float dx = v1[0] - v2[0];
	float dy = v1[1] - v2[1];
	float dz = v1[2] - v2[2];
	
	float Distance = sqrt(dx * dx + dy * dy + dz * dz);

	return Distance;
};

// Collect indices of faces which contains one vertex
bool Model::MakeVertexLinks(VertexLink **VertexLinks)
{
	// Allocate memory
	(*VertexLinks) = new VertexLink [VerticesCount];	
	if (!(*VertexLinks)) return false;

	// Initialize storage
	size_t MemorySize = sizeof(VertexLink) * VerticesCount;
	ZeroMemory(*VertexLinks, MemorySize);

	// Calculate number of faces
	for (int i = 0; i < FacesCount; i++)
	{
		FaceIndices &f = Faces[i].Vertices;
		
		for (int j = 0; j < 3; j++)
			(*VertexLinks)[f[j]].FacesCount++;
	}

	bool StatusOk = true;

	// Initialize vertex to faces links
	for (int i = 0; i < VerticesCount; i++)
	{
		VertexLink &vl = (*VertexLinks)[i];
		
		vl.Faces = new uint32_t [vl.FacesCount];
		vl.Indices = new uint32_t [vl.FacesCount];
		vl.FacesCount = 0;

		// Memory check...
		if (!vl.Faces || !vl.Indices) 
		{
			StatusOk = false;
			break;
		}
	}

	if (StatusOk)
	{
		// Fill links to faces
		for (int i = 0; i < FacesCount; i++)
		{
			Face &f = Faces[i];
		
			for (int j = 0; j < 3; j++)
			{
				VertexLink &vl = (*VertexLinks)[f.Vertices[j]];
				
				vl.Faces[vl.FacesCount] = (uint32_t) i;
				vl.Indices[vl.FacesCount] = (uint32_t) j;

				vl.ColorID = f.ColorID;
				vl.Reflection = f.Reflection;
				
				vl.TotalArea += f.Area;
				vl.FacesCount++;
			}
		}	
	}

	return StatusOk;
};

// Free vertex links memory...
void Model::FreeVertexLinks(VertexLink **VertexLinks, int Length)
{
	for (int i = 0; i < Length; i++)
	{
		VertexLink &vl = (*VertexLinks)[i];

		if (vl.Faces) delete [] vl.Faces;
		if (vl.Indices) delete [] vl.Indices;
	}

	delete [] (*VertexLinks);
};

// Faces normals must present. Angle must be in degress. A number of 
// normals for the same vertex can be calculated. Note, that one vertex can 
// belongs only to faces with the same ColorID and Reflection (see Welding)
bool Model::MakeVerticesNormalsEdged(VertexLink *VertexLinks, int Angle)
{
	bool StatusOk = true;

	// Maximum number of vertices normals
	Vertex *NewVerticesNormals = new Vertex [FacesCount * 3];
	
	// Reflection value for each normal
	int *NewVerticesNormalsRef = new int [FacesCount * 3];
	
	// Memory check
	if (!NewVerticesNormals || !NewVerticesNormalsRef)
	{
		if (NewVerticesNormals) delete [] NewVerticesNormals;
		if (NewVerticesNormalsRef) delete [] NewVerticesNormalsRef;
		
		return false;
	}

	// Counter
	int NewVerticesNormalsCount = 0;

	// All normals normalized to 1.0!
	// Maximum angle between faces normals, 
	// which could be smoothed by normals
	float ProductLimit = (float) cos(Angle * Pi_10 / 180);

	// Set limit for angle between normals to 
	// decrease number of normals used (5 degrees)
	float ProductLimit2 = (float) cos(5.0 * Pi_10 / 180);

	// Main calculation loop
	for (int i = 0; i < VerticesCount; i++)
	{
		VertexLink &vl1 = VertexLinks[i];

		for (int j = 0; j < vl1.FacesCount; j++)
		{
			uint32_t idx1 = vl1.Faces[j];
			uint32_t vert1 = vl1.Indices[j];
			
			Vertex &fn1 = FacesNormals[idx1];
			Face &f1 = Faces[idx1];
				
			Vertex VertexNormal;
			VertexNormal[0] = fn1[0] * f1.Area;
			VertexNormal[1] = fn1[1] * f1.Area;
			VertexNormal[2] = fn1[2] * f1.Area;

			float TotalArea = f1.Area;

			// Accumulate faces normals
			for (int k = 0; k < vl1.FacesCount; k++)
			{
				if (j == k) continue;

				uint32_t idx2 = vl1.Faces[k];

				Vertex &fn2 = FacesNormals[idx2];
				Face &f2 = Faces[idx2];

				if ((f1.ColorID != f2.ColorID) ||
					(f1.Reflection != f2.Reflection)) continue;

				float Product = VectorMultiplyVector(fn1, fn2);
					
				// Cosinus is used...
				if (Product >= ProductLimit)
				{
					VertexNormal[0] += fn2[0] * f2.Area;
					VertexNormal[1] += fn2[1] * f2.Area;
					VertexNormal[2] += fn2[2] * f2.Area;
					
					TotalArea += f2.Area;
				}
			}

			// Normalize smoothed vertex-normal
			if (TotalArea) VectorNormalize(VertexNormal);

			bool NormalPresent = false;
			uint32_t NormalPresentIndex = 0;
	
			// Add only "unique" normals
			for (int k = 0; k < NewVerticesNormalsCount; k++)
			{
				// Check for the same ColorID
				int Reflection2 = NewVerticesNormalsRef[k];
				
				// Skip faces with different reflections
				if (f1.Reflection != Reflection2) continue;

				Vertex &vn = NewVerticesNormals[k];
				float Product = VectorMultiplyVector(vn, VertexNormal);

				// Cosinus is used...
				if (Product >= ProductLimit2)
				{
					NormalPresentIndex = (uint32_t) k;
					NormalPresent = true;
					break;
				}
			}
				
			if (!NormalPresent)
			{
				// Define new normal
				Vertex &vn = NewVerticesNormals[NewVerticesNormalsCount];
				VectorCopyVector(vn, VertexNormal);

				// Set index of the vertex-normal
				f1.Normals[vert1] = NewVerticesNormalsCount;
				NewVerticesNormalsRef[NewVerticesNormalsCount] = f1.Reflection;

				// Increase counter
				NewVerticesNormalsCount++;
			}
			else
			{
				// Use alredy existing normal			
				f1.Normals[vert1] = NormalPresentIndex;
			}
		}
	}

	// Allocate memory for unique normals
	Vertex *UniVerticesNormals = new Vertex [NewVerticesNormalsCount];
	if (!UniVerticesNormals) StatusOk = false;

	if (StatusOk)
	{
		// Copy unique normals
		size_t MemorySize = sizeof(Vertex) * NewVerticesNormalsCount;
		CopyMemory(UniVerticesNormals, NewVerticesNormals, MemorySize);

		// Detele existing vertices normals
		if (VerticesNormals) delete [] VerticesNormals;

		// Set new vertices normals
		VerticesNormals = UniVerticesNormals;
		VerticesNormalsCount = NewVerticesNormalsCount;
	}

	// Free temporary storages
	delete [] NewVerticesNormals;
	delete [] NewVerticesNormalsRef;

	return StatusOk;
};

// Weld some vertices together using its weight. Vertices, which belongs to 
// faces with different reflection or color will be welded if IgnoreColors is set
bool Model::MakeVerticesWeld(VertexLink *VertexLinks, int WeldDelta, bool IgnoreColors)
{
	bool StatusOk = true;

	// Maximum number of vertices
	Vertex *NewVertices = new Vertex [VerticesCount];

	// Counter
	int NewVerticesCount = 0;

	// Processed flags
	bool *VertexProcessed = new bool [VerticesCount];

	// Memory check
	if (!NewVertices || !VertexProcessed) 
	{
		if (NewVertices) delete [] NewVertices;
		if (VertexProcessed) delete [] VertexProcessed;

		return false;
	}

	// Initialize flags
	ZeroMemory(VertexProcessed, sizeof(bool) * VerticesCount);

	for (int i = 0; i < VerticesCount; i++)
	{
		// Skip already processed...
		if (VertexProcessed[i]) continue;

		Vertex &v1 = Vertices[i];
		VertexLink &vl1 = VertexLinks[i];

		// Skip vertices with no faces
		if (!vl1.FacesCount) continue;

		// Process all faces here.
		// Even NULL faces, because
		// I need right indices of 
		// its vertices to eliminate 
		// such NULL faces later

		// Average
		Vertex Average;		
		VectorZero(Average);

		// Define average weight
		float AverageWeight = 0.0f;
		
		// Compare with *itself* and others
		for (int j = i; j < VerticesCount; j++)
		{
			// Skip already processed...
			if (VertexProcessed[j]) continue;
			Vertex &v2 = Vertices[j];

			// Distance between vertices
			float Distance = VectorDistanceVector(v1, v2);			
			
			if (Distance < WeldDelta)
			{
				// Vertex link for vertex (j)
				VertexLink &vl2 = VertexLinks[j];

				// Skip vertices without faces
				if (!vl2.FacesCount) continue;

				// Skip faces with different colors
				if (!IgnoreColors && ((vl1.ColorID != vl2.ColorID) || 
					(vl1.Reflection != vl2.Reflection))) continue;
					
				// Correct indices for vertex (j)
				for (int k = 0; k < vl2.FacesCount; k++)
				{
					// Indices of vertex (j)
					uint32_t idx2 = vl2.Faces[k];
					uint32_t vert2 = vl2.Indices[k];

					// Set new indices for (j)
					Face &f = Faces[idx2];
					f.Vertices[vert2] = NewVerticesCount;
				}
				
				// Set processed flag
				VertexProcessed[j] = true;

				// Add to average
				Vertex AverageAdd;
				VectorMultiplyNumber2(v2, vl2.TotalArea, AverageAdd);
				VectorPlusVector(Average, AverageAdd);

				// Add weight
				AverageWeight += vl2.TotalArea;
			}
		}

		// Finishing...
		if (AverageWeight) 
			VectorDivideNumber(Average, AverageWeight);
		else
			VectorCopyVector(Average, v1);

		// As I skip processed vertices above,
		// then I have only unique vertices here
		Vertex &nv = NewVertices[NewVerticesCount];
		VectorCopyVector(nv, Average);
		
		// Increase counter
		NewVerticesCount++;
	}

	// Allocate memory for unique vertices
	Vertex *UniVertices = new Vertex [NewVerticesCount];
	if (!UniVertices) StatusOk = false;

	if (StatusOk)
	{
		// Copy unique vertices
		size_t MemorySize = sizeof(Vertex) * NewVerticesCount;
		CopyMemory(UniVertices, NewVertices, MemorySize);

		// Detele existing vertices
		if (Vertices) delete [] Vertices;

		// Set new vertices
		Vertices = UniVertices;
		VerticesCount = NewVerticesCount;
	}

	// Free temporary vertices
	delete [] NewVertices;

	// Free vertex processed
	delete [] VertexProcessed;

	// Recalculate normals and dims 
	// as geomtery could be changed
	MakeFacesNormals();
	MakeVerticesNormals();
	CalculateDims();

	return StatusOk;
};

// Actually just removes faces some of 
// which vertices have same coordinates
bool Model::RemoveSingularFaces()
{
	bool StatusOk = true;

	// Maximum number of faces
	bool *FacesFlags = new bool [FacesCount];
	if (!FacesFlags) return false;

	// Counter
	int NewFacesCount = 0;

	for (int i = 0; i < FacesCount; i++)
	{
		FaceIndices &f = Faces[i].Vertices;
		bool BadFace = false;

		// Each with each...
		for (int j = 0; j < 2; j++)
		{
			Vertex &v1 = Vertices[f[j]];

			for (int k = j + 1; k < 3; k++)
			{
				Vertex &v2 = Vertices[f[k]];
				BadFace = VectorSameVector(v1, v2);

				if (BadFace) break;
			}

			if (BadFace) break;
		}

		// Set face flag
		FacesFlags[i] = BadFace;
		if (!BadFace) NewFacesCount++;
	}
	
	// Really needed?
	if (NewFacesCount != FacesCount)
	{
		// Allocate memory for good faces
		Face *NewFaces = new Face [NewFacesCount];
		Vertex *NewFacesNormals = new Vertex [NewFacesCount];

		// Memory check
		if (!NewFaces || !NewFacesNormals) 
		{
			if (NewFaces) delete [] NewFaces;
			if (NewFacesNormals) delete [] NewFacesNormals;

			StatusOk = false;
		}

		// Counter
		NewFacesCount = 0;

		if (StatusOk)
		{
			for (int i = 0; i < FacesCount; i++)
			{
				if (!FacesFlags[i])
				{
					// Set new face
					NewFaces[NewFacesCount] = Faces[i];
				
					// Set new face-normal
					Vertex &fn = FacesNormals[i];
					Vertex &fnn = NewFacesNormals[NewFacesCount];

					VectorCopyVector(fnn, fn);

					NewFacesCount++;
				}
			}

			// Detele existing arrays
			if (Faces) delete [] Faces;
			if (FacesNormals) delete [] FacesNormals;

			// Set new arrays
			Faces = NewFaces;
			FacesCount = NewFacesCount;

			FacesNormals = NewFacesNormals;
			FacesNormalsCount = NewFacesCount;
		}
	}

	// Free...
	delete [] FacesFlags;

	// Dimensions could be changed
	CalculateDims();

	return StatusOk;
};

// Faces normals must present. Only one normal 
// for one vertex is being calculated here
bool Model::MakeVerticesNormals()
{
	// Temporary vertices normals array
	int NewVerticesNormalsCount = VerticesCount;
	Vertex *NewVerticesNormals = new Vertex [NewVerticesNormalsCount];
	
	// Memory check
	if (!NewVerticesNormals) return false;

	// Clear vertex normals memory
	uint32_t MemorySize = sizeof(Vertex) * NewVerticesNormalsCount;
	ZeroMemory(NewVerticesNormals, MemorySize);

	for (int i = 0; i < FacesCount; i++)
	{
		FaceIndices &fv = Faces[i].Vertices;
		FaceIndices &fn = Faces[i].Normals;

		Vertex &n = FacesNormals[i];

		for (int j = 0; j < 3; j++)
		{
			fn[j] = fv[j]; // Equal indices

			Vertex &vn = NewVerticesNormals[fn[j]];

			vn[0] += n[0];
			vn[1] += n[1];
			vn[2] += n[2];
		}
	}

	// Normalize vertices normals
	for (int i = 0; i < NewVerticesNormalsCount; i++)
		VectorNormalize(NewVerticesNormals[i]);

	// Delete existing vertices normals
	if (VerticesNormals) delete [] VerticesNormals;

	// Set new vertices normals
	VerticesNormalsCount = NewVerticesNormalsCount;
	VerticesNormals = NewVerticesNormals;

	return true;
};

// Calculates face normals
bool Model::MakeFacesNormals()
{
	// Temporary faces normals array
	int NewFacesNormalsCount = FacesCount;
	Vertex *NewFacesNormals = new Vertex [NewFacesNormalsCount];
	
	// Memory check
	if (!NewFacesNormals) return false;

	Vertex *A, *B, *C;
	Vertex AC, AB;

	for (int i = 0; i < FacesCount; i++)
	{
		Face &f = Faces[i];

		A = Vertices + f.Vertices[0];
		B = Vertices + f.Vertices[1];
		C = Vertices + f.Vertices[2];
		
		MakeVector(*A, *C, AC);
		MakeVector(*A, *B, AB);

		Vertex &fn = NewFacesNormals[i];
		
		// Not normalized normal
		VectorGetNormal2(AB, AC, fn);

		// Norma of the normal
		float Norm = VectorNorm(fn);

		// Normalize normal
		if (Norm) VectorDivideNumber(fn, Norm);

		// Set area of face
		f.Area = Norm / 2;
	}

	// Delete existing faces normals
	if (FacesNormals) delete [] FacesNormals;

	// Set new vertices normals
	FacesNormalsCount = NewFacesNormalsCount;
	FacesNormals = NewFacesNormals;

	return true;
};

void Model::InvertAxis(bool xAxis, bool yAxis, bool zAxis)
{
	for (int i = 0; i < VerticesCount; i++)	
	{
		Vertex &v = Vertices[i];
		
		if (xAxis) v[0] = -v[0];
		if (yAxis) v[1] = -v[1];
		if (zAxis) v[2] = -v[2];
	}
};

float Model::CalculateRadius(bool ModelBasis)
{
	float rMax = -FLT_MAX;

	if (ModelBasis)
	{
		for (int i = 0; i < VerticesCount; i++)
		{
			Vertex &v = Vertices[i];
			
			Vertex sv;
			for (int i = 0; i < 3; i ++)
				sv[i] = v[i] - Offset[i];
			
			float Norm = VectorNorm(sv);
			if (Norm > rMax) rMax = Norm;
		}
	}
	else
	{
		for (int i = 0; i < VerticesCount; i++)
		{
			Vertex &v = Vertices[i];
			
			float Norm = VectorNorm(v);
			if (Norm > rMax) rMax = Norm;
		}
	}

	return rMax;
};

void Model::CalculateDims()
{
	Min[0] = Min[1] = Min[2] =  FLT_MAX;
	Max[0] = Max[1] = Max[2] = -FLT_MAX;

	for (int i = 0; i < VerticesCount; i++)
	{
		Vertex &v = Vertices[i];

		for (int j = 0; j < 3; j++)
		{
			if (v[j] > Max[j]) Max[j] = v[j];
			if (v[j] < Min[j]) Min[j] = v[j];
		}
	}

	for (int i = 0; i < 3; i++)	
		Offset[i] = (Min[i] + Max[i]) / 2;
};

void Model::SetOffset(Vertex &MinusOffset)
{
	for (int i = 0; i < VerticesCount; i++)
	{
		Vertex &v = Vertices[i];

		v[0] -= MinusOffset[0];
		v[1] -= MinusOffset[1];
		v[2] -= MinusOffset[2];
	}

	for (int i = 0; i < 3; i++)
	{
		Max[i] -= MinusOffset[i];
		Min[i] -= MinusOffset[i];

		Offset[i] -= MinusOffset[i];
	}
};

void Model::SetScale(float Scale)
{
	for (int i = 0; i < VerticesCount; i++)
	{
		Vertex &v = Vertices[i];

		v[0] *= Scale;
		v[1] *= Scale;
		v[2] *= Scale;
	}

	for (int i = 0; i < 3; i++)
	{
		Min[i] *= Scale;
		Max[i] *= Scale;

		Offset[i] *= Scale;
	}
};

bool PtInRange(Vertex &A, Vertex &B, Vertex &P)
{
	bool InRangeX = ((A[0] <= P[0]) && (P[0] <= B[0])) || 
					((A[0] >= P[0]) && (P[0] >= B[0]));

	bool InRangeY = ((A[1] <= P[1]) && (P[1] <= B[1])) || 
					((A[1] >= P[1]) && (P[1] >= B[1]));

	return (InRangeX && InRangeY);
};

// A, B - beginning and ending of the vector.
// P - point to test. If (0) returned then point 
// on vector, (-1) - point to the left side of the 
// vector, (+1) - point to the right side
int PtRelativeVector(Vertex &A, Vertex &B, Vertex &P, CheckPlanes Plane)
{
	int f1, f2;

	switch (Plane)
	{
		case PLANE_XY : { f1 = 0; f2 = 1; break; }
		case PLANE_YZ : { f1 = 1; f2 = 2; break; }
		case PLANE_ZX : { f1 = 2; f2 = 0; break; }
	}
	
	float d1 = B[f1] - A[f1];
	float d2 = B[f2] - A[f2];

	float p1 = P[f1] - A[f1];
	float p2 = P[f2] - A[f2];

	float product = d1 * p2 - d2 * p1;

	if (fabs(product) >= FLT_EPSILON)	
	{
		if (product > 0) return 1;
		else return -1;
	}
	else	
	{
		bool OnLine = PtInRange(A, B, P);
		if (OnLine) return 0;
		else return -1;
	}
};

// Zero returned if point on the side,
// (1) if point inside and (-1) - outside
// Plane - is a plane to check
int PtInTriangle(Vertex &A, Vertex &B, Vertex &C, Vertex &P, CheckPlanes Plane)
{
	int Result = 0;
	
	Result = PtRelativeVector(A, B, P, Plane);
	if (Result != 1) return Result;
		
	Result = PtRelativeVector(B, C, P, Plane);
	if (Result != 1) return Result;
	
	Result = PtRelativeVector(C, A, P, Plane);
	if (Result != 1) return Result;

	return 1;
};

void MakeFaceBox(Vertex &A, Vertex &B, Vertex &C, FaceBox &fb)
{
	// Correct face box for each 
	// coordinate of the vertex
	for (int i = 0; i < 3; i++)
	{
		// Initialize 
		fb.Min[i] =  FLT_MAX;
		fb.Max[i] = -FLT_MAX;

		// Find it!
		if (A[i] > fb.Max[i]) fb.Max[i] = A[i];
		if (A[i] < fb.Min[i]) fb.Min[i] = A[i];

		if (B[i] > fb.Max[i]) fb.Max[i] = B[i];
		if (B[i] < fb.Min[i]) fb.Min[i] = B[i];

		if (C[i] > fb.Max[i]) fb.Max[i] = C[i];
		if (C[i] < fb.Min[i]) fb.Min[i] = C[i];

		// Get sizes of the face box
		fb.Size[i] = fb.Max[i] - fb.Min[i];		
	}
};

// Project model in Z-direction and return some values
bool Model::CalculateProjection(ProjectionParamsStruct &pps)
{
	// Set input values
	int Size_X = pps.Size_X;
	int Size_Y = pps.Size_Y;
	int Size_Z = pps.Size_Z;

	Vertex *LocalVertices = pps.LocalVertices;
	bool OnlyUpperBuffer = pps.OnlyUpperBuffer;

	// Go!
	int ModelArea = Size_X * Size_Y;
	float *Upper = new float [ModelArea];
	float *Lower = new float [ModelArea];
	
	float zLevelMax = -FLT_MAX;

	// Not enougth memory
	if ((!Upper) || (!Lower))
	{
		if (Upper) delete [] Upper;
		if (Lower) delete [] Lower;
		
		return false;
	}

	// Prepare buffers
	for (int j = 0; j < Size_Y; j++)
	{
		int BufferLine = j * Size_X;

		for (int i = 0; i < Size_X; i++)
		{
			int BufferPos = BufferLine + i;
			
			Upper[BufferPos] = -FLT_MAX;
			Lower[BufferPos] =  FLT_MAX;
		}
	}

	// Go! For each face...
	for (int k = 0; k < FacesCount; k++)
	{
		FaceIndices &f = Faces[k].Vertices;
		Vertex &fn = FacesNormals[k];

		// Look, fn[2] - size of the normal in Z-direction.
		// So, this will check wheter face is perpendicular
		// to projection plane. If it's perpendicular, then
		// nothing will be projected. Skip such faces...
		if (fabs(fn[2]) < FLT_EPSILON) continue;

		// Links to face vertices
		Vertex *F0, *F2;

		F0 = &LocalVertices[f[0]];
		F2 = &LocalVertices[f[2]];

		Vertex *A, *B, *C;
		B = &LocalVertices[f[1]];

		// Are A, B, C in right order?
		if (fn[2] > 0.0) { A = F0; C = F2; }
		else { A = F2; C = F0; }

		FaceBox fb;
		MakeFaceBox(*A, *B, *C, fb);
		
		int FaceSize_X = Round(fb.Size[0]);
		int FaceSize_Y = Round(fb.Size[1]);
		
		int FaceOffset_X = Round(fb.Min[0]);
		int FaceOffset_Y = Round(fb.Min[1]);

		float d = - (fn[0] * (*A)[0] + 
					 fn[1] * (*A)[1] + 
					 fn[2] * (*A)[2]);
		
		// Scan face field in both directions
		for (int j = 0; j <= FaceSize_Y; j++)
		{
			int y = FaceOffset_Y + j;

			for (int i = 0; i <= FaceSize_X; i++)
			{
				int x = FaceOffset_X + i;

				Vertex P;
				P[0] = (float) x;
				P[1] = (float) y;

				// This is FLAT check. 
				// Z-value of point P not needed.
				// This check could be performed faster...
				if (PtInTriangle(*A, *B, *C, P, PLANE_XY) >= 0)
				{
					int BufferPos = y * Size_X + x;
					float zValue = - (d + fn[0] * P[0] + fn[1] * P[1]) / fn[2];

					// Just one bad surface check...
					if (zValue < fb.Min[2]) zValue = fb.Min[2];
					if (zValue > fb.Max[2]) zValue = fb.Max[2];

					// Place zValue to buffers
					float &UpperValue = Upper[BufferPos];
					float &LowerValue = Lower[BufferPos];			

					if (zValue > UpperValue) UpperValue = zValue;
					if (zValue < LowerValue) LowerValue = zValue;
					
					// For normalization to 0..255
					if (zValue > zLevelMax) zLevelMax = zValue;
				}
			}
		}
	}

	// SEEMS BUFFERS MUST BE READY HERE!
	// START TO CALCULATE PHYSICAL PARAMS! YEAH!

	// Float, double... 
	// I like floats...
	double &T0 = pps.prs.T0 = 0.0;
	double &Tx = pps.prs.Tx = 0.0;
	double &Ty = pps.prs.Ty = 0.0;
	double &Tz = pps.prs.Tz = 0.0;

	double &Txx = pps.prs.Txx = 0.0;
	double &Tyy = pps.prs.Tyy = 0.0;
	double &Tzz = pps.prs.Tzz = 0.0;

	double &Txy = pps.prs.Txy = 0.0;
	double &Tyz = pps.prs.Tyz = 0.0;
	double &Tzx = pps.prs.Tzx = 0.0;

	// Or like this inplace of 
	// Upper[...] and Lower[...]
	// float *UpperValue = Upper;
	// float *LowerValue = Lower;
	// UpperValue++;
	// LowerValue++;

	// Ready to calculate params
	for (int j = 0; j < Size_Y; j++)
	{
		int BufferLine = j * Size_X;

		for (int i = 0; i < Size_X; i++)
		{
			int BufferPos = BufferLine + i;
			
			float &UpperValue = Upper[BufferPos];
			float &LowerValue = Lower[BufferPos];

			// Min value for buffers is 0, it's achieved
			// because of appropriate moving before:
			//		VectorMinusVector(v, Min);

			// What if model is just a plane at Z = 0?
			// We can be sure, that it will be processed
			// right if set empty <bufer>Values to (-1)
			if (UpperValue == -FLT_MAX) UpperValue = -1.0f;
			if (LowerValue ==  FLT_MAX) LowerValue = -1.0f;

			bool SameValues = (UpperValue == LowerValue);
			bool NeedPoint = (!SameValues || (SameValues && (UpperValue != -1.0f)));
			
			// Go!
			if (NeedPoint)
			{
				// Upper and Lower values
				double sUpper = (double) UpperValue;
				double sLower = (double) LowerValue;

				// Preparing
				if (OnlyUpperBuffer) sLower = 0.0;
				double zDelta = sUpper - sLower;
				
				// Null thickness surface check.
				// See comments about (-1) a bit above
			
				if (zDelta < DBL_EPSILON) 
				{
					zDelta = NULL_THICKNESS_WEIGHT;
					float HalfWeight = NULL_THICKNESS_WEIGHT / 2;
					
					sUpper += HalfWeight;
					sLower -= HalfWeight;
				}

				// Coordinates...
				double x = (double) i;
				double y = (double) j;

				double sUpper_s = sUpper * sUpper;
				double sLower_s = sLower * sLower;
				
				double zDelta2 = (sUpper_s - sLower_s) / 2;
				double zDelta3 = (sUpper_s * sUpper - sLower_s * sLower) / 3;

				T0 += zDelta;
				Tx += zDelta * x;
				Ty += zDelta * y;
				Tz += zDelta2;

				Txx += zDelta * x * x;
				Tyy += zDelta * y * y;
				Tzz += zDelta3;

				Txy += zDelta * x * y;
				Tyz += zDelta2 * y;
				Tzx += zDelta2 * x;
			}			
		}
	}

	// We have sqr(Scale) more points of 
	// calculation. So, we need to divide rusults 
	// by sqr(Scale) as we just summ all of them

	float Scale = pps.Scale;
	double Scale_s = Scale * Scale;
	double Scale_t = Scale_s * Scale;

	T0 /= Scale_s * Scale;
	Tx /= Scale_s * Scale_s;
	Ty /= Scale_s * Scale_s;
	Tz /= Scale_s * Scale_s;

	Txx /= Scale_s * Scale_t;
	Tyy /= Scale_s * Scale_t;
	Tzz /= Scale_s * Scale_t;

	Txy /= Scale_s * Scale_t;
	Tyz /= Scale_s * Scale_t;
	Tzx /= Scale_s * Scale_t;

	// --------------------
	// CALCULATION FINISHED 
	// --------------------

// Debug section
#ifdef WRITE_PROJECTION_BUFFERS
	float zScale = 1.0;
	if (zLevelMax > 0.0) zScale = 255 / (zLevelMax + 1);
	
	// DEBUG
	BYTE *cUpper = new BYTE [ModelArea];
	BYTE *cLower = new BYTE [ModelArea];

	// DEBUG
	for (int j = 0; j < Size_Y; j++)
	{
		for (int i = 0; i < Size_X; i++)
		{
			int BufferPos = j * Size_X + i;
			
			if (Upper[BufferPos] == -1.0f) cUpper[BufferPos] = 0;
			else cUpper[BufferPos] = Round(Upper[BufferPos] * zScale);

			if (Lower[BufferPos] == -1.0f) cLower[BufferPos] = 0;
			else cLower[BufferPos] = Round(Lower[BufferPos] * zScale);
		}
	}

	// DEBUG
	HANDLE hFile = CreateFileA("H:\\out_vangers.raw", GENERIC_WRITE, 0, NULL, 
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFile != INVALID_HANDLE_VALUE)
	{
		DWORD w;
		WriteFile(hFile, (LPVOID) cUpper, ModelArea * sizeof(BYTE), &w, NULL);
		WriteFile(hFile, (LPVOID) cLower, ModelArea * sizeof(BYTE), &w, NULL);

		CloseHandle(hFile);
	}

	// DEBUG
	if (cUpper) delete [] cUpper;
	if (cLower) delete [] cLower;
#endif

	// Free buffers
	if (Upper) delete [] Upper;
	if (Lower) delete [] Lower;

	return true;
};

// Is it much simpler to calculate these buffers 
// using some tricks with OpenGL? YES, I THINK SO! :)
// But it's interesting to me to make it by MY hands...

// Some code in this function is copied from original M_TOOL 
// programmed by Alexander "Sicher" Kotliar (mass center error fixed)
bool Model::CalculatePhysicalParams(PhysicalParamsStruct &pps, bool OnlyUpperBuffer)
{	
	// Vertices array for physical calculations
	Vertex *LocalVertices = new Vertex [VerticesCount];
	if (!LocalVertices) return false;

	// Copy existing geometry
	CopyMemory(LocalVertices, Vertices, VerticesCount * sizeof(Vertex));
	
	// -----------------------------
	// USE ONLY LOCALVERTICES BELOW!
	// -----------------------------

	// Total plain sizes of the model
	float FloatSize_X = (Max[0] - Min[0]);
	float FloatSize_Y = (Max[1] - Min[1]);
	float FloatSize_Z = (Max[2] - Min[2]);

	// Null or broken buffers...
	if ((FloatSize_X < FLT_EPSILON) || 
		(FloatSize_Y < FLT_EPSILON)) return false;

	// Resize for accuracy and
	// to limit calculation time
	const float MAX_SIZE = 249.0;

	float MaxSize = FloatSize_X;
	if (FloatSize_Y > MaxSize) MaxSize = FloatSize_Y;

	float PlainScale = MAX_SIZE / MaxSize;
	// float PlainScale = 1.0f;

	// Size of the model	
	int Size_X = Round((Max[0] - Min[0]) * PlainScale) + 1;
	int Size_Y = Round((Max[1] - Min[1]) * PlainScale) + 1;
	int Size_Z = Round((Max[2] - Min[2]) * PlainScale) + 1;

	// Preparing LocalVertices. 
	// Applying offset and scaling
	for (int i = 0; i < VerticesCount; i++)
	{
		Vertex &v = LocalVertices[i];

		// Move points to right position
		// and scale them as needed
		VectorMinusVector(v, Min);
		VectorMultiplyNumber(v, PlainScale);
	}	

	ProjectionParamsStruct prps;
	
	// Initialize projection params
	prps.OnlyUpperBuffer = OnlyUpperBuffer;
	prps.LocalVertices = LocalVertices;
	prps.Scale = PlainScale;
	prps.Size_X = Size_X;
	prps.Size_Y = Size_Y;
	prps.Size_Z = Size_Z;

	// XY PLANE (Z DIRECTION)
	// MAIN CALCULATIONS HERE
	CalculateProjection(prps);

	// Delete local data copy 
	// as it's not needed any more.
	// It was used in CalculateProjection()
	delete [] LocalVertices;

	// Calculation... Go!
	pps.Volume = prps.prs.T0;

	// Leave as is
	double Density = 1.0;
	double Mass = pps.Volume;

	// Mass center
	DBV &mc = pps.MassCenter;

	// One minus...
	mc[0] = prps.prs.Tx / pps.Volume;
	mc[1] = prps.prs.Ty / pps.Volume;
	mc[2] = prps.prs.Tz / pps.Volume;

	double &Txx = prps.prs.Txx;
	double &Tyy = prps.prs.Tyy;
	double &Tzz = prps.prs.Tzz;

	double &Tyz = prps.prs.Tyz;
	double &Tzx = prps.prs.Tzx;
	double &Txy = prps.prs.Txy;

	/*    
		0 1 2
	    -----
	0 | 0 1 2 
	1 | 3 4 5
	2 | 6 7 8
			  */
	
	// Inertia tensor
	DBM &J = pps.IntertiaTensor;
	
	J[0] = Density * (Tyy + Tzz);
	J[4] = Density * (Tzz + Txx);
	J[8] = Density * (Txx + Tyy);
	
	J[1] = J[3] = -Density * Txy;
	J[5] = J[7] = -Density * Tyz;
	J[2] = J[6] = -Density * Tzx;

	// Translate inertia tensor to mass center
	J[0] -= Mass * (mc[1] * mc[1] + mc[2] * mc[2]);
	J[4] -= Mass * (mc[2] * mc[2] + mc[0] * mc[0]);
	J[8] -= Mass * (mc[0] * mc[0] + mc[1] * mc[1]);
	
	J[1] = (J[3] += Mass * mc[0] * mc[1]);
	J[5] = (J[7] += Mass * mc[1] * mc[2]);
	J[2] = (J[6] += Mass * mc[2] * mc[0]);

	// Translate mass center to center of model
	mc[0] -= FloatSize_X / 2;
	mc[2] -= FloatSize_Z / 2;

#ifdef USE_M_TOOL_FEATURE_GEOMETRY
	J[1] = J[3] = -J[3];
	J[5] = J[7] = -J[7];

	mc[1] = FloatSize_Y - mc[1];
	mc[1] -= FloatSize_X / 2;
#else
	mc[1] -= FloatSize_Y / 2;
#endif

	return true;
};

void Model::CalculateFaceMiddle8(FaceIndices &f, Vertex8 &FaceMiddle)
{
	float sx, sy, sz;
	sx = sy = sz = 0.0f;

	for (int i = 0; i < 3; i++)
	{
		Vertex &v = Vertices[f[i]];
			
		sx += v[0];
		sy += v[1];
		sz += v[2];
	}

	// Two minuses...
	FaceMiddle[0] = Round(sx / 3);
	FaceMiddle[1] = Round(sy / 3);
	FaceMiddle[2] = Round(sz / 3);
};

void Model::CalculateVertex8(Vertex &v, Vertex8 &v8)
{
	// Two minuses...
	v8[0] = Round(v[0]);
	v8[1] = Round(v[1]);
	v8[2] = Round(v[2]);
};

void Model::CalculateNormal8(Vertex &n, Vertex8 &n8)
{
	float Norm = VectorNorm(n);
	
	if (Norm == 0.0)
	{
		n8[0] = 0;
		n8[1] = 0;
		n8[2] = 0;

		return ;
	}

	float scale = NORMAL / Norm;		
		
	Vertex Normalized;
	Normalized[0] = n[0] * scale;
	Normalized[1] = n[1] * scale;
	Normalized[2] = n[2] * scale;

	// Strange 8-bit vertex		
	CalculateVertex8(Normalized, n8);
};

// ------------ 
// SAVE SECTION
// ------------

//bool Model::SaveBinaryVertices(HANDLE hFile)
//{
//	DWORD Size = sizeof(VertexSavingStruct) * VerticesCount;
//	VertexSavingStruct *vs = new VertexSavingStruct [VerticesCount];
	
//	if (!vs) return false;
	
//	// Fill vertices saving structs
//	for (int i = 0; i < VerticesCount; i++)
//	{
//		// Ordinary vertex
//		Vertex &v = Vertices[i];
//		VertexSavingStruct &vss = vs[i];

//		// Two minuses...
//		vss.v[0] = v[0];
//		vss.v[1] = v[1];
//		vss.v[2] = v[2];
		
//		// Strange 8-bit vertex
//		CalculateVertex8(v, vss.v8);
//		vss.Sort = 0;
//	}

//	DWORD w = 0;
//	WriteFile(hFile, (LPVOID) vs, Size, &w, NULL);

//	delete [] vs;
	
//	if (w != Size) return false;

//	return true;
//};

//bool Model::SaveBinaryVerticesNormals(HANDLE hFile)
//{
//	DWORD Size = sizeof(NormalSavingStruct) * VerticesNormalsCount;
//	NormalSavingStruct *vns = new NormalSavingStruct [VerticesNormalsCount];
	
//	if (!vns) return false;
	
//	// Set reflection values to normals
//	for (int i = 0; i < FacesCount; i++)
//	{
//		Face &f = Faces[i];
		
//		for (int j = 0; j < 3; j++)
//		{
//			uint32_t idx = f.Normals[j];
//			vns[idx].Reflection = f.Reflection;
//		}
//	}

//	// Fill vertices normals saving structs
//	for (int i = 0; i < VerticesNormalsCount; i++)
//	{
//		// Strange 8-bit normal
//		CalculateNormal8(VerticesNormals[i], vns[i].n8);
//		vns[i].Sort = 0;
//	}

//	DWORD w = 0;
//	WriteFile(hFile, (LPVOID) vns, Size, &w, NULL);

//	delete [] vns;
	
//	if (w != Size) return false;

//	return true;
//};

//bool Model::SaveBinaryFaces(HANDLE hFile)
//{
//	DWORD Size = sizeof(FaceSavingStruct) * FacesCount;
//	FaceSavingStruct *fs = new FaceSavingStruct [FacesCount];
	
//	if (!fs) return false;
	
//	// Fill faces saving struct
//	for (int i = 0; i < FacesCount; i++)
//	{
//		FaceSavingStruct &fss = fs[i];

//		fss.FaceInfo.Sort = 0;
//		fss.FaceInfo.VerticesCount = 3;
//		fss.FaceInfo.ColorShift = 0;

//		// Strange 8-bit normal
//		CalculateNormal8(FacesNormals[i], fss.FaceInfo.Normal);
		
//		Face &f = Faces[i];
//		fss.FaceInfo.ColorID = f.ColorID;
//		fss.FaceInfo.Reflection = f.Reflection;

//		// Strange 8-bit face middle
//		CalculateFaceMiddle8(f.Vertices, fss.FaceInfo.Middle);

//		// Assign indices
//		for (int j = 0; j < 3; j++)
//		{
//			// Reverse order!
//			FaceVertexStruct &fvs = fss.Indices[2 - j];

//			fvs.VertexIndex = f.Vertices[j];
//			fvs.NormalIndex = f.Normals[j];
//		}
//	}

//	DWORD w = 0;
//	WriteFile(hFile, (LPVOID) fs, Size, &w, NULL);

//	delete [] fs;
	
//	if (w != Size) return false;

//	return true;
//};

//bool Model::SaveBinaryParams(HANDLE hFile, bool Bound)
//{
//	// Fill saving struct
//	ModelSavingParamsStruct msp;
//	ZeroMemory(&msp, sizeof(ModelSavingParamsStruct));

//	msp.C3D_VERSION_3 = 8;
	
//	// Geometry params
//	msp.FacesCount = FacesCount;
//	msp.VerticesCount = VerticesCount;
//	msp.FacesVerticesCount = FacesCount * 3;
//	msp.VerticesNormalsCount = VerticesNormalsCount;
	
//	msp.xMax = Round(Max[0]);
//	msp.yMax = Round(Max[1]);
//	msp.zMax = Round(Max[2]);

//	msp.xMin = Round(Min[0]);
//	msp.yMin = Round(Min[1]);
//	msp.zMin = Round(Min[2]);

//	msp.xOff = Round(Offset[0]);
//	msp.yOff = Round(Offset[1]);
//	msp.zOff = Round(Offset[2]);

//	msp.rMax = Round(CalculateRadius(false));
//	msp.Phi = msp.Psi = msp.Theta = 83;

//	msp.InertiaTensor[0] = 1.0;
//	msp.InertiaTensor[4] = 1.0;
//	msp.InertiaTensor[8] = 1.0;

//	if (!Bound)
//	{
//		// Physical params
//		PhysicalParamsStruct pps;
//		CalculatePhysicalParams(pps, false);

//		msp.Volume = pps.Volume;
	
//		for (int i = 0; i < 9; i++)
//			msp.InertiaTensor[i] = pps.IntertiaTensor[i];

//		for (int i = 0; i < 3; i++)
//			msp.MassCenter[i] = pps.MassCenter[i];
//	}

//	DWORD w = 0;
//	DWORD Size = sizeof(ModelSavingParamsStruct);
//	WriteFile(hFile, (LPVOID) &msp, Size, &w, NULL);
	
//	bool StatusOk = (w == Size);
//	return StatusOk;
//};

bool PutIntoList(SortData **Start, float Value, int FaceIndex)
{
	SortData *Current = *Start;
	SortData *InsertAfter = NULL;

	// No items in the list.
	// Fill first item with data
	if (Current -> FaceIndex == -1)
	{
		Current -> FaceCenter = Value;
		Current -> FaceIndex = FaceIndex;

		return true;
	}

	// New item
	SortData *Add = new SortData;
	if (!Add) return false;

	// Fill data
	Add -> FaceCenter = Value;
	Add -> FaceIndex = FaceIndex;

	// Find place
	while (Current)
	{
		if (Current -> FaceCenter > Value) break;
		
		InsertAfter = Current;
		Current = Current -> Next;
	}

	if (!InsertAfter)
	{
		// Insert before first item
		Add -> Next = *Start;
		*Start = Add;
	}
	else
	{
		Add -> Next = InsertAfter -> Next;
		InsertAfter -> Next = Add;
	}

	return true;
};

//// Saves sorted polygons in
//// x-order, y-order, z-order
//bool Model::SaveBinarySortings(HANDLE hFile)
//{
//	// Initialize lists
//	SortData *Start[3] = {NULL, NULL, NULL};

//	// Prepare lists
//	bool StatusOk = true;
//	for (int i = 0; i < 3; i++)
//	{
//		Start[i] = new SortData;
//		if (!Start[i])
//		{
//			StatusOk = false;
//			break;
//		}
		
//		Start[i] -> Next = NULL;
//		Start[i] -> FaceIndex = -1;
//		Start[i] -> FaceCenter = INT_MIN;
//	}

//	// Error...
//	if (!StatusOk)
//	{
//		for (int i = 0; i < 3; i++)
//			if (Start[i]) delete Start[i];
		
//		return false;
//	}

//	// Go!
//	for (int i = 0; i < FacesCount; i++)
//	{
//		FaceIndices &f = Faces[i].Vertices;

//		Vertex FaceCenter;
//		FaceCenter[0] = 0.0f;
//		FaceCenter[1] = 0.0f;
//		FaceCenter[2] = 0.0f;

//		for (int j = 0; j < 3; j++)
//		{
//			Vertex &v = Vertices[f[j]];
			
//			FaceCenter[0] += v[0];
//			FaceCenter[1] += v[1];
//			FaceCenter[2] += v[2];
//		}

//		for (int j = 0; j < 3; j++)
//		{
//			FaceCenter[j] /= 3;
//			PutIntoList(&Start[j], FaceCenter[j], i);
//		}
//	}

//	// Saving array
//	int *SavingData;
//	SavingData = new int [FacesCount];
//	if (!SavingData) StatusOk = false;

//	// Save...
//	if (StatusOk)
//	{
//		for (int i = 0; i < 3; i++)
//		{
//			SortData *Current = Start[i];
			
//			// Backward order...
//			for (int j = 0; j < FacesCount; j++)
//			{
//				// Check...
//				if (!Current)
//				{
//					StatusOk = false;
//					break;
//				}

//				SavingData[j] = Current -> FaceIndex;
//				Current = Current -> Next;
//			}

//			// Check...
//			if (!StatusOk) break;

//			// Saving
//			DWORD w = 0;
//			DWORD Size = sizeof(int) * FacesCount;
//			WriteFile(hFile, (LPVOID) SavingData, Size, &w, NULL);
	
//			bool StatusOk = (w == Size);

//			// Check...
//			if (!StatusOk) break;
//		}
//	}

//	// Free array
//	delete [] SavingData;

//	// Delete arrays
//	for (int i = 0; i < 3; i++)
//	{
//		SortData *Deleted = NULL;
//		SortData *Current = Start[i];

//		while (Current)
//		{
//			Deleted = Current;
//			Current = Current -> Next;
//			delete Deleted;
//		}
//	}

//	return StatusOk;
//};

// ------------
// LOAD SECTION
// ------------

//bool Model::LoadBinarySortings(HANDLE hFile, int SavedFacesCount)
//{
//	int *SavingData;
//	SavingData = new int [SavedFacesCount];
//	if (!SavingData) return false;

//	bool StatusOk = true;
//	DWORD Size = sizeof(int) * SavedFacesCount;

//	// Reading
//	for (int i = 0; i < 3; i++)
//	{
//		DWORD w = 0;
//		ReadFile(hFile, (LPVOID) SavingData, Size, &w, NULL);
	
//		StatusOk = (w == Size);
//		if (!StatusOk) break;
//	}

//	// Free array
//	delete [] SavingData;

//	return StatusOk;
//};

//bool Model::LoadBinaryFaces(HANDLE hFile)
//{
//	bool Result = true;

//	Face_v *NewFaces_v = new Face_v [FacesCount];
//	Vertex *NewFacesNormals_v = new Vertex [FacesCount];

//	// Memory check
//	if (!NewFaces_v || !NewFacesNormals_v)
//	{
//		if (NewFaces_v) delete [] NewFaces_v;
//		if (NewFacesNormals_v) delete [] NewFacesNormals_v;

//		return false;
//	}
	
//	// Reset memory
//	ZeroMemory(NewFaces_v, sizeof(Face_v) * FacesCount);
	
//	// Number of 3-vertex faces
//	int NewFacesCount = 0;

//	// Read faces saving struct
//	for (int i = 0; i < FacesCount; i++)
//	{
//		DWORD w = 0;
//		FaceSavingInfo fsi;
//		ReadFile(hFile, (LPVOID) &fsi, sizeof(FaceSavingInfo), &w, NULL);
		
//		if (w != sizeof(FaceSavingInfo))
//		{
//			Result = false;
//			break;
//		}

//		Face_v &fv = NewFaces_v[i];
//		fv.VerticesCount = fsi.VerticesCount;
//		fv.Normals = new uint32_t [fsi.VerticesCount];
//		fv.Vertices = new uint32_t [fsi.VerticesCount];
		
//		FaceVertexStruct *fvs = new FaceVertexStruct [fsi.VerticesCount];

//		if (!fv.Normals || !fv.Vertices || !fvs)
//		{
//			if (fv.Vertices) delete [] fv.Vertices;
//			if (fv.Normals) delete [] fv.Normals;

//			if  (fvs) delete [] fvs;
			
//			fv.Vertices = NULL;
//			fv.Normals = NULL;

//			Result = false;
//			break;
//		}

//		w = 0;
//		DWORD Size = sizeof(FaceVertexStruct) * fsi.VerticesCount;

//		// Read indices
//		ReadFile(hFile, (LPVOID) fvs, Size, &w, NULL);
//		Result = (w == Size);

//		if (!Result)
//		{
//			delete [] fvs;
//			break;
//		}
			
//		if (Result)
//		{
//			int FacesNumber = 1;
//			if (fsi.VerticesCount > 3)
//				FacesNumber = fsi.VerticesCount - 2;
		
//			NewFacesCount += FacesNumber;

//			fv.ColorID = (ColorsIDs) fsi.ColorID;
//			fv.Reflection = fsi.Reflection;
			
//			// Face normal
//			Vertex &fnv = NewFacesNormals_v[i];
//			for (int j = 0; j < 3; j++)
//				fnv[j] = fsi.Normal[j];

//			// Normalize
//			VectorNormalize(fnv);

//			// Assign indices
//			for (int j = 0; j < fv.VerticesCount; j++)
//			{
//				// Reverse order!
//				FaceVertexStruct &fvss = fvs[fv.VerticesCount - 1 - j];

//				fv.Vertices[j] = fvss.VertexIndex;
//				fv.Normals[j] = fvss.NormalIndex;
//			}
//		}

//		delete [] fvs;
//	}

//	// Count of 3-vertex faces
//	int Faces3Count = 0;

//	if (Result)
//	{
//		if (Faces) delete [] Faces;
//		if (FacesNormals) delete [] FacesNormals;

//		// Reallocate memory for faces
//		Faces = new Face [NewFacesCount];
//		FacesNormals = new Vertex [NewFacesCount];

//		// Memory check
//		if (!Faces || !FacesNormals)
//		{
//			if (Faces) delete [] Faces;
//			if (FacesNormals) delete [] FacesNormals;

//			Faces = NULL;
//			FacesNormals = NULL;

//			Result = false;
//		}
//	}

//	if (Result)
//	{
//		// If NewFacesCount equal to FacesCount
//		// then I deal with 3-vertex faces. If
//		// they are not equal, then faces could
//		// have more than 3 vertices. In this
//		// case, I will convert n-vertex faces
//		// to 3-vertex faces (see below)
		
//		if (NewFacesCount == FacesCount)
//		{
//			// They are equal here
//			Faces3Count = NewFacesCount;

//			// Set indices
//			for (int i = 0; i < FacesCount; i++)
//			{
//				Face &f = Faces[i];
//				Vertex &fn = FacesNormals[i];
				
//				Face_v &fv = NewFaces_v[i];
//				Vertex &fvn = NewFacesNormals_v[i];

//				// Normal
//				VectorCopyVector(fn, fvn);

//				f.ColorID = fv.ColorID;
//				f.Reflection = fv.Reflection;
				
//				for (int j = 0; j < 3; j++)
//				{
//					f.Vertices[j] = fv.Vertices[j];
//					f.Normals[j] = fv.Normals[j];
//				}
//			}
//		}
//		else
//		{
//			// Conversion from n-vertex
//			// faces to 3-vertex faces
//			for (int i = 0; i < FacesCount; i++)
//			{
//				Face_v &fv = NewFaces_v[i];
//				Vertex &fnv = NewFacesNormals_v[i];

//				int FacesNumber = 1;
//				if (fv.VerticesCount > 3)
//					FacesNumber = fv.VerticesCount - 2;

//				for (int j = 0; j < FacesNumber; j++)
//				{
//					Face &f = Faces[Faces3Count];
				
//					VectorCopyVector(FacesNormals[Faces3Count], fnv);
	
//					f.ColorID = fv.ColorID;
//					f.Reflection = fv.Reflection;
			
//					f.Vertices[0] = fv.Vertices[0];
//					f.Normals[0] = fv.Normals[0];

//					for (int k = 1; k < 3; k++)
//					{
//						f.Vertices[k] = fv.Vertices[k + j];
//						f.Normals[k] = fv.Normals[k + j];
//					}

//					// Increase
//					Faces3Count++;
//				}
//			}
//		}
//	}

//	for (int i = 0; i < FacesCount; i++)
//	{
//		Face_v &fv = NewFaces_v[i];
		
//		if (fv.Vertices) delete [] fv.Vertices;
//		if (fv.Normals) delete [] fv.Normals;
//	}
	
//	delete [] NewFaces_v;
//	delete [] NewFacesNormals_v;

//	if (Result)
//	{
//		FacesCount = Faces3Count;
//		FacesNormalsCount = Faces3Count;
//	}

//	return Result;
//};

//bool Model::LoadBinaryVerticesNormals(HANDLE hFile)
//{
//	DWORD Size = sizeof(NormalSavingStruct) * VerticesNormalsCount;
//	NormalSavingStruct *vns = new NormalSavingStruct [VerticesNormalsCount];
	
//	if (!vns) return false;
	
//	DWORD w = 0;
//	ReadFile(hFile, (LPVOID) vns, Size, &w, NULL);

//	bool Result = (w == Size);

//	if (Result)
//	{
//		// Fill vertices normals saving structs
//		for (int i = 0; i < VerticesNormalsCount; i++)
//		{
//			Vertex &n = VerticesNormals[i];
//			Vertex8 &n8 = vns[i].n8;
	
//			for (int j = 0; j < 3; j++)
//				n[j] = n8[j];

//			VectorNormalize(n);
//		}
//	}

//	delete [] vns;
	
//	return Result;
//};

//bool Model::LoadBinaryVertices(HANDLE hFile)
//{
//	DWORD Size = sizeof(VertexSavingStruct) * VerticesCount;
//	VertexSavingStruct *vs = new VertexSavingStruct [VerticesCount];
	
//	if (!vs) return false;

//	DWORD w = 0;
//	ReadFile(hFile, (LPVOID) vs, Size, &w, NULL);
	
//	bool Result = (w == Size);

//	if (Result)
//	{
//		// Read vertices saving structs
//		for (int i = 0; i < VerticesCount; i++)
//		{
//			// Ordinary vertex
//			Vertex &v = Vertices[i];
//			VertexSavingStruct &vss = vs[i];

//			// Two minuses...
//			v[0] = vss.v[0];
//			v[1] = vss.v[1];
//			v[2] = vss.v[2];
//		}
//	}

//	delete [] vs;

//	// Vertices read. I can
//	// find bounding box
//	CalculateDims();

//	return Result;
//};

//bool Model::LoadBinaryParams(HANDLE hFile)
//{
//	ModelSavingParamsStruct msp;
	
//	DWORD w = 0;
//	DWORD Size = sizeof(ModelSavingParamsStruct);
//	ReadFile(hFile, (LPVOID) &msp, Size, &w, NULL);
//	if (w != Size) return false;

//	// Check...
//	if (msp.FacesCount < 0) return false;
//	if (msp.FacesVerticesCount < 0) return false;

//	if (msp.VerticesCount < 0) return false;
//	if (msp.VerticesNormalsCount < 0) return false;
	
//	// Geometry params
//	FacesCount = msp.FacesCount;
//	FacesNormalsCount = msp.FacesCount;

//	VerticesCount = msp.VerticesCount;
//	VerticesNormalsCount = msp.VerticesNormalsCount;
	
//	EmptyModel();

//	// Allocate memory
//	Faces = new Face [FacesCount];
//	FacesNormals = new Vertex [FacesNormalsCount];
	
//	Vertices = new Vertex [VerticesCount];
//	VerticesNormals = new Vertex [VerticesNormalsCount];

//	// Memory check
//	if (!Faces || !FacesNormals ||
//		!Vertices || !VerticesNormals)
//	{
//		EmptyModel();
//		return false;
//	}

//	return true;
//};

void MakeVertex(Vertex &v, float x, float y, float z)
{
	v[0] = x;
	v[1] = y;
	v[2] = z;
};

void MakeFace(Face &f, int v1, int v2, int v3)
{
	f.Vertices[0] = (uint32_t) v1;
	f.Vertices[1] = (uint32_t) v2;
	f.Vertices[2] = (uint32_t) v3;
};

void FacesFromRect(Face &f1, Face &f2, int p, int Segments)
{
	int v1 = p;
	int v2 = p + (Segments + 1);
	int v3 = p + (Segments + 1) + 1;
	int v4 = p + 1;
	
	MakeFace(f1, v1, v2, v3);
	MakeFace(f2, v1, v3, v4);
};

// Make triangulated box which is bound of 
// model with choosen subdivs level into m
bool Model::MakeModelBound(Model *m, int Segments)
{
	if (Segments < 1) Segments = 1;
	if (Segments > 99) Segments = 99;
	
	m -> EmptyModel();

	int RectsOnSide = Segments * Segments;
	int PointsOnSide = (Segments + 1) * (Segments + 1);

	m -> FacesCount = 6 * 2 * RectsOnSide;
	m -> VerticesCount = 6 * PointsOnSide;
	m -> FacesNormalsCount = m -> FacesCount;
	m -> VerticesNormalsCount = m -> VerticesCount;

	m -> Faces = new Face [m -> FacesCount];
	m -> Vertices = new Vertex [m -> VerticesCount];
	m -> FacesNormals = new Vertex [m -> FacesNormalsCount];
	m -> VerticesNormals = new Vertex [m -> VerticesNormalsCount];

	if (!m -> Faces || !m -> FacesNormals || 
		!m -> Vertices || !m -> VerticesNormals)
	{
		m -> EmptyModel();
		return false;
	}
	
	float dx = (Max[0] - Min[0]) / Segments;
	float dy = (Max[1] - Min[1]) / Segments;
	float dz = (Max[2] - Min[2]) / Segments;

	// Calculate coordinates
	for (int j = 0; j < Segments + 1; j++)
	for (int i = 0; i < Segments + 1; i++)
	{
		int Index = j * (Segments + 1) + i;

		Vertex *mVertex = &(m -> Vertices[Index]);

		// XY PLANE (Z-MAX)
		(*mVertex)[0] = Max[0] - i * dx;
		(*mVertex)[1] = Min[1] + j * dy;
		(*mVertex)[2] = Max[2];

		// XY PLANE (Z-MIN)
		mVertex += PointsOnSide;
		(*mVertex)[0] = Min[0] + i * dx;
		(*mVertex)[1] = Min[1] + j * dy;
		(*mVertex)[2] = Min[2];

		// ZX PLANE (Y-MAX)
		mVertex += PointsOnSide;
		(*mVertex)[0] = Min[0] + i * dx;
		(*mVertex)[1] = Max[1];
		(*mVertex)[2] = Min[2] + j * dz;

		// ZX PLANE (Y-MIN)
		mVertex += PointsOnSide;
		(*mVertex)[0] = Max[0] - i * dx;
		(*mVertex)[1] = Min[1];
		(*mVertex)[2] = Min[2] + j * dz;

		// YZ PLANE (X-MAX)
		mVertex += PointsOnSide;
		(*mVertex)[0] = Max[0];
		(*mVertex)[1] = Max[1] - i * dy;
		(*mVertex)[2] = Min[2] + j * dz;

		// YZ PLANE (X-MIN)
		mVertex += PointsOnSide;
		(*mVertex)[0] = Min[0];
		(*mVertex)[1] = Min[1] + i * dy;
		(*mVertex)[2] = Min[2] + j * dz;
	}

	// Iteration variable
	Face *mFaces = m -> Faces;

	// Construct faces
	for (int j = 0; j < Segments; j++)
	for (int i = 0; i < Segments; i++)
	{
		int Index = j * (Segments + 1) + i;
		
		for (int k = 0; k < 6; k++)
		{
			FacesFromRect(*mFaces, *(mFaces + 1), Index, Segments);
			Index += PointsOnSide;
			mFaces += 2;
		}
	}

	m -> MakeFacesNormals();
	m -> MakeVerticesNormals();
	m -> CalculateDims();

	// Set colors for bound polygons
	for (int i = 0; i < m -> FacesCount; i++)
	{
		Face &mf = m -> Faces[i];
		mf.Reflection = BOUND_REFLECTION;

		int LowerVerticesCount = 0;
		for (int j = 0; j < 3; j++)
		{
			Vertex &mv = m -> Vertices[mf.Vertices[j]];
			if (mv[2] == m -> Min[2]) LowerVerticesCount++;
		}

		switch (LowerVerticesCount)
		{
			case 0 : { mf.ColorID = (ColorsIDs) UPPER_BODY_POLYGON; break; }
			case 1 : 
			case 2 : { mf.ColorID = (ColorsIDs) SIDE_BODY_POLYGON; break; }
			case 3 : { mf.ColorID = (ColorsIDs) LOWER_BODY_POLYGON; break; }
		}
	}

	return true;
};

// Morph model pointed by m to this. This is the target, m - source
bool Model::MorphModelBound(Model *m, bool SetColors)
{

#ifdef USE_M_TOOL_FEATURE_BOUND
	// Shows whether vertex belongs 
	// to the lower level of the box
	bool *LockVertex = NULL;

	if (SetColors)
	{
		LockVertex = new bool [m -> VerticesCount];
		if (!LockVertex) return false;

		ZeroMemory(LockVertex, sizeof(bool) * m -> VerticesCount);
	
		for (int i = 0; i < m -> FacesCount; i++)
		{
			Face &mf = m -> Faces[i];
			
			if (mf.ColorID == LOWER_BODY_POLYGON)
				for (int j = 0; j < 3; j++)
					LockVertex[mf.Vertices[j]] = true;
		}
	}
#endif

	for (int i = 0; i < m -> VerticesCount; i++)
	{
		Vertex &sv = m -> Vertices[i];

		double Morph[3] = {0.0, 0.0, 0.0};
		double MorphWeight = 0.0;

		// Accumulate vertex with weighting
		for (int j = 0; j < VerticesCount; j++)
		{
			Vertex &tv = Vertices[j];

			Vertex Distance;
			double SourceWeight[3];
			
			// Distance between source and target vertices
			VectorMinusVector2(tv, sv, Distance);
			double Weight = (double) VectorNorm(Distance);

			// The closer, the more
			Weight = pow(Weight, 24);

			// Source vertex will be used with it's weight
			SourceWeight[0] = (double) tv[0] / Weight;
			SourceWeight[1] = (double) tv[1] / Weight;
			SourceWeight[2] = (double) tv[2] / Weight;

			Morph[0] += SourceWeight[0];
			Morph[1] += SourceWeight[1];
			Morph[2] += SourceWeight[2];

			MorphWeight += 1 / Weight;
		}
		
		// Weighting back
		if (MorphWeight)
		{
			Morph[0] /= MorphWeight;
			Morph[1] /= MorphWeight;
			Morph[2] /= MorphWeight;

#ifdef USE_M_TOOL_FEATURE_BOUND
			sv[1] = (float) Morph[1];
			
			if (!SetColors || !LockVertex[i])
			{
				sv[0] = (float) Morph[0];
				sv[2] = (float) Morph[2];
			}
#else
			sv[0] = (float) Morph[0];
			sv[1] = (float) Morph[1];
			sv[2] = (float) Morph[2];
#endif

		}
	}

	for (int i = 0; i < m -> FacesCount; i++)
	{
		Face &mf = m -> Faces[i];
			
		if (SetColors)
		{
			// Move lowest polygons to the bottom
			if (mf.ColorID == LOWER_BODY_POLYGON)
			{
				for (int j = 0; j < 3; j++)
				{
					Vertex &mv = m -> Vertices[mf.Vertices[j]];
					mv[2] = m -> Min[2];
				}
			}
		}
		else
		{
			mf.ColorID = (ColorsIDs) UPPER_BODY_POLYGON;
		}
	}

#ifdef USE_M_TOOL_FEATURE_BOUND
	if (LockVertex) delete [] LockVertex;
#endif

	m -> CalculateDims();
	m -> MakeFacesNormals();
	m -> MakeVerticesNormals();

	return true;
};

// Optimization before saving
bool Model::OptimizeGeometry(bool IgnoreColors)
{
	bool Result = true;

	// Get weld and smooth limits
//	int WeldLimit = (int) MechousParams[MECHOUS_PARAM_WELD_LIMIT];
//	int AngleLimit = (int) MechousParams[MECHOUS_PARAM_ANGLE_LIMIT];
	int WeldLimit = (int)2.5;
	int AngleLimit = 60;

	// Vertex to faces links
	VertexLink *VertexLinks;
	
	// Create vertex links
	Result = MakeVertexLinks(&VertexLinks);
	int VertexLinksLength = VerticesCount;
	
	if (Result) 
	{
		// Perform welding of vertices
		Result = MakeVerticesWeld(VertexLinks, WeldLimit, IgnoreColors);
		FreeVertexLinks(&VertexLinks, VertexLinksLength);

#ifdef SHOW_OPTIMIZATION_STAGES
	Model *m = Models.AddModel();
	CloneModel(m);
	m -> SetModelName("Debug model: WeldVertices", false);
	m -> ModelType = MODEL_UNKNOWN;
	Models.AddListBoxModel(m);
#endif

		if (Result) 
		{
			// Remove bad faces
			Result = RemoveSingularFaces();

#ifdef SHOW_OPTIMIZATION_STAGES
	Model *m = Models.AddModel();
	CloneModel(m);
	m -> SetModelName("Debug model: SingularFaces", false);
	m -> ModelType = MODEL_UNKNOWN;
	Models.AddListBoxModel(m);
#endif

			if (Result)
			{
				// Recreate vertex links as vertices 
				// array could be changed while welding
				Result = MakeVertexLinks(&VertexLinks);
				VertexLinksLength = VerticesCount;
		
				// Perform smoothing of normals
				if (Result) Result = MakeVerticesNormalsEdged(VertexLinks, AngleLimit);

				// Free memory
				FreeVertexLinks(&VertexLinks, VertexLinksLength);

#ifdef SHOW_OPTIMIZATION_STAGES
	Model *m = Models.AddModel();
	CloneModel(m);
	m -> SetModelName("Debug model: EdgedNormals", false);
	m -> ModelType = MODEL_UNKNOWN;
	Models.AddListBoxModel(m);
#endif

			}
		}
	}
	else
	{
		// Some error occured...
		FreeVertexLinks(&VertexLinks, VertexLinksLength);
	}

	return Result;
};

// Saves geomerty data.
// Full analog of Model::saveC3D() 
// method from original M_TOOL utility.

// If Bound flag is set, then no geometry data will
// be calculated. The default values will be saved

//bool Model::SaveBinary(HANDLE hFile, bool Bound)
//{
//	bool Result = true;
	
//	Result = SaveBinaryParams(hFile, Bound);
//	if (!Result) return false;

//	Result = SaveBinaryVertices(hFile);
//	if (!Result) return false;

//	Result = SaveBinaryVerticesNormals(hFile);
//	if (!Result) return false;

//	Result = SaveBinaryFaces(hFile);
//	if (!Result) return false;
	
//	Result = SaveBinarySortings(hFile);
//	if (!Result) return false;

//	return true;
//};

//bool Model::LoadBinary(HANDLE hFile)
//{
//	bool Result = true;

//	Result = LoadBinaryParams(hFile);
//	if (!Result) return false;
//	int SavedFacesCount = FacesCount;

//	Result = LoadBinaryVertices(hFile);
//	if (!Result) return false;

//	Result = LoadBinaryVerticesNormals(hFile);
//	if (!Result) return false;

//	Result = LoadBinaryFaces(hFile);
//	if (!Result) return false;
	
//	Result = LoadBinarySortings(hFile, SavedFacesCount);
//	if (!Result) return false;

//	return true;
//};

// -------------
// ASCII ROUTINE
// -------------

//bool Model::SaveAsciiFaces(HANDLE hFile, DWORD StartVertexIndex)
//{
//	const int FaceLineLen = 22;
//	char *FaceLine = new char[FaceLineLen];
//	if (!FaceLine) return false;

//	bool Result = true;
//	for (int i = 0; i < FacesCount; i++)
//	{
//		FaceIndices &f = Faces[i].Vertices;
		
//		FaceIndices fn = {0, 0, 0};
//		fn[0] = f[0] + StartVertexIndex + 1;
//		fn[1] = f[1] + StartVertexIndex + 1;
//		fn[2] = f[2] + StartVertexIndex + 1;

//		_snprintf_s(FaceLine, FaceLineLen, _TRUNCATE,
//			"f %5d %5d %5d\r\n", fn[0], fn[1], fn[2]);
		
//		DWORD w = 0;
//		WriteFile(hFile, FaceLine, FaceLineLen - 1, &w, NULL);

//		if (FaceLineLen - 1 != w)
//		{
//			Result = false;
//			break;
//		}
//	}

//	delete [] FaceLine;
//	return Result;
//};

//bool Model::SaveAsciiVertices(HANDLE hFile)
//{
//	const int VertexLineLen = 43;
//	char *VertexLine = new char[VertexLineLen];
//	if (!VertexLine) return false;

//	DWORD w = 0;
//	bool Result = true;
//	for (int i = 0; i < VerticesCount; i++)
//	{
//		Vertex &v = Vertices[i];

//		// Reverse order. Fixup? wtf
//		ZeroMemory(VertexLine, VertexLineLen);
//		_snprintf_s(VertexLine, VertexLineLen, _TRUNCATE,
//			"v % 12.6f % 12.6f % 12.6f\r\n", -v[0], v[2], v[1]);
		
//		WriteFile(hFile, VertexLine, VertexLineLen - 1, &w, NULL);

//		if (VertexLineLen - 1 != w)
//		{
//			Result = false;
//			break;
//		}
//	}

//	// Empty line
//	WriteFile(hFile, "\r\n", 2, &w, NULL);
//	if (w != 2) Result = false;

//	delete [] VertexLine;
//	return Result;
//};

//bool Model::SaveAscii(HANDLE hFile, DWORD StartVertexIndex)
//{
//	DWORD GroupNameLen = 2 + (DWORD) strlen(ModelName) + 3;
//	char *GroupName = new char [GroupNameLen];

//	if (!GroupName) return false;
	
//	// Write group identifier
//	ZeroMemory(GroupName, GroupNameLen);
//	_snprintf_s(GroupName, GroupNameLen, _TRUNCATE, "g %s\r\n", ModelName);
	
//	DWORD w = 0;
//	WriteFile(hFile, (LPVOID) GroupName, GroupNameLen - 1, &w, NULL);
//	delete [] GroupName;

//	// Error writing to the file
//	if (GroupNameLen - 1 != w) return false;
	
//	// Write vertices
//	bool Result = SaveAsciiVertices(hFile);
//	if (!Result) return false;

//	// Write faces
//	Result = SaveAsciiFaces(hFile, StartVertexIndex);
//	if (!Result) return false;

//	// Empty line
//	WriteFile(hFile, "\r\n", 2, &w, NULL);
//	if (w != 2) Result = false;

//	return Result;
//};
