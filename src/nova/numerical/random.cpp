#include "nova/numerical/random.h"


using namespace std;
using namespace MG;


/* Random Abstract Class */
MG_AbstractRandom::MG_AbstractRandom(const MG_AbstractRandom& aRight) : MG_XLObject(aRight), myName(aRight.myName)
{}

void MG_AbstractRandom::Swap(MG_AbstractRandom& aRight)
{
	MG_XLObject::Swap(aRight);
	myName.swap(aRight.myName);
}

MG_AbstractRandom::MG_AbstractRandom(const string& aName) : myName(aName)
{}


/* Random Class */
/*
 * Perf: TAUS, GFSR4, MT19937, RANLXS0, RANLXS1, MRG, RANLUX, RANLXD1, RANLXS2, CMRG, RANLUX389, RANLXD2
 */
const gsl_rng_type* MG_Random::From_MGType_To_GSLType(const RAND_TYPE& aType)
{
	switch(aType)
	{
	case BOROSH13			: return gsl_rng_borosh13; // Knuth
	case COVEYOU			: return gsl_rng_coveyou; // Coveyou
	case CMRG				: return gsl_rng_cmrg; // L'Ecuyer
	case FISHMAN18			: return gsl_rng_fishman18; // Knuth
	case FISHMAN20			: return gsl_rng_fishman20; // Knuth
	case FISHMAN2X			: return gsl_rng_fishman2x;
	case GFSR4				: return gsl_rng_gfsr4;
	case KNUTHRAN			: return gsl_rng_knuthran;
	case KNUTHRAN2			: return gsl_rng_knuthran2;
	case KNUTHRAN2002		: return gsl_rng_knuthran2002;
	case LECUYER21			: return gsl_rng_lecuyer21; // Knuth
	case MINSTD				: return gsl_rng_minstd; // Park&  Miller minimal standard algo
	case MRG				: return gsl_rng_mrg; // L'Ecuyer, Blouin&  Coutre
	case MT19937			: return gsl_rng_mt19937; // Mersenne Twister, by Makoto Matsumoto&  Takuji Nishimura 2002
	case MT19937_1999		: return gsl_rng_mt19937_1999;
	case MT19937_1998		: return gsl_rng_mt19937_1998;
	case R250				: return gsl_rng_r250; // Kirkpatrick&  Stoll
	case RAN0				: return gsl_rng_ran0;
	case RAN1				: return gsl_rng_ran1;
	case RAN2				: return gsl_rng_ran2;
	case RAN3				: return gsl_rng_ran3;
	case RAND				: return gsl_rng_rand;
	case RAND48				: return gsl_rng_rand48;
	case RANDOM128_BSD		: return gsl_rng_random128_bsd;
	case RANDOM128_GLIBC2	: return gsl_rng_random128_glibc2;
	case RANDOM128_LIBC5	: return gsl_rng_random128_libc5;
	case RANDOM256_BSD		: return gsl_rng_random256_bsd;
	case RANDOM256_GLIBC2	: return gsl_rng_random256_glibc2;
	case RANDOM256_LIBC5	: return gsl_rng_random256_libc5;
	case RANDOM32_BSD		: return gsl_rng_random32_bsd;
	case RANDOM32_GLIBC2	: return gsl_rng_random32_glibc2;
	case RANDOM32_LIBC5		: return gsl_rng_random32_libc5;
	case RANDOM64_BSD		: return gsl_rng_random64_bsd;
	case RANDOM64_GLIBC2	: return gsl_rng_random64_glibc2;
	case RANDOM64_LIBC5		: return gsl_rng_random64_libc5;
	case RANDOM8_BSD		: return gsl_rng_random8_bsd;
	case RANDOM8_GLIBC2		: return gsl_rng_random8_glibc2;
	case RANDOM8_LIBC5		: return gsl_rng_random8_libc5;
	case RANDOM_BSD			: return gsl_rng_random_bsd;
	case RANDOM_GLIBC2		: return gsl_rng_random_glibc2;
	case RANDOM_LIBC5		: return gsl_rng_random_libc5;
	case RANDU				: return gsl_rng_randu;
	case RANF				: return gsl_rng_ranf; // CRAY
	case RANLUX				: return gsl_rng_ranlux;
	case RANLUX389			: return gsl_rng_ranlux389;
	case RANLXD1			: return gsl_rng_ranlxd1; // Lüscher algorithm, 48-bit, luxury level 1, more decorrelated
	case RANLXD2			: return gsl_rng_ranlxd2; // Lüscher algorithm, 48-bit, luxury level 2, more decorrelated
	case RANLXS0			: return gsl_rng_ranlxs0; // Lüscher algorithm, 24-bit
	case RANLXS1			: return gsl_rng_ranlxs1; // Lüscher algorithm, 24-bit, luxury level 1, more decorrelated
	case RANLXS2			: return gsl_rng_ranlxs2; // Lüscher algorithm, 24-bit, luxury level 2, more decorrelated
	case RANMAR				: return gsl_rng_ranmar; // RANMAR, by Marsaglia, Zaman&  Tsang
	case SLATEC				: return gsl_rng_slatec;
	case TAUS				: return gsl_rng_taus; // Tausworthe generator, by L'Ecuyer
	case TAUS2				: return gsl_rng_taus2; // Tausworthe generator improved, by L'Ecuyer
	case TAUS113			: return gsl_rng_taus113;
	case TRANSPUTER			: return gsl_rng_transputer; // INMOS Transputer Development System
	case TT800				: return gsl_rng_tt800;
	case UNI				: return gsl_rng_uni;
	case UNI32				: return gsl_rng_uni32;
	case VAX				: return gsl_rng_vax; // VAX
	case WATERMAN14			: return gsl_rng_waterman14; // Knuth
	case ZUF				: return gsl_rng_zuf;
	default					: return NULL;
	}
}

