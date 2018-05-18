#include "slots.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>

int main()
{
  try
  {
    std::vector<std::vector<size_t>> A;
    std::vector<std::vector<size_t>> reels1 = { 
      { 1, 2, 4, 4, 3 },
      { 2, 3, 1, 2, 4, 3 },
      { 3, 2, 1, 4, 4, 2 }
    };
    std::vector<std::vector<size_t>> reels2 = { 
      { 1,2,3 },
      { 2,3,3,1 },
      { 3,1,1,2 }
    };
    std::vector<std::vector<size_t>> winTable1 = { 
      { 1, 2, 7 },
      { 0, 1, 8 },
      { 0, 3, 6 },
      { 0, 2, 8 } 
    };
    std::vector<std::vector<size_t>> winTable2 = { 
      { 0,1,7 },
      { 0,1,9 },
      { 0,1,6 } 
    };

    std::cout << SlotGameParams::pointTest(reels1, winTable1, std::make_pair(177,180)) << std::endl;
    std::cout << SlotGameParams::pointTest(reels2, winTable2, std::make_pair(46, 48)) << std::endl;

    std::string pathReels = "../reels.txt";
    std::string pathWinTable = "../winTable.txt";
    std::ifstream fin1(pathReels);
    std::ifstream fin2(pathWinTable);
    
    SlotGameParams sampleSlot(readReels(fin1),readWinTable(fin2));
    std::cout << "\nCalculated Practically RTP : " << sampleSlot.calcRTPBruteForce().first << "  " << sampleSlot.calcRTPBruteForce().second;
    std::cout << "\nCalculated in theory RTP: " << sampleSlot.calcRTPUsingFrequency().first << " " << sampleSlot.calcRTPUsingFrequency().second;

    for (int i = 0; i < 4; i++)
    {
      for (int j = 0; j < 5; j++)
      {
        //std::cout << i * 5 + j + 1 << " :\n";
        //std::cout << randomParamsTest(std::chrono::system_clock::now().time_since_epoch().count(), pow(10, i + 2), "../test/randtests.txt") << std::endl << std::endl;
        randomParamsTest(std::chrono::system_clock::now().time_since_epoch().count(), pow(10, i + 2)/*, "../test/randtests.txt"*/);
      }
    }
  }
  catch (std::exception err)
  {
    std::cout << err.what();
  }
  system("pause");
  return 0;
}