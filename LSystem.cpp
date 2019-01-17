
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <assert.h>
#include <time.h>

#include "LSystem.h"
#include "AST.h"
#include "OBJExporter.h"
#include "Serialize.h"

using namespace std;

#ifdef _DEBUG
#define DIAG(x) x
#else
#define DIAG(x)
#endif

namespace LSystem
{
	LSystem::LSystem(const AST::Grammar * aGrammar)
		: mParseErrors(0)
		, mMaxIterations(1000)
		, mCurrentIteration(0)
		, mLeftSqBracket(GetToken("["))
		, mRightSqBracket(GetToken("]"))
		, mTotalCodeBytes(0)
	{
		if (Parse(aGrammar))
		{
			DIAG(cerr << "Rules has been built successfully: " << mRules.size() << " rules total" << endl);
		}
	}

	LSystem::LSystem(std::istream & aStream)
	{
		Deserialize(aStream);
	}

	LSystem::~LSystem(void)
	{
	}

	void LSystem::Serialize(std::ostream & a)
	{
		Serialize::Serialize(a, mIdentifiers);
		Serialize::Serialize(a, mTokenToString);
	}

	void LSystem::Deserialize(std::istream & a)
	{
		int ids = -1;
		a >> ids;
		mIdentifiers.clear();
		for (auto i = 0; i < ids; i++)
		{
			std::string s;
			int id = -1;
			a >> s;
			a >> id;
			IdentifierTable::value_type e(s, id);
			mIdentifiers.insert(e);
		}

		a >> ids;
		mTokenToString.clear();
		for (auto i = 0; i < ids; i++)
		{
			std::string s;
			a >> s;
			mTokenToString.push_back(s);
		}
	}

	Token LSystem::GetToken(const std::string & aValue)
	{
		if (aValue.length() == 0)
			return -1;

		auto it = mIdentifiers.find(aValue);
		if (it == mIdentifiers.end())
		{
			const Token newId = (int)mIdentifiers.size();
			mIdentifiers[aValue] = newId;
			mTokenToString.push_back(aValue);
			return newId;
		}
		else
			return it->second;
	}

	const std::string& LSystem::GetTokenName(Token aToken) const
	{
		ASSERT(aToken < (int)mTokenToString.size());
		return mTokenToString[aToken];
	}

	bool LSystem::Parse(const AST::Grammar * aGrammar)
	{
		mParseErrors = 0;

		Parse(aGrammar->GetImports());
		Parse(aGrammar->GetNamespaceList());
		ParseAxiom(aGrammar->GetAxiom());

		return mParseErrors == 0;
	}

	void LSystem::Parse(const AST::Namespace * aNamespace)
	{
		if (aNamespace == nullptr)
			return;

		auto t = GetToken(aNamespace->GetName()->GetValue());

		mCurrentNamespace.push_back(t);

		auto nslist = aNamespace->GetNamespaces();
		if (nslist != nullptr)
			for (auto n : nslist->GetNamespaces())
				Parse(n);

		Parse(aNamespace->GetRuleDefs());
		
		mCurrentNamespace.pop_back();
	}

	void LSystem::Parse(const AST::NamespaceList * aNamespaceList)
	{
		if (aNamespaceList == nullptr)
			return;

		for (auto ns : aNamespaceList->GetNamespaces())
			Parse(ns);
	}

	void LSystem::Parse(const AST::ImportList * aImportList)
	{
		// TODO....
	}

