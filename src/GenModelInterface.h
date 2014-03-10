// GenModelInterface.h : main header file for the GenModelDLL DLL
//

#pragma once

#include "GenModel.h"

#ifdef WIN64
    #define GenModelDLL_API __declspec(dllexport)
    #define STDCALL __stdcall
    #define GENMODEL_EXTERN_C_BEGIN 
    #define GENMODEL_EXTERN_C_END 
#else
    #define GenModelDLL_API
    #define STDCALL
    #define GENMODEL_EXTERN_C_BEGIN extern "C" {
    #define GENMODEL_EXTERN_C_END }
#endif

#ifdef CPLEX_MODULE
	#define CPLEX_CREATE gmmap.push_back(new GenModelCplex())
    #define CPLEX_EXIST true
#else
	#define CPLEX_CREATE throw string("Cplex solver not available")
    #define CPLEX_EXIST false
#endif

#ifdef GUROBI_MODULE
	#define GUROBI_CREATE gmmap.push_back(new GenModelGurobi())
    #define GUROBI_EXIST true
#else
	#define GUROBI_CREATE throw string("Gurobi solver not available")
    #define GUROBI_EXIST false
#endif

#ifdef HG_MODULE
	#define HG_CREATE gmmap.push_back(new GenModelHG())
    #define HG_EXIST true
#else
	#define HG_CREATE throw string("Hypergraph solver not available")
    #define HG_EXIST false
#endif

#ifdef GLPK_MODULE
	#define GLPK_CREATE gmmap.push_back(new GenModelGlpk())
    #define GLPK_EXIST true
#else
	#define GLPK_CREATE throw string("Glpk solver not available")
    #define GLPK_EXIST false
#endif

#ifdef SCIP_MODULE
    #define SCIP_CREATE gmmap.push_back(new GenModelScip())
    #define SCIP_EXIST true
#else
    #define SCIP_CREATE throw string("Scip solver not available")
    #define SCIP_EXIST false
#endif

#ifdef OSI_MODULE
    #define OSI_CREATE gmmap.push_back(new GenModelOsi())
    #define OSI_EXIST true
#else
    #define OSI_CREATE throw string("Coin solver not available")
    #define OSI_EXIST false
#endif

GENMODEL_EXTERN_C_BEGIN

GenModelDLL_API double STDCALL _FindConstraintMaxLhs(long row, long token);
GenModelDLL_API double STDCALL _FindConstraintMinLhs(long row, long token);
GenModelDLL_API long STDCALL _MakeConstraintFeasible(long row, long token);
GenModelDLL_API long STDCALL _WriteProblemToLpFile(char* filename, long token);
GenModelDLL_API long STDCALL _WriteSolutionToFile(char* filename, long token);
GenModelDLL_API long STDCALL _AddConst(char* cname, double rhs, char sense, long token);
GenModelDLL_API bool STDCALL _AddConstBulk(char* cname, double* rhs, long length, char sense, long token);
GenModelDLL_API long STDCALL _AddVar(char* nn, double o, double l, double u, char t, long token);
GenModelDLL_API bool STDCALL _AddVarBulk(char* nn, double* o, long length, double l, double u, char t, long token);
GenModelDLL_API long STDCALL _AddNz(long row, long col, double val, long token);
GenModelDLL_API long STDCALL _AddNzToLast(long col, double val, long token);
GenModelDLL_API long STDCALL _AddNzBulk(long* rows, long* cols, double* values, long valuesLength, long rowCount, long colCount, long iterations, long token);
GenModelDLL_API long STDCALL _SetQpCoef(long i, long j, double val, long token);
GenModelDLL_API long STDCALL _SetNumbers(long token);
GenModelDLL_API long STDCALL _SetLongParam(char* param, long val, long token);
GenModelDLL_API long STDCALL _SetDblParam(char* param, double val, long token);
GenModelDLL_API long STDCALL _SetBoolParam(char* param, bool val, long token);
GenModelDLL_API long STDCALL _SetStrParam(char* param, char* val, long token);
GenModelDLL_API long STDCALL _CreateNewModel(char type, char* name);
GenModelDLL_API bool STDCALL _IsSolverAvailable(char type);
GenModelDLL_API long STDCALL _CopyOrder(long token, int count, int* ind, int* weight);
GenModelDLL_API long STDCALL _DeleteModel(long token);
GenModelDLL_API long STDCALL _CreateModel(long token);
GenModelDLL_API long STDCALL _SolveModel(long token);
GenModelDLL_API bool STDCALL _GetSolVars(double* values, long length, long token);
GenModelDLL_API bool STDCALL _HasSolution(long token);
GenModelDLL_API bool STDCALL _GetDualPrices(double* values, long length, long token);
GenModelDLL_API bool STDCALL _GetSlacks(double* values, long length, long token);
GenModelDLL_API bool STDCALL _GetReducedCosts(double* values, long length, long token);
GenModelDLL_API bool STDCALL _GetRowValues(double* values, long length, long rowIndex, long token);
GenModelDLL_API bool STDCALL _GetObjCoef(double* values, long length, long token);
GenModelDLL_API bool STDCALL _GetBounds(double* lb, double* ub, long length, long token);
GenModelDLL_API double STDCALL _GetLowerBound(long col, long token);
GenModelDLL_API double STDCALL _GetUpperBound(long col, long token);
GenModelDLL_API bool STDCALL _SetLowerBound(long col, double val, long token);
GenModelDLL_API bool STDCALL _SetUpperBound(long col, double val, long token);
GenModelDLL_API double STDCALL _GetRHS(long row, long token);
GenModelDLL_API bool STDCALL _SetRHS(long row, double val, long token);
GenModelDLL_API char STDCALL _GetSense(long row, long token);
GenModelDLL_API bool STDCALL _SetSense(long row, char sense, long token);
GenModelDLL_API double STDCALL _GetObjVal(long token);
GenModelDLL_API long STDCALL _ChangeBulkBounds(int count, int* indices, char* types, double* values, long token);
GenModelDLL_API long STDCALL _ChangeBulkObjectives(int count, int* indices, double* values, long token);
GenModelDLL_API long STDCALL _DeleteMipStarts(long token);
GenModelDLL_API double STDCALL _GetMIPRelativeGap(long token);

GENMODEL_EXTERN_C_END

template<class T> class InterfaceVector {
public:
    InterfaceVector();
    InterfaceVector(int size);
    ~InterfaceVector();
    void SetSize(int size);
    void Delete();
    void Set(int index, T val);
    T Get(int index);
    T* Ptr();
    
private:
    int size;
    T * val;
};