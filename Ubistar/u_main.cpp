/*!
 *  \brief     Solution entry point
 *  \details   Execution starts here
 *  \author    Daulet Tumbayev
 *  \date      2021
 */

/************************************************
 *  Includes
 ***********************************************/

#include "u_astar.h"

#include <Windows.h>
#include <tchar.h>
#include <tuple>
#include <string>
#include <memory>
#include <iostream>
#include <shlwapi.h>

/************************************************
 *  Namespaces
 ***********************************************/

using namespace ubistar;
using namespace std;

/************************************************
 *  Shortcuts & global constants
 ***********************************************/

// just to shorten
using InputTuple = tuple<basic_string<TCHAR>, BYTE, BYTE, BYTE, BYTE, BOOL>;

constexpr BYTE MAP_ROWS = 126;
constexpr BYTE MAP_COLS = 126;

/************************************************
 *  Forward declaration
 ***********************************************/

 /*!
 *  Processing input
 *  \param argc equals to 6 or 7
 *  \param argv contains the following pattern:
 *              astar.exe MapFileName StartX StartY EndX EndY [showmap]
 *              where MapFileName - path to file with map (read more in README)
 *                    StartX StartY - start position
 *                    EndX EndY - end position
 *                    showmap - if specified, show the output map
 *  \return tuple with inputs {MapFileName, StartX, StartY, EndX, EndY, showmap}
 */
InputTuple ProcessInput(const int& argc, TCHAR* argv[]);

/************************************************
 *  Executable entry point
 ***********************************************/

/*!
*  Executable entry point
*  \param argc equals to 6 or 7
*  \param argv contains the following pattern:
*              astar.exe MapFileName StartX StartY EndX EndY [showmap]
*              where MapFileName - path to file with map (read more in README)
*                    StartX StartY - start position
*                    EndX EndY - end position
*                    showmap - if specified, show the output map
*  \return 0 in success, or error code
*/
int _tmain(int argc, TCHAR* argv[])
{
  // unpacking input params
  basic_string<TCHAR> mapPath;
  BYTE startX;
  BYTE startY;
  BYTE endX;
  BYTE endY;
  BOOL showmap;

  try
  {
    tie(mapPath, startX, startY, endX, endY, showmap) = ProcessInput(argc, argv);
  }
  catch (exception& e)
  {
    cerr << e.what() << endl;
    return ERROR_INVALID_DATA;
  }

  unique_ptr<AStar> pathFinder = make_unique<AStar>(mapPath, MAP_COLS, MAP_ROWS, showmap);
  auto result = pathFinder->FindPath(startX, startY, endX, endY);
  pathFinder->Print();
}

/************************************************
 *  Functions impl
 ***********************************************/

BYTE ProcessInputNumber(TCHAR* input, const BYTE& min, const BYTE& max)
{
  auto temp = stoi(input);

  if (temp < min || temp > max)
  {
    throw runtime_error("Input coordinate is out of range");
  }

  return static_cast<BYTE>(temp);
}

InputTuple ProcessInput(const int& argc, TCHAR* argv[])
{
  // according to spec there could be 6 or 7 args
  constexpr BYTE MIN_INPUT_AMOUNT = 6;
  constexpr BYTE MAX_INPUT_AMOUNT = 7;

  // map is grid 126 x 126, with start in 0, 0
  constexpr BYTE MIN_MAP_X = 0;
  constexpr BYTE MIN_MAP_Y = 0;
  constexpr BYTE MAX_MAP_X = MAP_COLS - 1;
  constexpr BYTE MAX_MAP_Y = MAP_ROWS - 1;

  // value which is present if we show map
  LPCTSTR SHOW_MAP = _T("showmap");
  
  if (argc < MIN_INPUT_AMOUNT || argc > MAX_INPUT_AMOUNT)
  {
    throw runtime_error("Amount of input args are wrong");
  }

  if (!PathFileExists(argv[1]))
  {
    throw runtime_error("Path to map file is wrong");
  }

  basic_string<TCHAR> mapPath = argv[1];
  BYTE startX = ProcessInputNumber(argv[2], MIN_MAP_X, MAX_MAP_X);
  BYTE startY = ProcessInputNumber(argv[3], MIN_MAP_Y, MAX_MAP_Y);
  BYTE endX = ProcessInputNumber(argv[4], MIN_MAP_X, MAX_MAP_X);
  BYTE endY = ProcessInputNumber(argv[5], MIN_MAP_Y, MAX_MAP_Y);
  BOOL showmap = false;

  // do we have last arg?
  if (MAX_INPUT_AMOUNT == argc && !_tcscmp(argv[6], SHOW_MAP))
  {
    showmap = true;
  }

  return { mapPath, startX, startY, endX, endY, showmap };
}