	void LSystem::Parse(const AST::RuleList* aRuleList)
	{
		for (auto rule : aRuleList->GetRules())
		{
			const AST::Predecessor*	pred = rule->GetPredecessor();
			const AST::Successor*	succ = rule->GetSuccessor();
			const auto kind = rule->GetKind();

			ModuleDefs leftContext	= Parse(pred->GetLeftContext());
			ModuleDef  varModule	= Parse(pred->GetVariable());
			ModuleDefs rightContext	= Parse(pred->GetRightContext());

			if (mVars.find(varModule.GetQualifiedName()) == mVars.end())
				Error(pred->GetVariable()->GetLocation(), "Module is not defined as a variable: " + GetTokenName(varModule.GetQualifiedName()));

			FormalArgumentMap formalArguments;
			AddFormalArguments(formalArguments, leftContext);
			AddFormalArguments(formalArguments, varModule.GetFormalArgumentMap());
			AddFormalArguments(formalArguments, rightContext);

			PrecompiledExp condition(*this);
			if (rule->GetCondition() != nullptr)
			{
				condition.Compile(rule->GetCondition(), formalArguments);
				//DIAG(condition.DebugDump());

				if (condition.GetResultType() != ArgumentType::Bool)
					Error(rule->GetCondition()->GetLocation(), "expression does not evaluate to boolean type");
				mTotalCodeBytes += condition.GetCodeSize();
			}

			const string label = GetFullNamespacePath() + rule->GetLabel()->GetValue();
			const Token labelToken = GetToken(label);
			const Location& loc = rule->GetLabel()->GetLocation();
			if (mLabels.insert(std::pair<Token, Location>(labelToken, loc)).second == false)
			{
				const Location& prevLoc = mLabels[labelToken];
				Error(loc, "label already defined: '" + label + "' at " + prevLoc.ToShortString());
			}

			Rule newRule(label, rule->GetLocation(), leftContext, varModule, rightContext, condition, rule->GetProbability());

			for (auto module : succ->GetSuccessors())
			{
				const auto fqn = module->GetName();
				const Token nameToken = GetToken(GetAbsoluteName(fqn));

				if (nameToken >= 2 &&
					(mVars.find(nameToken) == mVars.end() && mConsts.find(nameToken) == mConsts.end()))
					Error(module->GetName()->GetLocation(), "'" + module->GetName()->GetFullValue()
						    + "': successor module name is neither defined as a variable nor as a constant");
				else
				{
					newRule.AddSuccessor(Parse(module, formalArguments));
				}
			}

			auto t = newRule.GetModule().GetQualifiedName();

			switch (kind)
			{
			case AST::Replace:		mRules[t].push_back(newRule); break;
			case AST::Decompose:	mDecompositions[t].push_back(newRule); break;
			case AST::Transform:	mHomomorphisms[t].push_back(newRule); break;
			default: assert(!"Internal error. Unknown rule kind.");
			}
		}
	}

	void LSystem::Parse(const AST::RuleDefs * aRuleDefs)
	{
		if (aRuleDefs == nullptr)
			return;

		// variables
		const AST::NameList * vnames = aRuleDefs->GetVariables();
		if (vnames != nullptr)
		{
			for (auto name : vnames->GetNames())
			{
				const auto s = GetFullNamespacePath() + name->GetValue();
				Token t = GetToken(s);
				auto it = mVars.find(t);
				if (it != mVars.end())
					Error(name->GetLocation(), name->GetValue() + ": variable is already defined at " + it->second.ToShortString());
				else
					mVars[t] = name->GetLocation();
			}
		}

		// constants
		const AST::NameList * cnames = aRuleDefs->GetConstants();
		if (cnames != nullptr)
		{
			for (auto name : cnames->GetNames())
			{
				Token t = GetToken(name->GetValue());
				auto it = mVars.find(t);
				if (it != mVars.end())
					Error(name->GetLocation(), name->GetValue() + ": symbol is already defined as a variable at " + it->second.ToShortString());
				else
					mConsts[GetToken(name->GetValue())] = name->GetLocation();
			}
		}

		const AST::RuleList* rules = aRuleDefs->GetRules();

		if (rules == nullptr)
			Error(aRuleDefs->GetLocation(), "no rules defined");
		else
			Parse(rules);
	}

