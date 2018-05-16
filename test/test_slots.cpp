#include "slots.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <random>

testStruct sampleTP1 = { {{ 1, 2, 4, 4, 3},{2, 3, 1, 2, 4, 3},{3, 2, 1, 4, 4, 2} }, 
{ {1, 2, 7},{0, 1, 8},{0, 3, 6},{0, 2, 8} },
0.983333 };
testStruct sampleTP2 = { {{1,2,3},{2,3,3,1},{3,1,1,2} }, {{0,1,7},{0,1,9},{0,1,6}}, 46.0 / 48 };

int main()
{
  try
  {
    std::cout << SlotGameParams::pointTest(sampleTP1) << std::endl;
    std::cout << SlotGameParams::pointTest(sampleTP2) << std::endl;
    for (int i = 0; i < 5; i++)
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