// GenModelInterface.cpp : Defines the initialization routines for the DLL.
//

#include "GenModelInterface.h"

#ifdef CPLEX_MODULE
#include "GenModelCplex.h"
#endif

#ifdef GUROBI_MODULE
#include "GenModelGurobi.h"
#endif

#ifdef HG_MODULEZ
#include "GenModelHG.h"
#endif

#ifdef GLPK_MODULE
#include "GenModelGlpk.h"
#endif

#ifdef OSI_MODULE
#include "GenModelOsi.h"
#endif

#ifdef SCIP_MODULE
#include "GenModelScip.h"
#endif

#include <vector>
#include <memory.h>

using namespace std;

vector<GenModel*> gmmap;

GENMODEL_EXTERN_C_BEGIN

GenModelDLL_API bool STDCALL _VerifyId(long token)
{
    if(token <0 || token >= gmmap.size())
    {
        throw string("Model id out of bound");
        return false;
    }
    else
        return true;
    
}

GenModelDLL_API long STDCALL _WriteProblemToLpFile(char* filename, long token)
{
    _VerifyId(token);
    return gmmap[token]->WriteProblemToLpFile(string(filename));
}

GenModelDLL_API long STDCALL _WriteSolutionToFile(char* filename, long token)
{
    _VerifyId(token);
    return gmmap[token]->WriteSolutionToFile(string(filename));
}

GenModelDLL_API double STDCALL _FindConstraintMaxLhs(long row, long token)
{
    _VerifyId(token);
    return gmmap[token]->FindConstraintMaxLhs(row);
}

GenModelDLL_API double STDCALL _FindConstraintMinLhs(long row, long token)
{
    _VerifyId(token);
    return gmmap[token]->FindConstraintMinLhs(row);
}

GenModelDLL_API long STDCALL _MakeConstraintFeasible(long row, long token)
{
    _VerifyId(token);
    return gmmap[token]->MakeConstraintFeasible(row);
}

GenModelDLL_API long STDCALL _AddConst(char* cname, double rhs, char sense, long token)
{
    _VerifyId(token);
    //printf("AddConst : name=%s, rhs=%f, sense=%c, token=%ld\n", cname, rhs, sense, token);
	string cnameAsString(cname);
	return gmmap[token]->AddConst(cnameAsString, rhs, sense);
}

GenModelDLL_API bool STDCALL _AddConstBulk(char* cname, double* rhs, long length, char sense, long token)
{
    _VerifyId(token);
	string cnameAsString(cname);
	for (int i = 0; i < length; i++)
	{
		gmmap[token]->AddConst(cnameAsString, rhs[i], sense);
	}
	return true;
}

GenModelDLL_API long STDCALL _AddVar(char* nn, double o, double l, double u, char t, long token)
{
    _VerifyId(token);
    //printf("AddVar : name=%s, obj=%f, lower=%f, upper=%f, type=%c, token=%ld\n", nn, o, l, u, t, token);
	string nnAsString(nn);
	return gmmap[token]->AddVar(nnAsString, o, l, u, t);
}

GenModelDLL_API bool STDCALL _AddVarBulk(char* nn, double* o, long length, double l, double u, char t, long token)
{
    _VerifyId(token);
	string nnAsString(nn);
	for (int i = 0; i < length; i++)
	{
		gmmap[token]->AddVar(nnAsString, o[i], l, u, t);
	}
	return true;
}

GenModelDLL_API long STDCALL _SetQpCoef(long i, long j, double val, long token)
{
    _VerifyId(token);
    return gmmap[token]->vars.SetQpCoef(i, j, val);
}

GenModelDLL_API long STDCALL _AddNz(long row, long col, double val, long token)
{
    _VerifyId(token);
	return gmmap[token]->AddNz(row, col, val);
}

