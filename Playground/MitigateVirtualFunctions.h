#pragma once
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
					mOrderSender.SendOrder(); // crucial point, devirtualization needed
				}
			}

			IOrderSender& mOrderSender;
		};
	}

	namespace faster
	{
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
				mOrderSender.SendOrder(); // crucial point, probably inlined
			}
			T mOrderSender;
		};
	}
}