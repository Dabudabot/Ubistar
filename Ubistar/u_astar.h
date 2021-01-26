#pragma once

#include <Windows.h>
#include <tchar.h>
#include <chrono>
#include <string>

namespace ubistar
{
  class Coordinate
  {
  public:

    Coordinate(BYTE x, BYTE y);
    ~Coordinate();

    BYTE GetX() const { return m_x; };
    BYTE GetY() const { return m_y; };

    friend ostream& operator<<(ostream& os, const Coordinate& c);

  private:

    BYTE m_x;
    BYTE m_y;
  };

  class AStar
  {
  public:

    AStar(std::basic_string<TCHAR> mapPath, BOOL showmap);
    ~AStar();

    BOOL FindPath(BYTE startX, BYTE startY, BYTE endX, BYTE endY);

    std::chrono::duration<std::chrono::milliseconds> GetLastDuration() const { return m_Duration; }
    BOOL isLastFound() const { return m_PathFound; }
    BOOL isMapShown() const { return m_ShowMap; }
    Coordinate GetLastStart() const { return m_Start; }
    Coordinate GetLastEnd() const { return m_End; }
    DOUBLE GetLastCost() const { return m_Cost; }

    VOID SetMapVisibility(BOOL showmap);

    friend ostream& operator<<(ostream& os, const AStar& a);

  private:

    std::basic_string<TCHAR> m_MapPath;
    Coordinate m_Start;
    Coordinate m_End;
    BOOL m_ShowMap;

    std::chrono::duration<std::chrono::milliseconds> m_Duration;
    DOUBLE m_Cost;
    BOOL m_PathFound;

  };
}