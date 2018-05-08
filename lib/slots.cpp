#include "slots.h"

SlotGameParams::SlotGameParams(int _numOfReels, int _numOfFruit)
{
  numOfReels = _numOfReels;
  numOfFruit = _numOfFruit;
  reelsLength = vector<int>(_numOfReels);
  reelsValue = vector<vector<int>>(_numOfReels);
  winTable = vector<vector<double>>(_numOfFruit, vector<double>(_numOfReels));
  tableOfProb = vector<vector<double>>(_numOfReels, vector<double>(_numOfFruit, 0));
}

void SlotGameParams::readReelsValue(string const & pathReelsValue)
{
  ifstream fin(pathReelsValue);
  if (!fin.is_open())
  {
    throw ERROR_OPEN_REELS_FAILED;
  }
  for (int i = 0; i < numOfReels; i++)
  {
    fin >> (reelsLength)[i];
    (reelsValue)[i].resize((reelsLength)[i]);
  }
  for (int i = 0; i < numOfReels; i++)
  {

    for (int j = 0; j < (reelsLength)[i]; j++)
    {
      fin >> (reelsValue)[i][j];
    }
  }
  numOfCombinations = 1;
  for (int i = 0; i < numOfReels; i++)
  {
    numOfCombinations *= reelsLength[i];
  }
}

void SlotGameParams::readWinTable(string const & pathWinTable)
{
  ifstream fin(pathWinTable);
  if (!fin.is_open())
  {
    throw ERROR_OPEN_WIN_TABLE_FAILED;
  }
    for (int i = 0; i < numOfFruit; i++)
    {
      for (int j = 0; j < numOfReels; j++)
      {
        fin >> (winTable)[i][j];
      }
    }
}

bool SlotGameParams::countProbabilityTable()
{
  for (int i = 0; i < numOfReels; i++)
  {
    for (int j = 0; j < reelsLength[i]; j++)
    {
      tableOfProb[i][(reelsValue[i][j] - 1)]++;
    }
    for (int j = 0; j < numOfFruit; j++)
    {
      tableOfProb[i][j] /= reelsLength[i];
    }
  }
  return true;
}

double SlotGameParams::theoreticalWin()
{
  this->countProbabilityTable();
  double theoreticalScore = 0;
  vector<double> wins(numOfReels);
  for (int f = 0; f < numOfFruit; f++)
  {
    wins = winTable[f];
    for (int i = 0; i < numOfReels; i++)
    {
      for (int k = 0; k <= i; k++)
      {
        wins[i] *= tableOfProb[k][f];
      }
      if (i + 1 < numOfReels)
      {
        wins[i] *= (1 - tableOfProb[i + 1][f]);
      }
      theoreticalScore += wins[i];
    }
  }
  return theoreticalScore;
}

double SlotGameParams::randomStartsWin(int numOfStarts)
{
  //srand(time(NULL));

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
  std::uniform_int_distribution<int> distribution(1, numOfCombinations - 1);
  if (numOfStarts < 1)
  {
    numOfStarts = numOfCombinations;
  }
  double randomStartsScore = 0;
  vector<int> tempComb(numOfReels);
  for (int i = 0; i < numOfStarts; i++)
  {
    for (int j = 0; j < numOfReels; j++)
    {
      tempComb[j] = reelsValue[j][distribution(generator) % reelsLength[j]];
    }
    randomStartsScore += winTable[tempComb[0] - 1][checkLine(tempComb) - 1];
  }
  return randomStartsScore / (numOfStarts);
}

double SlotGameParams::everyStartsWin()
{
  vector<int> tempComb(numOfReels);
  double totalScore = 0;
  make_permutation(0, totalScore, tempComb);
  return totalScore/ numOfCombinations;
}

void SlotGameParams::make_permutation(int j, double &totalScore, vector<int>& tempComb)
{
  for (int i = 0; i < reelsLength[j]; i++)
  {
    tempComb[j] = reelsValue[j][i];
    if (j == (numOfReels - 1))
    {
      totalScore += winTable[tempComb[0] - 1][checkLine(tempComb) - 1];
    }
    else
    {
      make_permutation(j + 1, totalScore, tempComb);
    }
  }
}

int checkLine(vector<int>& line)
{
  for (int i = 1; i < line.size(); i++)
  {
    if (line[i - 1] != line[i])
      return i;
  }
  return line.capacity();
}