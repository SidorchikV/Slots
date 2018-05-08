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
  catch (int er)
  {
    switch (er)
    {
    case ERROR_OPEN_REELS_FAILED:
      cout << "Can not open file with reels values." << endl;
      break;
    case ERROR_OPEN_WIN_TABLE_FAILED:
      cout << "Can not open file with win table values." << endl;
      break;
    default:
      cout << "error!" << endl;
      break;
    }
  }
  system("pause");
  return 0;
}