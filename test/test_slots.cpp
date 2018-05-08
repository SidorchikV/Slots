#include "slots.h"

int main()
{
  string pathReels = "../reels.txt";
  string pathWinTable = "../winTable.txt";
  SlotGameParams sampleSlot = SlotGameParams(3, 4);
  try
  {
    sampleSlot.readWinTable(pathWinTable);
    sampleSlot.readReelsValue(pathReels);
    cout << "\nProbabilistic average win : " << sampleSlot.randomStartsWin(0) << endl;
    cout << "\nReal average win : " << sampleSlot.everyStartsWin() << endl;
    cout << "\nTheoretical average win : " << sampleSlot.theoreticalWin() << endl << endl;
  }
  catch (std::exception err)
  {
    cout << err.what() << endl;
  }
  system("pause");
  return 0;
}