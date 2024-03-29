#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdlib> // Include this for rand() and srand()
#include <ctime> // Include this for time()

// Global variable for storing the sum
long long sum = 0;

// Mutex for synchronizing access to the sum
std::mutex mtx;

// Function for calculating the sum of elements in a part of the array
void calculate_sum(std::vector<int>& arr, int start, int end) {
    long long partial_sum = 0;
    for (int i = start; i < end; ++i) {
        partial_sum += arr[i];
    }

    // Lock the mutex before updating the global sum
    mtx.lock();
    sum += partial_sum;
    mtx.unlock();
}

int main() {
    // Size of the array
    int size = 1000000;

     // Initialize the array with random numbers
    srand(time(0)); // Seed the random number generator with the current time
    std::vector<int> arr(size);
    for (int i = 0; i < size; ++i) {
        arr[i] = rand(); // Generate a random number
    }

    // Number of threads
    int num_threads = 10;

    // Calculate the size of the parts
    int part_size = arr.size() / num_threads;

    // Vector for storing the threads
    std::vector<std::thread> threads;

    // Create and start the threads
    for (int i = 0; i < num_threads; ++i) {
        int start = i * part_size;
        int end = (i == num_threads - 1) ? arr.size() : (i + 1) * part_size;
        threads.push_back(std::thread(calculate_sum, std::ref(arr), start, end));
    }

    // Wait for all threads to finish
    for (auto& t : threads) {
        t.join();
    }

    // Print the sum
    std::cout << "Sum: " << sum << std::endl;

    return 0;
}