	void LSystem::AddFormalArguments(FormalArgumentMap& aTarget, const ModuleDefs& aModules)
	{
		for (const auto& m : aModules)
			AddFormalArguments(aTarget, m.GetFormalArgumentMap());
	}

	void LSystem::AddFormalArguments(FormalArgumentMap& aTarget, const FormalArgumentMap& aSource)
	{
		for (auto s : aSource)
		{
			const FormalArgument& arg = s.second;

			if (aTarget.insert(s).second == false)
				Error(arg.mLocation, "parameter identifier '" + GetTokenName(arg.mToken) + "': is already specified");
		}
	}

	void LSystem::ParseAxiom(const AST::Successor* aSuccessor)
	{
		ASSERT(aSuccessor != nullptr);
		aSuccessor->GetSuccessors();

		FormalArgumentMap empty;

		for (auto x : aSuccessor->GetSuccessors())
		{
			ModuleExp mx = Parse(x, empty);

			StateElement element;
			static const ArgumentValueMap vempty;
			EvaluateModuleExpression(mx, vempty, element);
			mCurrentState.push_back(element);
		}
	}

	LSystem::ModuleExp LSystem::Parse(const AST::ModuleExp * aModuleExp, const FormalArgumentMap& aArgs)
	{
		ASSERT(aModuleExp != nullptr);
		ASSERT(aModuleExp->GetName() != nullptr);
		ModuleExp result;

		result.mLocation = aModuleExp->GetLocation();
		result.mName = Parse(aModuleExp->GetName());

		const AST::Expression* parameterList = aModuleExp->GetParameters();

		if (parameterList != nullptr)
		{
			ASSERT(parameterList->GetOpCode() == AST::Expression::ExpressionList);
			const AST::Expressions& parameters = parameterList->GetOperands();

			for (auto e : parameters)
			{
				PrecompiledExp x(*this);
				x.Compile(e, aArgs);
				//DIAG(x.DebugDump());
				result.mArgumentExpressions.push_back(x);
				mTotalCodeBytes += x.GetCodeSize();
			}
		}
		return result;
	}

	Token LSystem::Parse(const AST::QualifiedName * aQualifiedName)
	{
		if (aQualifiedName->IsAbsolute())
		{
			return GetToken(aQualifiedName->GetFullValue());
		}
		else
		{
			std::string fqn;
			for (auto n : mCurrentNamespace)
				fqn += GetTokenName(n) + '.';
			fqn += aQualifiedName->GetNamePart();
			return GetToken(fqn);
		}
	}

	LSystem::ModuleDef::ModuleDef(Token aName, Token aFQName, const Location& aLocation)
		: mName(aName)
		, mQualifiedName(aFQName)
		, mLocation(aLocation)
	{
	}

	bool LSystem::ModuleDef::AddFormalArgument(const FormalArgument& aArgument)
	{
		const bool retval = mFormalArgumentMap.insert(std::make_pair(aArgument.mToken, aArgument)).second;

		if (retval)
			mFormalArguments.push_back(aArgument);

		return retval;
	}

	LSystem::Rule::Rule(const std::string& aLabel, const Location& aLocation, const ModuleDefs& aLeftContext, const ModuleDef& aModule, 
		const ModuleDefs& aRightContext, const PrecompiledExp& aCondition, float aProbability)
		: mLabel(aLabel)
		, mLocation(aLocation)
		, mCondition(aCondition)
		, mLeftContext(aLeftContext)
		, mModule(aModule)
		, mRightContext(aRightContext)
		, mProbability(aProbability)
	{
	}

	void LSystem::Rule::AddSuccessor(const LSystem::ModuleExp& aSuccessor)
	{
		mSuccessors.push_back(aSuccessor);
	}

	ArgumentType::Type LSystem::ModuleDef::GetArgumentType(Token aArgumentName) const
	{
		auto it = mFormalArgumentMap.find(aArgumentName);
		ASSERT(it != mFormalArgumentMap.end());
		return it->second.mType;
	}

