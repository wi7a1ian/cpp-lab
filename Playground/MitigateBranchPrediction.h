#pragma once
#include <memory>
#include <string>

namespace MitigateVirtualFunctions
{
	namespace regular 
	{
		enum class Side { Buy, Sell };

		void CheckRiskLimitations(Side side, float price) {}
		void SendOrder(Side side, float price) {}

		float CalcPrice(Side side, float value, float credit) {
			return side == Side::Buy ? value - credit : value + credit;
		}

		void RunLogic(Side side, float fairValue, float credit) {
			const float orderPrice = CalcPrice(side, fairValue, credit); // hotpoint, branch
			CheckRiskLimitations(side, orderPrice); // hotpoint, branch
			SendOrder(side, orderPrice); // hotpoint, branch
		}
	}

	namespace faster
	{
		//struct Side { struct Buy; struct Sell; };
		enum class Side { Buy = 0, Sell };

		//template<typename T>
		template<Side T>
		float CalcPrice(float value, float credit);

		template<>
		float CalcPrice<Side::Buy>(float value, float credit) { return value - credit; }

		template<>
		float CalcPrice<Side::Sell>(float value, float credit) { return value + credit; }

		template<Side T>
		void RunLogic(float fairValue, float credit) {
			const float orderPrice = CalcPrice<T>(fairValue, credit);
			// ...
		}
	}
}