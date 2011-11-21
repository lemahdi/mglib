/*
 * Copyright			: 2010 by MG
 * File					: typedef.h
 * Version				: 0.1.3
 * Last changed			: 11 JUL 2010
 * Purpose				: useful definitions file
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>


/* Some pre-declarations from the parser */
#define yyconst const
typedef size_t yy_size_t;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string (yyconst char *yy_str  );
void yy_flush_buffer (YY_BUFFER_STATE b  );
void yy_delete_buffer (YY_BUFFER_STATE b  );


/* Some useful math constant definitions */
#define K_PI						3.141592653589793
#define K_ROOTTWO					1.4142135623730951
#define K_ONEOVERROOTTWOPI			0.3989422804014327
#define K_ONEOVERROOTPI				0.56418958354775628
#define K_ROOTTWOPI					2.5066282746310002
#define K_LOGROOTTWOPI				0.918938533204672
#define K_NORMALCUMULATIVEBOUND		7.
#define K_INFTY						1.e+20
#define	K_EPS						1.e-12


/* Error file for Lexer and Parser */
#define ERROR_FILE "error_log.txt"


/* Some useful functions */
#define ASSIGN_OPERATOR(CLASS)					\
	inline CLASS& CLASS::operator= (CLASS aFrom)		\
	{											\
		Swap(aFrom);							\
		return *this;							\
	}

#define FAKE_ASSIGN_OPERATOR(CLASS)									\
	inline CLASS& CLASS::operator= (const CLASS& )							\
	{																\
		assert("Assignement operator not allowed for this class");	\
	}

#define CLONE_METHOD(CLASS) \
	inline MG_Object* Clone(void) { return new CLASS(*this); }

#define SWAP_DECL(CLASS)	\
	void Swap(CLASS& aRight);

#define COPY_CTOR_DECL(CLASS)	\
	CLASS(const CLASS& aRight);

#define MG_THROW(MSG)		\
	throw MG_Exception(__FILE__, __LINE__, MSG);

#define MG_FRIEND_ARITH_OP_LEFT(CLASS,OP,SCALAR)					\
	friend CLASS operator OP (CLASS aLeft, const SCALAR& aRight)	\
	{ return aLeft OP= aRight; }

#define MG_FRIEND_ARITH_OP_RIGHT(CLASS,OP,SCALAR)					\
	friend CLASS operator OP (const SCALAR& aLeft, CLASS aRight)	\
	{ return aRight OP= aLeft; }

#define MG_FRIEND_ARITH_OP(CLASS,OP)							\
	friend CLASS operator OP (const CLASS& aLeft, CLASS aRight)	\
	{ return aRight OP= aLeft; }


/* Coord is is a pair.
 * This pair is the coordinates for a cell in a table.
 * The first element coordinates are (0,0).
 */
#define Coord std::pair<unsigned int,unsigned int>
#define PairCoord std::pair< Coord,Coord >
#define MMCoord std::multimap< Coord,Coord >

/* Coord / Node */
#define CoordNode std::pair<Coord,MG_Node*>

/* Map of CoordNode
 * Maximum of 9997 elements
 */
#define NHASH 9997
#define CoordNodeMap std::map<unsigned int,CoordNode*>


/* some useful definitions */
#define MAX_DESC_TABLE_COLUMNS 9999

#define NON_PERSISTENT_XL_OBJECT -1

#define MEMORY_CONTROL_MODE

#define MG_NAMESPACE_BEGIN namespace MG {
#define MG_NAMESPACE_END }

#define NameFuncMap		std::map<std::string,MG_FuncPtr>
#define NameFuncPair	std::pair<std::string,MG_FuncPtr>

#define CoordXLObject	std::pair<Coord,MG_XLObjectPtr>
#define CacheMap		std::multimap<std::string,CoordXLObject >
#define UnCacheMap		std::map<size_t,MG_XLObjectPtr>


