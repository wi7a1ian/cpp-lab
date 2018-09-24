#pragma once

#include <iostream>           
#include <string>           
#include <atomic>             // std::thread
#include <thread>             // std::thread
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable
#include <queue> 
#include <future> 

namespace ConcurrencyTest
{
	struct ConcurrencyExamples {
		std::mutex mut;
		std::queue <std::string> tasks;
		std::condition_variable cv;

		void ConcurrencyExample1() {
			std::atomic<bool> action_needed;

			if (action_needed) {
				std::lock_guard<std::mutex> lock(mut);
				if (action_needed) {
					//take_action();
					action_needed = false;
				}
			}
		}

		void ConcurrencyExample2() 
		{
			std::vector<std::string> todo{ "Write program",
				"Fix bug",
				"Write unit tests",
				"Bring me coffee",
				"Clean my car",
				"go home" };

			std::thread t_boss{ [&todo, this] {ConcurrencyExample2_AddTasks(todo); } };
			std::thread t_worker{ [&] {ConcurrencyExample2_Worker(); } };

			t_boss.join();
			t_worker.join();
		}

		void ConcurrencyExample2_Worker()
		{
			while (true)
			{
				std::unique_lock<std::mutex> guard(mut);
				cv.wait(guard, [&] {return !tasks.empty(); });

				auto task = tasks.front();
				tasks.pop();
				std::cout << "Worker doing:   " << task << std::endl;
				if (task == "go home") break;
			}
		}

		void ConcurrencyExample2_AddTasks(const std::vector<std::string>& todo)
		{
			for (const auto& task : todo) {
				std::lock_guard<std::mutex> guard(mut);
				std::cout << "Boss delegates: " << task << std::endl;
				tasks.push(task);
				cv.notify_one();

				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
		}

		void ConcurrencyExample3()
		{
			auto future1 = std::async(std::launch::async, 
				[this](auto x) {ConcurrencyExample2(); return x; }, "Done");

			auto future2 = std::async(std::launch::async,
				[this](auto x) {ConcurrencyExample2(); return x; }, "Done");

			auto future3 = std::async(std::launch::async,
				[this](auto x) {ConcurrencyExample2(); return x; }, "Done");

			std::future_status status;
			do {
				status = future1.wait_for(std::chrono::seconds(1));
				if (status == std::future_status::deferred) {
					std::cout << "deferred\n";
				}
				else if (status == std::future_status::timeout) {
					std::cout << "timeout\n";
				}
				else if (status == std::future_status::ready) {
					std::cout << "ready!\n";
				}
			} while (status != std::future_status::ready);

			std::cout << future1.get() << future2.get() << future3.get();

		}

		void ConcurrencyExample4()
		{
			std::packaged_task< std::string(std::string) > task([this](auto x) {ConcurrencyExample2(); return x; });
			auto future = task.get_future();
			
			task("Done");

			std::cout << future.get() << std::endl;
		}

		void ConcurrencyExample5()
		{
			std::promise<std::string> promise;
			auto future = promise.get_future();

			std::cout << "Requesting promise" << std::endl;
			std::thread([&promise, this] { 
				std::cout << "Setting promise" << std::endl; 
				promise.set_value("Done"); 
			}).detach(); // in order to destroy a C++ thread object either join() needs to be called (and completed) or detach() must be called

			std::cout << "Waiting for promise" << std::endl;
			std::cout << future.get();
		}

	};
}