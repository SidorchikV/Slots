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

void SlotGameParams::readReels(std::string const & pathReelsValue)
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
  this->countNumOfCombinations();
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
  std::vector<size_t> line(numOfReels, 0);
  size_t score = 0;
  std::vector<size_t> fruitLine(numOfReels);
  do
  {
    for (int j = 0; j < numOfReels; j++)
    {
      fruitLine[j] = reelsValue[j][line[j]];
    }
    score += winTable[fruitLine[0] - 1][checkLine(fruitLine) - 1];

  } while (inc(line));
  return (double)score / numOfCombinations;
}

bool SlotGameParams::pointTest(testStruct & TS)
{
  if (TS.reelsValue.size() != (TS.winTable[0]).size())
    return false;
  SlotGameParams SGP(TS.reelsValue.size(), TS.winTable.size());
  for (size_t i = 0; i < SGP.numOfFruit; i++)
  {
    for (size_t j = 0; j < SGP.numOfReels; j++)
    {
      SGP.winTable[i][j] = TS.winTable[i][j];
    }
  }
  for (size_t i = 0; i < SGP.numOfReels; i++)
  {
    SGP.reelsLength[i] = (TS.reelsValue[i]).size();
    SGP.reelsValue[i].resize(SGP.reelsLength[i]);
  }
  for (size_t i = 0; i < SGP.numOfReels; i++)
  {
    for (size_t j = 0; j < SGP.reelsLength[i]; j++)
    {
      SGP.reelsValue[i][j] = TS.reelsValue[i][j];
    }
  }
  SGP.countNumOfCombinations();
  return ((SGP.calcRTP() - TS.realRTP) < 0.00001);
}

SlotGameParams  SlotGameParams::randomReels(unsigned seed)
{
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<size_t> dist_reel_count(2,6);
  std::uniform_int_distribution<size_t> dist_fruit_count(2,6);
  std::uniform_int_distribution<size_t> dist_reel_len(0,9);
  std::uniform_int_distribution<size_t> dist_win_value(0,8);
  SlotGameParams SGP(dist_reel_count(generator), dist_fruit_count(generator));
  std::uniform_int_distribution<size_t> dist_fruit_on_reel(1, SGP.numOfFruit);
  for (size_t i = 0; i < SGP.numOfFruit; i++)
  {
    for (size_t j = 0; j < SGP.numOfReels; j++)
    {
      SGP.winTable[i][j] = dist_win_value(generator);
    }
    std::sort(SGP.winTable[i].begin(), SGP.winTable[i].end()); 
  }
  for (size_t i = 0; i < SGP.numOfReels; i++)
  {
    SGP.reelsLength[i] = dist_reel_len(generator)*5 + dist_reel_len(generator) + 1;
    SGP.reelsValue[i].resize(SGP.reelsLength[i]);
  }
  for (size_t i = 0; i < SGP.numOfReels; i++)
  {
    for (size_t j = 0; j < SGP.reelsLength[i]; j++)
    {
      SGP.reelsValue[i][j] = dist_fruit_on_reel(generator);
    }
  }
  SGP.countNumOfCombinations();
  return std::move(SGP);
}

bool SlotGameParams::inc(std::vector<size_t>& line)
{
  bool isOverflow = false;
  size_t i = numOfReels;
  do
  {
    i--;
    if (line[i] == (reelsLength[i]-1))
    {
      isOverflow = true;
      line[i] = 0;
    }
    else
    {
      isOverflow = false;
      line[i]++;
    }
  } while (isOverflow && i);
  return (!isOverflow);
}

size_t SlotGameParams::countNumOfCombinations()
{
  numOfCombinations = 1;
  for (size_t i = 0; i < numOfReels; i++)
  {
    numOfCombinations *= reelsLength[i];
  }
  return numOfCombinations;
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