MG_Random::MG_Random(	const MG_Random& aRight)
					:	MG_AbstractRandom(aRight)
					,	myInitialSeed(aRight.myInitialSeed)
{
	myGen = gsl_rng_clone(aRight.myGen);
}

void MG_Random::Swap(MG_Random& aRight)
{
	MG_AbstractRandom::Swap(aRight);
	swap(myInitialSeed, aRight.myInitialSeed);
	swap(myGen, aRight.myGen);
}

MG_Random::MG_Random(const RAND_TYPE& aType) : MG_AbstractRandom()
{
	myXLName = MG_RND_XL_NAME;

	const gsl_rng_type* vGSLType = From_MGType_To_GSLType(aType);
	myGen = gsl_rng_alloc(vGSLType);
	myInitialSeed = gsl_rng_get(myGen);
	myName = string(gsl_rng_name(myGen));
}

MG_Random::~MG_Random(void)
{
	gsl_rng_free(myGen);
}

void MG_Random::Reset(void)
{
	SetSeed(myInitialSeed);
}

void MG_Random::ToString(FILE* aFile) const
{
	void* vState = gsl_rng_state(myGen);
	size_t vSize = gsl_rng_size(myGen);
	fwrite(vState, vSize, 1, aFile);
}

void MG_Random::Draw(MG_Vector& aValues)
{
	for(size_t i=0; i<aValues.Size(); ++i)
		aValues[i] = gsl_rng_uniform(myGen);
}


/* Quasi Random Class */
/*
 * Perf: 
 */
const gsl_qrng_type* MG_QuasiRandom::From_MGType_To_GSLType(const QUASIRAND_TYPE& aType)
{
	switch(aType)
	{
	case NIEDERREITER2: return gsl_qrng_niederreiter_2;
	case SOBOL: return gsl_qrng_sobol;
	case HALTON: return gsl_qrng_halton;
	case REVERSEHALTON: return gsl_qrng_reversehalton;
	default: return NULL;
	}
}

MG_QuasiRandom::MG_QuasiRandom	(	const MG_QuasiRandom& aRight)
								:	MG_AbstractRandom(aRight)
								,	myValues(aRight.myValues)
{
	myGen = gsl_qrng_clone(aRight.myGen);
}

void MG_QuasiRandom::Swap(MG_QuasiRandom& aRight)
{
	MG_AbstractRandom::Swap(aRight);
	swap(myGen, aRight.myGen);
	myValues.Swap(aRight.myValues);
}

MG_QuasiRandom::MG_QuasiRandom	(	const QUASIRAND_TYPE& aType
								,	const size_t& aDim)
								:	MG_AbstractRandom()
								,	myValues(aDim)
{
	myXLName = MG_QRND_XL_NAME;

	const gsl_qrng_type* vGSLType = From_MGType_To_GSLType(aType);
	myGen = gsl_qrng_alloc(vGSLType, aDim);
	myName = string(gsl_qrng_name(myGen));
}

MG_QuasiRandom::~MG_QuasiRandom(void)
{
	gsl_qrng_free(myGen);
}

void MG_QuasiRandom::Reset(void)
{
	gsl_qrng_init(myGen);
}

void MG_QuasiRandom::ToString(FILE* aFile) const
{
	void* vState = gsl_qrng_state(myGen);
	size_t vSize = gsl_qrng_size(myGen);
	fwrite(vState, vSize, 1, aFile);
}

double MG_QuasiRandom::DrawUniform()
{
	size_t vIdx = gsl_qrng_get(myGen, myValues.GetPtr()->data);
	return myValues[vIdx];
}

void MG_QuasiRandom::Draw(MG_Vector& aValues)
{
	Reset();
	size_t vSize = GetSize();
	for(size_t i=0; i<vSize; ++i)
		gsl_qrng_get(myGen, myValues.GetPtr()->data);
	aValues = myValues;
}
