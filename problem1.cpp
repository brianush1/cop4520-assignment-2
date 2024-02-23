#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <algorithm>
#include <chrono>
#include <random>

#define GUEST_COUNT 8

void guest(int id, std::atomic<bool> &cupcake, std::atomic<bool> &finished) {
	std::random_device rdev;
	std::mt19937 rng(rdev());
	auto sleepDistr = std::uniform_int_distribution<int>(10, 100);

	// guest 0 is special
	if (id == 0) {
		// keep track of how many times we've added the cupcake back
		int numTimes = 0;

		// keep re-entering the maze until the game is over
		while (!finished) {

			// sleep for a random amount of time to simulate going through the maze
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepDistr(rng)));

			// If when we get through the maze, the cupcake isn't there, that means someone
			// must've eaten it. Since the other guests' strategy says that they only eat
			// the cupcake at most once, that means a new, unique guest has eaten the cupcake,
			// so we update numTimes and add the cupcake back

			bool expected = false;
			if (cupcake.compare_exchange_strong(expected, true)) {
				std::cout << "Guest 0: Another guest has been through the maze!" << std::endl;

				numTimes += 1;

				if (numTimes == GUEST_COUNT - 1) {
					// if all the guests (except for guest 0) have eaten the cupcake,
					// we're done

					finished = true;
				}
			}
			else {
				std::cout << "Guest 0: No one else has gone through the maze" << std::endl;
			}

		}

	}

	// all the other guests have the same strategy
	else {

		// each guest only eats the cupcake up to one time
		bool ateAlready = false;

		// keep re-entering the maze until the game is over
		while (!finished) {

			// sleep for a random amount of time to simulate going through the maze
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepDistr(rng)));

			if (ateAlready) {
				// leave the cupcake as-is

				std::cout << "Guest " << id << " went through the maze again; didn't touch the cupcake" << std::endl;
			}
			else {
				// eat the cupcake if it's there

				bool expected = true;
				if (cupcake.compare_exchange_strong(expected, false)) {
					std::cout << "Guest " << id << " went through the maze and ate the cupcake" << std::endl;

					ateAlready = true;
				}
				else {
					std::cout << "Guest " << id << " went through the maze, but there was no cupcake" << std::endl;
				}
			}

		}

	}
}

void minotaur() {

	// create a variable to represent whether the cupcake is currently present or not
	std::atomic<bool> cupcake = true;

	// create a variable to represent whether we're done or not
	// if this is true, all the threads will choose to exit
	std::atomic<bool> finished = false;

	// create the threads for all the guests
	std::vector<std::thread> threads;
	for (int id = 0; id < GUEST_COUNT; ++id) {
		threads.emplace_back(guest, id, std::ref(cupcake), std::ref(finished));
	}

	// wait for all threads to finish
	for (std::thread &thr : threads)
		thr.join();

	std::cout << "All guests have gone through the maze" << std::endl;

}

int main() {
	minotaur();

	return 0;
}
