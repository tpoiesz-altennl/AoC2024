#include "Day22.h"
#include "../Util.h"
#include <unordered_map>

unsigned int MakeNewNumber(unsigned int secret)
{
	secret = (secret ^ (secret << 6)) & 0xffffff;
	secret = secret ^ (secret >> 5);
	secret = (secret ^ (secret << 11)) & 0xffffff;
	return secret;
}

int Day22::Solution1()
{
	std::vector<unsigned int> input = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__);
	//std::vector<unsigned int> input = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__, true);

	unsigned int iterations = 2000;
	u64 total = 0;
	for (unsigned int secret : input)
	{
		for (unsigned int i = 0; i < iterations; ++i)
		{
			secret = MakeNewNumber(secret);
		}
		total += secret;
	}

	std::cout << total;
	return 0;
}

std::vector<char> BuildPriceList(unsigned int secret, unsigned int numIters)
{
	std::vector<char> prices({ static_cast<char>(secret % 10) });
	for (unsigned int i = 0; i < numIters; ++i)
	{
		secret = MakeNewNumber(secret);
		prices.push_back(secret % 10);
	}
	return prices;
}

std::vector<char> BuildPriceChangeList(const std::vector<char>& prices)
{
	std::vector<char> priceChanges;
	for (unsigned int i = 0; i < prices.size() - 1; ++i)
	{
		priceChanges.push_back(prices[i + 1] - prices[i]);
	}
	return priceChanges;
}

struct vec4c
{
	vec4c() : x(0), y(0), z(0), w(0)
	{ }
	vec4c(char _x, char _y, char _z, char _w) : x(_x), y(_y), z(_z), w(_w)
	{ }

	char x;
	char y;
	char z;
	char w;

	bool operator==(const vec4c& other) const
	{
		return x == other.x && y == other.y && z == other.z && w == other.w;
	}

	horizontal_vector<int> to_int() const
	{
		return horizontal_vector<int>({ static_cast<int>(x), static_cast<int>(y), static_cast<int>(z), static_cast<int>(w) });
	}
};

namespace std
{
	// Generated by ChatGPT
	template <>
	struct hash<vec4c> {
		size_t operator()(const vec4c& v) const {
			// Hash individual components and combine them
			size_t h1 = std::hash<char>{}(v.x);  // Hash for the x component
			size_t h2 = std::hash<char>{}(v.y);  // Hash for the y component
			size_t h3 = std::hash<char>{}(v.z);  // Hash for the z component
			size_t h4 = std::hash<char>{}(v.w);  // Hash for the w component

			// Combine the two hash values
			// The XOR and shifting ensures a good distribution of hash values
			return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3);  // Shift to avoid collision
		}
	};
}

using price_predictor = std::unordered_map<vec4c, short>;

price_predictor BuildPricePredictionList(const std::vector<char>& prices, const std::vector<char>& priceChanges)
{
	price_predictor predictionList;
	for (unsigned int i = 3; i < priceChanges.size(); ++i)
	{
		vec4c fluctuation(priceChanges[i - 3], priceChanges[i - 2], priceChanges[i - 1], priceChanges[i]);
		predictionList.emplace(fluctuation, prices[i + 1]);
	}
	return predictionList;
}

void MergePricePredictionLists(price_predictor& a, price_predictor b)
{
	// Combine all entries found in both
	for (auto itA = a.begin(); itA != a.end(); ++itA)
	{
		if (auto inB = b.find(itA->first); inB != b.end())
		{
			itA->second += inB->second;
			b.erase(inB); // Don't need to consider this entry again
		}
	}
	// Also add all entries only found in b
	for (auto itB = b.begin(); itB != b.end(); ++itB)
	{
		a.emplace(itB->first, itB->second);
	}
}

int Day22::Solution2()
{
	std::vector<unsigned int> input = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__);
	//std::vector<unsigned int> input = FileUtil::ReadInputIntoVec<unsigned int>(__FILE__, true);

	unsigned int iterations = 2000;
	Testing::DebugFile dbg(__FILE__);
	std::vector<price_predictor> pricePredictions;
	for (unsigned int buyer : input)
	{
		std::vector<char> buyerPrices = BuildPriceList(buyer, iterations);
		std::vector<char> priceChanges = BuildPriceChangeList(buyerPrices);
		pricePredictions.push_back(BuildPricePredictionList(buyerPrices, priceChanges));
	}
	price_predictor combinedPricePredictor = pricePredictions[0];
	for (unsigned int i = 1; i < pricePredictions.size(); ++i)
	{
		MergePricePredictionLists(combinedPricePredictor, pricePredictions[i]);
	}

	int bestPrice = 0;
	vec4c bestSequence;
	for (auto it = combinedPricePredictor.begin(); it != combinedPricePredictor.end(); ++it)
	{
		if (it->second > bestPrice)
		{
			bestPrice = it->second;
			bestSequence = it->first;
		}
	}

	std::cout << bestSequence.to_int() << ": " << bestPrice;
	return 0;
}