/* XL names */
#define MG_DFMODEL_XL_NAME		"DFMOD"
#define MG_BSMODEL_XL_NAME		"BSMOD"
#define MG_ROBOT_XL_NAME		"ROBOT"
#define MG_ZERO_XL_NAME			"ZEROC"
#define MG_DIVS_XL_NAME			"DIVSC"
#define MG_IRVOL_XL_NAME		"IRVOL"
#define MG_EQVOL_XL_NAME		"EQVOL"
#define MG_GENSEC_XL_NAME		"GLSEC"
#define MG_RND_XL_NAME			"RANDM"
#define MG_QRND_XL_NAME			"QRAND"
#define MG_NORDIST_XL_NAME		"NDIST"
#define MG_FSOLVER_XL_NAME		"FSOLV"//should define a grammar for functions
#define MG_FDFSOLVER_XL_NAME	"DFSLV"//should define a grammar for functions
#define MG_LINREG_XL_NAME		"LIREG"
#define MG_MULREG_XL_NAME		"MLREG"
#define MG_HW1FMODEL_XL_NAME	"HW1FM"


/* CountedPtr definitions */
#define MG_CachePtr						CountedPtr<MG::MG_Cache>
#define MG_FileErrorPtr					CountedPtr<MG::MG_FileError>
#define MG_ObjectPtr					CountedPtr<MG::MG_Object>
#define MG_XLObjectPtr					CountedPtr<MG::MG_XLObject>
#define MG_FuncPtr						CountedPtr<MG::MG_Func>
#define MG_FuncBuilderPtr				CountedPtr<MG::MG_FuncBuilder>
#define MG_RandomPtr					CountedPtr<MG::MG_Random>
#define MG_SamplerPtr					CountedPtr<MG::MG_Sampler>
#define MG_CdfNormalPtr					CountedPtr<MG::MG_CdfNormal>
#define MG_UnaryFuncPtr					CountedPtr<MG::MG_UnaryFunc>
#define MG_RobotPtr						CountedPtr<MG::MG_Robot>
#define MG_MarketDataPtr				CountedPtr<MG::MG_MarketData>
#define MG_ZeroCurvePtr					CountedPtr<MG::MG_ZeroCurve>
#define MG_VolatilityCurvePtr			CountedPtr<MG::MG_VolatilityCurve>
#define MG_IRVolatilityCurvePtr			CountedPtr<MG::MG_IRVolatilityCurve>
#define MG_InterpolatorPtr				CountedPtr<MG::MG_Interpolator>
#define MG_LinearInterpolatorPtr		CountedPtr<MG::MG_LinearInterpolator>
#define MG_FunctionPtr					CountedPtr<MG::MG_Function>


/* enum for node direction */
enum NODE_DIRECTION {
	BACKWARD_NODE,
	NODIR_NODE,
	FORWARD_NODE
};


/* enum for comparison operator */
enum COMPARISON_OP
{
	GT_OP,
	LT_OP,
	NE_OP,
	EQ_OP,
	GE_OP,
	LE_OP
};


/* enum for nodes types */
enum NODE_TYPE {
	NODEF_NODE,
	ADD_NODE,
	SUB_NODE,
	MUL_NODE,
	DIV_NODE,
	NEG_NODE,
	CMP_NODE,
	NUM_NODE,
	DATE_NODE,
	REF_NODE,
	ARG_NODE,
	FUNC_NODE
};


/* enum for date displaying form */
enum DATE_DISPLAY {
	ENG_DATE,		// 2010/12/25
	FR_DATE,		// 25/12/2010
	ENG_M3L_DATE,	// 2010-Dec-25
	FR_M3L_DATE		// 25-Dec-2010
};


/* interpolation types */
#define interpoltypeNone			0x00000000
#define interpoltypeStepUpLeft		0x00000001
#define interpoltypeStepUpRight		0x00000002
#define interpoltypeLinear			0x00000003
#define interpoltypePolynomial		0x00000004
#define interpoltypeCubicSpline		0x00000005
#define interpoltypeConstant		0x00000006

#define interpoltypeMask			0x0000000f

#define maxBitInterpoltype			4
#define maxInterpoltypesNb			8	// maximum interpolations that could be expressed using interpoltypeXXX