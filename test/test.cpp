#include "slots.h"

int main()
{
	string pathReels = "../reels.txt";
	string pathWinTable = "../winTable.txt";
	SlotGameParams sampleSlot = SlotGameParams(3, 4);
	sampleSlot.readWinTable(&pathWinTable);
	sampleSlot.readReelsValue(&pathReels);
	//double teoScore = sampleSlot.theoreticalWin();
	//double totScore = sampleSlot.everyStartsWin();
	//double randScore = sampleSlot.randomStartsWin(0);

	cout << "\nProbabilistic average win : " << sampleSlot.randomStartsWin(0) << endl;
	cout << "\nReal average win : " << sampleSlot.everyStartsWin() << endl;
	cout << "\nTheoretical average win : " << sampleSlot.theoreticalWin() << endl << endl;

	system("pause");
	return 0;
}