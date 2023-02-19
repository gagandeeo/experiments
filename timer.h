#include <chrono>

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
