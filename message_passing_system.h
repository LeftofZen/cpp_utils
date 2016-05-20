#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <functional>
#include <typeindex>

// MessageSystem holds n SubMessage systems, one for each type of message it sends
// All creation of the sub-message systems is automatic and handled internally
// SubMessageSystem is only required until using a compiler with std::any (C++17) 
// support. Then the additional abstraction layer(SubMessageSystem) isn't needed.
struct MessageSystem
{
	struct ISubMessageSystem
	{};

	// Implementation of the Observer pattern for a type T
	template <typename T>
	struct SubMessageSystem : public ISubMessageSystem
	{
		void SendMessage(const T& tMsg)
		{
			for (const auto& listener : mListeners)
			{
				listener(tMsg);
			}
		}

		void AddListener(std::function<void(const T&)> funcPtr)
		{
			mListeners.push_back(funcPtr);
		}

		std::vector<std::function<void(const T&)>> mListeners;
	};


	template <typename T>
	void SendMessage(const T& msg)
	{
		// broken into 2 lines for readability
		auto* subMsgSystem = msgSubSystemMap.at(std::type_index(typeid(T))).get();
		static_cast<SubMessageSystem<T>*>(subMsgSystem)->SendMessage(msg);
	}

	// take in std::function
	template <typename T>
	void AddListener(std::function<void(const T&)> funcPtr)
	{
		auto mapKey = std::type_index(typeid(T));

		// if message subsystem doesn't exist, add it on the fly
		if (!RangedFindKey(msgSubSystemMap, mapKey))
		{
			//map.emplace(mapKey, std::make_unique<SubMessageSystem<T>>());
			msgSubSystemMap.emplace(mapKey, std::make_unique<SubMessageSystem<T>>());
		}

		static_cast<SubMessageSystem<T>*>(msgSubSystemMap.at(mapKey).get())->AddListener(funcPtr);
	}

	// take in c-style func ptr
	template <typename T>
	void AddListener(void(*funcPtr)(const T&))
	{
		AddListener(std::function<void(const T&)> { funcPtr });
	}

	std::map<std::type_index, std::unique_ptr<ISubMessageSystem>> msgSubSystemMap;
};


/* Example
 *
struct Trade{};
struct InstrStatus{};
using CustomMsg = std::tuple<int, char, Trade>;

void TradeListener(const Trade& trade)
{
	(void) trade;
	std::cout << "works" << std::endl;
}

void InstrStatusListener(const InstrStatus& trade)
{
	(void)trade;
	std::cout << "works2" << std::endl;
}

void CustomMessageListener(const CustomMsg& trade)
{
	(void)trade;
	std::cout << "works3" << std::endl;
}

int main(int argc, char** argv)
{
	MessageSystem msgSystem;

	// add by func ptr
	msgSystem.AddListener(&TradeListener);

	// add by std:function
	std::function<void(const InstrStatus&)> mTradeFunc2 { &InstrStatusListener };
	msgSystem.AddListener(mTradeFunc2);

	msgSystem.AddListener(&CustomMessageListener);

	// msg structs
	Trade trade;
	InstrStatus instrStatus;

	std::tuple<int, char, Trade> c;

	// sending messages
	msgSystem.SendMessage(trade);
	msgSystem.SendMessage(instrStatus);
	msgSystem.SendMessage(c);

	std::cin.get();
	return 0;
}

*/
