#ifndef VOLINT_H
#define VOLINT_H


namespace volint {
	struct Polyhedron;
	struct PolyhedronFace {
	  int numVerts;
	  double norm[3];
	  double w;
	  int* verts;
	  Polyhedron *poly;
	} ;

	struct Polyhedron {
	  int numVerts, numFaces;
	  double(* verts)[3];
	  PolyhedronFace* faces;
	};

	class VolumeIntegral {
	public:
		void compute(Polyhedron& p, double J[3][3], double r[3], double& volume);
	private:
		void compVolumeIntegrals(Polyhedron *p);
		void compFaceIntegrals(PolyhedronFace* f);
		void compProjectionIntegrals(PolyhedronFace* f);
		void compNormals(Polyhedron* p);

		int A;   /* alpha */
		int B;   /* beta */
		int C;   /* gamma */

		/* projection integrals */
		double P1, Pa, Pb, Paa, Pab, Pbb, Paaa, Paab, Pabb, Pbbb;

		/* face integrals */
		double Fa, Fb, Fc, Faa, Fbb, Fcc, Faaa, Fbbb, Fccc, Faab, Fbbc, Fcca;

		/* volume integrals */
		double T0, T1[3], T2[3], TP[3];
	};
}
#endif // VOLINT_H
