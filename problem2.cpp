#include <iostream>
#include <vector>
#include <atomic>
#include <mutex>
#include <thread>
#include <algorithm>
#include <chrono>
#include <random>

#define GUEST_COUNT 8

void guest(int id, std::atomic<bool> &roomAvail) {

	// each guest is gonna look at the vase 10 times
	for (int iter = 0; iter < 10; ++iter) {

		bool expected = true;

		// spin until the room is available
		while (!roomAvail.compare_exchange_weak(expected, false)) { expected = true; }

		std::cout << "Guest " << id << " says 'Wow, what a vase...'" << std::endl;

		// make the room available again
		// this is safe since we know we have ownership of the room
		roomAvail = true;

	}

}


int main() {

	// create a variable to represent whether the room is available or not
	std::atomic<bool> roomAvail = true;

	// create the threads for all the guests
	std::vector<std::thread> threads;
	for (int id = 0; id < GUEST_COUNT; ++id) {
		threads.emplace_back(guest, id, std::ref(roomAvail));
	}

	// wait for all threads to finish
	for (std::thread &thr : threads)
		thr.join();

	std::cout << "All guests have finished looking at the vase" << std::endl;

	return 0;
}
