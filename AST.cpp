#include <algorithm>
#include "AST.h"

namespace LSystem
{
	namespace AST
	{
		ASTNode::ASTNode(const Location &aLocation)
			: mLocation(aLocation)
		{
		}

		ASTNode::~ASTNode()
		{
		}

		Grammar::Grammar(const Location &aLocation, ImportList* aImports, NamespaceList* aNamespaceList, Successor* aAxiom)
			: ASTNode(aLocation)
			, mImports(aImports)
			, mAxiom(aAxiom)
			, mNamespaceList(aNamespaceList)
		{
			if (aImports != NULL)
				mLocation += aImports->GetLocation();
			if (aNamespaceList != NULL)
				mLocation += aNamespaceList->GetLocation();
			if (aAxiom != NULL)
				mLocation += aAxiom->GetLocation();
		}

		Grammar::~Grammar()
		{
			delete mImports;
			delete mAxiom;
			delete mNamespaceList;
		}

		Namespace::Namespace(const Location &aLocation, Name* aName, NamespaceList* aNamespaces, RuleDefs* aRuleDefs)
			: ASTNode(aLocation)
			, mName(aName)
			, mNamespaces(aNamespaces)
			, mRuleDefs(aRuleDefs)
		{
		}

		void Namespace::AddNamespace(Namespace * aNamespace)
		{
			if (aNamespace != nullptr)
			{
				mNamespaces->AddNamespace(aNamespace);
				mLocation += aNamespace->GetLocation();
			}
		}

		Name::Name(const Location &aLocation, const char* aName)
			: ASTNode(aLocation)
			, mValue(aName)
		{
		}

		ArgumentSpec::ArgumentSpec(Name* aName, ArgumentType::Type aType)
			: ASTNode(aName->GetLocation())
			, mName(aName)
			, mType(aType)
		{
		}

		ArgumentSpec::~ArgumentSpec()
		{
			delete mName;
		}

		Rule::Rule(RuleKind aKind, Name* aLabel, float aProbability, Predecessor* aPredecessor, Expression* aCondition, Successor* aSuccessor)
			: ASTNode(aLabel->GetLocation() + aPredecessor->GetLocation() + aSuccessor->GetLocation())
			, mKind(aKind)
			, mLabel(aLabel)
			, mPredecessor(aPredecessor)
			, mCondition(aCondition)
			, mSuccessor(aSuccessor)
			, mProbability(aProbability)
		{
			if (aCondition != NULL)
				mLocation += aCondition->GetLocation();
		}

		Rule::~Rule()
		{
			delete mLabel;
			delete mPredecessor;
			delete mCondition;
			delete mSuccessor;
		}

		RuleList::~RuleList(void)
		{
			for (auto rule : mRules)
				delete rule;
		}

		void RuleList::AddRule(Rule* aRule)
		{
			mLocation += aRule->GetLocation();
			mRules.push_back(aRule);
		}

		RuleList::RuleList(const Location &aLocation)
			: ASTNode(aLocation)
		{

		}

		RuleSections::RuleSections(const Location& aLocation)
			: ASTNode(aLocation)
			, mPlainRules(NULL)
			, mDecompositions(NULL)
			, mHomomorphisms(NULL)
		{
		}

		RuleSections::RuleSections(RuleList* aPlainRules, RuleList* aDecompositions, RuleList* aHomomorphisms)
			: ASTNode(aPlainRules->GetLocation())
			, mPlainRules(aPlainRules)
			, mDecompositions(aDecompositions)
			, mHomomorphisms(aHomomorphisms)
		{
			if (mDecompositions != NULL)
				mLocation += mDecompositions->GetLocation();
			if (mHomomorphisms != NULL)
				mLocation += mHomomorphisms->GetLocation();
		}

		RuleSections::~RuleSections()
		{
			delete mPlainRules;
			delete mDecompositions;
			delete mHomomorphisms;
		}

		void RuleSections::SetPlainRules(RuleList* aRules)
		{
			ASSERT(aRules != NULL);
			mPlainRules = aRules;
			mLocation = mPlainRules->GetLocation();
		}

		void RuleSections::SetHomomorphisms(RuleList* aRules)
		{
			ASSERT(aRules != NULL);
			mHomomorphisms = aRules;
			mLocation += mHomomorphisms->GetLocation();
		}

		void RuleSections::SetDecompositions(RuleList* aRules)
		{
			ASSERT(aRules != NULL);
			mDecompositions = aRules;
			mLocation += mDecompositions->GetLocation();
		}

