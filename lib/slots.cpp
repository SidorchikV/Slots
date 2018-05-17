#include "slots.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cassert>

SlotGameParams::SlotGameParams(size_t _numOfReels, size_t _numOfFruit) : 
  reels(std::vector<std::vector<size_t>>(_numOfReels)),
  winTable(std::vector<std::vector<size_t>>(_numOfFruit, std::vector<size_t>(_numOfReels)))
{}

void SlotGameParams::readReels(std::string const & pathreels)
{
  std::ifstream fin(pathreels);
  size_t tempLength = 0;
  if (!fin.is_open())
  {
    throw std::invalid_argument("Can not open file with reels values. Check the path you entered: " + pathreels + "\n");
  }
  for (size_t i = 0; i < numOfReels(); i++)
  {
    fin >> tempLength;
    reels[i].resize(tempLength);
  }
  for (size_t i = 0; i < numOfReels(); i++)
  {
    for (size_t j = 0; j < reelsLength(i); j++)
    {
      fin >> reels[i][j];
    }
  }
}

void SlotGameParams::readWinTable(std::string const & pathWinTable)
{
  std::ifstream fin(pathWinTable);
  if (!fin.is_open())
  {
    throw std::invalid_argument("Can not open file with win table values. Check the path you entered: " + pathWinTable + "\n");
  }
  for (size_t i = 0; i < numOfFruit(); i++)
  {
    for (size_t j = 0; j < numOfReels(); j++)
    {
      fin >> winTable[i][j];
    }
  }
}

size_t SlotGameParams::numOfReels() const
{
  return reels.size();
}

size_t SlotGameParams::numOfFruit() const
{
  return winTable.size();
}

size_t SlotGameParams::reelsLength(size_t i) const
{
  assert(i < numOfReels());
  return reels[i].size();
}

double SlotGameParams::calcInTheoryRTP()
{
  std::vector<std::vector<size_t>> tableOfProb(numOfReels(), std::vector<size_t>(numOfFruit(), 0));
  for (size_t i = 0; i < numOfReels(); i++)
  {
    for (size_t j = 0; j < reelsLength(i); j++)
    {
      tableOfProb[i][(reels[i][j] - 1)]++;
    }
  }
  std::vector<size_t> denominators(numOfReels(), 1);
  denominators[0] = reelsLength(0);
  if (numOfReels() > 1)
    denominators[0] *= reelsLength(1);
  for (size_t i = 1; i < numOfReels() - 1; i++)
  {
    denominators[i] = denominators[i - 1] * reelsLength(i + 1);
  }
  denominators[numOfReels() - 1] = denominators[numOfReels() - 2];
  double theoreticalScore = 0;
  std::vector<size_t> wins(numOfReels());
  for (size_t f = 0; f < numOfFruit(); f++)
  {
    for (size_t i = 0; i < numOfReels(); i++)
    {
      wins[i] = winTable[f][i];
      for (size_t k = 0; k <= i; k++)
      {
        wins[i] *= tableOfProb[k][f];
      }
      if (i + 1 < numOfReels())
      {
        wins[i] *= reelsLength(i + 1) - tableOfProb[i + 1][f];
      }
      theoreticalScore += (double)wins[i]/denominators[i];
    }
  }
  return theoreticalScore;
}

double SlotGameParams::estimateRTP(size_t numOfStarts, unsigned seed)
{
  std::default_random_engine generator(seed);
  std::vector<std::uniform_int_distribution<size_t>> distribution;
  for (size_t j = 0; j < numOfReels(); j++)
  {
    distribution.push_back(std::uniform_int_distribution<size_t>(0, reelsLength(j) - 1));
  }
  if (numOfStarts < 1)
  {
    numOfStarts = numOfCombinations();
  }
  size_t randomStartsScore = 0;
  std::vector<size_t> tempComb(numOfReels());
  for (size_t i = 0; i < numOfStarts; i++)
  {
    for (size_t j = 0; j < numOfReels(); j++)
    {
      tempComb[j] = reels[j][distribution[j](generator)];
    }
    randomStartsScore += winTable[tempComb[0] - 1][checkLine(tempComb) - 1];
  }
  return (double)randomStartsScore / numOfStarts;
}

