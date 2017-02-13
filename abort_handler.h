#pragma once

#include <cstdlib>

namespace AbortHandler
{
	struct IAborter
	{
		virtual void Abort() const = 0;
	};


	struct Aborter final : public IAborter
	{
		void Abort() const override
		{
			//std::cout << "abort" << std::endl;
			//std::abort();
		}
	};

	struct MockAborter final : public IAborter
	{
		void Abort() const override
		{
			//std::cout << "mock_abort" << std::endl;
			//std::abort();
		}
	};

	namespace Handlers
	{
		constexpr static const Aborter DefaultAbortHandler;
		constexpr static const MockAborter DefaultMockAbortHandler;
	}

	namespace
	{
		static const IAborter* currentAbortHandler = &Handlers::DefaultAbortHandler;
	}

	inline static void SetAbortHandler(const IAborter& abortHandler)
	{
		currentAbortHandler = &abortHandler;
	}

	inline static void Abort(const IAborter& abortHandler = *currentAbortHandler)
	{
		abortHandler.Abort();
	}
}