		void NameList::AddName(Name* aName)
		{
			mLocation += aName->GetLocation();
			mNames.push_back(aName);
		}

		void NameList::AddNameList(NameList* aNames)
		{
			mLocation += aNames->GetLocation();
			mNames.insert(mNames.end(), aNames->mNames.begin(), aNames->mNames.end());
			aNames->mNames.clear();
			delete aNames;
		}

		NameList::NameList(const Location &aLocation)
			: ASTNode(aLocation)
		{
		}

		NameList::~NameList()
		{
			for (auto name : mNames)
				delete name;
		}

		void ArgumentSpecList::AddArgumentSpec(ArgumentSpec* aArgumentSpec)
		{
			mLocation += aArgumentSpec->GetLocation();
			mArgumentSpecs.push_back(aArgumentSpec);
		}

		void ArgumentSpecList::AddArgumentSpecList(ArgumentSpecList* aArgumentSpecs)
		{
			mLocation += aArgumentSpecs->GetLocation();
			mArgumentSpecs.insert(mArgumentSpecs.end(), aArgumentSpecs->mArgumentSpecs.begin(), aArgumentSpecs->mArgumentSpecs.end());
			aArgumentSpecs->mArgumentSpecs.clear();
			delete aArgumentSpecs;
		}

		ArgumentSpecList::ArgumentSpecList()
			: ASTNode(Location())
		{
		}

		ArgumentSpecList::~ArgumentSpecList()
		{
			for (auto argspec : mArgumentSpecs)
				delete argspec;
		}


		Predecessor::Predecessor(ModuleDefList* aLeftContext, ModuleDef* aVariable, ModuleDefList* aRightContext)
			: ASTNode(aVariable->GetLocation())
			, mLeftContext(aLeftContext)
			, mVariable(aVariable)
			, mRightContext(aRightContext)
		{
			if (aLeftContext != NULL)
				mLocation += aLeftContext->GetLocation();
			if (aRightContext != NULL)
				mLocation += aRightContext->GetLocation();
		}

		Predecessor::~Predecessor(void)
		{
			delete mLeftContext;
			delete mVariable;
			delete mRightContext;
		}

		ModuleDef::ModuleDef(QualifiedName* aName, ArgumentSpecList* aArguments)
			: ASTNode(aArguments != NULL ? aName->GetLocation() + aArguments->GetLocation() : aName->GetLocation())
			, mName(aName)
			, mArguments(aArguments)
		{
		}

		ModuleDef::~ModuleDef()
		{
			delete mName;
			delete mArguments;
		}

		ModuleExp::ModuleExp(QualifiedName* aName, Expression* aParameters)
			: ASTNode(aName->GetLocation())
			, mName(aName)
			, mParameters(aParameters)
		{
		}

		ModuleExp::~ModuleExp()
		{
			delete mName;
			delete mParameters;
		}

		Successor::Successor(const Location &aLocation)
			: ASTNode(aLocation)
		{
		}

		Successor::~Successor()
		{
			for (auto s : mSuccessors)
				delete s;
		}

		void Successor::AddModuleExp(ModuleExp* aModule)
		{
			mLocation += aModule->GetLocation();
			mSuccessors.push_back(aModule);
		}

		void Successor::MergeSuccessor(Successor* aSuccessorList)
		{
			ASSERT(aSuccessorList != nullptr);
			ASSERT(aSuccessorList != this);

			mLocation += aSuccessorList->GetLocation();
			mSuccessors.insert(mSuccessors.end(), aSuccessorList->mSuccessors.begin(), aSuccessorList->mSuccessors.end());
			aSuccessorList->mSuccessors.clear();
		}

		ModuleDefList::ModuleDefList(const Location &aLocation)
			: ASTNode(aLocation)
		{
		}

		ModuleDefList::~ModuleDefList()
		{
			for (auto m : mModuleDefs)
				delete m;
		}

		void ModuleDefList::AddModuleDef(ModuleDef* aModuleDef)
		{
			mLocation += aModuleDef->GetLocation();
			mModuleDefs.push_back(aModuleDef);
		}

		void ModuleDefList::MergeModuleDefList(ModuleDefList* aModuleDefList)
		{
			mLocation += aModuleDefList->GetLocation();
			mModuleDefs.insert(mModuleDefs.end(), aModuleDefList->mModuleDefs.begin(), aModuleDefList->mModuleDefs.end());
			aModuleDefList->mModuleDefs.clear();
		}

