
#include <assert.h>
#include <iostream>
#include <sstream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL 1

#include "GLM/gtx/norm.hpp"

#include "PrecompiledExp.h"
#include "LSystem.h"

using namespace std;

namespace LSystem
{
	PrecompiledExp::PrecompiledExp(LSystem &aLSystem)
		: mLSystem(aLSystem)
		, mResultType(ArgumentType::Unspecified)
	{
	}

	PrecompiledExp::PrecompiledExp(const PrecompiledExp &o)
		: mLSystem(o.mLSystem)
		, mCode(o.mCode)
		, mResultType(o.mResultType)
	{
	}

	PrecompiledExp& PrecompiledExp::operator =(const PrecompiledExp& o)
	{
		mCode = o.mCode;
		mResultType = o.mResultType;
		return *this;
	}

	const Location& PrecompiledExp::GetLocation(int ip) const
	{
		ASSERT(!mDebugInfo.empty());
		auto it = mDebugInfo.lower_bound(ip);
		return it->second;
	}

	void PrecompiledExp::AddLocation(const Location& aLocation)
	{
		const int ip = (int)mCode.Size();
		auto it = mDebugInfo.find(ip);
		if (it == mDebugInfo.end())
			mDebugInfo.insert(std::make_pair(ip, aLocation));
	}

	void PrecompiledExp::DebugDump() const
	{
		Location coord;
		for (int i = 0; i < (int)mCode.Size(); )
		{
			const Opcode o = mCode.Get<Opcode>(i);
			const Location& loc = GetLocation(i);

			if (loc.GetFirstLine() != coord.GetFirstLine())
			{
				coord = loc;
				cerr << "; " << coord.ToShortString() << ":" << endl;
			}

			cerr << i << ".:\t" << o << "\t";

			i += sizeof(Opcode);

			switch (o)
			{
			case IPUSH:		cerr << ToString(o) << " " << mCode.Get<int>(i) << endl;	i += sizeof(int);	break;
			case FPUSH:		cerr << ToString(o) << " " << mCode.Get<float>(i) << endl;	i += sizeof(float);	break;
			case BPUSH:		cerr << ToString(o) << " " << mCode.Get<bool>(i) << endl;	i += sizeof(bool);	break;
			case V2PUSH:	cerr << ToString(o) << endl;	break;
			case V3PUSH:	cerr << ToString(o) << endl;	break;
			case V4PUSH:	cerr << ToString(o) << endl;	break;

			case ILOAD:
			case FLOAD:
			case BLOAD:
			case V2LOAD:
			case V3LOAD:
			case V4LOAD:
				cerr << ToString(o) << " [" << mCode.Get<Token>(i) << "]"
					<< "\t; " << mLSystem.GetTokenName(mCode.Get<Token>(i)) << endl;	i += sizeof(int);	break;

			default:	cerr << ToString(o) << endl;	break;
			}
		}
	}

	ArgumentType::Type PrecompiledExp::CompileTypeCast(const AST::Expression* aExpression, const FormalArgumentMap& aArgs)
	{
		const AST::Expressions& operands = aExpression->GetOperands();
		ASSERT(operands.size() == 2);

		const AST::Expression* l = operands[0];
		const AST::Expression* r = operands[1];
		ASSERT(l != NULL);
		ASSERT(r != NULL);

		ArgumentType::Type rt = Compile(r, aArgs);

		switch (l->GetOpCode())
		{
		case AST::Expression::IntType:
			switch (rt)
			{
			case ArgumentType::Bool:
				Emit(BTOI, aExpression->GetLocation());
				break;
			case ArgumentType::Float:
				Emit(FTOI, aExpression->GetLocation());
				break;
			}
			mResultType = ArgumentType::Int;
			break;
		case AST::Expression::FloatType:
			switch (rt)
			{
			case ArgumentType::Bool:
				Emit(BTOF, aExpression->GetLocation());
				break;
			case ArgumentType::Int:
				Emit(ITOF, aExpression->GetLocation());
				break;
			}
			mResultType = ArgumentType::Float;
			break;
		case AST::Expression::BoolType:
			switch (rt)
			{
			case ArgumentType::Int:
				Emit(ITOB, aExpression->GetLocation());
				break;
			case ArgumentType::Float:
				Emit(FTOB, aExpression->GetLocation());
				break;
			}
			mResultType = ArgumentType::Bool;
			break;
		default:
			ASSERT(!"Cast to invalid type.");
			mResultType = ArgumentType::Unspecified;
			break;
		}

		return mResultType;
	}

	ArgumentType::Type PrecompiledExp::Compile(const AST::Expression *aExpression, const FormalArgumentMap& aArgs)
	{
		if (aExpression == NULL)
			return ArgumentType::Unspecified;

		const AST::Expression::Type astOp = aExpression->GetOpCode();

		if (astOp == AST::Expression::FuncCall)
			mResultType = CompileFuncCall(aExpression, aArgs);
		else if (astOp == AST::Expression::TypeCast)
			mResultType = CompileTypeCast(aExpression, aArgs);
		else if (astOp == AST::Expression::ExpressionList)
		{
			const AST::Expressions& operands = aExpression->GetOperands();
			ArgumentTypes argTypes;
			CompileAllOperands(aExpression, argTypes, aArgs);
			mResultType = ArgumentType::Unspecified;
		}
		else
			mResultType = CompileOperator(aExpression, aArgs);

		return mResultType;
	}

