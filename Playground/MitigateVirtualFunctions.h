#pragma once
#include <memory>
#include <string>

namespace MitigateVirtualFunctions
{
	namespace regular 
	{
		struct IOrderManager
		{
			virtual ~IOrderManager() = default;
			virtual void MainLoop() = 0;
		};

		struct IOrderSender
		{
			virtual ~IOrderSender() = default;
			virtual void SendOrder() = 0;
		};

		struct OrderSenderA : public IOrderSender {
			void SendOrder() override {}
		};

		struct OrderSenderB : public IOrderSender {
			void SendOrder() override {}
		};

		struct OrderManager : public IOrderManager
		{
			OrderManager(IOrderSender& sender) : mOrderSender(sender) {};

			void MainLoop() final {
				while (true)
				{
					mOrderSender.SendOrder(); // hotpath, devirtualization needed
				}
			}

			IOrderSender& mOrderSender;
		};
	}

	namespace faster
	{
		struct IOrderManager
		{
			virtual ~IOrderManager() = default;
			virtual void MainLoop() = 0;
		};

		struct OrderSenderA {
			void SendOrder() {}
		};

		struct OrderSenderB {
			void SendOrder() {}
		};

		template<typename T>
		struct OrderManager : public IOrderManager
		{
			void MainLoop() final {
				mOrderSender.SendOrder(); // hotpath, probably inlined
			}
			T mOrderSender;
		};

		std::unique_ptr<IOrderManager> Factory(const char* config)
		{
			if (std::string("ChooseA") == config)
			{
				return std::make_unique<OrderManager<OrderSenderA>>();
			} 
			else if (std::string("ChooseB") == config)
			{
				return std::make_unique<OrderManager<OrderSenderB>>();
			}
			else
			{
				throw;
			}
		}
	}
}