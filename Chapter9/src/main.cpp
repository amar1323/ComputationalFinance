#include <algorithm>
#include <concepts>
#include <execution>
#include <iostream>
#include <memory>
#include <vector>

using S_E_Iter = std::pair<std::vector<int>::iterator, std::vector<int>::iterator>;
// Define the partitioning policy interface
template <typename T>
concept PartitionPolicy = requires(const T& policy, std::vector<int>& data) {
                              { policy.partition(data) } -> std::same_as<std::vector<S_E_Iter>>;
                          };

// Define the parallelization policy interface
template <typename T>
concept ParallelPolicy = requires(const T& policy, std::vector<int>& data, std::vector<S_E_Iter>& partitions) {
                             { policy.sort_partitions(data, partitions) } -> std::same_as<void>;
                         };

// Define a default partitioning policy
class DefaultPartitionPolicy {
   public:
    std::vector<S_E_Iter> partition(std::vector<int>& data) const {
        std::vector<S_E_Iter> partitions;
        int n = data.size();
        int p = 4;
        int partition_size = n / p;
        int start = 0;
        int end = partition_size;

        for (int i = 0; i < p; i++) {
            // Handle the last partition
            if (i == p - 1) {
                end = n;
            }

            partitions.push_back(make_pair(data.begin() + start, data.begin() + end));

            start += partition_size;
            end += partition_size;
        }
        return std::move(partitions);
    }
};

// Define a default parallelization policy
class DefaultParallelPolicy {
   public:
    void sort_partitions(std::vector<int>& data, std::vector<S_E_Iter>& partitions) const {
        std::for_each(std::execution::par, partitions.begin(), partitions.end(), [&](S_E_Iter& partition) {
            std::sort(partition.first, partition.second);
        });
        for(size_t i = 0; i<partitions.size()-1;i++){
            std::inplace_merge(partitions[i].first, partitions[i].second, partitions[i+1].second);
        }
    }
};

// Define a class that uses policies for sorting
template <PartitionPolicy TPartitionPolicy= DefaultPartitionPolicy, ParallelPolicy TParallelPolicy = DefaultParallelPolicy>
class ParallelSort {
   public:
    ParallelSort(const TPartitionPolicy& partition_policy = TPartitionPolicy(), const TParallelPolicy& parallel_policy = TParallelPolicy())
        : partition_policy_(partition_policy), parallel_policy_(parallel_policy) {}

    void sort(std::vector<int>& data) {
        std::vector<S_E_Iter> partitions = partition_policy_.partition(data);
        parallel_policy_.sort_partitions(data, partitions);
    }

   private:
    TPartitionPolicy partition_policy_;
    TParallelPolicy parallel_policy_;
};

// Define a custom partitioning policy
class CustomPartitionPolicy {
   public:
    std::vector<S_E_Iter> partition(std::vector<int>& data) const {
        std::vector<S_E_Iter> partitions;
        partitions.push_back(make_pair(data.begin(), data.end()));
        return partitions;
    }
};

// Define a custom parallelization policy
class CustomParallelPolicy {
   public:
    void sort_partitions(std::vector<int>& data, std::vector<S_E_Iter>& partitions) const {
        std::sort(std::execution::par, data.begin(), data.end());
    }
};

int main() {
    // Sort with default policies
    std::vector<int> data1 = {3, 2, 1, 4, 5, 8, 7, 6};
    ParallelSort<> sorter1;
    sorter1.sort(data1);
    std::cout << "Default partitioning and parallelization policies: ";
    for (const auto& item : data1) {
      std::cout << item << "   ";
    }
    
    std::cout<< std::endl;

    std::vector<int> data2 = {3, 2, 1, 4, 5, 8, 7, 6};
    ParallelSort<CustomPartitionPolicy,CustomParallelPolicy> sorter2;
    sorter2.sort(data2);
    std::cout << "Custom partitioning and parallelization policies: ";
    for (const auto& item : data2) {
      std::cout << item << "   ";
    }
    
    std::cout<< std::endl;

    std::vector<int> data3 = {3, 2, 1, 4, 5, 8, 7, 6};
    std::sort(std::execution::par, data3.begin(), data3.end());
    std::cout << "std partitioning and parallelization policies: ";
    for (const auto& item : data3) {
      std::cout << item << "   ";
    }

    std::cout<< std::endl;
    return 0;
}