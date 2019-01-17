#pragma once

#include <map>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

#include "Common.h"
#include "AST.h"

namespace LSystem
{
	#define SCOPE_OPCODES() \
		T(NOP, "NOP","NOP")\
		T(IPUSH, "IPUSH","IPUSH")\
		T(FPUSH, "FPUSH","FPUSH")\
		T(BPUSH, "BPUSH","BPUSH")\
		T(V2PUSH, "V2PUSH","V2PUSH")\
		T(V3PUSH, "V3PUSH","V3PUSH")\
		T(V4PUSH, "V4PUSH","V4PUSH")\
		T(ILOAD, "ILOAD","ILOAD")\
		T(FLOAD, "FLOAD","FLOAD")\
		T(BLOAD, "BLOAD","BLOAD")\
		T(V2LOAD, "V2LOAD","V2LOAD")\
		T(V3LOAD, "V3LOAD","V3LOAD")\
		T(V4LOAD, "V4LOAD","V4LOAD")\
		T(INEG, "INEG","INEG")\
		T(FNEG, "FNEG","FNEG")\
		T(V2NEG, "V2NEG","V2NEG")\
		T(V3NEG, "V3NEG","V3NEG")\
		T(V4NEG, "V4NEG","V4NEG")\
		T(IMUL, "IMUL","IMUL")\
		T(FMUL, "FMUL","FMUL")\
		T(V2MUL, "V2MUL","V2MUL")\
		T(V3MUL, "V3MUL","V3MUL")\
		T(V4MUL, "V4MUL","V4MUL")\
		T(V2SMUL, "V2SMUL","V2SMUL")\
		T(V3SMUL, "V3SMUL","V3SMUL")\
		T(V4SMUL, "V4SMUL","V4SMUL")\
		T(SV2MUL, "SV2MUL","SV2MUL")\
		T(SV3MUL, "SV3MUL","SV3MUL")\
		T(SV4MUL, "SV4MUL","SV4MUL")\
		T(IDIV, "IDIV","IDIV")\
		T(FDIV, "FDIV","FDIV")\
		T(V2SDIV, "V2SDIV","V2SDIV")\
		T(V3SDIV, "V3SDIV","V3SDIV")\
		T(V4SDIV, "V4SDIV","V4SDIV")\
		T(SV2DIV, "SV2DIV", "SV2DIV")\
		T(SV3DIV,"SV3DIV","SV3DIV")\
		T(SV4DIV,"SV4DIV","SV4DIV")\
		T(FMOD,"FMOD","FMOD")\
		T(IMOD,"IMOD","IMOD")\
		T(IADD,"IADD","IADD")\
		T(FADD,"FADD","FADD")\
		T(V2ADD,"V2ADD","V2ADD")\
		T(V3ADD,"V3ADD","V3ADD")\
		T(V4ADD,"V4ADD","V4ADD")\
		T(V2SADD,"V2SADD","V2SADD")\
		T(V3SADD,"V3SADD","V3SADD")\
		T(V4SADD,"V4SADD","V4SADD")\
		T(SV2ADD,"SV2ADD","SV2ADD")\
		T(SV3ADD,"SV3ADD","SV3ADD")\
		T(SV4ADD,"SV4ADD","SV4ADD")\
		T(ISUB,"ISUB","ISUB")\
		T(FSUB,"FSUB","FSUB")\
		T(V2SUB,"V2SUB","V2SUB")\
		T(V3SUB,"V3SUB","V3SUB")\
		T(V4SUB,"V4SUB","V4SUB")\
		T(V2SSUB,"V2SSUB","V2SSUB")\
		T(V3SSUB,"V3SSUB","V3SSUB")\
		T(V4SSUB,"V4SSUB","V4SSUB")\
		T(SV2SUB,"SV2SUB","SV2SUB")\
		T(SV3SUB,"SV3SUB","SV3SUB")\
		T(SV4SUB,"SV4SUB","SV4SUB")\
		T(SHR,"SHR","SHR")\
		T(SHL,"SHL","SHL")\
		T(ILT,"ILT","ILT")\
		T(FLT,"FLT","FLT")\
		T(IGT,"IGT","IGT")\
		T(FGT,"FGT","FGT")\
		T(ILE,"ILE","ILE")\
		T(FLE,"FLE","FLE")\
		T(IGE,"IGE","IGE")\
		T(FGE,"FGE","FGE")\
		T(IEQ,"IEQ","IEQ")\
		T(FEQ,"FEQ","FEQ")\
		T(BEQ,"BEQ","BEQ")\
		T(V2EQ,"V2EQ","V2EQ")\
		T(V3EQ,"V3EQ","V3EQ")\
		T(V4EQ,"V4EQ","V4EQ")\
		T(INE,"INE","INE")\
		T(FNE,"FNE","FNE")\
		T(BNE,"BNE","BNE")\
		T(V2NE,"V2NE","V2NE")\
		T(V3NE,"V3NE","V3NE")\
		T(V4NE,"V4NE","V4NE")\
		T(BWAND,"BWAND","BWAND")\
		T(BWOR,"BWOR","BWOR")\
		T(BWNEG,"BWNEG","BWNEG")\
		T(AND,"AND","AND")\
		T(OR,"OR","OR")\
		T(XOR,"XOR","XOR")\
		T(NOT,"NOT","NOT")\
		T(JP,"JP","JP")\
		T(JPT,"JPT","JPT")\
		T(JPF,"JPF","JPF")\
		T(CBN,"CBN","CBN")\
		T(FSIN,"FSIN","FSIN")\
		T(FCOS,"FCOS","FCOS")\
		T(FTAN,"FTAN","FTAN")\
		T(FASIN,"FASIN","FASIN")\
		T(FACOS,"FACOS","FACOS")\
		T(FATAN,"FATAN","FATAN")\
		T(FATAN2,"FATAN2","FATAN2")\
		T(FLOG,"FLOG","FLOG")\
		T(FLOG10,"FLOG10","FLOG10")\
		T(FPOW,"FPOW","FPOW")\
		T(FEXP,"FEXP","FEXP")\
		T(FSQRT,"FSQRT","FSQRT")\
		T(FCEIL,"FCEIL","FCEIL")\
		T(FFLOOR,"FFLOOR","FFLOOR")\
		T(FROUND,"FROUND","FROUND")\
		T(FABS,"FABS","FABS")\
		T(FMIN,"FMIN","FMIN")\
		T(FMAX,"FMAX","FMAX")\
		T(FTOI,"FTOI","FTOI")\
		T(FTOB,"FTOB","FTOB")\
		T(ITOF,"ITOF","ITOF")\
		T(ITOB,"ITOB","ITOB")\
		T(BTOI,"BTOI","BTOI")\
		T(BTOF,"BTOF","BTOF")\
		T(V2X,"V2X","V2X")\
		T(V2Y,"V2Y","V2Y")\
		T(V2YX,"V2YX","V2YX")\
		T(V3X,"V3X","V3X")\
		T(V3Y,"V3Y","V3Y")\
		T(V3Z,"V3Z","V3Z")\
		T(V3XY,"V3XY","V3XY")\
		T(V3XZ,"V3XZ","V3XZ")\
		T(V3YX,"V3YX","V3YX")\
		T(V3YZ,"V3YZ","V3YZ")\
		T(V3ZX,"V3ZX","V3ZX")\
		T(V3ZY,"V3ZY","V3ZY")\
		T(V3XYZ,"V3XYZ","V3XYZ")\
		T(V3XZY,"V3XZY","V3XZY")\
		T(V3YXZ,"V3YXZ","V3YXZ")\
		T(V3YZX,"V3YZX","V3YZX")\
		T(V3ZXY,"V3ZXY","V3ZXY")\
		T(V3ZYX,"V3ZYX","V3ZYX")\
		T(V4X,"V4X","V4X")\
		T(V4Y,"V4Y","V4Y")\
		T(V4Z,"V4Z","V4Z")\
		T(V4W,"V4W","V4W")\
		T(V4XY,"V4XY","V4XY")\
		T(V4YX,"V4YX","V4YX")\
		T(V4XZ,"V4XZ","V4XZ")\
		T(V4ZX,"V4ZX","V4ZX")\
		T(V4YZ,"V4YZ","V4YZ")\
		T(V4ZY,"V4ZY","V4ZY")\
		T(V4YW,"V4YW","V4YW")\
		T(V4WY,"V4WY","V4WY")\
		T(V4XW,"V4XW","V4XW")\
		T(V4WX,"V4WX","V4WX")\
		T(V4XYZ,"V4XYZ","V4XYZ")\
		T(V4XYW,"V4XYW","V4XYW")\
		T(V4XZY,"V4XZY","V4XZY")\
		T(V4XZW,"V4XZW","V4XZW")\
		T(V4XWY,"V4XWY","V4XWY")\
		T(V4XWZ,"V4XWZ","V4XWZ")\
		T(V4YXZ,"V4YXZ","V4YXZ")\
		T(V4YXW,"V4YXW","V4YXW")\
		T(V4YZX,"V4YZX","V4YZX")\
		T(V4YZW,"V4YZW","V4YZW")\
		T(V4YWX,"V4YWX","V4YWX")\
		T(V4YWZ,"V4YWZ","V4YWZ")\
		T(V4ZXY,"V4ZXY","V4ZXY")\
		T(V4ZXW,"V4ZXW","V4ZXW")\
		T(V4ZYX,"V4ZYX","V4ZYX")\
		T(V4ZYW,"V4ZYW","V4ZYW")\
		T(V4ZWX,"V4ZWX","V4ZWX")\
		T(V4ZWY,"V4ZWY","V4ZWY")\
		T(V4WXY,"V4WXY","V4WXY")\
		T(V4WXZ,"V4WXZ","V4WXZ")\
		T(V4WYX,"V4WYX","V4WYX")\
		T(V4WYZ,"V4WYZ","V4WYZ")\
		T(V4WZX,"V4WZX","V4WZX")\
		T(V4WZY,"V4WZY","V4WZY")\
		T(V4XYZW,"V4XYZW","V4XYZW")\
		T(V4XYWZ,"V4XYWZ","V4XYWZ")\
		T(V4XZYW,"V4XZYW","V4XZYW")\
		T(V4XZWY,"V4XZWY","V4XZWY")\
		T(V4XWYZ,"V4XWYZ","V4XWYZ")\
		T(V4XWZY,"V4XWZY","V4XWZY")\
		T(V4YXZW,"V4YXZW","V4YXZW")\
		T(V4YXWZ,"V4YXWZ","V4YXWZ")\
		T(V4YZXW,"V4YZXW","V4YZXW")\
		T(V4YZWX,"V4YZWX","V4YZWX")\
		T(V4YWXZ,"V4YWXZ","V4YWXZ")\
		T(V4YWZX,"V4YWZX","V4YWZX")\
		T(V4ZXYW,"V4ZXYW","V4ZXYW")\
		T(V4ZXWY,"V4ZXWY","V4ZXWY")\
		T(V4ZYXW,"V4ZYXW","V4ZYXW")\
		T(V4ZYWX,"V4ZYWX","V4ZYWX")\
		T(V4ZWXY,"V4ZWXY","V4ZWXY")\
		T(V4ZWYX,"V4ZWYX","V4ZWYX")\
		T(V4WXYZ,"V4WXYZ","V4WXYZ")\
		T(V4WXZY,"V4WXZY","V4WXZY")\
		T(V4WYXZ,"V4WYXZ","V4WYXZ")\
		T(V4WYZX,"V4WYZX","V4WYZX")\
		T(V4WZXY,"V4WZXY","V4WZXY")\
		T(V4WZYX,"V4WZYX","V4WZYX")\
		T(V2DOT,"V2DOT","V2DOT")\
		T(V3DOT,"V3DOT","V3DOT")\
		T(V4DOT,"V4DOT","V4DOT")\
		T(V3CROSS,"V3CROSS","V3CROSS")\
		T(V2LEN,"V2LEN","V2LEN")\
		T(V3LEN,"V3LEN","V3LEN")\
		T(V4LEN,"V4LEN","V4LEN")\
		T(V2LEN2,"V2LEN2","V2LEN2")\
		T(V3LEN2,"V3LEN2","V3LEN2")\
		T(V4LEN2,"V4LEN2","V4LEN2")\
		T(V2NORM,"V2NORM","V2NORM")\
		T(V3NORM,"V3NORM","V3NORM")\
		T(V4NORM,"V4NORM","V4NORM")\
		T(V2REFL,"V2REFL","V2REFL")\
		T(V3REFL,"V3REFL","V3REFL")\
		T(V4REFL,"V4REFL","V4REFL")\
		T(RAND,"RAND","RAND")\
		T(FRAND,"FRAND","FRAND")\