GenModelDLL_API long STDCALL _AddNzToLast(long col, double val, long token)
{
    _VerifyId(token);
    //printf("AddNzToLast : col=%d, val=%f, token=%ld\n", col, val, token);
	return gmmap[token]->AddNzToLast(col, val);
}

GenModelDLL_API long STDCALL _AddNzBulk(long* rows, long* cols, double* values, long valuesLength, long rowCount, long colCount, long iterations, long token)
{
    _VerifyId(token);
	//printf ("AddNzBulk %ld,%ld,%ld,%ld\n",valuesLength, rowCount, colCount, iterations);
    
	int rowIndex = 0;
	int colIndex = 0;
	for(int iter = 1; iter <= iterations; iter++)
	{
		//printf ("Iter %ld\n",iter);
		for(int i = 0; i < valuesLength; i++)
		{
			//printf ("i ri ci %ld,%ld,%ld\n",i,rows[i] + rowIndex, cols[i] + colIndex);
			gmmap[token]->AddNz(rows[i] + rowIndex, cols[i] + colIndex, values[i]);
		}
		rowIndex += rowCount;
		colIndex += colCount;
		//printf ("ri ci %ld,%ld\n", rowIndex, colIndex);
	}
	return true;
}

GenModelDLL_API long STDCALL _SetNumbers(long token)
{
    _VerifyId(token);
	return gmmap[token]->SetNumbers();
}

GenModelDLL_API long STDCALL _SetLongParam(char* param, long val, long token)
{
    _VerifyId(token);
	string paramAsString(param);
	return gmmap[token]->SetLongParam(paramAsString, val);
}

GenModelDLL_API long STDCALL _SetDblParam(char* param, double val, long token)
{
    _VerifyId(token);
	string paramAsString(param);
	return gmmap[token]->SetDblParam(paramAsString, val);
}

GenModelDLL_API long STDCALL _SetBoolParam(char* param, bool val, long token)
{
    _VerifyId(token);
    //printf("SetBoolParam : param=%s, val=%d, token=%ld\n", param, val, token);
	string paramAsString(param);
	return gmmap[token]->SetBoolParam(paramAsString, val);
}

GenModelDLL_API long STDCALL _SetStrParam(char* param, char* val, long token)
{
    _VerifyId(token);
	string paramAsString(param);
	string valueAsString(val);
	return gmmap[token]->SetStrParam(paramAsString, valueAsString);
}

GenModelDLL_API long STDCALL _CreateNewModel(char type, char* name = NULL)
{
	long ret = gmmap.size();
    switch (type) {
        case 'C':
            (CPLEX_EXIST ? CPLEX_CREATE : (GLPK_EXIST ? GLPK_CREATE : throw "Neither Cplex nor Glpk (deafault fallback solver) are available"));
            break;
        case 'G':
            (GUROBI_EXIST ? GUROBI_CREATE : (GLPK_EXIST ? GLPK_CREATE : throw "Neither Gurobi nor Glpk (deafault fallback solver) are available"));
            break;
        case 'H':
            (HG_EXIST ? HG_CREATE : (GLPK_EXIST ? GLPK_CREATE : throw "Neither Hypergraph nor Glpk (deafault fallback solver) are available"));
            break;
        case 'K':
            (GLPK_EXIST ? GLPK_CREATE : throw "Glpk (deafault fallback solver) is not available");
            break;
        case 'S':
            (SCIP_EXIST ? SCIP_CREATE : (GLPK_EXIST ? GLPK_CREATE : throw "Neither Scip nor Glpk (deafault fallback solver) are available"));
            break;
        case 'O':
            (OSI_EXIST ? OSI_CREATE : (GLPK_EXIST ? GLPK_CREATE : throw "Neither Osi nor Glpk (deafault fallback solver) are available"));
            break;
        default:
            throw string("Unknown solver");
            return 1;
    }
	if(gmmap.size() == ret)
        return 1;
	gmmap[ret]->name = "DefaultGenModel";
    if(name != NULL)
        gmmap[ret]->name = string(name);
    
    return ret;
}

