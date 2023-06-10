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

    static void init_vars() {
        BENCHMARKING = false;
    }
};

#endif

std::string timestamp_now_nanoseconds() {
    long long timestamp = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now()).time_since_epoch().count();
    long long nanoseconds = timestamp % 1000000000;
    long long seconds = timestamp / 1000000000;
    std::ostringstream oss; 
    oss << seconds << "." << nanoseconds;
    return oss.str();
}

long long timestamp_now_seconds() {
    return std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now()).time_since_epoch().count();
}

void set_publish_over(int benchmark_id, std::string scenario_id) {
    if (!BenchmarkingMetrics::BENCHMARKING) {
        return;
    }
    std::ostringstream oss; 
    oss << "data/latency/" << scenario_id << ".csv";
    std::ofstream outfile(oss.str(), std::ios_base::app);
    outfile << timestamp_now_nanoseconds() << "," << benchmark_id << "\n";
}

void count_publish_throughput(std::string scenario_id) {
    if (!BenchmarkingMetrics::BENCHMARKING) {
        return;
    }
    std::ostringstream oss; 
    oss << "data/throughput/produce/" << scenario_id << ".csv";
    std::ofstream outfile(oss.str(), std::ios_base::app);
    outfile << timestamp_now_seconds() << "\n";
}

void count_consumer_throughput(std::string scenario_id) {
    if (!BenchmarkingMetrics::BENCHMARKING) {
        return;
    }
    std::ostringstream oss; 
    oss << "data/throughput/consume/" << scenario_id << ".csv";
    std::ofstream outfile(oss.str(), std::ios_base::app);
    outfile << timestamp_now_seconds() << "\n";
}

#endif
