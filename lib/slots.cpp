#include "slots.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cassert>
#include <cstddef>

SlotGameParams::SlotGameParams(size_t _numOfReels, size_t _numOfFruit) : 
  reels(reels_t(_numOfReels)),
  winTable(reels_t(_numOfFruit, std::vector<size_t>(_numOfReels)))
{}

SlotGameParams::SlotGameParams(reels_t & _reels, wins_t & _winTable) 
  : reels(_reels), winTable(_winTable)
{}

reels_t readReels(std::istream &fin)
{
  reels_t reels;
  size_t tempLength = 0;
  if (!fin)
  {
    throw std::invalid_argument("Can not open file with reels values.\n");
  }

  fin >> tempLength;
  reels.resize(tempLength);
  for (size_t i = 0; i < reels.size(); i++)
  {
    fin >> tempLength;
    reels[i].resize(tempLength);
  }

  for (size_t i = 0; i < reels.size(); i++)
  {
    for (size_t j = 0; j < reels[i].size(); j++)
    {
      fin >> reels[i][j];
    }
  }
  return reels;
}

wins_t readWinTable(std::istream &fin)
{
  if (!fin)
  {
    throw std::invalid_argument("Can not open file with win table values.\n");
  }
  size_t numOfFruit = 0, numOfReels = 0;
  fin >> numOfFruit;
  fin >> numOfReels;
  wins_t winTable(numOfFruit, std::vector<size_t>(numOfReels));

  for (size_t i = 0; i < numOfFruit; i++)
  {
    for (size_t j = 0; j < numOfReels; j++)
    {
      fin >> winTable[i][j];
    }
  }
  return winTable;
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


std::vector<std::vector<size_t>> SlotGameParams::countProbabilityTable()
{
  std::vector<std::vector<size_t>> tableOfProb(numOfReels(), std::vector<size_t>(numOfFruit(), 0));
  for (size_t i = 0; i < numOfReels(); i++)
  {
    for (size_t j = 0; j < reelsLength(i); j++)
    {
      tableOfProb[i][(reels[i][j] - 1)]++;
    }
  }
  return tableOfProb;
}
std::vector<size_t> SlotGameParams::countDenominators()
{
  std::vector<size_t> denominators(numOfReels(), 1);
  denominators[0] = reelsLength(0);
  if (numOfReels() > 1)
    denominators[0] *= reelsLength(1);
  for (size_t i = 1; i < numOfReels() - 1; i++)
  {
    denominators[i] = denominators[i - 1] * reelsLength(i + 1);
  }
  denominators[numOfReels() - 1] = denominators[numOfReels() - 2];
  return denominators;
}
rational_t SlotGameParams::calcRTPUsingFrequency()
{
  std::vector<std::vector<size_t>> tableOfProb(countProbabilityTable());
  std::vector<size_t> denominators(countDenominators());
  size_t numOfComb = numOfCombinations();
  size_t theoreticalScore = 0;
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
      theoreticalScore += wins[i]*(numOfComb/denominators[i]);
    }
  }
  return std::make_pair(theoreticalScore, numOfComb);
}

rational_t SlotGameParams::estimateRTP(size_t numOfStarts, unsigned seed)
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
  return std::make_pair(randomStartsScore, numOfStarts);
}

rational_t SlotGameParams::calcRTPBruteForce()
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
  return std::make_pair(score, numOfCombinations());
}

bool SlotGameParams::pointTest(reels_t reels, wins_t winTable, rational_t realRTP)
{
  assert(reels.size() == (winTable[0]).size());
  SlotGameParams SGP(reels, winTable);
  return ((SGP.calcRTPBruteForce() == realRTP));
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

SlotGameParams::SlotGameParams(SlotGameParams&& o)
  : reels(std::move(o.reels))
  , winTable(std::move(o.winTable))
{}
SlotGameParams &SlotGameParams::operator=(SlotGameParams &&o)
{
  reels = std::move(o.reels);
  winTable = std::move(o.winTable);
  return *this;
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
  //double calcRTPBruteForce =  sampleSlot1.calcRTPBruteForce();
  rational_t calcRTPUsingFrequency = sampleSlot1.calcRTPUsingFrequency();
  rational_t estimateRTP = sampleSlot1.estimateRTP(numOfStarts);
  /*if (calcRTPBruteForce != calcRTPUsingFrequency)
  return false;*/
  double percent = fabs(1 - ((double)estimateRTP.first*calcRTPUsingFrequency.second / ((double)calcRTPUsingFrequency.first*estimateRTP.second))) * 100;
  if (path != "")
  {
    std::ofstream fout(path, std::ios::app);
    if (!fout.is_open())
    {
      throw std::invalid_argument("Can not open file to write results. Check the path you entered:" + path + "\n");
    }
    fout << sampleSlot1.numOfCombinations() << " " << numOfStarts ;
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