		Expression::Expression(Type aOpCode, const Location &aLocation)
			: ASTNode(aLocation)
			, mOpCode(aOpCode)
			, mName("")
			, mIntegerValue(0)
			, mFloatValue(0)
			, mBoolValue(false)
		{
		}

		Expression::Expression(const Location &aLocation, int aIntegerValue)
			: ASTNode(aLocation)
			, mOpCode(Integer)
			, mName("")
			, mIntegerValue(aIntegerValue)
			, mFloatValue(0)
			, mBoolValue(false)
		{
		}

		Expression::Expression(const Location &aLocation, float aFloatValue)
			: ASTNode(aLocation)
			, mOpCode(Float)
			, mName("")
			, mIntegerValue(0)
			, mFloatValue(aFloatValue)
			, mBoolValue(false)
		{
		}

		Expression::Expression(const Location &aLocation, bool aBoolValue)
			: ASTNode(aLocation)
			, mOpCode(Boolean)
			, mName("")
			, mIntegerValue(0)
			, mFloatValue(0)
			, mBoolValue(aBoolValue)
		{
		}

		Expression::Expression(const Location &aLocation, glm::vec2 aValue)
			: ASTNode(aLocation)
			, mOpCode(Vec2)
			, mName("")
			, mIntegerValue(0)
			, mFloatValue(0)
			, mVec2Value(aValue)
		{
		}

		Expression::Expression(const Location &aLocation, glm::vec3 aValue)
			: ASTNode(aLocation)
			, mOpCode(Vec3)
			, mName("")
			, mIntegerValue(0)
			, mFloatValue(0)
			, mVec3Value(aValue)
		{
		}

		Expression::Expression(const Location &aLocation, glm::vec4 aValue)
			: ASTNode(aLocation)
			, mOpCode(Vec4)
			, mName("")
			, mIntegerValue(0)
			, mFloatValue(0)
			, mVec4Value(aValue)
		{
		}

		Expression::Expression(const Location &aLocation, const char* aName)
			: ASTNode(aLocation)
			, mOpCode(QualifiedName)
			, mName(aName)
			, mIntegerValue(0)
			, mFloatValue(0)
		{
		}

		Expression::~Expression()
		{
			for (auto o : mOperands)
				delete o;
		}

		int Expression::AddOperand(Expression* aOperand)
		{
			ASSERT(aOperand != NULL);
			ASSERT(aOperand != this);
			ASSERT(std::find(mOperands.begin(), mOperands.end(), aOperand) == mOperands.end());

			mOperands.push_back(aOperand);
			return (int)mOperands.size();
		}

		ImportList::ImportList(const Location & aLocation, QualifiedName * aImport)
			: ASTNode(aLocation)
		{
			AddImport(aImport);
		}

		ImportList::~ImportList()
		{
			for (auto i : mImports)
				delete i;
		}

		void ImportList::AddImport(QualifiedName * aQualifiedName)
		{
			if (aQualifiedName != nullptr)
			{
				mLocation += aQualifiedName->GetLocation();
				mImports.push_back(aQualifiedName);
			}
		}
		
		QualifiedName::QualifiedName(const Location & aLocation, const char * aValue)
			: ASTNode(aLocation)
		{
			mNamePart = aValue;
			mFullValue = aValue;
			mValues.push_back(aValue);
		}
		
		void QualifiedName::AddName(const char * aValue)
		{
			mNamePart = aValue;
			mValues.push_back(aValue);
			mFullValue += ".";
			mFullValue += aValue;
		}

		RuleDefs::RuleDefs(const Location & aLocation, NameList * aVariables, NameList * aConstants, RuleList * aRules)
			: ASTNode(aLocation)
			, mVariables(aVariables)
			, mConstants(aConstants)
			, mRules(aRules)
		{
		}

		NamespaceList::NamespaceList(const Location & aLocation, Namespace * aNamespace)
			: ASTNode(aLocation)
		{
			mLocation += aNamespace->GetLocation();
			AddNamespace(aNamespace);
		}

		NamespaceList::~NamespaceList()
		{
			for (auto ns : mNamespaces)
				delete ns;
		}

		void NamespaceList::AddNamespace(Namespace * aNamespace)
		{
			mLocation += aNamespace->GetLocation();
			mNamespaces.push_back(aNamespace);
		}
}
}
