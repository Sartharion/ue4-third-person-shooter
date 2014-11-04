
#pragma once

template <class TClass>
class TMethodDelegate
{
public:
	/** TMethodDelegate with no parameters */
	template <typename TRetValue>
	class NoParams
	{
	public:
		typedef TRetValue(TClass::*TMethodPtr)();
		typedef TRetValue(TClass::*TMethodPtr_Const)() const;

	private:
		inline NoParams() {}
	};

	/** TMethodDelegate with one parameter */
	template <typename TRetValue, typename TParam0>
	class OneParam
	{
	public:
		typedef TRetValue(TClass::*TMethodPtr)(TParam0);
		typedef TRetValue(TClass::*TMethodPtr_Const)(TParam0) const;

	private:
		inline OneParam() {};
	};

	/** TMethodDelegate with two parameters */
	template <typename TRetValue, typename TParam0, typename TParam1>
	class TwoParams
	{
	public:
		typedef TRetValue(TClass::*TMethodPtr)(TParam0, TParam1);
		typedef TRetValue(TClass::*TMethodPtr_Const)(TParam0, TParam1) const;

	private:
		inline TwoParams() {};
	};

private:
	inline TMethodDelegate() {}
};
