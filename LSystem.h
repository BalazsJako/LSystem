// -*- c++ -*-
#pragma once

#include <map>
#include <string>
#include <vector>

#include "AST.h"
#include "PrecompiledExp.h"

namespace LSystem
{
	namespace AST
	{
		class Grammar;
	}

	class LSystem
	{
	public:
		// One element in the state
		struct StateElement
		{
			Token					mModuleName;		// the token of the name of the module
			ArgumentValueList		mArgumentValues;	// actual argument values for the module

			StateElement() : mModuleName(-1) {}
		};

		typedef std::vector<StateElement>	State;
		typedef std::vector<Token>			Tokens;

		LSystem(const AST::Grammar * aGrammar);
		LSystem(std::istream& aStream);

		~LSystem(void);

		Token GetToken(const std::string & aValue);
		const std::string& GetTokenName(Token aToken) const;

		void Run();
		void Error(const Location &aLocation, const std::string &aMessage);

		void Serialize(std::ostream& a);

	private:
		typedef std::map<std::string, Token> IdentifierTable;
		typedef std::map<Token, Location> TokenToLocation;
		typedef std::vector<PrecompiledExp> ArgumentExprList;

		// Formal definition of a module, with the name and all the arguments with their types 
		struct ModuleDef
		{
		public:
			ModuleDef(Token aName, Token aFQName, const Location& aLocation);
			bool						AddFormalArgument(const FormalArgument& aArgument);
			ArgumentType::Type			GetArgumentType(Token aArgumentName) const;
			const Token&				GetName() const { return mName; }
			const Token&				GetQualifiedName() const { return mQualifiedName; }
			const FormalArgumentMap&	GetFormalArgumentMap() const { return mFormalArgumentMap; }
			const FormalArguments&		GetArguments() const { return mFormalArguments; }

		private:
			Token				mName;
			Token				mQualifiedName;
			Location			mLocation;
			FormalArguments		mFormalArguments;
			FormalArgumentMap	mFormalArgumentMap;
		};

		typedef std::vector<ModuleDef>	ModuleDefs;

		// A module call expression: module name and all the arguent expressions to be evaluated
		struct ModuleExp
		{
			Token mName;
			Location mLocation;
			ArgumentExprList mArgumentExpressions;
		};

		typedef std::vector<ModuleExp> ModuleExpressions;

		// Rewriting rule in the system
		class Rule
		{
		public:
			Rule(const std::string& aLabel, const Location& aLocation, const ModuleDefs& aLeftContext, const ModuleDef& aModule, 
				const ModuleDefs& aRightContext, const PrecompiledExp& aCondition, float aProbability);

			void	AddSuccessor(const ModuleExp& aSuccessor);
			const	ModuleDefs&	GetLeftContext() const { return mLeftContext; }
			const	ModuleDef&	GetModule() const { return mModule; }
			const	ModuleDefs& GetRightContext() const { return mRightContext; }
			const	PrecompiledExp& GetCondition() const { return mCondition; }
			float	GetProbability() const { return mProbability; }

			const ModuleExpressions& GetSuccessors() const { return mSuccessors; }

			const std::string& GetLabel() const { return mLabel; }
			const Location& GetLocation() const { return mLocation; }

			void Serialize(std::ostream& a);

		private:
			std::string	mLabel;			// the name of the rule
			Location mLocation;
			PrecompiledExp mCondition;	// acceptance condition to be met
			ModuleDefs	mLeftContext;	// the left context, if any
			ModuleDef	mModule;		// the module itself
			ModuleDefs	mRightContext;	// the right context, if any

			float		mProbability;

			ModuleExpressions mSuccessors;
		};

		typedef std::vector<Rule>							Rules;
		typedef std::pair<const Rule *, ArgumentValueMap>	MatchingRule;
		typedef std::vector<MatchingRule>					MatchingRules;
		typedef std::map<Token, Rules>						RulesByToken;

		template <typename K, typename R>
		const Rules * FindRules( const K &aKey, const R &aRules )
		{
			typename R::const_iterator it = aRules.find(aKey);
			if (it != aRules.end())
				return &it->second;
			else
				return nullptr;
		}

		LSystem(const LSystem &);

		bool		Parse(const AST::Grammar * aGrammar);
		void		Parse(const AST::Namespace* aNamespace);
		void		Parse(const AST::NamespaceList* aNamespaceList);
		void		Parse(const AST::ImportList* aImportList);
		void		Parse(const AST::RuleList* aRuleList);
		void		Parse(const AST::RuleDefs* aRuleDefs);
		ModuleDef	Parse(const AST::ModuleDef * aModuleDef);
		ModuleDefs	Parse(const AST::ModuleDefList * aModuleDef);
		ModuleExp	Parse(const AST::ModuleExp * aModuleExp, const FormalArgumentMap& aArgs);
		Token		Parse(const AST::QualifiedName* aQualifiedName);
		void		ParseAxiom(const AST::Successor* aSuccessor);
		void		AddFormalArguments(FormalArgumentMap& aTarget, const FormalArgumentMap& aSource);
		void		AddFormalArguments(FormalArgumentMap& aTarget, const ModuleDefs& aModules);

		bool		IsMatching(const StateElement& aElement, const ModuleDef& aModule);
		bool		IsMatchingLeft(const State& aState, int from, const ModuleDefs& aModules);
		bool		IsMatchingRight(const State& aState, int from, const ModuleDefs& aModules);

		void		Apply(State& aTargetState, const Rule* aRule, const ArgumentValueMap& aArgs);
		int			Apply(const RulesByToken& aRules, const State& aSourceState, State& aTargetState);
		void		EvaluateModuleExpression(const ModuleExp& aModuleExpression, const ArgumentValueMap& aArguments, StateElement& aResult) const;
		void		AddArgumentValues(const ModuleDef& aModule, ArgumentValueMap& aMap, const StateElement& aElement);
		void		AddArgumentValuesLeft(const State& aState, const ModuleDefs& aModules, ArgumentValueMap& aMap, int from);
		void		AddArgumentValuesRight(const State& aState, const ModuleDefs& aModules, ArgumentValueMap& aMap, int from);
		std::string GetFullNamespacePath() const;
		std::string GetAbsoluteName(const AST::QualifiedName* aQualifiedName) const;

		bool		Iterate();
		void		DumpState(const State& aState);

		void DumpArgumentList(const ArgumentValueList & aArgs);

		void Deserialize(std::istream& a);

		IdentifierTable	mIdentifiers;
		std::vector<std::string> mTokenToString;

		RulesByToken mRules;
		RulesByToken mHomomorphisms;
		RulesByToken mDecompositions;
		MatchingRules mMatchingRules;

		Tokens mCurrentNamespace;
		TokenToLocation mLabels;
		TokenToLocation mVars;
		TokenToLocation mConsts;

		State mCurrentState;
		State mNextState;
		State mHomomorphState;

		int mParseErrors;
		int mMaxIterations;
		int mCurrentIteration;
		int mLeftSqBracket;
		int mRightSqBracket;
		int mTotalCodeBytes;
	};
}