	class PrecompiledExp
	{
	public:
		typedef enum
		{
			UFlag	= 1 << ArgumentType::Unspecified,
			IFlag	= 1 << ArgumentType::Int,
			FFlag	= 1 << ArgumentType::Float,
			BFlag	= 1 << ArgumentType::Bool,
			V2Flag	= 1 << ArgumentType::Vec2,
			V3Flag	= 1 << ArgumentType::Vec3,
			V4Flag	= 1 << ArgumentType::Vec4,
			VFlag	= V2Flag | V3Flag | V4Flag,
			AllFlag	= IFlag | FFlag | BFlag | V2Flag | V3Flag | V4Flag,
		} ArgumentTypeFlags;

		PrecompiledExp(LSystem& aLSystem);
		PrecompiledExp(const PrecompiledExp& o);
		PrecompiledExp& operator =(const PrecompiledExp& o);

		ArgumentType::Type Compile(const AST::Expression * aExpression, const FormalArgumentMap& aArgs);
		void DebugDump() const;

		template <typename T>
		T Evaluate(const ArgumentValueMap& aArgs) const
		{
			State stack;
			Evaluate(aArgs, stack);
			ASSERT(stack.ElementCount() == 1);
			ASSERT(stack.GetType(0) == ArgumentType::GetType<T>());
			// ASSERT(stack.mIndex[0].mType == GetType<T>());
			const T v = stack.operator []<T>(0);
			return v;
		}

