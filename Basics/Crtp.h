#pragma once
namespace CRPTest
{
	// https://www.fluentcpp.com/2017/05/16/what-the-crtp-brings-to-code/
	// https://eli.thegreenplace.net/2013/12/05/the-cost-of-dynamic-virtual-calls-vs-static-crtp-dispatch-in-c

	namespace ForAddingFunctionalityWhereDerivedShouldOnlyBeUsed
	{
		template <typename T>
		struct NumericalFunctions
		{
			void scale(double multiplicator)
			{
				T& underlying = static_cast<T&>(*this);
				underlying.setValue(underlying.getValue() * multiplicator);
			}
			void square()
			{
				T& underlying = static_cast<T&>(*this);
				underlying.setValue(underlying.getValue() * underlying.getValue());
			}
			void setToOpposite()
			{
				scale(-1);
			};
		};

		class Sensitivity : public NumericalFunctions<Sensitivity>
		{
		public:
			double getValue() const
			{
				return m_member;
			}
			void setValue(double value) 
			{
				m_member = value;
			}

			// rest of the sensitivity's rich interface...

		private:
			double m_member{42};
		};
	}

	namespace ForStaticInterfacesWherebaseCanBeUsed
	{
		// Interface:
		template <typename T>
		class Amount
		{
		public:
			double getValue() const
			{
				return static_cast<T const&>(*this).getValue();
			}
		};

		// Implementations:
		class Constant42 : public Amount<Constant42>
		{
		public:
			double getValue() const { return 42; }
		};

		class Variable : public Amount<Variable>
		{
		public:
			explicit Variable(int value) : value_(value) {}
			double getValue() const { return value_; }
		private:
			int value_;
		};

		// Consumer
		template<typename T>
		void printUsingInterface(Amount<T> const& amount)
		{
			std::cout << amount.getValue() << '\n';
		}

		template<typename T>
		void printUsingImplementation(T const& amount)
		{
			std::cout << amount.getValue() << '\n';
		}
	
		void Test();
	}

	namespace BaseForCRTP
	{
		// https://www.fluentcpp.com/2017/05/19/crtp-helper/

		/*template <typename T>
		struct crtp
		{
			T& underlying() { return static_cast<T&>(*this); }
			T const& underlying() const { return static_cast<T const&>(*this); }
		};*/

		template <typename T, template<typename> class crtpType>
		class crtp
		{
		protected:
			T& impl() { return static_cast<T&>(*this); }
			T const& impl() const { return static_cast<T const&>(*this); }
		private:
			crtp() {}
			friend crtpType<T>;
		};

		// 2 Functionalities
		template <typename T>
		struct Scale : crtp<T, Scale>
		{
			void scale(double multiplicator)
			{
				this->impl().setValue(this->impl().getValue() * multiplicator);
			}
		};

		template <typename T>
		struct Square : crtp<T, Square>
		{
			void square()
			{
				this->impl().setValue(this->impl().getValue() * this->impl().getValue());
			}
		};

		// Implementation using two functionalities
		class Sensitivity : public Scale<Sensitivity>, public Square<Sensitivity>
		{
		public:
			double getValue() const { return value_; }
			void setValue(double value) { value_ = value; }

		private:
			double value_;
		};

		void Test();
	}

	namespace VariadicCRTP
	{

		template<typename Derived>
		class ExtraFeature1
		{
		public:
			void extraMethod1()
			{
				auto derived = static_cast<Derived&>(*this);
				derived.basicMethod();
				derived.basicMethod();
				derived.basicMethod();
			}
		};

		template<typename Derived>
		class ExtraFeature2
		{
		public:
			void extraMethod2()
			{
				auto derived = static_cast<Derived&>(*this);
				// does something else with derived.basicMethod() ...
			}
		};

		template<typename Derived>
		class ExtraFeature3
		{
		public:
			void extraMethod3()
			{
				auto derived = static_cast<Derived&>(*this);
				// does something else with derived.basicMethod() ...
			}
		};

		template<typename Derived>
		class ExtraFeature4
		{
		public:
			void extraMethod4()
			{
				auto derived = static_cast<Derived&>(*this);
				// does something else with derived.basicMethod() ...
			}
		};

		namespace Solution1
		{
			template<template<typename> typename... Skills>
			class X : public Skills<X<Skills...>>...
			{
			public:
				void basicMethod() {}
			};

			using X14 = X<ExtraFeature1, ExtraFeature4>;
			using X1234 = X<ExtraFeature1, ExtraFeature2, ExtraFeature3, ExtraFeature4>;
			/*
			X14 x;
			x.extraMethod1();
			x.extraMethod4();

			X1234 x;
			x.extraMethod1();
			x.extraMethod2();
			x.extraMethod3();
			x.extraMethod4();
			*/
		}

		namespace Solution2
		{
			template<template<typename> class SkillSet>
			class X : public SkillSet<X<SkillSet>>
			{
			public:
				void basicMethod() {}
			};

			template<typename Derived>
			class AllFour : public ExtraFeature1<Derived>, public ExtraFeature2<Derived>, public ExtraFeature3<Derived>, public ExtraFeature4<Derived>
			{};

			using X1234 = X<AllFour>;
			/*
			X1234 x;
			x.extraMethod1();
			x.extraMethod2();
			x.extraMethod3();
			x.extraMethod4();
			*/
		}

		namespace Solution3
		{
			template<template<typename> class SkillSet>
			class X : public SkillSet<X<SkillSet>>
			{
			public:
				void basicMethod() {}
			};

			template<template<typename> class... Skills>
			struct make_skills
			{
				template<typename Derived>
				struct templ : Skills<Derived>...
				{

				};
			};

			using X1234 = X<make_skills<ExtraFeature1, ExtraFeature2, ExtraFeature3, ExtraFeature4>::templ>;
			/*
			X1234 x;
			x.extraMethod1();
			x.extraMethod2();
			x.extraMethod3();
			x.extraMethod4();
			*/
		}

		void Test();
	}
}