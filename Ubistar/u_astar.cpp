#include "u_astar.h"

#include <iostream>

using namespace ubistar;
using namespace std;

ostream& operator<<(ostream& os, const Coordinate& c)
{
  os << "(" << c.GetX() << ", " << c.GetY() << ")";

  return os;
}

ostream& operator<<(ostream& os, const AStar& a)
{
  os << endl;

  os << "Start position: " << a.GetLastStart() << endl;
  os << "End position: " << a.GetLastEnd() << endl << endl;

  os << "Path found: " << (a.isLastFound() ? "true" : "false") << endl;
  os << "Path cost: " << a.GetLastCost() << endl << endl;

  os << "Total duration: " << a.GetLastDuration().count().count() << endl;

  return os;
}