double SlotGameParams::calcPracticallyRTP()
{
  std::vector<size_t> line(numOfReels(), 0);
  size_t score = 0;
  std::vector<size_t> fruitLine(numOfReels());
  do
  {
    for (int j = 0; j < numOfReels(); j++)
    {
      fruitLine[j] = reels[j][line[j]];
    }
    score += winTable[fruitLine[0] - 1][checkLine(fruitLine) - 1];

  } while (inc(line));
  return (double)score / numOfCombinations();
}

bool SlotGameParams::pointTest(std::vector<std::vector<size_t>> reels, std::vector<std::vector<size_t>> winTable, double realRTP)
{
  assert(reels.size() == (winTable[0]).size());
  SlotGameParams SGP(reels.size(), winTable.size());
  SGP.winTable.assign(winTable.begin(), winTable.end());
  SGP.reels.assign(reels.begin(), reels.end());
  return ((SGP.calcPracticallyRTP() - realRTP) < 0.00001);
}

SlotGameParams  SlotGameParams::randomReels(unsigned seed)
{
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<size_t> dist_reel_count(2,6);
  std::uniform_int_distribution<size_t> dist_fruit_count(2,6);
  std::uniform_int_distribution<size_t> dist_reel_len(0,9);
  std::uniform_int_distribution<size_t> dist_win_value(0,8);

  SlotGameParams SGP(dist_reel_count(generator), dist_fruit_count(generator));

  std::uniform_int_distribution<size_t> dist_fruit_on_reel(1, SGP.numOfFruit());
  
  for (size_t i = 0; i < SGP.numOfFruit(); i++)
  {
    for (size_t j = 0; j < SGP.numOfReels(); j++)
    {
      SGP.winTable[i][j] = dist_win_value(generator);
    }
    std::sort(SGP.winTable[i].begin(), SGP.winTable[i].end()); 
  }
  for (size_t i = 0; i < SGP.numOfReels(); i++)
  {
    SGP.reels[i].resize(dist_reel_len(generator) * 5 + dist_reel_len(generator) + 1);
    for (size_t j = 0; j < SGP.reelsLength(i); j++)
    {
      SGP.reels[i][j] = dist_fruit_on_reel(generator);
    }
  }
  return std::move(SGP);
}

bool SlotGameParams::inc(std::vector<size_t>& line)
{
  bool isOverflow = false;
  size_t i = numOfReels();
  do
  {
    i--;
    if (line[i] == (reelsLength(i)-1))
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

size_t SlotGameParams::numOfCombinations() const
{
  size_t numOfCombs = 1;
  for (size_t i = 0; i < numOfReels(); i++)
  {
     numOfCombs *= reelsLength(i);
  }
  return numOfCombs;
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

bool randomParamsTest(unsigned seed, size_t numOfStarts, std::string const &path)
{
  auto sampleSlot1 = SlotGameParams::randomReels(seed);
  //double calcPracticallyRTP =  sampleSlot1.calcPracticallyRTP();
  double calcInTheoryRTP = sampleSlot1.calcInTheoryRTP();
  double estimateRTP = sampleSlot1.estimateRTP(numOfStarts);
  /*if (calcPracticallyRTP != calcInTheoryRTP)
  return false;*/
  double percent = fabs(estimateRTP - calcInTheoryRTP) * 100 / calcInTheoryRTP;
  if (path != "")
  {
    std::ofstream fout(path, std::ios::app);
    if (!fout.is_open())
    {
      throw std::invalid_argument("Can not open file to write results. Check the path you entered:" + path + "\n");
    }
    fout << sampleSlot1.numOfCombinations() << " " << numOfStarts ;
    fout << " " <<calcInTheoryRTP << " " << estimateRTP ;
    fout << " " << percent << std::endl;
  }
  else
  {
    std::cout << "\nnumOfCombinations = " << sampleSlot1.numOfCombinations() << std::endl;
    std::cout << "numOfStarts = " << numOfStarts << std::endl;
    std::cout << "Percent = " << percent << std::endl;
  }
  return (percent <= 1);
}
