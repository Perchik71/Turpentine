#include "GameScript.h"
#include "ParamInfos.h"
#include "MersenneTwister.h"

static const float kPi = 3.1415926535897932384626433832795f;
static const float kDegToRad = kPi / 180.0f;
static const float kRadToDeg = 180.0f / kPi;

template <float (* Fn)(float)>
bool UnaryMathFn(COMMAND_ARGS)
{
	*result = 0;

	float arg = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &arg))
		return true;

	*result = Fn(arg);

	return true;
}

// argument is converted from degrees to radians
template <float (*Fn)(float)>
bool UnaryMathFn_DegToRad(COMMAND_ARGS)
{
	*result = 0;

	float arg = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &arg))
		return true;

	*result = Fn(arg * kDegToRad);

	return true;
}

// result is converted from radians to degrees
template <float (*Fn)(float)>
bool UnaryMathFn_RadToDeg(COMMAND_ARGS)
{
	*result = 0;

	float arg = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &arg))
		return true;

	*result = Fn(arg) * kRadToDeg;

	return true;
}

template <float (*Fn)(float, float)>
bool BinaryMathFn(COMMAND_ARGS)
{
	*result = 0;

	float arg1 = 0, arg2 = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &arg1, &arg2))
		return true;

	*result = Fn(arg1, arg2);

	return true;
}

template <float (*Fn)(float, float)>
bool BinaryMathFn_RadToDeg(COMMAND_ARGS)
{
	*result = 0;

	float arg1 = 0, arg2 = 0;
	if(!ExtractArgs(EXTRACT_ARGS, &arg1, &arg2))
		return true;

	*result = Fn(arg1, arg2) * kRadToDeg;

	return true;
}

CommandInfo kCommandInfo_SquareRoot =
{
	"SquareRoot", "sqrt",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <sqrtf>
};

CommandInfo kCommandInfo_Sin =
{
	"rSin", "rsin",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <sinf>
};

CommandInfo kCommandInfo_Cos =
{
	"rCos", "rcos",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <cosf>
};

CommandInfo kCommandInfo_Tan =
{
	"rTan", "rtan",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <tanf>
};

CommandInfo kCommandInfo_ASin =
{
	"rASin", "rasin",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <asinf>
};

CommandInfo kCommandInfo_ACos =
{
	"rACos", "racos",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <acosf>
};

CommandInfo kCommandInfo_ATan =
{
	"rATan", "ratan",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <atanf>
};

CommandInfo kCommandInfo_Log =
{
	"Log", "log",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <logf>
};

CommandInfo kCommandInfo_Exp =
{
	"Exp", "exp",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <expf>
};

CommandInfo kCommandInfo_Log10 =
{
	"Exp", "exp",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <log10f>
};

CommandInfo kCommandInfo_Floor =
{
	"Floor", "flr",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <floorf>
};

CommandInfo kCommandInfo_Ceil =
{
	"Ceil", "ceil",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <ceilf>
};

CommandInfo kCommandInfo_Abs =
{
	"Abs", "abs",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <fabsf>
};

float FloatRand(float min, float max)
{
	if(max < min)
	{
		float temp = min;
		min = max;
		max = temp;
	}

	float range = max - min;

	double value = MersenneTwister::genrand_real2() * range;

	return float(value + min);
}

CommandInfo kCommandInfo_Rand =
{
	"Rand", "r",
	0, "", 0,
	2, kParams_TwoFloats,
	&BinaryMathFn <FloatRand>
};

CommandInfo kCommandInfo_Pow =
{
	"Pow", "pow",
	0, "", 0,
	2, kParams_TwoFloats,
	&BinaryMathFn <powf>
};

CommandInfo kCommandInfo_ATan2 =
{
	"rATan2", "ratan2",
	0, "", 0,
	2, kParams_TwoFloats,
	&BinaryMathFn <atan2f>
};

CommandInfo kCommandInfo_Sinh =
{
	"rSinh", "rsinh",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <sinhf>
};

CommandInfo kCommandInfo_Cosh =
{
	"rCosh", "rcosh",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <coshf>
};

CommandInfo kCommandInfo_Tanh =
{
	"rTanh", "rtanh",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn <tanhf>
};

CommandInfo kCommandInfo_dSin =
{
	"Sin", "dsin",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_DegToRad <sinf>
};

CommandInfo kCommandInfo_dCos =
{
	"Cos", "dcos",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_DegToRad <cosf>
};

CommandInfo kCommandInfo_dTan =
{
	"Tanh", "dtanh",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_DegToRad <tanf>
};

CommandInfo kCommandInfo_dASin =
{
	"ASin", "dasin",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_RadToDeg <asinf>
};

CommandInfo kCommandInfo_dACos =
{
	"ACos", "dacos",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_RadToDeg <acosf>
};

CommandInfo kCommandInfo_dATan =
{
	"ATan", "datan",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_RadToDeg <tanf>
};

CommandInfo kCommandInfo_dATan2 =
{
	"ATan2", "datan2",
	0, "", 0,
	2, kParams_TwoFloats,
	&BinaryMathFn_RadToDeg <atan2f>
};

CommandInfo kCommandInfo_dSinh =
{
	"Sinh", "dsinh",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_DegToRad <sinhf>
};

CommandInfo kCommandInfo_dCosh =
{
	"Cosh", "dcosh",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_DegToRad <coshf>
};

CommandInfo kCommandInfo_dTanh =
{
	"Tanh", "dtanh",
	0, "", 0,
	1, kParams_OneFloat,
	&UnaryMathFn_DegToRad <tanhf>
};
