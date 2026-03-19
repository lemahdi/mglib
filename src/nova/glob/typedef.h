/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.3
 * Date					: 11 JUL 2010
 * Purpose				: useful definitions file
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>


/* Some pre-declarations from the parser */
#define yyconst const
typedef size_t yy_size_t;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string (yyconst char *yy_str  );
void yy_flush_buffer (YY_BUFFER_STATE b  );
void yy_delete_buffer (YY_BUFFER_STATE b  );


/* Some useful math constant definitions */
constexpr double K_PI					= 3.141592653589793;
constexpr double K_ROOTTWO				= 1.4142135623730951;
constexpr double K_ONEOVERROOTTWOPI	= 0.3989422804014327;
constexpr double K_ONEOVERROOTPI		= 0.56418958354775628;
constexpr double K_ROOTTWOPI			= 2.5066282746310002;
constexpr double K_LOGROOTTWOPI		= 0.918938533204672;
constexpr double K_NORMALCUMULATIVEBOUND= 7.;
constexpr double K_INFTY				= 1.e+20;
constexpr double K_EPS					= 1.e-12;


/* Error file for Lexer and Parser */
#define ERROR_FILE "error_log.txt"


/* Some useful functions */
#define ASSIGN_OPERATOR(CLASS)					\
	inline CLASS& operator= (CLASS aFrom)		\
	{											\
		Swap(aFrom);							\
		return *this;							\
	}

#define FAKE_ASSIGN_OPERATOR(CLASS)									\
	inline CLASS& operator= (const CLASS& )							\
	{																\
		assert("Assignment operator not allowed for this class");	\
		return *this;												\
	}

#define CLONE_METHOD(CLASS) \
	inline MG_Object* Clone() const override { return new CLASS(*this); }

#define SWAP_DECL(CLASS)	\
	void Swap(CLASS& aRight);

#define COPY_CTOR_DECL(CLASS)	\
	CLASS(const CLASS& aRight);

#define MG_THROW(MSG)		\
	throw MG_Exception(__FILE__, __LINE__, MSG);

