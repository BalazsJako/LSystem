#pragma once

#include <vector>
#include <string>
#include <string.h>

#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "Location.h"
#include "Common.h"

namespace LSystem
{
	namespace AST
	{
		class Name;
		class QualifiedName;
		class NameList;
		class Namespace;
		class NamespaceList;
		class ImportList;
		class Rule;
		class RuleList;
		class RuleDefs;
		class Predecessor;
		class Successor;
		class Expression;
		class ModuleDefList;
		class ModuleDef;
		class ModuleExp;
		class Grammar;

		enum RuleKind
		{
			Replace,
			Decompose,
			Transform
		};

		typedef std::vector<Expression*> Expressions;

		class ASTNode
		{
		public:
			ASTNode(const Location &aLocation);
			virtual ~ASTNode(void);

			const Location& GetLocation() const { return mLocation; }

		protected:
			Location mLocation;
		};

		class NamespaceList : public ASTNode
		{
		public:
			typedef std::vector<Namespace*> Namespaces;

			NamespaceList(const Location &aLocation, Namespace* aNamespace);
			~NamespaceList();

			void AddNamespace(Namespace* aNamespace);

			const Namespaces& GetNamespaces() const { return mNamespaces; }

		private:
			Namespaces mNamespaces;
		};

		class Namespace : public ASTNode
		{
		public:
			Namespace(const Location &aLocation, Name* aName, NamespaceList* aNamespaces, RuleDefs* aRuleDefs);
			const Name* GetName() const { return mName; }
			const RuleDefs* GetRuleDefs() const { return mRuleDefs; }
			const NamespaceList* GetNamespaces() const { return mNamespaces; }

			void AddNamespace(Namespace* aNamespace);

		private:
			Name* mName;
			NamespaceList* mNamespaces;
			RuleDefs* mRuleDefs;
		};

		class Grammar : public ASTNode
		{
		public:
			Grammar(const Location &aLocation, ImportList* aImports, NamespaceList* aNamespaceList, Successor* aAxiom);
			virtual ~Grammar();

			const Successor* GetAxiom() const { return mAxiom; }
			const NamespaceList* GetNamespaceList() const { return mNamespaceList; }
			const ImportList* GetImports() const { return mImports; }

		private:
			ImportList* mImports;
			Successor*	mAxiom;
			NamespaceList* mNamespaceList;
		};

		class Name : public ASTNode
		{
		public:
			Name(const Location &aLocation, const char* aValue);

			const std::string& GetValue() const { return mValue; }
		private:
			std::string mValue;
		};

		class QualifiedName : public ASTNode
		{
		public:
			typedef std::vector<std::string> Values;
			QualifiedName(const Location &aLocation, const char* aValue);

			void AddName(const char* aValue);

			const Values& GetValues() const { return mValues; }
			const std::string& GetFullValue() const { return mFullValue; }
			const std::string& GetNamePart() const { return mNamePart; }

			bool IsAbsolute() const { return mValues.size() > 1; }
		private:
			Values mValues;
			std::string mFullValue;
			std::string mNamePart;
		};

		class ArgumentSpec : public ASTNode
		{
		public:
			ArgumentSpec(Name* aName, ArgumentType::Type aType);
			virtual ~ArgumentSpec();

			inline const Name*	GetName() const { return mName; }
			inline ArgumentType::Type	GetType() const { return mType; }

		private:
			Name* mName;
			ArgumentType::Type	mType;
		};

		class ImportList : public ASTNode
		{
		public:
			typedef std::vector<QualifiedName*> Imports;

			ImportList(const Location& aLocation, QualifiedName* aImport);
			~ImportList();

			void AddImport(QualifiedName* aQualifiedName);

			inline const Imports& GetImports() const { return mImports; }

		private:
			Imports mImports;
		};

