#include "slots.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>

typedef struct testParams
{
  size_t numOfReels;
  size_t numOfFruit;
  size_t numOfStarts;
  size_t seed;
};
void readTestParam(std::string const & pathTestParam, testParams & tParams)
{
  std::ifstream fin(pathTestParam);
  if (!fin.is_open())
  {
    throw std::invalid_argument("Can not open file with slot params. Check the path you entered: " + pathTestParam + "\n");
  }
  fin >> tParams.numOfReels;
  fin >> tParams.numOfFruit;
  fin >> tParams.numOfStarts;
  fin >> tParams.seed;
}

testStruct sampleTP = { {{ 1, 2, 4, 4, 3},{2, 3, 1, 2, 4, 3},{3, 2, 1, 4, 4, 2} }, 
{ {1, 2, 7},{0, 1, 8},{0, 3, 6},{0, 2, 8} },
0.983333 };

int main()
{
  std::cout << SlotGameParams::pointTest(sampleTP) << std::endl;
  /*std::string pathReels;
  std::string pathWinTable;
  std::string path = "../test/test_cases/";
  testParams tP;
  for (int i = 1; i <= 6; i++)
  {
    try
    {
      std::cout << "\nTest #" << i << std::endl;
      pathReels = path + std::to_string(i) + "/reels.txt";
      pathWinTable = path + std::to_string(i) + "/winTable.txt";
      readTestParam(path + std::to_string(i) + "/testParams.txt", tP);
      SlotGameParams sampleSlot(tP.numOfReels, tP.numOfFruit);
      sampleSlot.readWinTable(pathWinTable);
      sampleSlot.readReels(pathReels);
      std::cout << "\nCalculated RTP : " << sampleSlot.calcRTP();
      std::cout << "\nEstimate RTP: " << sampleSlot.estimateRTP();
      if (tP.seed <= 0)
        std::cout << "\nApproximate RTP (default parameters): " << sampleSlot.approxRTP(tP.numOfStarts) << std::endl;
      else
        std::cout << "\nApproximate RTP: " << sampleSlot.approxRTP(tP.numOfStarts, tP.seed) << std::endl;
    }
    catch (std::exception err)
    {
      std::cout << err.what();
    }
  }*/
 /* std::cout << "\n\nRandom parameters:" << std::endl;
  std::cout << "\nTest #7" << std::endl;
  auto sampleSlot1 = SlotGameParams::randomReels();
  std::cout << "\nCalculated RTP : " << sampleSlot1.calcRTP();
  std::cout << "\nEstimate RTP: " << sampleSlot1.estimateRTP();
  std::cout << "\nApproximate RTP (default parameters): " << sampleSlot1.approxRTP(0) << std::endl;
  std::cout << "\nTest #8" << std::endl;
  auto sampleSlot2 = SlotGameParams::randomReels(std::chrono::system_clock::now().time_since_epoch().count());
  std::cout << "\nCalculated RTP : " << sampleSlot2.calcRTP();
  std::cout << "\nEstimate RTP: " << sampleSlot2.estimateRTP();
  std::cout << "\nApproximate RTP (default parameters): " << sampleSlot2.approxRTP(0) << std::endl;*/
  system("pause");
  return 0;
}