#define MG_FRIEND_ARITH_OP_LEFT(CLASS,OP,SCALAR)					\
	friend CLASS operator OP (CLASS aLeft, const SCALAR& aRight)	\
	{ aLeft OP##= aRight; return aLeft; }

#define MG_FRIEND_ARITH_OP_RIGHT(CLASS,OP,SCALAR)					\
	friend CLASS operator OP (const SCALAR& aLeft, CLASS aRight)	\
	{ aRight OP##= aLeft; return aRight; }

#define MG_FRIEND_ARITH_OP(CLASS,OP)							\
	friend CLASS operator OP (const CLASS& aLeft, CLASS aRight)	\
	{ aRight OP##= aLeft; return aRight; }


/* Coord is is a pair.
 * This pair is the coordinates for a cell in a table.
 * The first element coordinates are (0,0).
 */
using Coord     = std::pair<unsigned int, unsigned int>;
using PairCoord = std::pair<Coord, Coord>;
using MMCoord   = std::multimap<Coord, Coord>;

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
#define MG_TERMSTRUCT_XL_NAME	"TERMS"
#define MG_IRINDEX_XL_NAME		"IRIDX"
#define MG_SCHED_XL_NAME		"SCHED"
#define MG_SWAPLEG_XL_NAME		"SWPLG"
#define MG_SWAP_XL_NAME			"SWAPS"
#define MG_FRA_XL_NAME			"FRALG"
#define MG_CALLPUT_XL_NAME		"CALPT"
#define MG_DIGITAL_XL_NAME		"DIGIT"
#define MG_SWAPTION_XL_NAME		"SWOPT"
#define MG_DFMODEL_XL_NAME		"DFMOD"
#define MG_BSMODEL_XL_NAME		"BSMOD"
#define MG_ROBOT_XL_NAME		"ROBOT"
#define MG_ZERO_XL_NAME			"ZEROC"
#define MG_DIVS_XL_NAME			"DIVSC"
#define MG_EQ_XL_NAME			"EQVOL"
#define MG_IRVOL_XL_NAME		"IRVOL"
#define MG_EQVOL_XL_NAME		"EQVOL"
#define MG_GENSEC_XL_NAME		"GLSEC"
#define MG_RND_XL_NAME			"RANDM"
#define MG_QRND_XL_NAME			"QRAND"
#define MG_NORDIST_XL_NAME		"NDIST"
#define MG_UDIST_XL_NAME		"UDIST"
#define MG_LOGNDIST_XL_NAME		"LNDST"
#define MG_FSOLVER_XL_NAME		"FSOLV"//should define a grammar for functions
#define MG_FDFSOLVER_XL_NAME	"DFSLV"//should define a grammar for functions
#define MG_LINREG_XL_NAME		"LIREG"
#define MG_MULREG_XL_NAME		"MLREG"
#define MG_MINIMIZER_XL_NAME	"MINIM"
#define MG_MONTECARLO_XL_NAME	"MTCRL"
#define MG_HW1FMODEL_XL_NAME	"HW1FM"
#define MG_GENPRICER_XL_NAME	"GENPR"


/* CountedPtr definitions – now using std::shared_ptr */
namespace MG {
	class MG_Cache;
	class MG_FileError;
	class MG_Object;
	class MG_XLObject;
	class MG_Func;
	class MG_FuncBuilder;
	class MG_GenSecurity;
	class MG_Random;
	class MG_RandDist;
	class MG_CdfNormal;
	class MG_NumericalMethod;
	class MG_MonteCarloMethod;
	class MG_PricingModel;
	class MG_IRPricingModel;
	class MG_MonteCarlo;
	class MG_UnaryFunc;
	class MG_Robot;
	class MG_MarketData;
	class MG_ZeroCurve;
	class MG_VolatilityCurve;
	class MG_IRVolatilityCurve;
	class MG_Interpolator;
	class MG_LinearInterpolator;
	class MG_Function;
	class MG_FFunction;
	class MG_Security;
	class MG_SwapLeg;
	class MG_Swap;

	using MG_StdVectDblPtr         = std::shared_ptr<std::vector<double>>;
	using MG_StdVectBoolPtr        = std::shared_ptr<std::vector<bool>>;
	using MG_CachePtr              = std::shared_ptr<MG_Cache>;
	using MG_FileErrorPtr          = std::shared_ptr<MG_FileError>;
	using MG_ObjectPtr             = std::shared_ptr<MG_Object>;
	using MG_XLObjectPtr           = std::shared_ptr<MG_XLObject>;
	using MG_FuncPtr               = std::shared_ptr<MG_Func>;
	using MG_FuncBuilderPtr        = std::shared_ptr<MG_FuncBuilder>;
	using MG_GenSecurityPtr        = std::shared_ptr<MG_GenSecurity>;
	using MG_RandomPtr             = std::shared_ptr<MG_Random>;
	using MG_RandDistPtr           = std::shared_ptr<MG_RandDist>;
	using MG_CdfNormalPtr          = std::shared_ptr<MG_CdfNormal>;
	using MG_NumericalMethodPtr    = std::shared_ptr<MG_NumericalMethod>;
	using MG_MonteCarloMethodPtr   = std::shared_ptr<MG_MonteCarloMethod>;
	using MG_PricingModelPtr       = std::shared_ptr<MG_PricingModel>;
	using MG_IRPricingModelPtr     = std::shared_ptr<MG_IRPricingModel>;
	using MG_MonteCarloPtr         = std::shared_ptr<MG_MonteCarlo>;
	using MG_UnaryFuncPtr          = std::shared_ptr<MG_UnaryFunc>;
	using MG_RobotPtr              = std::shared_ptr<MG_Robot>;
	using MG_MarketDataPtr         = std::shared_ptr<MG_MarketData>;
	using MG_ZeroCurvePtr          = std::shared_ptr<MG_ZeroCurve>;
	using MG_VolatilityCurvePtr    = std::shared_ptr<MG_VolatilityCurve>;
	using MG_IRVolatilityCurvePtr  = std::shared_ptr<MG_IRVolatilityCurve>;
	using MG_InterpolatorPtr       = std::shared_ptr<MG_Interpolator>;
	using MG_LinearInterpolatorPtr = std::shared_ptr<MG_LinearInterpolator>;
	using MG_FunctionPtr           = std::shared_ptr<MG_Function>;
	using MG_FFunctionPtr          = std::shared_ptr<MG_FFunction>;
	using MG_SecurityPtr           = std::shared_ptr<MG_Security>;
	using MG_SwapLegPtr            = std::shared_ptr<MG_SwapLeg>;
	using MG_SwapPtr               = std::shared_ptr<MG_Swap>;
}


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
#define interpoltypeConstant		0x00000001
#define interpoltypeStepUpLeft		0x00000002
#define interpoltypeStepUpRight		0x00000003
#define interpoltypeLinear			0x00000004
#define interpoltypeContinuous		0x00000005
#define interpoltypePolynomial		0x00000006
#define interpoltypeCubicSpline		0x00000007
#define interpoltypeAkima			0x00000008

#define interpoltypeMask			0x0000000f

#define maxBitInterpoltype			4
#define maxInterpoltypesNb			8	// maximum interpolations that could be expressed using interpoltypeXXX