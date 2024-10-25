//==============================================================================
// File       : BinomalDistribution.h
// Author     : riyufuchi
// Created on : Oct 22, 2024
// Last edit  : Oct 25, 2024
// Copyright  : Copyright (c) 2024, riyufuchi
// Description: ConsoleArt
//==============================================================================

#ifndef OTHER_BINOMIALDISTRIBUTION_H_
#define OTHER_BINOMIALDISTRIBUTION_H_

#include <math.h>

#include "IDistribution.hpp"

namespace Other
{
class BinomialDistribution : IDisribution
{
private:
	u_bigInt resultHolder;
	int n; // n from binomial distribution formula
	bigDouble p; // chance
public:
	BinomialDistribution();
	BinomialDistribution(int n, bigDouble p);
	~BinomialDistribution();
	u_bigInt factorial(int number);
	u_bigInt partialFactorial(int number, int steps);
	bigDouble power(bigDouble number, bigDouble power);
	u_bigInt combinationNumber(int n, int k);
	bigDouble distribute(int k);
	bigDouble distribute(int k, bigDouble p, int n);
	// Overrides
	bigDouble ex() override;
	bigDouble varX() override;
	// Setters
	void setP(bigDouble p);
	void setN(int n);
	// Getters
	bigDouble getP() const;
	int getN() const;
};
} /* namespace ConsoleArt */
#endif /* OTHER_BINOMIALDISTRIBUTION_H_ */