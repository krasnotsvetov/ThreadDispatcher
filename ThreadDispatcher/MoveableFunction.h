#pragma once
#ifndef MOVEABLE_FUNCTION
#define MOVEABLE_FUNCTION

#include <memory>

namespace Dispatcher {
	namespace Utils {

		template <typename FunctionType>
		class MoveableFunction;

		template <typename ReturnType, typename ... ArgumentTypes>
		class MoveableFunction<ReturnType(ArgumentTypes...)> {
		public:
			MoveableFunction() = delete;
			template <typename Function>
			MoveableFunction(Function function) : myInvoker(new Invoker<Function>(std::move(function))) {}
			ReturnType operator()(ArgumentTypes... args) {
				return myInvoker->Invoke(args...);
			}

			template <typename Function>
			MoveableFunction(MoveableFunction<Function>&& other) :
				myInvoker(std::move(other.myInvoker))
			{
				
			}

		private:
			class InvokerBase {
			public:
				InvokerBase() {}
				virtual ReturnType Invoke(ArgumentTypes... args) = 0;
				virtual ~InvokerBase() {};
			};

			template<typename Function>
			class Invoker : public InvokerBase {
			public:
				Invoker(Function&& func) : InvokerBase(), function(std::move(func)) {}				virtual ReturnType Invoke(ArgumentTypes... args)				{
					return function(args ...);
				}
			private:
				Function function;
			};

			std::unique_ptr<InvokerBase> myInvoker;
		};
#endif // !MOVEABLE_FUNCTION
	}
}