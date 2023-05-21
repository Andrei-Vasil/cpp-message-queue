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
    static std::string BENCHMARK_LATENCY_FILE;
    static std::string BENCHMARK_THROUGHPUT_PRODUCER_FILE;
    static std::string BENCHMARK_THROUGHPUT_CONSUMER_FILE;

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
        
        BENCHMARK_ID = 5;
        
        std::ostringstream oss_latency; 
        oss_latency << "data/latency/" << yyyy_mm_dd_now() << "-" << BENCHMARK_ID << ".csv";
        BENCHMARK_LATENCY_FILE = oss_latency.str();

        std::ostringstream oss_throughput_produce; 
        oss_throughput_produce << "data/throughput/produce" << yyyy_mm_dd_now() << "-" << BENCHMARK_ID << ".csv";
        BENCHMARK_THROUGHPUT_PRODUCER_FILE = oss_throughput_produce.str();

        std::ostringstream oss_throughput_consume; 
        oss_throughput_consume << "data/throughput/consume" << yyyy_mm_dd_now() << "-" << BENCHMARK_ID << ".csv";
        BENCHMARK_THROUGHPUT_CONSUMER_FILE = oss_throughput_consume.str();
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

void set_publish_over(int benchmark_id) {
    if (!BenchmarkingMetrics::BENCHMARKING) {
        return;
    }
    std::ofstream outfile(BenchmarkingMetrics::BENCHMARK_LATENCY_FILE, std::ios_base::app);
    outfile << timestamp_now_nanoseconds() << "," << benchmark_id << "\n";
}

void count_publish_throughput() {
    if (!BenchmarkingMetrics::BENCHMARKING) {
        return;
    }
    std::ofstream outfile(BenchmarkingMetrics::BENCHMARK_THROUGHPUT_PRODUCER_FILE, std::ios_base::app);
    outfile << timestamp_now_seconds() << "\n";
}

void count_consumer_throughput() {
    if (!BenchmarkingMetrics::BENCHMARKING) {
        return;
    }
    std::ofstream outfile(BenchmarkingMetrics::BENCHMARK_THROUGHPUT_CONSUMER_FILE, std::ios_base::app);
    outfile << timestamp_now_seconds() << "\n";
}

#endif
