#pragma once

#include <vector>
#include <map>
#include <assert.h>

#include "Location.h"

namespace LSystem
{
	class LSystem;

	typedef int Token;

	typedef std::vector<Token> Tokens;

	struct ArgumentType
	{
		typedef enum
		{
			Unspecified,
			Int,
			Float,
			Bool,
			Vec2,
			Vec3,
			Vec4,
			MaxArgumentTypes,
		} Type;

        template <typename T> static Type GetType();
	};

	template <> inline ArgumentType::Type ArgumentType::GetType<bool>()			{ return Bool; }
	template <> inline ArgumentType::Type ArgumentType::GetType<int>()			{ return Int; }
	template <> inline ArgumentType::Type ArgumentType::GetType<float>()		{ return Float; }
	template <> inline ArgumentType::Type ArgumentType::GetType<glm::vec2>()	{ return Vec2; }
	template <> inline ArgumentType::Type ArgumentType::GetType<glm::vec3>()	{ return Vec3; }
	template <> inline ArgumentType::Type ArgumentType::GetType<glm::vec4>()	{ return Vec4; }

	typedef std::vector<ArgumentType::Type> ArgumentTypes;

	// A formal argument: source code location + name + type
	struct FormalArgument
	{
		FormalArgument(const Location& aLocation, Token aToken, ArgumentType::Type aType)
			: mLocation(aLocation), mToken(aToken), mType(aType)
		{}

		Location		mLocation;
		Token			mToken;
		ArgumentType::Type	mType;
	};

	typedef std::vector<FormalArgument>	FormalArguments;
	typedef std::map<Token, FormalArgument>	FormalArgumentMap;

	class CodeStorage
	{
	public:
		template <typename T>
		void Push(T aValue)
		{
			const int address = (int) mData.size();
			mData.resize(address + sizeof(T));
			reinterpret_cast<T&>(mData[address]) = aValue;
		}