	ArgumentType::Type PrecompiledExp::CompileFuncCall(const AST::Expression* aExpression, const FormalArgumentMap& aArgs)
	{
		const AST::Expressions& operands = aExpression->GetOperands();
		ASSERT(operands.size() == 2);

		const AST::Expression* left = operands[0];
		const AST::Expression* right = operands[1];

		ASSERT(left->GetOpCode() == AST::Expression::QualifiedName);

		ArgumentTypes types;
		CompileAllOperands(right, types, aArgs);

		static const struct FunctionDescr
		{
			const char*		mName;
			int				mNumArgs;
			Opcode			mOpcode;
			ArgumentType::Type	mReturnType;
			ArgumentType::Type	mArg0Type;
			ArgumentType::Type	mArg1Type;
		} funcs[] = {
			{ "sin",		1, FSIN,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "cos",		1, FCOS,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "tan",		1, FTAN,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "asin",		1, FASIN,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "acos",		1, FACOS,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "atan",		1, FATAN,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "atan2",		2, FATAN2,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Float },
			{ "log",		1, FLOG,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "log10",		1, FLOG10,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "pow",		2, FPOW,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Float },
			{ "mod",		2, FMOD,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Float },
			{ "exp",		1, FEXP,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "sqrt",		1, FSQRT,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "ceil",		1, FCEIL,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "floor",		1, FFLOOR,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "round",		1, FROUND,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "abs",		1, FABS,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified },
			{ "min",		2, FMIN,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Float },
			{ "max",		2, FMAX,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Float },
			{ "dot",		2, V2DOT,			ArgumentType::Float, ArgumentType::Vec2,	ArgumentType::Vec2	},
			{ "dot",		2, V3DOT,			ArgumentType::Float, ArgumentType::Vec3,	ArgumentType::Vec3	},
			{ "dot",		2, V4DOT,			ArgumentType::Float, ArgumentType::Vec4,	ArgumentType::Vec4	},
			{ "cross",		2, V3CROSS,			ArgumentType::Float, ArgumentType::Vec3,	ArgumentType::Vec3	},
			{ "length",		1, V2LEN,			ArgumentType::Float, ArgumentType::Vec2,	ArgumentType::Unspecified	},
			{ "length",		1, V3LEN,			ArgumentType::Float, ArgumentType::Vec3,	ArgumentType::Unspecified	},
			{ "length",		1, V4LEN,			ArgumentType::Float, ArgumentType::Vec4,	ArgumentType::Unspecified	},
			{ "length2",	1, V2LEN2,			ArgumentType::Float, ArgumentType::Vec2,	ArgumentType::Unspecified	},
			{ "length2",	1, V3LEN2,			ArgumentType::Float, ArgumentType::Vec3,	ArgumentType::Unspecified	},
			{ "length2",	1, V4LEN2,			ArgumentType::Float, ArgumentType::Vec4,	ArgumentType::Unspecified	},
			{ "normalize",	1, V2NORM,			ArgumentType::Vec2, ArgumentType::Vec2,		ArgumentType::Unspecified	},
			{ "normalize",	1, V3NORM,			ArgumentType::Vec3, ArgumentType::Vec3,		ArgumentType::Unspecified	},
			{ "normalize",	1, V4NORM,			ArgumentType::Vec4, ArgumentType::Vec4,		ArgumentType::Unspecified	},
			{ "reflect",	2, V3REFL,			ArgumentType::Vec3, ArgumentType::Vec3,		ArgumentType::Vec3			},
			{ "rand",		1, RAND,			ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Unspecified	},
			{ "frand",		1, FRAND,			ArgumentType::Float, ArgumentType::Float,	ArgumentType::Unspecified	},
		};
		static const int size = sizeof(funcs) / sizeof(funcs[0]);

		const std::string f = left->GetName();
		const AST::Expressions& arguments = operands[1]->GetOperands();

		std::vector<int> matchingIndices;

		int i;
		for (i = 0; i < size; ++i)
		{
			const FunctionDescr& d = funcs[i];
			if (f == d.mName)
			{
				ASSERT(arguments.size() <= 2);

				if (arguments.size() != d.mNumArgs
					|| types[0] != d.mArg0Type
					|| (arguments.size() == 2 && types[1] != d.mArg1Type))
				{
					matchingIndices.push_back(i);
					continue;
				}

				break;
			}
		}

		if (i < size)
		{
			const FunctionDescr& d = funcs[i];
			mResultType = d.mReturnType;
			Emit(d.mOpcode, aExpression->GetLocation());
		}
		else
		{
			if (matchingIndices.empty())
			{
				mLSystem.Error(left->GetLocation(), "uknown function: " + left->GetName());
				mResultType = ArgumentType::Unspecified;
			}
			else
			{
				mLSystem.Error(left->GetLocation(), "argument signature does not match to any function: " + left->GetName());
				mResultType = ArgumentType::Unspecified;
			}
		}

		return mResultType;
	}

