#include "slots.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <cassert>
#include <cstddef>

typedef std::uniform_int_distribution<size_t> ud;

reels_t readReels(std::istream &fin)
{
  reels_t reels;

  size_t reels_size = 0;
  fin >> reels_size;
  reels.resize(reels_size);

  for(auto &reel: reels)
  {
    size_t reel_size = 0;
    fin >> reel_size;
    reel.resize(reel_size);

    for(auto &fruit: reel)
      fin >> fruit;
  }

  return std::move(reels);
}
wins_t readWinTable(std::istream &fin, size_t reels_size)
{
  wins_t winTable;

  size_t numOfFruit = 0;
  fin >> numOfFruit;
  winTable.resize(numOfFruit);

  for(auto &win_line: winTable)
  {
    win_line.resize(reels_size);

    for(auto &win: win_line)
      fin >> win;
  }

  return std::move(winTable);
}
template <class rng_t>
reels_t random_reels(rng_t &rng, size_t reel_count, size_t fruit_count)
{
  reels_t reels( reel_count );

  for (auto &reel: reels)
  {
    reel.resize( ud(1,9)(rng) );
    for (auto &fruit: reel)
      fruit = ud(0,fruit_count-1)(rng);
  }

  return std::move(reels);
}
template <class rng_t>
wins_t random_wins(rng_t &rng, size_t reel_count, size_t fruit_count)
{
  wins_t wins( fruit_count );

  for (auto &win_line: wins)
  {
    win_line.resize( reel_count );
    for (auto &win: win_line)
      win = ud(0,10)(rng);

    std::sort(win_line.begin(), win_line.end());
  }

  return std::move(wins);
}

SlotGameParams::SlotGameParams(size_t numOfReels, size_t numOfFruit)
  : reels(numOfReels)
  , winTable(numOfFruit, win_line_t(numOfReels))
{}
SlotGameParams::SlotGameParams(reels_t const &reels, wins_t const &winTable) 
  : reels(reels), winTable(winTable)
{}
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
SlotGameParams SlotGameParams::randomReels(unsigned seed)
{
  std::default_random_engine rng(seed);
  size_t reel_count = ud(2,6)(rng);
  size_t fruit_count = ud(3,7)(rng);
  auto reels = random_reels(rng, reel_count, fruit_count);
  auto wins = random_wins(rng, reel_count, fruit_count);
  return SlotGameParams(reels, wins);
}

size_t SlotGameParams::numOfReels() const
{
  return reels.size();
}
size_t SlotGameParams::numOfFruit() const
{
  return winTable.size();
}
size_t SlotGameParams::reelLength(size_t reel_no) const
{
  assert(reel_no < numOfReels());
  return reels[reel_no].size();
}
size_t SlotGameParams::numOfCombinations() const
{
  size_t numOfCombs = 1;
  for (size_t i = 0; i < numOfReels(); i++)
  {
     numOfCombs *= reelLength(i);
  }
  return numOfCombs;
}

size_t checkLine(std::vector<size_t> const & line)
{
  size_t i;
  for (i = 0; i+1 < line.size() && line[i] == line[i+1]; i++);
  return i;
}

bool SlotGameParams::inc(iline_t& line) const
{
  size_t i = 0;

  for(; i != numOfReels(); line[i++] = 0)
    if(++line[i] < reelLength(i))
      break;

  return i != numOfReels();
}
prob_table_t SlotGameParams::countProbabilityTable() const
{
  prob_table_t tableOfProb(numOfReels(), std::vector<size_t>(numOfFruit(), 0));

  for (size_t i = 0; i < numOfReels(); i++)
  {
    for (size_t j = 0; j < reelLength(i); j++)
      tableOfProb[i][ reels[i][j] ]++;
  }

  return std::move(tableOfProb);
}
denominators_t SlotGameParams::countDenominators() const
{
  denominators_t denominators(numOfReels(), 1);

  denominators[0] = reelLength(0);
  if (numOfReels() > 1)
    denominators[0] *= reelLength(1);
  for (size_t i = 1; i < numOfReels() - 1; i++)
  {
    denominators[i] = denominators[i - 1] * reelLength(i + 1);
  }
  denominators[numOfReels() - 1] = denominators[numOfReels() - 2];

  return std::move(denominators);
}

rational_t SlotGameParams::calcRTPUsingFrequency() const
{
  size_t score = 0;

  size_t numOfComb = numOfCombinations();
  prob_table_t tableOfProb = countProbabilityTable();
  denominators_t denominators = countDenominators();
  std::vector<size_t> wins(numOfReels());

  for (size_t f = 0; f < numOfFruit(); f++)
  {
    for (size_t i = 0; i < numOfReels(); i++)
    {
      wins[i] = winTable[f][i];
      for (size_t k = 0; k <= i; k++)
        wins[i] *= tableOfProb[k][f];

      if (i + 1 < numOfReels())
        wins[i] *= reelLength(i + 1) - tableOfProb[i + 1][f];

      score += wins[i]*(numOfComb/denominators[i]);
    }
  }

  return std::make_pair(score, numOfComb);
}
rational_t SlotGameParams::estimateRTP(size_t numOfStarts, unsigned seed) const
{
  std::default_random_engine rng(seed);
  typedef std::uniform_int_distribution<size_t> ud;

  size_t score = 0;

  line_t line(numOfReels());
  for (size_t i = 0; i < numOfStarts; i++)
  {
    for (size_t j = 0; j < numOfReels(); j++)
      line[j] = reels[j][ ud(0, reelLength(j)-1)(rng) ];

    score += winTable[ line[0] ][ checkLine(line) ];
  }

  return std::make_pair(score, numOfStarts);
}
rational_t SlotGameParams::calcRTPBruteForce() const
{
  size_t score = 0;

  iline_t ireels(numOfReels(), 0);
  line_t line(numOfReels());
  do
  {
    for (int j = 0; j < numOfReels(); j++)
      line[j] = reels[j][ ireels[j] ];

    score += winTable[ line[0] ][ checkLine(line) ];
  } while (inc(ireels));

  return std::make_pair(score, numOfCombinations());
}