	LSystem::ModuleDefs LSystem::Parse(const AST::ModuleDefList * aModuleDef)
	{
		ModuleDefs r;

		if (aModuleDef == nullptr)
			return r;

		for (const auto& m : aModuleDef->GetModuleDefs())
			r.push_back(Parse(m));

		return r;
	}

	LSystem::ModuleDef LSystem::Parse(const AST::ModuleDef* aModuleDef)
	{
		if (aModuleDef != nullptr)
		{
			ModuleDef result(
				GetToken(aModuleDef->GetName()->GetNamePart()),
				Parse(aModuleDef->GetName()), 
				aModuleDef->GetLocation());

			const AST::ArgumentSpecList* list = aModuleDef->GetArguments();
			if (list != nullptr)
			{
				for (auto arg : list->GetArgumentSpecs())
				{
					const Token n = GetToken(arg->GetName()->GetValue());
					const ArgumentType::Type t = arg->GetType();

					FormalArgument fa(arg->GetLocation(), n, t);

					if (!result.AddFormalArgument(fa))
						Error(fa.mLocation, "parameter identifier '" + GetTokenName(n) + "' is already specified");
				}
			}
			return result;
		}

		return ModuleDef(-1, -1, Location());
	}

	bool LSystem::Iterate()
	{
		++mCurrentIteration;

		DIAG(cerr << "Iteration: " << mCurrentIteration << endl);

		int rulesApplied = 0;
		int decompositions = 0;
		int homomorphisms = 0;

		DIAG(cerr << "Applying decomposition rule(s): " << endl);
		int d = 0;
		mNextState.clear();
		while (0 < (d = Apply(mDecompositions, mCurrentState, mNextState))) 
		{
			decompositions += d;
			std::swap(mCurrentState, mNextState);
			mNextState.clear();
			DIAG(cerr << '.');
		}
		DIAG(cerr << endl);
	
		DIAG(cerr << "Applying homomorphism rule(s): ");
		int h = 0;
		mHomomorphState.clear();
		if (0 < (h = Apply(mHomomorphisms, mCurrentState, mHomomorphState)))
		{
			State tmp;
			homomorphisms += h;
			std::swap(tmp, mHomomorphState);
			while (0 < (h = Apply(mHomomorphisms, tmp, mHomomorphState))) 
			{
				homomorphisms += h;
				std::swap(tmp, mHomomorphState);
				mHomomorphState.clear();
			}
		}
		DIAG(cerr << endl);

		DIAG(cerr << "Applying rewriting rule(s): ");
		mNextState.clear();
		rulesApplied += Apply(mRules, mCurrentState, mNextState);
		std::swap(mCurrentState, mNextState);
		DIAG(cerr << endl);

		cerr << "Applied rules: " << rulesApplied << ", decompositions: " << decompositions << ", homomorphisms: " << homomorphisms << endl;

		return rulesApplied > 0;
	}