		class Rule : public ASTNode
		{
		public:
			Rule(RuleKind aKind, Name* aLabel, float aProbability, Predecessor* aPredecessor, Expression* aCondition, Successor* aSuccessor);
			virtual ~Rule();

			RuleKind			GetKind() const { return mKind; }
			const Name*			GetLabel()	const	{ return mLabel; }
			const Predecessor*	GetPredecessor() const	{ return mPredecessor; }
			const Expression*	GetCondition()	const	{ return mCondition; }
			const Successor*	GetSuccessor()	const	{ return mSuccessor; }
			float				GetProbability() const	{ return mProbability; }

		private:
			RuleKind mKind;
			Name* mLabel;
			Predecessor* mPredecessor;
			Expression*	mCondition;
			Successor* mSuccessor;
			float mProbability;
		};

		class RuleDefs : public ASTNode
		{
		public:
			RuleDefs(const Location &aLocation, NameList* aVariables, NameList* aConstants, RuleList* aRules);
			const NameList*	GetVariables() const { return mVariables; }
			const NameList*	GetConstants() const { return mConstants; }
			const RuleList*	GetRules() const { return mRules; }

		private:
			NameList* mVariables;
			NameList* mConstants;
			RuleList* mRules;
		};

		class RuleList : public ASTNode
		{
		public:
			typedef std::vector<Rule*> Rules;

			RuleList(const Location &aLocation);
			virtual ~RuleList(void);

			void			AddRule(Rule* aRule);
			const Rules&	GetRules() const { return mRules; }

		private:
			Rules mRules;
		};

		class RuleSections : public ASTNode
		{
		public:
			RuleSections(const Location& aLocation);
			RuleSections(RuleList* aPlainRules, RuleList* aDecompositions, RuleList* aHomomorphisms);
			virtual ~RuleSections();

			void SetPlainRules(RuleList* aRules);
			void SetHomomorphisms(RuleList* aRules);
			void SetDecompositions(RuleList* aRules);

			inline const RuleList* GetPlainRules() const { return mPlainRules; }
			inline const RuleList* GetHomomorphisms() const { return mHomomorphisms; }
			inline const RuleList* GetDecompositions() const { return mDecompositions; }

		private:
			RuleList* mPlainRules;
			RuleList* mDecompositions;
			RuleList* mHomomorphisms;
		};

		class NameList : public ASTNode
		{
		public:
			typedef std::vector<const Name *> Names;

			NameList(const Location &aLocation);
			virtual ~NameList(void);

			void			AddName(Name* aName);
			void			AddNameList(NameList* aNames);
			const Names&	GetNames() const { return mNames; }

		private:
			Names mNames;
		};

		class ArgumentSpecList : public ASTNode
		{
		public:
			typedef std::vector<ArgumentSpec *> ArgumentSpecs;

			ArgumentSpecList();
			virtual ~ArgumentSpecList(void);

			void	AddArgumentSpec(ArgumentSpec* aArgumentSpec);
			void	AddArgumentSpecList(ArgumentSpecList* aArgumentSpecs);
			const	ArgumentSpecs&	GetArgumentSpecs() const { return mArgumentSpecs; }

		private:
			ArgumentSpecs mArgumentSpecs;
		};

		class Predecessor : public ASTNode
		{
		public:
			Predecessor(ModuleDefList* aLeftContext, ModuleDef* aVariable, ModuleDefList* aRightContext);
			virtual ~Predecessor(void);

			const ModuleDefList*	GetLeftContext()	const { return mLeftContext; }
			const ModuleDef*		GetVariable()		const { return mVariable; }
			const ModuleDefList*	GetRightContext()	const { return mRightContext; }

		private:
			ModuleDefList*	mLeftContext;
			ModuleDef*		mVariable;
			ModuleDefList*	mRightContext;
		};

		class Successor : public ASTNode
		{
		public:
			typedef std::vector<ModuleExp *> Successors;

			Successor(const Location &aLocation);
			virtual ~Successor();

