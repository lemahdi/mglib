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
#define PI						3.141592653589793
#define ROOTTWO					1.4142135623730951
#define ONEOVERROOTTWOPI		0.3989422804014327
#define ONEOVERROOTPI			0.56418958354775628
#define ROOTTWOPI				2.5066282746310002
#define LOGROOTTWOPI			0.918938533204672
#define NORMALCUMULATIVEBOUND	7.
#define INFTY					1.e+20						
#define	EPS						1.e-12						


/* Error file for Lexer and Parser */
#define ERROR_FILE "error_log.txt"


/* Some useful functions */
#define ASSIGN_OPERATOR(CLASS)					\
	CLASS& CLASS::operator= (CLASS aFrom)		\
	{											\
		Swap(aFrom);							\
		return *this;							\
	}

#define FAKE_ASSIGN_OPERATOR(CLASS)									\
	CLASS& CLASS::operator= (const CLASS& )							\
	{																\
		assert("Assignement operator not allowed for this class");	\
	}

#define CLONE_METHOD(CLASS) \
	MG_Object* Clone(void) { return new CLASS(*this); }

#define SWAP_DECL(CLASS)	\
	void Swap(CLASS& aRight);

#define COPY_CTOR_DECL(CLASS)	\
	CLASS(const CLASS& aRight);

#define MG_THROW(MSG)		\
	throw MG_Exception(__FILE__, __LINE__, MSG);


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
#define MG_BSMODEL_XL_NAME	"BSMOD"
#define MG_ZERO_XL_NAME		"ZERO"
#define MG_DIVS_XL_NAME		"DIVS"
#define MG_IRVOL_XL_NAME	"IRVOL"
#define MG_GENSEC_XL_NAME	"GENSEC"


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
#define MG_ZeroCurvePtr					CountedPtr<MG::MG_ZeroCurve>
#define MG_IRVolatilityCurvePtr			CountedPtr<MG::MG_IRVolatilityCurve>
#define MG_InterpolatorPtr				CountedPtr<MG::MG_Interpolator>
#define MG_LinearInterpolatorPtr		CountedPtr<MG::MG_LinearInterpolator>


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


/* enum for dimensions */
enum INTERPOL_DIM {
	ID_ROW,
	ID_COL
};


/* interpolation types */
#define interpoltypeNone			0x00000000
#define interpoltypeStepUpLeft		0x00000001
#define interpoltypeStepUpRight		0x00000002
#define interpoltypeLinear			0x00000003

#define interpoltypeMask			0x0000000f

#define maxBitInterpoltype			4
#define maxInterpoltypesNb			8	// maximum interpolations that could be expressed using interpoltypeXXX