	int LSystem::Apply(const RulesByToken& aRules, const State& aSourceState,  State& aTargetState)
	{
		int rulesApplied = 0;

		for (unsigned i = 0; i < (unsigned) aSourceState.size(); ++i)
		{
			const StateElement& curr = aSourceState[i];
			const Token v = curr.mModuleName;
			const auto it = aRules.find(v);

			const Rules* rules = (it == aRules.end() ? nullptr : &it->second);
			if (rules == nullptr || rules->empty())
			{
				// no rule has been found: do the default (indentity) action
				aTargetState.push_back(curr);
			}
			else
			{
				mMatchingRules.clear();
				float probTotal = 0.0;

				for (const auto& r: *rules)
				{
					const Rule* rule = &r;
					const ModuleDefs& lc	= rule->GetLeftContext();
					const ModuleDef&  mc	= rule->GetModule();
					const ModuleDefs& rc	= rule->GetRightContext();

					if (IsMatchingLeft(aSourceState, i - 1, lc) && IsMatching(curr, mc) && IsMatchingRight(aSourceState, i + 1, rc))
					{
						MatchingRule matchingRule;
						matchingRule.first = rule;
						ArgumentValueMap& avals = matchingRule.second;

						AddArgumentValuesLeft(aSourceState, lc, matchingRule.second, i - 1);
						AddArgumentValues(mc, matchingRule.second, curr);
						AddArgumentValuesRight(aSourceState, rc, matchingRule.second, i + 1);

						const PrecompiledExp& condition = rule->GetCondition();
						DIAG(cerr << "\nTesing: " << rule->GetLabel() << " (" << condition.GetCodeSize() << "): ");
						bool result = true;
						if (!condition.IsEmpty())
							result = condition.Evaluate<bool>(avals);
						if (result)
						{
							DIAG(cerr << "Condition: " << condition.IsEmpty() << ", " << (int) result << "; ");
							mMatchingRules.resize(mMatchingRules.size() + 1);
							MatchingRule& tmp = mMatchingRules.back();
							std::swap(tmp, matchingRule);

							if (rule->GetProbability() != -1.0)
								probTotal += rule->GetProbability();
						}
					}
				}

				if (mMatchingRules.empty())
				{
					aTargetState.push_back(curr);
				}
				else
				{
					if (probTotal == 0.0)
					{
						for (auto it : mMatchingRules)
						{
							const Rule* rule = it.first;
							const ArgumentValueMap& args = it.second;

							Apply(aTargetState, rule, args);
							++rulesApplied;
						}
					}
					else
					{
						const float threshold = probTotal * (float) rand() / (float) RAND_MAX;
						float accum = 0.0;

						for (auto it : mMatchingRules)
						{
							const Rule* rule = it.first;
							const ArgumentValueMap& args = it.second;
							const float p = rule->GetProbability();

							accum += p;
							if (threshold <= accum)
							{
								Apply(aTargetState, rule, args);
								++rulesApplied;
								break;
							}
						}
					}
				}
			}
		}

		return rulesApplied;
	}

	bool LSystem::IsMatchingLeft(const State& aState, int from, const ModuleDefs& aModules)
	{
		for (auto it = aModules.rbegin(); it != aModules.rend(); ++it, --from)
			if (!IsMatching(aState[from % aState.size()], *it))
				return false;
		return true;
	}

	bool LSystem::IsMatchingRight(const State& aState, int from, const ModuleDefs& aModules)
	{
		for (auto it = aModules.begin(); it != aModules.end(); ++it, ++from)
			if (!IsMatching(aState[from % aState.size()], *it))
				return false;
		return true;
	}

	bool LSystem::IsMatching(const StateElement& aElement, const ModuleDef& aModule)
	{
		const ArgumentValueList& vargs = aElement.mArgumentValues;

		if (aElement.mModuleName != aModule.GetQualifiedName())
			return false;

		if (vargs.ElementCount() != (int) aModule.GetArguments().size())
			return false;

		const FormalArguments& fargs = aModule.GetArguments();

		int i = 0;
		for (auto it = fargs.begin(); it != fargs.end(); ++it, ++i)
		{
			const FormalArgument& farg = *it;
			const ArgumentType::Type atype = vargs.GetType(i);
			if (farg.mType != atype)
				return false;
		}

		return true;
	}

	void LSystem::Apply(State& aTargetState, const Rule* aRule, const ArgumentValueMap& aArgs)
	{
		ASSERT(aRule != nullptr);
		DIAG(cerr << aRule->GetLocation().GetFileName() << "(" << aRule->GetLocation().GetFirstLine() << ") " 
			<< aRule->GetLabel() << ": " << GetTokenName(aRule->GetModule().GetQualifiedName()) << " ==>\n");

		const ModuleExpressions& succ = aRule->GetSuccessors();
		for (const auto& moduleExpr : aRule->GetSuccessors())
		{
			aTargetState.resize(aTargetState.size() + 1);
			StateElement& result = aTargetState.back();
			ASSERT(result.mModuleName == -1);
			EvaluateModuleExpression(moduleExpr, aArgs, result);
			DIAG(cerr << "\t\t" << GetTokenName(moduleExpr.mName));
			DIAG(DumpArgumentList(result.mArgumentValues));
			DIAG(cerr << endl);
		}

		DIAG(cerr << endl);
	}