	ArgumentType::Type PrecompiledExp::CompileOperator(const AST::Expression* aExpression, const FormalArgumentMap& aArgs)
	{
		struct CodeEntry
		{
			struct OperandPair
			{
				ArgumentType::Type	mResultType;
				Opcode			mOpCode;

				OperandPair() : mResultType(ArgumentType::Unspecified), mOpCode(NOP) {}
			};

			const char * mName;						// name of the operator (for error reporting)
			int mNumArguments;						// number of arguments [0..3]
			OperandPair mValidPairs[ArgumentType::MaxArgumentTypes][ArgumentType::MaxArgumentTypes];

			CodeEntry(const char* aName, int aNumArguments) : mName(aName), mNumArguments(aNumArguments) { }

			CodeEntry& AddValidPair(ArgumentType::Type aLeftType, ArgumentType::Type aRightType, ArgumentType::Type aResultType, Opcode aOpcode)
			{
				//ASSERT(op.mResultType == ArgumentType::Unspecified);
				//ASSERT(op.mOpCode == NOP);

				mValidPairs[aLeftType][aRightType].mResultType = aResultType;
				mValidPairs[aLeftType][aRightType].mOpCode = aOpcode;

				return *this;
			}
		};

		static const CodeEntry table[AST::Expression::MaxOpCode] =
		{
			  CodeEntry("call", 0)
			, CodeEntry("list", 2)
			, CodeEntry("+", 1)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Unspecified,	ArgumentType::Int,	IADD)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Unspecified,	ArgumentType::Float,	FADD)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Unspecified,	ArgumentType::Vec2,	V2ADD)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Unspecified,	ArgumentType::Vec3,	V3ADD)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Unspecified,	ArgumentType::Vec4,	V4ADD)
			, CodeEntry("-", 1)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Unspecified,	ArgumentType::Int,	INEG)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Unspecified,	ArgumentType::Float,	FNEG)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Unspecified,	ArgumentType::Vec2,	V2NEG)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Unspecified,	ArgumentType::Vec3,	V3NEG)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Unspecified,	ArgumentType::Vec4,	V4NEG)
			, CodeEntry("~", 1)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Unspecified,	ArgumentType::Int,	INEG)
			, CodeEntry("!", 1)
				.AddValidPair(ArgumentType::Bool,	ArgumentType::Unspecified,	ArgumentType::Bool,	IADD)
			, CodeEntry("*", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	IMUL)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Float,	FMUL)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Vec2,		ArgumentType::Vec2,	V2MUL)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Vec3,		ArgumentType::Vec3,	V3MUL)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Vec4,		ArgumentType::Vec4,	V4MUL)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Float,	ArgumentType::Vec2,	V2SMUL)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Float,	ArgumentType::Vec3,	V3SMUL)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Float,	ArgumentType::Vec4,	V4SMUL)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec2,		ArgumentType::Vec2,	SV2MUL)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec3,		ArgumentType::Vec3,	SV3MUL)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec4,		ArgumentType::Vec4,	SV4MUL)
			, CodeEntry("/", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	IDIV)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Float,	FDIV)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Float,	ArgumentType::Vec2,	V2SDIV)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Float,	ArgumentType::Vec3,	V3SDIV)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Float,	ArgumentType::Vec4,	V4SDIV)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec2,	ArgumentType::Vec2,	SV2DIV)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec3,	ArgumentType::Vec3,	SV3DIV)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec4,	ArgumentType::Vec4,	SV4DIV)
			, CodeEntry("%", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	IMOD)
			, CodeEntry("+", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	IADD)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Float,	FADD)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Vec2,		ArgumentType::Vec2,	V2ADD)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Vec3,		ArgumentType::Vec3,	V3ADD)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Vec4,		ArgumentType::Vec4,	V4ADD)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Float,	ArgumentType::Vec2,	V2SADD)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Float,	ArgumentType::Vec3,	V3SADD)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Float,	ArgumentType::Vec4,	V4SADD)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec2,		ArgumentType::Vec2,	SV2ADD)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec3,		ArgumentType::Vec3,	SV3ADD)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec4,		ArgumentType::Vec4,	SV4ADD)
			, CodeEntry("-", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	ISUB)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Float,	FSUB)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Vec2,		ArgumentType::Vec2,	V2SUB)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Vec3,		ArgumentType::Vec3,	V3SUB)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Vec4,		ArgumentType::Vec4,	V4SUB)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Float,	ArgumentType::Vec2,	V2SSUB)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Float,	ArgumentType::Vec3,	V3SSUB)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Float,	ArgumentType::Vec4,	V4SSUB)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec2,		ArgumentType::Vec2,	SV2SUB)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec3,		ArgumentType::Vec3,	SV3SUB)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Vec4,		ArgumentType::Vec4,	SV4SUB)
			, CodeEntry("<<", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	SHL)
			, CodeEntry(">>", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	SHR)
			, CodeEntry("<", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Bool,	ILT)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Bool,	FLT)
			, CodeEntry(">", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Bool,	IGT)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Bool,	FGT)
			, CodeEntry("<=", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Bool,	ILE)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Bool,	FLE)
			, CodeEntry(">=", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Bool,	IGE)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Bool,	FGE)
			, CodeEntry("==", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Bool,	IEQ)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Bool,	FEQ)
				.AddValidPair(ArgumentType::Bool,	ArgumentType::Bool,		ArgumentType::Bool,	BEQ)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Vec2,		ArgumentType::Bool,	V2EQ)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Vec3,		ArgumentType::Bool,	V3EQ)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Vec4,		ArgumentType::Bool,	V4EQ)
			, CodeEntry("!=", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Bool,	INE)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Float,	ArgumentType::Bool,	FNE)
				.AddValidPair(ArgumentType::Bool,	ArgumentType::Bool,		ArgumentType::Bool,	BNE)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Vec2,		ArgumentType::Bool,	V2NE)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Vec3,		ArgumentType::Bool,	V3NE)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Vec4,		ArgumentType::Bool,	V4NE)
			, CodeEntry("&", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	BWAND)
			, CodeEntry("|", 2)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Int,		ArgumentType::Int,	BWOR)
			, CodeEntry("&&", 2)
				.AddValidPair(ArgumentType::Bool,	ArgumentType::Bool,		ArgumentType::Bool,	AND)
			, CodeEntry("||", 2)
				.AddValidPair(ArgumentType::Bool,	ArgumentType::Bool,		ArgumentType::Bool,	OR)
			, CodeEntry("^", 2)
				.AddValidPair(ArgumentType::Bool,	ArgumentType::Bool,		ArgumentType::Bool,	XOR)
			, CodeEntry("?", 3)
			, CodeEntry("integer constant", 0)
			, CodeEntry("floating constant", 0)
			, CodeEntry("boolean constant", 0)
			, CodeEntry("vec2 constant",  0)
			, CodeEntry("vec3 constant", 0)
			, CodeEntry("vec4 constant", 0)
			, CodeEntry("name reference", 0)
				.AddValidPair(ArgumentType::Int,	ArgumentType::Unspecified,	ArgumentType::Int,	ILOAD)
				.AddValidPair(ArgumentType::Float,	ArgumentType::Unspecified,	ArgumentType::Float,	FLOAD)
				.AddValidPair(ArgumentType::Bool,	ArgumentType::Unspecified,	ArgumentType::Bool,	BLOAD)
				.AddValidPair(ArgumentType::Vec2,	ArgumentType::Unspecified,	ArgumentType::Vec2,	V2LOAD)
				.AddValidPair(ArgumentType::Vec3,	ArgumentType::Unspecified,	ArgumentType::Vec3,	V3LOAD)
				.AddValidPair(ArgumentType::Vec4,	ArgumentType::Unspecified,	ArgumentType::Vec4,	V4LOAD)
			, CodeEntry("int type", 0)
			, CodeEntry("float type", 0)
			, CodeEntry("bool type", 0)
			, CodeEntry("vec2 type", 0)
			, CodeEntry("vec3 type", 0)
			, CodeEntry("vec4 type", 0)
			, CodeEntry("type cast", 2)
			, CodeEntry(".", 1)
		};

		const AST::Expression::Type astOp = aExpression->GetOpCode();
		const CodeEntry &e = table[astOp];

		static const char * typeNames[] = { "<invalid>", "int", "float", "bool", "vec2", "vec3", "vec4" };

		ArgumentTypes argumentTypes;
		CompileAllOperands(aExpression, argumentTypes, aArgs);

		const AST::Expressions& operands = aExpression->GetOperands();

		switch (astOp)
		{
		case AST::Expression::Integer:
			Emit(IPUSH, aExpression->GetLocation());
			Emit(aExpression->GetIntegerValue(), aExpression->GetLocation());
			mResultType = ArgumentType::Int;
			break;
		case AST::Expression::Float:
			Emit(FPUSH, aExpression->GetLocation());
			Emit(aExpression->GetFloatValue(), aExpression->GetLocation());
			mResultType = ArgumentType::Float;
			break;
		case AST::Expression::Boolean:
			Emit(BPUSH, aExpression->GetLocation());
			Emit(aExpression->GetBoolValue(), aExpression->GetLocation());
			mResultType = ArgumentType::Bool;
			break;
		case AST::Expression::Vec2:
			ASSERT(argumentTypes.size() == 2);
			for (int i = 0; i < (int)argumentTypes.size(); ++i)
				if (argumentTypes[i] != ArgumentType::Float)
					mLSystem.Error(aExpression->GetOperands()[i]->GetLocation(), "vec2: constructor arguments must be floating type");
			Emit(V2PUSH, aExpression->GetLocation());
			mResultType = ArgumentType::Vec2;
			break;
		case AST::Expression::Vec3:
			ASSERT(argumentTypes.size() == 3);
			for (int i = 0; i < (int)argumentTypes.size(); ++i)
				if (argumentTypes[i] != ArgumentType::Float)
					mLSystem.Error(aExpression->GetOperands()[i]->GetLocation(), "vec3: constructor arguments must be floating type");
			Emit(V3PUSH, aExpression->GetLocation());
			mResultType = ArgumentType::Vec3;
			break;
		case AST::Expression::Vec4:
			ASSERT(argumentTypes.size() == 4);
			for (int i = 0; i < (int)argumentTypes.size(); ++i)
				if (argumentTypes[i] != ArgumentType::Float)
					mLSystem.Error(aExpression->GetOperands()[i]->GetLocation(), "vec4: constructor arguments must be floating type");
			Emit(V4PUSH, aExpression->GetLocation());
			mResultType = ArgumentType::Vec4;
			break;
		case AST::Expression::QualifiedName:
		{
			const Token t = mLSystem.GetToken(aExpression->GetName());
			auto it = aArgs.find(t);
			if (it == aArgs.end())
			{
				mLSystem.Error(aExpression->GetLocation(), std::string("unknown identifier: ") + aExpression->GetName());
				mResultType = ArgumentType::Unspecified;
				return mResultType;
			}
			else
			{
				mResultType = it->second.mType;
				Emit(e.mValidPairs[it->second.mType][ArgumentType::Unspecified].mOpCode, aExpression->GetLocation());
				Emit(t, aExpression->GetLocation());
			}
		}
		break;
		case AST::Expression::FuncCall:
		case AST::Expression::TypeCast:
			ASSERT(!"internal error");
			break;
		case AST::Expression::MemberSelect:
		{
			ASSERT(e.mNumArguments == 1);
			ASSERT(operands.size() == 2);
			ASSERT(operands[1]->GetOpCode() == AST::Expression::QualifiedName);
			CompileAllOperands(operands[0], argumentTypes, aArgs);
			const std::string fieldName = operands[1]->GetName();

			switch (argumentTypes[0])
			{
			case ArgumentType::Vec2:
			{
				static const struct Swizzle
				{
					std::string mName;
					PrecompiledExp::Opcode mOpCode;
					ArgumentType::Type mResultType;
				} swizzles[] =
				{
					{ "x",	V2X,	ArgumentType::Float },
					{ "y",	V2Y,	ArgumentType::Float },
					{ "yx", V2YX,	ArgumentType::Vec2 },
				};

				for (int i = 0; i < sizeof(swizzles) / sizeof(swizzles[0]); ++i)
				{
					const Swizzle& s = swizzles[i];
					if (s.mName == fieldName)
					{
						Emit(s.mOpCode, aExpression->GetLocation());
						mResultType = s.mResultType;
						goto done;
					}
				}

				mLSystem.Error(operands[1]->GetLocation(), fieldName + ": invalid field selector");
				mResultType = ArgumentType::Unspecified;
			}
		done:
			break;
			case ArgumentType::Vec3:
			{
				static const struct Swizzle
				{
					std::string mName;
					PrecompiledExp::Opcode mOpCode;
					ArgumentType::Type mResultType;
				} swizzles[] =
				{
					{ "x",		V3X,	ArgumentType::Float },
					{ "y",		V3Y,	ArgumentType::Float },
					{ "z",		V3Z,	ArgumentType::Float },

					{ "xy",		V3XY,	ArgumentType::Vec2 },
					{ "xz",		V3XZ,	ArgumentType::Vec2 },
					{ "yx",		V3YX,	ArgumentType::Vec2 },
					{ "yz",		V3YZ,	ArgumentType::Vec2 },
					{ "zy",		V3ZY,	ArgumentType::Vec2 },
					{ "zx",		V3ZX,	ArgumentType::Vec2 },

					{ "xyz",	V3XYZ,	ArgumentType::Vec3 },
					{ "xzy",	V3XZY,	ArgumentType::Vec3 },
					{ "yxz",	V3YXZ,	ArgumentType::Vec3 },
					{ "yzx",	V3YZX,	ArgumentType::Vec3 },
					{ "zxy",	V3ZXY,	ArgumentType::Vec3 },
					{ "zyx",	V3ZYX,	ArgumentType::Vec3 },
				};

				for (int i = 0; i < sizeof(swizzles) / sizeof(swizzles[0]); ++i)
				{
					const Swizzle& s = swizzles[i];
					if (s.mName == fieldName)
					{
						Emit(s.mOpCode, aExpression->GetLocation());
						mResultType = s.mResultType;
						goto done;
					}
				}

				mLSystem.Error(operands[1]->GetLocation(), fieldName + ": invalid field selector");
				mResultType = ArgumentType::Unspecified;
			}
			break;
			case ArgumentType::Vec4:
			{
				static const struct Swizzle
				{
					std::string mName;
					PrecompiledExp::Opcode mOpCode;
					ArgumentType::Type mResultType;
				} swizzles[] =
				{
					{ "x",		V4X,	ArgumentType::Float },
					{ "y",		V4Y,	ArgumentType::Float },
					{ "z",		V4Z,	ArgumentType::Float },
					{ "w",		V4W,	ArgumentType::Float },

					{ "xy",		V4XY,	ArgumentType::Vec2 },
					{ "yx",		V4YX,	ArgumentType::Vec2 },
					{ "xz",		V4XZ,	ArgumentType::Vec2 },
					{ "zx",		V4ZX,	ArgumentType::Vec2 },
					{ "xw",		V4XW,	ArgumentType::Vec2 },
					{ "wx",		V4WX,	ArgumentType::Vec2 },
					{ "yz",		V4YZ,	ArgumentType::Vec2 },
					{ "zy",		V4ZY,	ArgumentType::Vec2 },
					{ "yw",		V4YW,	ArgumentType::Vec2 },
					{ "wy",		V4WY,	ArgumentType::Vec2 },

					{ "xyz",	V4XYZ,	ArgumentType::Vec3 },
					{ "xyw",	V4XYW,	ArgumentType::Vec3 },
					{ "xzy",	V4XZY,	ArgumentType::Vec3 },
					{ "xzw",	V4XZW,	ArgumentType::Vec3 },
					{ "xwy",	V4XWY,	ArgumentType::Vec3 },
					{ "xwz",	V4XWZ,	ArgumentType::Vec3 },
					{ "yxz",	V4YXZ,	ArgumentType::Vec3 },
					{ "yxw",	V4YXW,	ArgumentType::Vec3 },
					{ "yzx",	V4YZX,	ArgumentType::Vec3 },
					{ "yzw",	V4YZW,	ArgumentType::Vec3 },
					{ "ywx",	V4YWX,	ArgumentType::Vec3 },
					{ "ywz",	V4YWZ,	ArgumentType::Vec3 },
					{ "zxy",	V4ZXY,	ArgumentType::Vec3 },
					{ "zxw",	V4ZXW,	ArgumentType::Vec3 },
					{ "zyx",	V4ZYX,	ArgumentType::Vec3 },
					{ "zyw",	V4ZYW,	ArgumentType::Vec3 },
					{ "zwx",	V4ZWX,	ArgumentType::Vec3 },
					{ "zwy",	V4ZWY,	ArgumentType::Vec3 },
					{ "wxy",	V4WXY,	ArgumentType::Vec3 },
					{ "wxz",	V4WXZ,	ArgumentType::Vec3 },
					{ "wyx",	V4WYX,	ArgumentType::Vec3 },
					{ "wyz",	V4WYZ,	ArgumentType::Vec3 },
					{ "wzx",	V4WZX,	ArgumentType::Vec3 },
					{ "wzy",	V4WZY,	ArgumentType::Vec3 },

					{ "xyzw",	V4XYZW,	ArgumentType::Vec4 },
					{ "xywz",	V4XYWZ,	ArgumentType::Vec4 },
					{ "xzyw",	V4XZYW,	ArgumentType::Vec4 },
					{ "xzwy",	V4XZWY,	ArgumentType::Vec4 },
					{ "xwyz",	V4XWYZ,	ArgumentType::Vec4 },
					{ "xwzy",	V4XWZY,	ArgumentType::Vec4 },

					{ "yxzw",	V4YXZW,	ArgumentType::Vec4 },
					{ "yxwz",	V4YXWZ,	ArgumentType::Vec4 },
					{ "yzxw",	V4YZXW,	ArgumentType::Vec4 },
					{ "yzwx",	V4YZWX,	ArgumentType::Vec4 },
					{ "ywxz",	V4YWXZ,	ArgumentType::Vec4 },
					{ "ywzx",	V4YWZX,	ArgumentType::Vec4 },

					{ "zxyw",	V4ZXYW,	ArgumentType::Vec4 },
					{ "zxwy",	V4ZXWY,	ArgumentType::Vec4 },
					{ "zyxw",	V4ZYXW,	ArgumentType::Vec4 },
					{ "zywx",	V4ZYWX,	ArgumentType::Vec4 },
					{ "zwxy",	V4ZWXY,	ArgumentType::Vec4 },
					{ "zwyx",	V4ZWYX,	ArgumentType::Vec4 },

					{ "wxyz",	V4WXYZ,	ArgumentType::Vec4 },
					{ "wxzy",	V4WXZY,	ArgumentType::Vec4 },
					{ "wyxz",	V4WYXZ,	ArgumentType::Vec4 },
					{ "wyzx",	V4WYZX,	ArgumentType::Vec4 },
					{ "wzxy",	V4WZXY,	ArgumentType::Vec4 },
					{ "wzyx",	V4WZYX,	ArgumentType::Vec4 },
				};

				for (int i = 0; i < sizeof(swizzles) / sizeof(swizzles[0]); ++i)
				{
					const Swizzle& s = swizzles[i];
					if (s.mName == fieldName)
					{
						Emit(s.mOpCode, aExpression->GetLocation());
						mResultType = s.mResultType;
						goto done;
					}
				}

				mLSystem.Error(operands[1]->GetLocation(), fieldName + ": invalid field selector");
				mResultType = ArgumentType::Unspecified;
			}
			break;
			default:
				ASSERT(!"Internal error: unknown left-hand side type for swizzle/member select operator");
			}
		}
		break;
		default:
			if (e.mNumArguments == 1)
			{
				const CodeEntry::OperandPair& op = e.mValidPairs[argumentTypes[0]][ArgumentType::Unspecified];
				if (op.mOpCode == NOP)
				{
					mLSystem.Error(operands[0]->GetLocation(), std::string(e.mName) + " operator is not allowed on type " + typeNames[argumentTypes[0]]);
					mResultType = ArgumentType::Unspecified;
				}
				else
				{
					Emit(op.mOpCode, aExpression->GetLocation());
					mResultType = op.mResultType;
				}
			}
			else if (e.mNumArguments == 2)
			{
				const CodeEntry::OperandPair& op = e.mValidPairs[argumentTypes[0]][argumentTypes[1]];
				if (op.mOpCode == NOP)
				{
					mLSystem.Error(aExpression->GetLocation(), std::string("operand types do not match: ") + typeNames[argumentTypes[0]] + " '" + e.mName + "' " + typeNames[argumentTypes[1]]);
					mResultType = ArgumentType::Unspecified;
				}
				else
				{
					Emit(op.mOpCode, aExpression->GetLocation());
					mResultType = op.mResultType;
				}
			}
			else
				ASSERT(!"internal error");
			break;
		}

		return mResultType;
	}

	void PrecompiledExp::Emit(PrecompiledExp::Opcode aOpcode, const Location &aLocation)
	{
		ASSERT(aOpcode != NOP);

		AddLocation(aLocation);
		mCode.Push(aOpcode);
	}

	void PrecompiledExp::Emit(int aInteger, const Location &aLocation)
	{
		AddLocation(aLocation);
		mCode.Push(aInteger);
	}

	void PrecompiledExp::Emit(float aFloat, const Location &aLocation)
	{
		AddLocation(aLocation);
		mCode.Push(aFloat);
	}

	void PrecompiledExp::EmitName(Token aName, const Location &aLocation)
	{
		AddLocation(aLocation);
		mCode.Push(aName);
	}

	void PrecompiledExp::Emit(glm::vec2 aVec2, const Location &aLocation)
	{
		AddLocation(aLocation);
		mCode.Push(aVec2);
	}

	void PrecompiledExp::Emit(glm::vec3 aVec3, const Location &aLocation)
	{
		AddLocation(aLocation);
		mCode.Push(aVec3);
	}


	void PrecompiledExp::Emit(glm::vec4 aVec4, const Location &aLocation)
	{
		AddLocation(aLocation);
		mCode.Push(aVec4);
	}

	const char* PrecompiledExp::ToString(Opcode o)
	{
		static const char * names[] = {
#define T(C,M,D) M,
			SCOPE_OPCODES()
#undef T
		};
		return names[o];
	}

	std::string PrecompiledExp::ToString(const glm::vec2& v)
	{
		std::ostringstream s;
		s << "vec2(" << v[0] << ", " << v[1] << "," << ")";
		return s.str();
	}

	std::string PrecompiledExp::ToString(const glm::vec3& v)
	{
		std::ostringstream s;
		s << "vec3(" << v[0] << ", " << v[1] << "," << v[2] << ")";
		return s.str();
	}

	std::string PrecompiledExp::ToString(const glm::vec4& v)
	{
		std::ostringstream s;
		s << "vec4(" << v[0] << ", " << v[1] << "," << v[2] << "," << v[3] << ")";
		return s.str();
	}

	void PrecompiledExp::Evaluate(const ArgumentValueMap& aArgs, State& aState) const
	{
		ASSERT(!mCode.IsEmpty());

		for (int ip = 0; ip < (int)mCode.Size(); )
		{
			const Opcode o = mCode.Get<Opcode>(ip);
			ip += sizeof(Opcode);
			const int sp = aState.ElementCount();
//			(cerr << ToString(o) << " ");
			switch (o)
			{
			case NOP:	break;

			case IPUSH:		aState.Push(mCode.Get<int>(ip));	ip += sizeof(int);		break;
			case FPUSH:		aState.Push(mCode.Get<float>(ip));	ip += sizeof(float);	break;
			case BPUSH:		aState.Push(mCode.Get<bool>(ip));	ip += sizeof(bool);		break;

			case V2PUSH:	
			{
				auto y = aState.Pop<float>();
				auto x = aState.Pop<float>();
				aState.Push(glm::vec2(x, y));	break;
			}
			case V3PUSH:	
			{
				auto z = aState.Pop<float>();
				auto y = aState.Pop<float>();
				auto x = aState.Pop<float>();
				aState.Push(glm::vec3(x, y, z));	break;
			}
			case V4PUSH:	
			{
				auto w = aState.Pop<float>();
				auto z = aState.Pop<float>();
				auto y = aState.Pop<float>();
				auto x = aState.Pop<float>();
				aState.Push(glm::vec4(x, y, z, w));	break;
			}

			case ILOAD:		aState.Push(aArgs.Get<int>(mCode.Get<int>(ip)));	ip += sizeof(int);	break;
			case FLOAD:		aState.Push(aArgs.Get<float>(mCode.Get<int>(ip)));	ip += sizeof(int);	break;
			case BLOAD:		aState.Push(aArgs.Get<bool>(mCode.Get<int>(ip)));	ip += sizeof(int);	break;
			case V2LOAD:	aState.Push(aArgs.Get<glm::vec2>(mCode.Get<int>(ip)));	ip += sizeof(int);	break;
			case V3LOAD:	aState.Push(aArgs.Get<glm::vec3>(mCode.Get<int>(ip)));	ip += sizeof(int);	break;
			case V4LOAD:	aState.Push(aArgs.Get<glm::vec4>(mCode.Get<int>(ip)));	ip += sizeof(int);	break;

			case INEG:	aState.Top<int>() = -aState.Top<int>();		break;
			case FNEG:	aState.Top<float>() = -aState.Top<float>();	break;
			case V2NEG:	aState.Top<glm::vec2>() = -aState.Top<glm::vec2>();	break;
			case V3NEG:	aState.Top<glm::vec3>() = -aState.Top<glm::vec3>();	break;
			case V4NEG:	aState.Top<glm::vec4>() = -aState.Top<glm::vec4>();	break;

			case BWNEG:	aState.Top<int>() = ~aState.Top<int>();	break;
			case NOT:	aState.Top<bool>() = !aState.Top<bool>();	break;

			case IMUL:	aState.Top<int>(-2) = aState.Top<int>(-2) *	aState.Top<int>(-1);							aState.Pop<int>();		break;
			case FMUL:	aState.Top<float>(-2) = aState.Top<float>(-2) *	aState.Top<float>(-1);						aState.Pop<float>();	break;
			case V2MUL:	aState.Top<glm::vec2>(-2) = aState.Top<glm::vec2>(-2) *	aState.Top<glm::vec2>(-1);	aState.Pop<glm::vec2>();	break;
			case V3MUL:	aState.Top<glm::vec3>(-2) = aState.Top<glm::vec3>(-2) *	aState.Top<glm::vec3>(-1);	aState.Pop<glm::vec3>();	break;
			case V4MUL:	aState.Top<glm::vec4>(-2) = aState.Top<glm::vec4>(-2) *	aState.Top<glm::vec4>(-1);	aState.Pop<glm::vec4>();	break;

			case V2SMUL: { const float s = aState.Pop<float>(); const glm::vec2 v = aState.Pop<glm::vec2>(); const glm::vec2 r = v * s; aState.Push(r);	}	break;
			case V3SMUL: { const float s = aState.Pop<float>(); const glm::vec3 v = aState.Pop<glm::vec3>(); const glm::vec3 r = v * s; aState.Push(r);	}	break;
			case V4SMUL: { const float s = aState.Pop<float>(); const glm::vec4 v = aState.Pop<glm::vec4>(); const glm::vec4 r = v * s; aState.Push(r);	}	break;

			case SV2MUL: { const glm::vec2 v = aState.Pop<glm::vec2>(); const float s = aState.Pop<float>(); const glm::vec2 r = v * s; aState.Push(r);	}	break;
			case SV3MUL: { const glm::vec3 v = aState.Pop<glm::vec3>(); const float s = aState.Pop<float>(); const glm::vec3 r = v * s; aState.Push(r);	}	break;
			case SV4MUL: { const glm::vec4 v = aState.Pop<glm::vec4>(); const float s = aState.Pop<float>(); const glm::vec4 r = v * s; aState.Push(r);	}	break;

			case IDIV:		aState.Top<int>(-2) = aState.Top<int>(-2) / aState.Top<int>(-1);							aState.Pop<int>();		break;
			case FDIV:		aState.Top<float>(-2) = aState.Top<float>(-2) / aState.Top<float>(-1);						aState.Pop<float>();	break;

			case V2SDIV: { const float s = aState.Pop<float>(); const glm::vec2 v = aState.Pop<glm::vec2>(); const glm::vec2 r = v / s; aState.Push(r);	}	break;
			case V3SDIV: { const float s = aState.Pop<float>(); const glm::vec3 v = aState.Pop<glm::vec3>(); const glm::vec3 r = v / s; aState.Push(r);	}	break;
			case V4SDIV: { const float s = aState.Pop<float>(); const glm::vec4 v = aState.Pop<glm::vec4>(); const glm::vec4 r = v / s; aState.Push(r);	}	break;

			case SV2DIV: { const glm::vec2 v = aState.Pop<glm::vec2>(); const float s = aState.Pop<float>(); const glm::vec2 r = v / s; aState.Push(r);	}	break;
			case SV3DIV: { const glm::vec3 v = aState.Pop<glm::vec3>(); const float s = aState.Pop<float>(); const glm::vec3 r = v / s; aState.Push(r);	}	break;
			case SV4DIV: { const glm::vec4 v = aState.Pop<glm::vec4>(); const float s = aState.Pop<float>(); const glm::vec4 r = v / s; aState.Push(r);	}	break;

			case IMOD:		aState.Top<int>(-2) = aState.Top<int>(-2) % aState.Top<int>(-1);							aState.Pop<int>();			break;

			case IADD:		aState.Top<int>(-2) = aState.Top<int>(-2) + aState.Top<int>(-1);				aState.Pop<int>();		break;
			case FADD:		aState.Top<float>(-2) = aState.Top<float>(-2) + aState.Top<float>(-1); aState.Pop<float>();	break;

			case V2ADD:		aState.Top<glm::vec2>(-2) = aState.Top<glm::vec2>(-2) + aState.Top<glm::vec2>(-1);	aState.Pop<glm::vec2>();	break;
			case V3ADD:		aState.Top<glm::vec3>(-2) = aState.Top<glm::vec3>(-2) + aState.Top<glm::vec3>(-1);	aState.Pop<glm::vec3>();	break;
			case V4ADD:		aState.Top<glm::vec4>(-2) = aState.Top<glm::vec4>(-2) + aState.Top<glm::vec4>(-1);	aState.Pop<glm::vec4>();	break;

			case V2SADD: { const float s = aState.Pop<float>(); const glm::vec2 v = aState.Pop<glm::vec2>(); const glm::vec2 r = v + s; aState.Push(r);	}	break;
			case V3SADD: { const float s = aState.Pop<float>(); const glm::vec3 v = aState.Pop<glm::vec3>(); const glm::vec3 r = v + s; aState.Push(r);	}	break;
			case V4SADD: { const float s = aState.Pop<float>(); const glm::vec4 v = aState.Pop<glm::vec4>(); const glm::vec4 r = v + s; aState.Push(r);	}	break;

			case SV2ADD: { const glm::vec2 v = aState.Pop<glm::vec2>(); const float s = aState.Pop<float>(); const glm::vec2 r = v + s; aState.Push(r);	}	break;
			case SV3ADD: { const glm::vec3 v = aState.Pop<glm::vec3>(); const float s = aState.Pop<float>(); const glm::vec3 r = v + s; aState.Push(r);	}	break;
			case SV4ADD: { const glm::vec4 v = aState.Pop<glm::vec4>(); const float s = aState.Pop<float>(); const glm::vec4 r = v + s; aState.Push(r);	}	break;

			case ISUB:	aState.Top<int>(-2) = aState.Top<int>(-2) - aState.Top<int>(-1);	aState.Pop<int>();	break;
			case FSUB:	aState.Top<float>(-2) = aState.Top<float>(-2) - aState.Top<float>(-1);	aState.Pop<float>();	break;
			case V2SUB:	aState.Top<glm::vec2>(-2) = aState.Top<glm::vec2>(-2) - aState.Top<glm::vec2>(-1);	aState.Pop<glm::vec2>();	break;
			case V3SUB:	aState.Top<glm::vec3>(-2) = aState.Top<glm::vec3>(-2) - aState.Top<glm::vec3>(-1);	aState.Pop<glm::vec3>();	break;
			case V4SUB:	aState.Top<glm::vec4>(-2) = aState.Top<glm::vec4>(-2) - aState.Top<glm::vec4>(-1);	aState.Pop<glm::vec4>();	break;

			case V2SSUB: { const float s = aState.Pop<float>(); const glm::vec2 v = aState.Pop<glm::vec2>(); const glm::vec2 r = v - s; aState.Push(r);	}	break;
			case V3SSUB: { const float s = aState.Pop<float>(); const glm::vec3 v = aState.Pop<glm::vec3>(); const glm::vec3 r = v - s; aState.Push(r);	}	break;
			case V4SSUB: { const float s = aState.Pop<float>(); const glm::vec4 v = aState.Pop<glm::vec4>(); const glm::vec4 r = v - s; aState.Push(r);	}	break;

			case SV2SUB: { const glm::vec2 v = aState.Pop<glm::vec2>(); const float s = aState.Pop<float>(); const glm::vec2 r = v - s; aState.Push(r);	}	break;
			case SV3SUB: { const glm::vec3 v = aState.Pop<glm::vec3>(); const float s = aState.Pop<float>(); const glm::vec3 r = v - s; aState.Push(r);	}	break;
			case SV4SUB: { const glm::vec4 v = aState.Pop<glm::vec4>(); const float s = aState.Pop<float>(); const glm::vec4 r = v - s; aState.Push(r);	}	break;

			case SHR:	aState.Top<int>(-2) = aState.Top<int>(-2) >> aState.Top<int>(-1);	aState.Pop<int>();	break;
			case SHL:	aState.Top<int>(-2) = aState.Top<int>(-2) << aState.Top<int>(-1);	aState.Pop<int>();	break;

			case ILT:	aState.Push<bool>(aState.Pop<int>() > aState.Pop<int>());		break;
			case FLT:	aState.Push<bool>(aState.Pop<float>() > aState.Pop<float>());	break;
			case IGT:	aState.Push<bool>(aState.Pop<int>() < aState.Pop<int>());		break;
			case FGT:	aState.Push<bool>(aState.Pop<float>() < aState.Pop<float>());	break;
			case ILE:	aState.Push<bool>(aState.Pop<int>() >= aState.Pop<int>());		break;
			case FLE:	aState.Push<bool>(aState.Pop<float>() >= aState.Pop<float>());	break;
			case IGE:	aState.Push<bool>(aState.Pop<int>() <= aState.Pop<int>());		break;
			case FGE:	aState.Push<bool>(aState.Pop<float>() <= aState.Pop<float>());	break;

			case IEQ:	aState.Push<bool>(aState.Pop<int>() == aState.Pop<int>());		break;
			case FEQ:	aState.Push<bool>(aState.Pop<float>() == aState.Pop<float>());	break;
			case BEQ:	aState.Push<bool>(aState.Pop<bool>() == aState.Pop<bool>());	break;
			case V2EQ:	aState.Push<bool>(aState.Pop<glm::vec2>() == aState.Pop<glm::vec2>());	break;
			case V3EQ:	aState.Push<bool>(aState.Pop<glm::vec3>() == aState.Pop<glm::vec3>());	break;
			case V4EQ:	aState.Push<bool>(aState.Pop<glm::vec4>() == aState.Pop<glm::vec4>());	break;

			case INE:	aState.Push<bool>(aState.Pop<int>() != aState.Pop<int>());		break;
			case FNE:	aState.Push<bool>(aState.Pop<float>() != aState.Pop<float>());	break;
			case BNE:	aState.Push<bool>(aState.Pop<bool>() != aState.Pop<bool>());		break;
			case V2NE:	aState.Push<bool>(aState.Pop<glm::vec2>() != aState.Pop<glm::vec2>());	break;
			case V3NE:	aState.Push<bool>(aState.Pop<glm::vec3>() != aState.Pop<glm::vec3>());	break;
			case V4NE:	aState.Push<bool>(aState.Pop<glm::vec4>() != aState.Pop<glm::vec4>());	break;

			case BWAND:	aState.Top<int>(-2) = aState.Top<int>(-2) &	aState.Top<int>(-1);		aState.Pop<int>();	break;
			case BWOR:	aState.Top<int>(-2) = aState.Top<int>(-2) | aState.Top<int>(-1);		aState.Pop<int>();	break;

			case AND:	aState.Top<bool>(-2) = aState.Top<bool>(-2) && aState.Top<bool>(-1);	aState.Pop<bool>();	break;
			case OR:	aState.Top<bool>(-2) = aState.Top<bool>(-2) || aState.Top<bool>(-1);	aState.Pop<bool>();	break;
			case XOR:	aState.Top<bool>(-2) = aState.Top<bool>(-2) ^ aState.Top<bool>(-1);		aState.Pop<bool>();	break;

			case JP:	ip = mCode.Get<int>(++ip);	break;
			case JPT:	if (aState.Pop<bool>())		ip = mCode.Get<int>(++ip);		break;
			case JPF:	if (!aState.Pop<bool>())	ip = mCode.Get<int>(++ip);		break;
			case CBN:	ip = mCode.Get<int>(++ip);	break;

			case FSIN:	aState.Top<float>(-1) = sinf(aState.Top<float>(-1));	break;
			case FCOS:	aState.Top<float>(-1) = cosf(aState.Top<float>(-1));	break;
			case FTAN:	aState.Top<float>(-1) = tanf(aState.Top<float>(-1));	break;
			case FASIN:	aState.Top<float>(-1) = asinf(aState.Top<float>(-1));	break;
			case FACOS:	aState.Top<float>(-1) = acosf(aState.Top<float>(-1));	break;
			case FATAN:	aState.Top<float>(-1) = acosf(aState.Top<float>(-1));	break;
			case FLOG:	aState.Top<float>(-1) = logf(aState.Top<float>(-1));	break;
			case FEXP:	aState.Top<float>(-1) = expf(aState.Top<float>(-1));	break;
			case FSQRT:	aState.Top<float>(-1) = sqrtf(aState.Top<float>(-1));	break;
			case FCEIL:	aState.Top<float>(-1) = ceilf(aState.Top<float>(-1));	break;
			case FABS:	aState.Top<float>(-1) = abs(aState.Top<float>(-1));	break;
			case ITOF:	aState.Top<float>(-1) = float(aState.Top<int>(-1));	break;
			case BTOF:	aState.Top<float>(-1) = float(aState.Top<bool>(-1));	break;
			case BTOI:	aState.Top<int>(-1) = int(aState.Top<bool>(-1));	break;
			case ITOB:	aState.Top<bool>(-1) = !!(aState.Top<int>(-1));	break;
			case FLOG10:	aState.Top<float>(-1) = log10f(aState.Top<float>(-1));	break;
			case FFLOOR:	aState.Top<float>(-1) = floorf(aState.Top<float>(-1));	break;
			case FROUND:	aState.Top<float>(-1) = floorf(aState.Top<float>(-1) + 0.5f);	break;

			case FMOD:		aState.Top<float>(-2) = fmod(aState.Top<float>(-2), aState.Top<float>(-1)); aState.Pop<float>(); break;
			case FPOW:		aState.Top<float>(-2) = powf(aState.Top<float>(-2), aState.Top<float>(-1)); aState.Pop<float>(); break;
			case FMIN:		aState.Top<float>(-2) = min(aState.Top<float>(-2), aState.Top<float>(-1)); aState.Pop<float>(); break;
			case FMAX:		aState.Top<float>(-2) = max(aState.Top<float>(-2), aState.Top<float>(-1)); aState.Pop<float>(); break;
			case FATAN2:	aState.Top<float>(-2) = atan2f(aState.Top<float>(-2), aState.Top<float>(-1)); aState.Pop<float>(); break;

			case V2X:		aState.Push(aState.Pop<glm::vec2>()[0]); break;
			case V2Y:		aState.Push(aState.Pop<glm::vec2>()[1]); break;

			case V2YX: { const glm::vec2 v = aState.Pop<glm::vec2>(); aState.Push(glm::vec2(v[1], v[0])); break; }

			case V3X:	aState.Push(aState.Pop<glm::vec3>()[0]); break;
			case V3Y:	aState.Push(aState.Pop<glm::vec3>()[1]); break;
			case V3Z:	aState.Push(aState.Pop<glm::vec3>()[2]); break;

			case V3XY: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec2(v[0], v[1])); break; }
			case V3XZ: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec2(v[0], v[2])); break; }
			case V3YX: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec2(v[1], v[0])); break; }
			case V3YZ: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec2(v[1], v[2])); break; }
			case V3ZX: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec2(v[2], v[0])); break; }
			case V3ZY: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec2(v[2], v[1])); break; }

			case V3XYZ: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec3(v[0], v[1], v[2])); break; }
			case V3XZY: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec3(v[0], v[2], v[1])); break; }
			case V3YXZ: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec3(v[1], v[0], v[2])); break; }
			case V3YZX: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec3(v[1], v[2], v[0])); break; }
			case V3ZXY: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec3(v[2], v[0], v[1])); break; }
			case V3ZYX: { const glm::vec3 v = aState.Pop<glm::vec3>(); aState.Push(glm::vec3(v[2], v[1], v[0])); break; }

			case V4X:	aState.Push(aState.Pop<glm::vec4>()[0]); break;
			case V4Y:	aState.Push(aState.Pop<glm::vec4>()[1]); break;
			case V4Z:	aState.Push(aState.Pop<glm::vec4>()[2]); break;
			case V4W:	aState.Push(aState.Pop<glm::vec4>()[3]); break;

			case V4XY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[0], v[1])); break; }
			case V4YX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[2], v[0])); break; }
			case V4XZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[0], v[2])); break; }
			case V4ZX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[2], v[0])); break; }
			case V4YZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[1], v[2])); break; }
			case V4ZY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[2], v[1])); break; }
			case V4YW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[1], v[3])); break; }
			case V4WY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[3], v[1])); break; }
			case V4XW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[0], v[3])); break; }
			case V4WX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec2(v[3], v[0])); break; }

			case V4XYZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[0], v[1], v[2])); break; }
			case V4XYW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[0], v[1], v[3])); break; }
			case V4XZY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[0], v[2], v[1])); break; }
			case V4XZW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[0], v[2], v[3])); break; }
			case V4XWY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[0], v[3], v[1])); break; }
			case V4XWZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[0], v[3], v[2])); break; }

			case V4YXZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[1], v[0], v[2])); break; }
			case V4YXW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[1], v[0], v[3])); break; }
			case V4YZX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[1], v[2], v[0])); break; }
			case V4YZW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[1], v[2], v[3])); break; }
			case V4YWX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[1], v[3], v[0])); break; }
			case V4YWZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[1], v[3], v[2])); break; }

			case V4ZXY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[2], v[0], v[1])); break; }
			case V4ZXW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[2], v[0], v[2])); break; }
			case V4ZYX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[2], v[1], v[0])); break; }
			case V4ZYW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[2], v[1], v[3])); break; }
			case V4ZWX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[2], v[3], v[0])); break; }
			case V4ZWY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[2], v[3], v[1])); break; }

			case V4WXY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[3], v[0], v[1])); break; }
			case V4WXZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[3], v[0], v[2])); break; }
			case V4WYX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[3], v[1], v[0])); break; }
			case V4WYZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[3], v[1], v[2])); break; }
			case V4WZX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[3], v[2], v[0])); break; }
			case V4WZY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec3(v[3], v[2], v[1])); break; }

			case V4XYZW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[0], v[1], v[2], v[3])); break; }
			case V4XYWZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[0], v[1], v[3], v[2])); break; }
			case V4XZYW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[0], v[2], v[1], v[3])); break; }
			case V4XZWY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[0], v[2], v[3], v[1])); break; }
			case V4XWYZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[0], v[3], v[1], v[2])); break; }
			case V4XWZY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[0], v[3], v[2], v[1])); break; }

			case V4YXZW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[1], v[0], v[2], v[3])); break; }
			case V4YXWZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[1], v[0], v[3], v[2])); break; }
			case V4YZXW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[1], v[2], v[0], v[3])); break; }
			case V4YZWX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[1], v[2], v[3], v[0])); break; }
			case V4YWXZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[1], v[3], v[0], v[2])); break; }
			case V4YWZX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[1], v[3], v[2], v[0])); break; }

			case V4ZXYW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[2], v[0], v[1], v[3])); break; }
			case V4ZXWY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[2], v[0], v[3], v[1])); break; }
			case V4ZYXW: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[2], v[1], v[0], v[3])); break; }
			case V4ZYWX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[2], v[1], v[3], v[0])); break; }
			case V4ZWXY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[2], v[3], v[0], v[1])); break; }
			case V4ZWYX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[2], v[3], v[1], v[0])); break; }

			case V4WXYZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[3], v[0], v[1], v[2])); break; }
			case V4WXZY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[3], v[0], v[2], v[1])); break; }
			case V4WYXZ: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[3], v[1], v[0], v[2])); break; }
			case V4WYZX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[3], v[1], v[2], v[0])); break; }
			case V4WZXY: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[3], v[2], v[0], v[1])); break; }
			case V4WZYX: { const glm::vec4 v = aState.Pop<glm::vec4>(); aState.Push(glm::vec4(v[3], v[2], v[1], v[0])); break; }

			case V2DOT: { const glm::vec2 v0 = aState.Pop<glm::vec2>(); const glm::vec2 v1 = aState.Pop<glm::vec2>(); aState.Push<float>(glm::dot(v1, v0)); break; }
			case V3DOT: { const glm::vec3 v0 = aState.Pop<glm::vec3>(); const glm::vec3 v1 = aState.Pop<glm::vec3>(); aState.Push<float>(glm::dot(v1, v0)); break; }
			case V4DOT: { const glm::vec4 v0 = aState.Pop<glm::vec4>(); const glm::vec4 v1 = aState.Pop<glm::vec4>(); aState.Push<float>(glm::dot(v1, v0)); break; }

			case V3CROSS: { const glm::vec3 v0 = aState.Pop<glm::vec3>(); const glm::vec3 v1 = aState.Pop<glm::vec3>(); aState.Push(glm::cross(v1, v0)); break; }

			case V2LEN: { aState.Push(glm::length(aState.Pop<glm::vec2>())); break; }
			case V3LEN: { aState.Push(glm::length(aState.Pop<glm::vec3>())); break; }
			case V4LEN: { aState.Push(glm::length(aState.Pop<glm::vec4>())); break; }

			case V2LEN2: { aState.Push(glm::length2(aState.Pop<glm::vec2>())); break; }
			case V3LEN2: { aState.Push(glm::length2(aState.Pop<glm::vec3>())); break; }
			case V4LEN2: { aState.Push(glm::length2(aState.Pop<glm::vec4>())); break; }

			case V2NORM: { aState.Push(glm::normalize(aState.Pop<glm::vec2>())); break; }
			case V3NORM: { aState.Push(glm::normalize(aState.Pop<glm::vec3>())); break; }
			case V4NORM: { aState.Push(glm::normalize(aState.Pop<glm::vec4>())); break; }
			case V2REFL: { const glm::vec2 v0 = aState.Pop<glm::vec2>(); const glm::vec2 v1 = aState.Pop<glm::vec2>(); aState.Push(glm::reflect(v1, v0)); break; }
			case V3REFL: { const glm::vec3 v0 = aState.Pop<glm::vec3>(); const glm::vec3 v1 = aState.Pop<glm::vec3>(); aState.Push(glm::reflect(v1, v0)); break; }
			case V4REFL: { const glm::vec4 v0 = aState.Pop<glm::vec4>(); const glm::vec4 v1 = aState.Pop<glm::vec4>(); aState.Push(glm::reflect(v1, v0)); break; }
			case RAND: { aState.Push(rand() % aState.Pop<int>()); break; }
			case FRAND: { aState.Push(static_cast<float>(rand()) / (static_cast <float>(RAND_MAX) / aState.Pop<float>())); break; }

			default: ASSERT(!"Invalid opcode");
			}
		}
	}

	void PrecompiledExp::CompileAllOperands(const AST::Expression* aExpression, ArgumentTypes& aTypes, const FormalArgumentMap& aFormalArguments)
	{
		ASSERT(aExpression != NULL);
		const AST::Expressions& operands = aExpression->GetOperands();

		if (aExpression->GetOpCode() == AST::Expression::MemberSelect)
		{
			// special case: member selector/swizzle operators are compiled as if the the left operand is a namespace
			// so we don't try to compile the right operand (name node) here
			const AST::Expressions& operands = aExpression->GetOperands();
			ASSERT(operands.size() == 2);
			ArgumentType::Type type = Compile(operands[0], aFormalArguments);
			aTypes.push_back(type);
			aTypes.push_back(ArgumentType::Unspecified);
		}
		else
		{
			for (const auto expression : operands)
			{
				ArgumentType::Type type = Compile(expression, aFormalArguments);
				aTypes.push_back(type);
			}
		}
	}
}
