// GaussianDistribution.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <numeric>


class Gaussain {
private:
	double mean;
	double stddev;

public:
	Gaussain(double mean = 0.0, double stddev = 1.0): mean(mean), stddev(stddev) {	}
	
	//fit model from data
	void fit(const std::vector<double>& data) {
		int n = data.size();
		if (n == 0) throw std::runtime_error("Empty dataset");

		mean = std::accumulate(data.begin(), data.end(), 0.0) / n;

		double variance = 0.0;
		for (double x : data) {
			variance += (x - mean) * (x - mean);
		}
		variance /= n;
		stddev = std::sqrt(variance);
	}

	//probability destiny function
	double pdf(double x) const {
		static const double invSqrt2pi = 0.3989422804014327;
		double a = (x - mean) / stddev;
		return (invSqrt2pi / stddev) * std::exp(-0.5 * a * a);
	}
	
	//cumulative distribution function
	double cdf(double x) const {
		return 0.5 * (1 + std::erf((x - mean) / (stddev * std::sqrt(2.0))));
	}

	//generate random data
	std::vector<double> sample(int n) const {
		std::random_device rd;
		std::mt19937 gen(rd());
		std::normal_distribution<> dist(mean, stddev);
		
		std::vector<double> values;
		values.reserve(n);
		for (int i = 0; i < n; ++i) {
			values.push_back(dist(gen));
		}
		return values;
	}

	//getters
	double getMean() const { return mean; }
	double getStddev() const { return stddev; }
};

int main()
{
	Gaussain g;
	// Fit from data
	std::vector<double> data = { 1.2, 0.9, 1.5, 1.1, 0.8 };
	g.fit(data);

	std::cout << "Fitted mean = " << g.getMean() << "\n";
	std::cout << "Fitted stddev = " << g.getStddev() << "\n";

	// PDF & CDF at x=1.0
	std::cout << "PDF(1.0) = " << g.pdf(1.0) << "\n";
	std::cout << "CDF(1.0) = " << g.cdf(1.0) << "\n";

	// Generate samples
	auto samples = g.sample(5);
	std::cout << "Random samples: ";
	for (auto v : samples) std::cout << v << " ";
	std::cout << "\n";

	return 0;
}