	void LSystem::EvaluateModuleExpression(const ModuleExp& aModuleExpression, const ArgumentValueMap& aArguments, StateElement& aResult) const
	{
		ASSERT(aModuleExpression.mName >= 0);

		Token name = aModuleExpression.mName;

		aResult.mModuleName = name;

		const ArgumentExprList& ax = aModuleExpression.mArgumentExpressions;

		for (const auto& expression : aModuleExpression.mArgumentExpressions)
		{
			const ArgumentType::Type type = expression.GetResultType();			
			ArgumentValueList& rargs = aResult.mArgumentValues;

			switch (type)
			{
			case ArgumentType::Int:		rargs.Push(expression.Evaluate<int>(aArguments));			break;
			case ArgumentType::Float:	rargs.Push(expression.Evaluate<float>(aArguments));			break;
			case ArgumentType::Bool:	rargs.Push(expression.Evaluate<bool>(aArguments));			break;
			case ArgumentType::Vec2:	rargs.Push(expression.Evaluate<glm::vec2>(aArguments));	break;
			case ArgumentType::Vec3:	rargs.Push(expression.Evaluate<glm::vec3>(aArguments));	break;
			case ArgumentType::Vec4:	rargs.Push(expression.Evaluate<glm::vec4>(aArguments));	break;
			default: ASSERT(!"Invalid argument type");	break;
			}
		}
	}

	void LSystem::AddArgumentValuesLeft(const State& aState, const ModuleDefs& aModules, ArgumentValueMap& aMap, int from)
	{
		for (auto it = aModules.rbegin(); it != aModules.rend(); ++it, --from)
			AddArgumentValues(*it, aMap, aState[from % aState.size()]);
	}

	void LSystem::AddArgumentValuesRight(const State& aState, const ModuleDefs& aModules, ArgumentValueMap& aMap, int from)
	{
		for (auto it = aModules.begin(); it != aModules.end(); ++it, ++from)
			AddArgumentValues(*it, aMap, aState[from % aState.size()]);
	}

	std::string LSystem::GetFullNamespacePath() const
	{
		std::string s;
		for (auto n : mCurrentNamespace)
			s += GetTokenName(n) + '.';
		return s;
	}

	std::string LSystem::GetAbsoluteName(const AST::QualifiedName * aQualifiedName) const
	{
		if (aQualifiedName->IsAbsolute())
			return aQualifiedName->GetFullValue();
		else
			return GetFullNamespacePath() + aQualifiedName->GetNamePart();
	}

	void LSystem::AddArgumentValues(const ModuleDef& aModule, ArgumentValueMap& aMap, const StateElement& aElement)
	{
		if (aModule.GetName() == -1)
			return;

		const FormalArguments& fargs = aModule.GetArguments();

		ASSERT(fargs.size() == aElement.mArgumentValues.ElementCount());

		int index = 0;
		for (auto it = fargs.begin(); it != fargs.end(); ++it, ++index)
		{
			const FormalArgument& farg = *it;
			const Token argumentName = farg.mToken;
			const ArgumentValueList& avals = aElement.mArgumentValues;
			const ArgumentType::Type type = avals.GetType(index);

			switch (type)
			{
			case ArgumentType::Bool:	aMap.Set(argumentName, avals.Get<bool>(index));			break;
			case ArgumentType::Float:	aMap.Set(argumentName, avals.Get<float>(index));		break;
			case ArgumentType::Int:		aMap.Set(argumentName, avals.Get<int>(index));			break;
			case ArgumentType::Vec2:	aMap.Set(argumentName, avals.Get<glm::vec2>(index));	break;
			case ArgumentType::Vec3:	aMap.Set(argumentName, avals.Get<glm::vec3>(index));	break;
			case ArgumentType::Vec4:	aMap.Set(argumentName, avals.Get<glm::vec4>(index));	break;
			default:					ASSERT(!"Invalid argument type.");	break;
			}
		}
	}