			void	AddModuleExp(ModuleExp* aModule);
			void	MergeSuccessor(Successor* aSuccessor);
			const	Successors& GetSuccessors() const { return mSuccessors; }

		private:
			Successors mSuccessors;
		};

		class ModuleDefList : public ASTNode
		{
		public:
			typedef std::vector<ModuleDef *> ModuleDefs;

			ModuleDefList(const Location &aLocation);
			virtual ~ModuleDefList();

			void	AddModuleDef(ModuleDef* aModuleDef);
			void	MergeModuleDefList(ModuleDefList* aModuleDefList);
			const	ModuleDefs& GetModuleDefs() const { return mModuleDefs; }

		private:
			ModuleDefs mModuleDefs;
		};

		class ModuleExp : public ASTNode
		{
		public:
			ModuleExp(QualifiedName* aName, Expression* aParameters);
			virtual ~ModuleExp();

			inline const QualifiedName*			GetName() const			{ return mName; }
			inline const Expression*	GetParameters() const	{ return mParameters; }

		private:
			QualifiedName* mName;
			Expression* mParameters;
		};

		// Module definition class
		// Holds name token and parameter list definition.
		class ModuleDef : public ASTNode
		{
		public:
			ModuleDef(QualifiedName* aName, ArgumentSpecList* aArguments);
			virtual ~ModuleDef();

			inline const QualifiedName*				GetName() const			{ return mName; }
			inline const ArgumentSpecList*	GetArguments() const	{ return mArguments; }

		private:
			QualifiedName* mName;
			ArgumentSpecList* mArguments;
		};

		class Expression : public ASTNode
		{
		public:
			typedef enum
			{
				FuncCall,			ExpressionList,		UnaryPlus,			UnaryMinus,
				Negate,				Not,				Multiply,			Divide,
				Remainder,			Add,				Subtract,			ShiftLeft,
				ShiftRight,			LessThan,			GreaterThan,		LessOrEqual,
				GreaterOrEqual,		Equals,				NotEquals,			BitwiseAnd,
				BitwiseOr,			And,				Or,					XOr,
				Conditional,		Integer,			Float,				Boolean,
				Vec2,				Vec3,				Vec4,				QualifiedName,
				IntType,			FloatType,			BoolType,			Vec2Type,
				Vec3Type,			Vec4Type,			TypeCast,			MemberSelect,
				MaxOpCode,
			} Type;

			Expression(Type aOpCode, const Location &aLocation);

			Expression(const Location &aLocation, int aValue );
			Expression(const Location &aLocation, bool aValue );
			Expression(const Location &aLocation, float aValue );
			Expression(const Location &aLocation, glm::vec2 aValue );
			Expression(const Location &aLocation, glm::vec3 aValue );
			Expression(const Location &aLocation, glm::vec4 aValue );
			Expression(const Location &aLocation, const char* aName );

			virtual ~Expression();

			const Expressions&	GetOperands() const		{ return mOperands; }
			Type				GetOpCode() const		{ return mOpCode; }

			inline int			GetIntegerValue() const	{ return mIntegerValue; }
			inline float		GetFloatValue() const	{ return mFloatValue; }
			inline bool			GetBoolValue() const	{ return mBoolValue; }
			inline glm::vec2	GetVec2Value() const	{ return mVec2Value; }
			inline glm::vec3	GetVec3Value() const	{ return mVec3Value; }
			inline glm::vec4	GetVec4Value() const	{ return mVec4Value; }
			inline const std::string GetName() const	{ return mName; }

			int AddOperand(Expression* aOperand);

		private:
			Type		mOpCode;
			Expressions	mOperands;

			std::string	mName;
			int			mIntegerValue;
			float		mFloatValue;
			bool		mBoolValue;
			glm::vec2	mVec2Value;
			glm::vec3	mVec3Value;
			glm::vec4	mVec4Value;
		};
	}
}
