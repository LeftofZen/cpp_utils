#include <array>
#include <cstddef>
#include <stack>

template <typename T, size_t SIZE>
class CarPark
{

public:
	CarPark()
	{
		// Setup initial free spaces
		for (size_t i = SIZE; i > 0; --i)
		{
			mFreeSpaces.push(i - 1);
		}
	}

	const size_t& Park(const T* const t)
	{
		size_t ticket = mFreeSpaces.top();
		mSpaces.at(ticket) = t;
		mFreeSpaces.pop();
		return ticket;
	}

	const T* const DePark(size_t ticket)
	{
		const T* const result = mSpaces.at(ticket);
		mSpaces.at(ticket) = nullptr;
		mFreeSpaces.push(ticket);
		return result;
	}

private:
	std::array<const T*, SIZE> mSpaces { { nullptr } };
	std::stack<size_t> mFreeSpaces;
};