	void LSystem::DumpState(const LSystem::State& aState)
	{
		DIAG(cerr << "==== State: ====\n");
		for (int i = 0; i < (int) aState.size(); ++ i)
		{
			const StateElement& state = aState[i];

			if (state.mModuleName == mRightSqBracket)
				cerr << "] ";
			else if (state.mModuleName == mLeftSqBracket)
				cerr << "[ ";
			else
			{
				const ArgumentValueList& args = state.mArgumentValues;
				cerr << mTokenToString[state.mModuleName];
				DumpArgumentList(args);
				cerr << "\n";
			}
		}
		cerr << "." << endl;
	}

	void LSystem::DumpArgumentList(const ArgumentValueList& aArgs)
	{
		if (!aArgs.IsEmpty())
		{
			cerr << "(";

			for (int i = 0; i < aArgs.ElementCount(); ++i)
			{
				if (i > 0)
					cerr << ", ";

				switch (aArgs.GetType(i))
				{
				case ArgumentType::Int:		cerr << aArgs.Get<int>(i);			break;
				case ArgumentType::Float:	cerr << aArgs.Get<float>(i);		break;
				case ArgumentType::Bool:	cerr << aArgs.Get<bool>(i);		break;
				case ArgumentType::Vec2:
				{
					const glm::vec2& v2 = aArgs.Get<glm::vec2>(i);
					cerr << "vec2(" << v2[0] << ", " << v2[1] << ")";
					break;
				}
				case ArgumentType::Vec3:
				{
					const glm::vec3& v3 = aArgs.Get<glm::vec3>(i);
					cerr << "vec3(" << v3[0] << ", " << v3[1] << ", " << v3[2] << ")";
					break;
				}
				case ArgumentType::Vec4:
				{
					const glm::vec4& v4 = aArgs.Get<glm::vec4>(i);
					cerr << "vec4(" << v4[0] << ", " << v4[1] << ", " << v4[2] << ", " << v4[3] << ")";
					break;
				}
				default: ASSERT(!"Invalid argument type");
				}
			}

			cerr << ")";
		}
	}

	void LSystem::Error( const Location &aLocation, const std::string &aMessage )
	{
		cerr << aLocation.ToString() << ": " << aMessage << endl;
		++mParseErrors;
	}

	void LSystem::Run()
	{
		if (mParseErrors > 0)
			return;

		srand((unsigned int) time(nullptr));

		DIAG(DumpState(mCurrentState));
		DIAG(cerr << "Total code: " << mTotalCodeBytes << " bytes." << endl);
		cerr << "Total code: " << mTotalCodeBytes << " bytes." << endl;
		for (mCurrentIteration = 0; mCurrentIteration < mMaxIterations; )
		{
			DIAG(cerr << "Iteration: " << mCurrentIteration << " Modules: " << mCurrentState.size() << "." << endl);
			if (!Iterate())
			{
				DIAG(cerr << "No applicable rules found. Terminating." << endl);
				break;
			}
//			DIAG(DumpState(mCurrentState));

//			if (mCurrentIteration % 10 == 0)
				cerr << "...iteration: " << mCurrentIteration << " Modules: " << mCurrentState.size() << "." << endl;
		}

		cerr << endl;
		cerr << "Total iterations: " << mCurrentIteration << " Modules: " << mCurrentState.size() << "." << endl;

		//DumpState(mCurrentState);

		OBJExporter x;
		x.Export(mHomomorphState, GetToken("Test.QuadT"), "test.obj");
	}
}
