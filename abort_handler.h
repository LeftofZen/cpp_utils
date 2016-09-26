#pragma once

// A singleton class that will call std::abort by default, but also have it's
// internal function call mocked out for testing
class AbortHandler
{
public:
	static void Abort()
	{
		static AbortHandler instance; // Guaranteed to be destroyed.
		instance.InternalAbort();
	}

	// Abiding by the rule of 5
	AbortHandler(const AbortHandler&) = delete;
	AbortHandler& operator=(const AbortHandler&) = delete;
	AbortHandler(AbortHandler&&) = delete;
	AbortHandler& operator=(AbortHandler&&) = delete;

protected:
	// this is virtual since we want to be able to mock it out in testing frameworks
	virtual void InternalAbort() const
	{
		std::abort();
	}

private:
	AbortHandler() = default;
};
