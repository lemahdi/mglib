#include "mgnumerical/random.h"


using namespace std;
using namespace MG;


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
	case MINSTD				: return gsl_rng_minstd; // Park & Miller minimal standard algo
	case MRG				: return gsl_rng_mrg; // L'Ecuyer, Blouin & Coutre
	case MT19937			: return gsl_rng_mt19937; // Mersenne Twister, by Makoto Matsumoto & Takuji Nishimura 2002
	case MT19937_1999		: return gsl_rng_mt19937_1999;
	case MT19937_1998		: return gsl_rng_mt19937_1998;
	case R250				: return gsl_rng_r250; // Kirkpatrick & Stoll
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
	case RANMAR				: return gsl_rng_ranmar; // RANMAR, by Marsaglia, Zaman & Tsang
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

MG_Random::MG_Random(const MG_Random &aRight)
{
	myGen = gsl_rng_clone(aRight.myGen);
}

void MG_Random::Swap(MG_Random& aRight)
{
	swap(myGen, aRight.myGen);
}

MG_Random::MG_Random(const RAND_TYPE& aType)
{
	const gsl_rng_type* vGSLType = From_MGType_To_GSLType(aType);
	myGen = gsl_rng_alloc(vGSLType);
}

MG_Random::~MG_Random(void)
{
	gsl_rng_free(myGen);
}

void MG_Random::ToString(FILE* aFile) const
{
	void* vState = gsl_rng_state(myGen);
	size_t vSize = gsl_rng_size(myGen);
	fwrite(vState, vSize, 1, aFile);
}
