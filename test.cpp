#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <algorithm>

// multithread controller 
// set to False for disabling multithread


// keep track for no of threads used in proccess
int no_running_threads = 0;

/* set limit to no of threads generating been used
should be less than 1984 (on x64-based processor 16GB RAM
on 11th Gen Intel(R) Core(TM) i5-1135G7)*/
#define use_multithread 1
int thread_threshold = 10;
int arr_size = 100000; // array size
// class for benchmarking parts of code
// you can find the step-by-step implementation for benchmark here
// https://youtu.be/YG4jexlSAjc 

class Timer
{
public:
	Timer()
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		Stop();
	}

	void Stop()
	{
		auto endTimePoint = std::chrono::high_resolution_clock::now();

		auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimePoint).time_since_epoch().count();

		auto duration = end - start;
		auto ms = duration * 0.001;
		auto sec = ms*0.001;

		std::cout << "Time Elapsed: " << duration << "us | " << ms << "ms | " << sec << "s" <<"\n";
	}

private:
	std::chrono::time_point< std::chrono::high_resolution_clock> m_StartTimepoint;
};

// Merge Function for two sorted partitions
static void Merge(std::vector<int>& arr, std::vector<int>& helper, int low, int mid, int high)
{
	for (int i = low; i < high + 1; i++)
	{
		helper[i] = arr[i];
	}

	int helperLeft = low;
	int helperRight = mid + 1;
	int current = low;

	while (helperLeft <= mid && helperRight <= high)
	{
		if (helper[helperLeft] <= arr[helperRight])
		{
			arr[current] = helper[helperLeft];
			helperLeft++;
		}
		else {
			arr[current] = helper[helperRight];
			helperRight++;
		}
		current++;
	}

	int remaining = mid - helperLeft;
	for (int i = 0; i <= remaining; i++)
	{
		arr[current + i] = helper[helperLeft + i];
	}
}

// Merge Sort Algorithm
static void MergeSort(std::vector<int>& arr, std::vector<int>& helper, int low, int high)
{
	if (low < high)
	{
		int mid = (low + high) / 2;
		#if use_multithread
			// check if thread count overflowed ?
			if(no_running_threads >= thread_threshold){
				// stop using multithread
				MergeSort(arr, helper, low, mid);
				MergeSort(arr, helper, mid+1, high);
			}else{
				// keep using multithread
				no_running_threads++;
				std::thread worker(MergeSort, std::ref(arr), std::ref(helper), low, mid);
				MergeSort(arr, helper, mid+1, high);
				// wait for thread to complete for merging results
				worker.join();
			}
		#else
			// without multithread
			MergeSort(arr, helper, low, mid);
			MergeSort(arr, helper, mid+1, high);
		#endif
		// merge sorted arrays
		Merge(arr, helper, low, mid, high);
	}
}

// print array
void printArray(std::vector<int> & arr)
{
	for (int i = 0; i < arr.size(); i++)
	{
		std::cout << arr[i] << " ";
	}
}

// For understanding basics of thread class follow here
// https://youtu.be/wXBcwHwIt_I

int main()
{
    
	std::vector<int> arr(arr_size); // array to sort
	std::vector<int> helper(arr_size); // helper array 

	// std::cout << "Enter arr_size: ";
	// std::cin >> arr_size;

	std::cout << "Array size: " << arr_size << "\n";
	// Fill array with random elements
    srand(123);	// define seed
	std::generate(arr.begin(), arr.end(), rand);

	// Timer Scope
	{
		Timer timer; // Initiate Timer object

		no_running_threads++; // No of running threads
		MergeSort(arr, helper, 0, arr_size-1); // Call merge sort on the array
	}
	std::cout << "No of threads used: " << no_running_threads <<"\n";

}