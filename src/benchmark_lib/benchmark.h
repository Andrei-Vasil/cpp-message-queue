#ifndef BENCHMARK
#define BENCHMARK

#include <ctime>
#include <fstream>
#include <sstream>

#ifndef BENCHMARKING_METRICS
#define BENCHMARKING_METRICS

class BenchmarkingMetrics {
public:
    static bool BENCHMARKING;
    static int BENCHMARK_ID;
    static std::string BENCHMARK_FILE;

    static std::string yyyy_mm_dd_now() {
        std::time_t rawtime;
        std::tm* timeinfo;
        char buffer [80];
        std::time(&rawtime);
        timeinfo = std::localtime(&rawtime);
        std::strftime(buffer, 80, "%Y-%m-%d", timeinfo);
        return buffer;
    }

    static void init_vars() {
        BENCHMARKING = true;
        BENCHMARK_ID = 1;
        std::ostringstream oss; 
        oss << "data/" << yyyy_mm_dd_now() << "-" << BENCHMARK_ID << ".csv";
        BENCHMARK_FILE = oss.str();
    }
};

#endif

std::string timestamp_now() {
    long long timestamp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    long long nanoseconds = timestamp % 1000000000;
    long long seconds = timestamp / 1000000000;
    std::ostringstream oss; 
    oss << seconds << "." << nanoseconds;
    return oss.str();
}

void set_publish_over(int benchmark_id) {
    if (!BenchmarkingMetrics::BENCHMARKING) {
        return;
    }
    std::ofstream outfile(BenchmarkingMetrics::BENCHMARK_FILE, std::ios_base::app);
    outfile << timestamp_now() << "," << benchmark_id << "\n";
}

#endif
