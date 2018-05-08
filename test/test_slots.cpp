#include "slots.h"
#include <iostream>

int main()
{
  std::string pathReels = "../reels.txt";
  std::string pathWinTable = "../winTable.txt";
  SlotGameParams sampleSlot = SlotGameParams(3, 4);
  try
  {
    sampleSlot.readWinTable(pathWinTable);
    sampleSlot.readReelsValue(pathReels);
    std::cout << "\nProbabilistic average win (default parameters): " << sampleSlot.randomStartsWin(0) << std::endl;
    std::cout << "\nProbabilistic average win : " << sampleSlot.randomStartsWin(5000, 789) << std::endl;
    std::cout << "\nReal average win : " << sampleSlot.everyStartsWin() << std::endl;
    std::cout << "\nTheoretical average win : " << sampleSlot.theoreticalWin() << std::endl << std::endl;
  }
  catch (std::exception err)
  {
    std::cout << err.what() << std::endl;
  }
  system("pause");
  return 0;
}