		template <typename T>
		const T& Get(int address) const
		{
			ASSERT(address <= (int) mData.size() - (int) sizeof(T));
			ASSERT(address >= 0);
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		const T& operator[](int address) const
		{
			ASSERT(address < mData.size() - sizeof(T));
			ASSERT(address >= 0);
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		T& operator[](int address)
		{
			ASSERT(address < mData.size() - sizeof(T));
			ASSERT(address >= 0);
			return reinterpret_cast<const T&>(mData[address]);
		}

		inline bool IsEmpty() const { return mData.empty(); }
		inline int Size() const { return (int) mData.size(); }

	private:
		std::vector<unsigned char> mData;

	};

	class ArgumentValueList
	{
	public:
		template <typename T>
		void Push(T aValue)
		{
			const Entry e((int) mData.size(), ArgumentType::GetType<T>());
			mData.resize(e.mAddress + sizeof(T));
			reinterpret_cast<T&>(mData[e.mAddress]) = aValue;
			mIndex.push_back(e);
		}

		template <typename T>
		T Pop()
		{
			ASSERT(!mIndex.empty());
			ASSERT(mIndex[mIndex.size() - 1].mType == ArgumentType::GetType<T>());
			const int s = (int)(mData.size() - sizeof(T));
			T r = reinterpret_cast<T&>(mData[s]);
			mData.resize(s);
			mIndex.pop_back();
			return r;
		}

		template <typename T>
		const T& Get(int index) const
		{
			ASSERT(index < (int) mIndex.size());
			ASSERT(mIndex[index].mType == ArgumentType::GetType<T>());
			const int address = mIndex[index].mAddress;
			ASSERT(address <= (int) mData.size() - (int) sizeof(T));
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		const T& operator[](int index) const
		{
			ASSERT(index < mIndex.size());
			ASSERT(mIndex[index].mType == ArgumentType::GetType<T>());
			const int address = mIndex[index].mAddress;
			ASSERT(address <= mData.size() - sizeof(T));
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		const T& Top() const
		{
			const int address = mData.size() - sizeof(T);
			ASSERT(address >= 0);
			ASSERT(mIndex[mIndex.size() - 1].mType == ArgumentType::GetType<T>());
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		T& Top()
		{
			const int address = (int)(mData.size() - sizeof(T));
			ASSERT(address >= 0);
			ASSERT(mIndex[mIndex.size() - 1].mType == ArgumentType::GetType<T>());
			return reinterpret_cast<T&>(mData[address]);
		}

		template <typename T>
		const T& Top(int index) const
		{
			ASSERT(index < 0);
			const int address = (int)(mData.size() + index * sizeof(T));
			ASSERT(address >= 0);
			ASSERT(mIndex[mIndex.size() + index].mType == ArgumentType::GetType<T>());
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		T& Top(int index)
		{
			ASSERT(index < 0);
			const int address = (int)(mData.size() + index * sizeof(T));
			ASSERT(address >= 0);
			ASSERT(mIndex[mIndex.size() + index].mType == ArgumentType::GetType<T>());
			return reinterpret_cast<T&>(mData[address]);
		}

		template <typename T>
		T& operator[](int index)
		{
			ASSERT(index < (int) mIndex.size());
			const int address = mIndex[index].mAddress;
			ASSERT(address <= (int) mData.size() - (int) sizeof(T));
			return reinterpret_cast<T&>(mData[address]);
		}

		inline ArgumentType::Type GetType(int index) const
		{
			ASSERT(index < (int) mIndex.size());
			return mIndex[index].mType;
		}

		inline bool IsEmpty() const { return mData.empty(); }
		inline int ElementCount() const { return (int) mIndex.size(); }

	private:
		struct Entry 
		{
			int mAddress;
			ArgumentType::Type mType;
			Entry(int aAddress, ArgumentType::Type aType) : mAddress(aAddress), mType(aType) {}
		};
		std::vector<unsigned char> mData;
		std::vector<Entry> mIndex;
	};

	// A special storage for argument values: stores argument values in a tightly packed storage, 
	// regardless of the types of particular arguments
	class ArgumentValueMap
	{
	public:
		struct ArgumentDesc
		{
			int					mAddress;
			ArgumentType::Type	mType;
		};

		typedef std::map<Token, ArgumentDesc>::const_iterator const_iterator;
		typedef std::map<Token, ArgumentDesc>::iterator iterator;
		
		ArgumentValueMap() : mSize(0) {}

		iterator begin() { return mIndex.begin(); }
		iterator end() { return mIndex.end(); }

		const_iterator begin() const { return mIndex.begin(); }
		const_iterator end() const { return mIndex.end(); }

		template <typename T>
		void Set(const Token aName, const T aValue)
		{
			IndexMap::const_iterator it = mIndex.find(aName);
			assert(it == mIndex.end());

			ArgumentDesc desc;
			desc.mAddress = (int) mData.size();
			desc.mType = ArgumentType::GetType<T>();

			mData.resize(desc.mAddress + sizeof(T));
			T& r = reinterpret_cast<T&>(mData[desc.mAddress]);
			r = aValue;
			++mSize;

			mIndex.insert(std::make_pair(aName, desc));
		}

		template <typename T>
		const T& Top() const
		{
			const int address = mData.size() - sizeof(T);
			ASSERT(address >= 0);
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		T& Top()
		{
			const int address = mData.size() - sizeof(T);
			ASSERT(address >= 0);
			return reinterpret_cast<const T&>(mData[address]);
		}

		template <typename T>
		const T& Get(Token aName) const
		{
			IndexMap::const_iterator it = mIndex.find(aName);
			ASSERT(it != mIndex.end());
			return reinterpret_cast<const T&>(mData[it->second.mAddress]);
		}

		template <typename T>
		const T& Get(const ArgumentDesc& aArgument) const
		{
			ASSERT(aArgument.mAddress < mData.size());
			return reinterpret_cast<const T&>(mData[aArgument.mAddress]);
		}

		ArgumentType::Type GetType(Token aName) const
		{
			IndexMap::const_iterator it = mIndex.find(aName);
			ASSERT(it != mIndex.end());
			return it->second.mType;
		}

		inline int Size() const { return mSize; }

	private:
		typedef std::map<Token, ArgumentDesc> IndexMap;

		IndexMap mIndex;
		std::vector<unsigned char> mData;
		int mSize;
	};
}
