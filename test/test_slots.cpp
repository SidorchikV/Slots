#include "slots.h"
#include <iostream>
#include <cassert>

bool pointTest(reels_t const &reels, wins_t const &winTable, rational_t realRTP);
bool randomParamsTest(unsigned seed = 31, size_t numOfStarts = 0);

bool pointTest(reels_t const &reels, wins_t const &winTable, rational_t realRTP)
{
  assert(reels.size() == (winTable[0]).size());
  SlotGameParams SGP(reels, winTable);
  return SGP.calcRTPBruteForce() == realRTP;
}
bool randomParamsTest(unsigned seed, size_t numOfStarts)
{
  auto g = SlotGameParams::randomReels(seed);
  //auto RTP = g.calcRTPBruteForce();
  auto RTP = g.calcRTPUsingFrequency();
  auto RTPest = g.estimateRTP(numOfStarts);

  double dRTP = double(RTP.first) / double(RTP.second);
  double dRTPest = double(RTPest.first) / double(RTPest.second);
  double delta = fabs(dRTP - dRTPest);
  double const eps = 0.01;

  std::cout 
    << "numOfCombinations = " << g.numOfCombinations() << std::endl
    << "numOfStarts = " << numOfStarts << std::endl
    << "RTP = " << RTP.first << " / " << RTP.second << " = " << dRTP << std::endl
    << "RTPest = " << dRTPest << std::endl
    << "delta = " << delta << std::endl
    << std::endl
    ;

  return delta < eps;
}

int main()
{
  reels_t reels1 = { 
    { 0, 1, 3, 3, 2 },
    { 1, 2, 0, 1, 3, 2 },
    { 2, 1, 0, 3, 3, 1 }
  };
  reels_t reels2 = { 
    { 0,1,2 },
    { 1,2,2,0 },
    { 2,0,0,1 }
  };
  wins_t winTable1 = { 
    { 1, 2, 7 },
    { 0, 1, 8 },
    { 0, 3, 6 },
    { 0, 2, 8 } 
  };
  wins_t winTable2 = { 
    { 0,1,7 },
    { 0,1,9 },
    { 0,1,6 } 
  };

  std::cout << pointTest(reels1, winTable1, std::make_pair(177,180)) << std::endl;
  std::cout << pointTest(reels2, winTable2, std::make_pair(46, 48)) << std::endl;

  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 5; j++)
      randomParamsTest(i+7*j, pow(10, i + 2));

  return 0;
}