GenModelDLL_API bool STDCALL _IsSolverAvailable(char type)
{
    switch (type) {
        case 'C':
            return CPLEX_EXIST;
            break;
        case 'G':
            return GUROBI_EXIST;
            break;
        case 'H':
            return HG_EXIST;
            break;
        case 'K':
            return GLPK_EXIST;
            break;
        case 'S':
            return SCIP_EXIST;
            break;
        case 'O':
            return OSI_EXIST;
            break;
        default:
            return false;
    }
	return false;
}

GenModelDLL_API long STDCALL _CopyOrder(long token, int count, int* ind, int* weight)
{
    _VerifyId(token);
#ifdef CPLEX_SOLVER
	CplexData* d = static_cast<CplexData*>(gmmap[token]->solverdata);
	CPXcopyorder(d->env, d->lp, count, ind, weight, NULL);
#else
	throw "Not implemented for your solver";
#endif
	return 0;
}

GenModelDLL_API long STDCALL _DeleteModel(long token)
{
    _VerifyId(token);
	delete gmmap[token];
	return 0;
}

GenModelDLL_API long STDCALL _CreateModel(long token)
{
    _VerifyId(token);
	gmmap[token]->SetNumbers();
	gmmap[token]->Init(gmmap[token]->name);
	gmmap[token]->CreateModel();
	return 0;
}

GenModelDLL_API long STDCALL _SolveModel(long token)
{
    _VerifyId(token);
	gmmap[token]->Solve();
	gmmap[token]->SetSol();
    /*for(int i = 0; i < gmmap[token]->nc; i++)
     {
     printf("%s: %f\n", gmmap[token]->vars.name[i].c_str(), gmmap[token]->vars.sol[i]);	// Solution des variables
     }*/
	return long(gmmap[token]->solstat);
}

GenModelDLL_API bool STDCALL _HasSolution(long token)
{
    _VerifyId(token);
	return gmmap[token]->hassolution;
}

GenModelDLL_API bool STDCALL _GetSolVars(double* values, long length, long token)
{
    _VerifyId(token);
	if (length != gmmap[token]->nc)
		throw string("Wrong length : GetSolVars()");
    
	for(int i = 0; i < gmmap[token]->nc; i++)
		values[i] = gmmap[token]->vars.sol[i];
    
	return true;
}

GenModelDLL_API bool STDCALL _GetDualPrices(double* values, long length, long token)
{
    _VerifyId(token);
	if (length != gmmap[token]->nr)
		throw string("Wrong length : GetDualPrices()");
    
	for(int i = 0; i < gmmap[token]->nr; i++)
		values[i] = gmmap[token]->consts[i].dual;
    
	return true;
}

GenModelDLL_API bool STDCALL _GetSlacks(double* values, long length, long token)
{
    _VerifyId(token);
	if (length != gmmap[token]->nr)
		throw string("Wrong length : GetSlacks()");
    
	for(int i = 0; i < gmmap[token]->nr; i++)
		values[i] = gmmap[token]->consts[i].slack;
    
    return true;
}

GenModelDLL_API bool STDCALL _GetReducedCosts(double* values, long length, long token)
{
    _VerifyId(token);
	if (length != gmmap[token]->nc)
		throw string("Wrong length : GetReducedCosts()");
    
	for(int i = 0; i < gmmap[token]->nc; i++)
		values[i] = gmmap[token]->vars.rc[i];
    
	return true;
}

GenModelDLL_API bool STDCALL _GetRowValues(double* values, long length, long rowIndex, long token)
{
    _VerifyId(token);
	if (length != gmmap[token]->nc || rowIndex >= gmmap[token]->nr)
	{
		return false;
	}
    
	memset(values, 0, sizeof(double)*length);
	for(int i = 0; i < int(gmmap[token]->consts[rowIndex].cols.size()); i++)
		values[gmmap[token]->consts[rowIndex].cols[i]] = gmmap[token]->consts[rowIndex].coefs[i];
	
	return true;
}