		inline ArgumentType::Type GetResultType() const { return mResultType; }
		inline bool IsEmpty() const { return mCode.IsEmpty(); }
		inline int GetCodeSize() const { return mCode.Size(); }

	private:
		typedef enum Opcode
		{
#define T(C,M,D) C,
			SCOPE_OPCODES()
#undef T
		} Opcode;

		struct BuiltinFunction
		{
			ArgumentType::Type mReturnType;
			int mNumArgs;
		};

		typedef ArgumentValueList State;
		typedef std::map<int, Location> DebugInfo;

		ArgumentType::Type CompileFuncCall(const AST::Expression* aExpression, const FormalArgumentMap& aArgs);
		ArgumentType::Type CompileOperator(const AST::Expression* aExpression, const FormalArgumentMap& aArgs);
		ArgumentType::Type CompileTypeCast(const AST::Expression* aExpression, const FormalArgumentMap& aArgs);
		void CompileAllOperands(const AST::Expression* aExpression, ArgumentTypes& aTypes, const FormalArgumentMap& aFormalArguments);

		void Emit(Opcode aOpcode, const Location& aLocation);
		void Emit(int aInteger, const Location& aLocation);
		void Emit(float aFloat, const Location& aLocation);
		void Emit(glm::vec2 aVec2, const Location& aLocation);
		void Emit(glm::vec3 aVec3, const Location& aLocation);
		void Emit(glm::vec4 aVec4, const Location& aLocation);
		void EmitName(Token aName, const Location& aLocation);
		const Location& GetLocation(int ip) const;
		void AddLocation(const Location& aLocation);

		void Evaluate(const ArgumentValueMap& aArgs, ArgumentValueList& aState) const;

		static const char* ToString(Opcode o);
		static std::string ToString(const glm::vec2& v);
		static std::string ToString(const glm::vec3& v);
		static std::string ToString(const glm::vec4& v);

		LSystem&			mLSystem; 
		CodeStorage			mCode;
		DebugInfo			mDebugInfo;
		ArgumentType::Type	mResultType;
	};
}