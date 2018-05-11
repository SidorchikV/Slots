#include "slots.h"
#include <fstream>
#include <random>

SlotGameParams::SlotGameParams(size_t _numOfReels, size_t _numOfFruit) : 
  numOfReels(_numOfReels), 
  numOfFruit(_numOfFruit), 
  reelsLength(std::vector<size_t>(_numOfReels)),
  reelsValue(std::vector<std::vector<size_t>>(_numOfReels)),
  winTable(std::vector<std::vector<size_t>>(_numOfFruit, std::vector<size_t>(_numOfReels))),
  tableOfProb(std::vector<std::vector<size_t>>(_numOfReels, std::vector<size_t>(_numOfFruit, 0)))
{}

void SlotGameParams::readReelsValue(std::string const & pathReelsValue)
{
  std::ifstream fin(pathReelsValue);
  if (!fin.is_open())
  {
    throw std::invalid_argument("Can not open file with reels values. Check the path you entered: " + pathReelsValue + "\n");
  }
  for (size_t i = 0; i < numOfReels; i++)
  {
    fin >> reelsLength[i];
    reelsValue[i].resize(reelsLength[i]);
  }
  for (size_t i = 0; i < numOfReels; i++)
  {
    for (size_t j = 0; j < reelsLength[i]; j++)
    {
      fin >> reelsValue[i][j];
    }
  }
  numOfCombinations = 1;
  for (size_t i = 0; i < numOfReels; i++)
  {
    numOfCombinations *= reelsLength[i];
  }
}

void SlotGameParams::readWinTable(std::string const & pathWinTable)
{
  std::ifstream fin(pathWinTable);
  if (!fin.is_open())
  {
    throw std::invalid_argument("Can not open file with win table values. Check the path you entered: " + pathWinTable + "\n");
  }
  for (size_t i = 0; i < numOfFruit; i++)
  {
    for (size_t j = 0; j < numOfReels; j++)
    {
      fin >> winTable[i][j];
    }
  }
}

void SlotGameParams::countProbabilityTable()
{
  for (size_t i = 0; i < numOfReels; i++)
  {
    for (size_t j = 0; j < reelsLength[i]; j++)
    {
      tableOfProb[i][(reelsValue[i][j] - 1)]++;
    }
  }
}

double SlotGameParams::estimateRTP()
{
  this->countProbabilityTable();
  std::vector<size_t> denominators(numOfReels, 1);
  denominators[0] = reelsLength[0];
  if (numOfReels > 1)
    denominators[0] *= reelsLength[1];
  for (size_t i = 1; i < numOfReels - 1; i++)
  {
    denominators[i] = denominators[i - 1] * reelsLength[i + 1];
  }
  denominators[numOfReels - 1] = denominators[numOfReels - 2];
  double theoreticalScore = 0;
  std::vector<size_t> wins(numOfReels);
  for (size_t f = 0; f < numOfFruit; f++)
  {
    for (size_t i = 0; i < numOfReels; i++)
    {
      wins[i] = winTable[f][i];
      for (size_t k = 0; k <= i; k++)
      {
        wins[i] *= tableOfProb[k][f];
      }
      if (i + 1 < numOfReels)
      {
        wins[i] *= reelsLength[i + 1] - tableOfProb[i + 1][f];
      }
      theoreticalScore += (double)wins[i]/denominators[i];
    }
  }
  return theoreticalScore;
}

double SlotGameParams::approxRTP(size_t numOfStarts, unsigned seed)
{
  std::default_random_engine generator(seed);
  std::vector<std::uniform_int_distribution<size_t>> distribution;
  for (size_t j = 0; j < numOfReels; j++)
  {
    distribution.push_back(std::uniform_int_distribution<size_t>(0, reelsLength[j] - 1));
  }
  if (numOfStarts < 1)
  {
    numOfStarts = numOfCombinations;
  }
  size_t randomStartsScore = 0;
  std::vector<size_t> tempComb(numOfReels);
  for (size_t i = 0; i < numOfStarts; i++)
  {
    for (size_t j = 0; j < numOfReels; j++)
    {
      tempComb[j] = reelsValue[j][distribution[j](generator)];
    }
    randomStartsScore += winTable[tempComb[0] - 1][checkLine(tempComb) - 1];
  }
  return (double)randomStartsScore / numOfStarts;
}

double SlotGameParams::calcRTP()
{
  std::vector<size_t> tempComb(numOfReels);
  size_t totalScore = 0;
  make_permutation(0, totalScore, tempComb);
  return (double)totalScore/ numOfCombinations;
}

void SlotGameParams::make_permutation(size_t j, size_t &totalScore, std::vector<size_t>& tempComb)
{
  for (size_t i = 0; i < reelsLength[j]; i++)
  {
    tempComb[j] = reelsValue[j][i];
    if (j == (numOfReels - 1))
    {
      totalScore += winTable[tempComb[0] - 1][checkLine(tempComb) - 1];
    }
    else
    {
      make_permutation(j + 1, totalScore, tempComb);
    }
  }
}

size_t checkLine(std::vector<size_t> const & line)
{
  for (size_t i = 1; i < line.size(); i++)
  {
    if (line[i - 1] != line[i])
      return i;
  }
  return line.size();
}