GenModelDLL_API bool STDCALL _GetObjCoef(double* values, long length, long token)
{
    _VerifyId(token);
	if (length != gmmap[token]->nc)
	{
		return false;
	}
    
	for(int i = 0; i < gmmap[token]->nc; i++)
	{
		values[i] = gmmap[token]->vars.obj[i];
	}
    
	return true;
}

GenModelDLL_API bool STDCALL _GetBounds(double* lb, double* ub, long length, long token)
{
    _VerifyId(token);
	if (length != gmmap[token]->nc)
	{
		return false;
	}
    
	for(int i = 0; i < gmmap[token]->nc; i++)
	{
		lb[i] = gmmap[token]->vars.lb[i];
		ub[i] = gmmap[token]->vars.ub[i];
	}
    
	return true;
}

GenModelDLL_API double STDCALL _GetLowerBound(long col, long token)
{
    _VerifyId(token);
	if (col != gmmap[token]->nc || col < 0)
        return (numeric_limits<double>::has_signaling_NaN ? numeric_limits<double>::signaling_NaN() : numeric_limits<double>::quiet_NaN());
    
	return gmmap[token]->vars.lb[col];
}

GenModelDLL_API double STDCALL _GetUpperBound(long col, long token)
{
    _VerifyId(token);
	if (col != gmmap[token]->nc || col < 0)
        return (numeric_limits<double>::has_signaling_NaN ? numeric_limits<double>::signaling_NaN() : numeric_limits<double>::quiet_NaN());
    
	return gmmap[token]->vars.ub[col];
}

GenModelDLL_API bool STDCALL _SetLowerBound(long col, double val, long token)
{
    _VerifyId(token);
	if (col != gmmap[token]->nc || col < 0)
        return false;
    gmmap[token]->vars.lb[col] = val;
	return true;
}

GenModelDLL_API bool STDCALL _SetUpperBound(long col, double val, long token)
{
    _VerifyId(token);
	if (col != gmmap[token]->nc || col < 0)
        return false;
    gmmap[token]->vars.ub[col] = val;
	return true;
}

GenModelDLL_API double STDCALL _GetRHS(long row, long token)
{
    _VerifyId(token);
	return gmmap[token]->consts[row].lrhs;
}

GenModelDLL_API bool STDCALL _SetRHS(long row, double val, long token)
{
    _VerifyId(token);
	gmmap[token]->consts[row].lrhs = val;
	return true;
}

GenModelDLL_API char STDCALL _GetSense(long row, long token)
{
    _VerifyId(token);
	return gmmap[token]->consts[row].sense;
}

GenModelDLL_API bool STDCALL _SetSense(long row, char sense, long token)
{
    _VerifyId(token);
	gmmap[token]->consts[row].sense = sense;
	return true;
}

GenModelDLL_API double STDCALL _GetObjVal(long token)
{
    _VerifyId(token);
	return gmmap[token]->objval;
}

GenModelDLL_API long STDCALL _ChangeBulkBounds(int count, int* indices, char* types, double* values, long token)
{
    _VerifyId(token);
	return gmmap[token]->ChangeBulkBounds(count, indices, types, values);
}

GenModelDLL_API long STDCALL _ChangeBulkObjectives(int count, int* indices, double* values, long token)
{
    _VerifyId(token);
	return gmmap[token]->ChangeBulkObjectives(count, indices, values);
}

GenModelDLL_API long STDCALL _DeleteMipStarts(long token)
{
    _VerifyId(token);
	return gmmap[token]->DeleteMipStarts();
}

GenModelDLL_API double STDCALL _GetMIPRelativeGap(long token)
{
    _VerifyId(token);
	return gmmap[token]->GetMIPRelativeGap();
}

GENMODEL_EXTERN_C_END