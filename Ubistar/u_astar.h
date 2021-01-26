#pragma once

#include "u_world.h"

#include <Windows.h>
#include <string>
#include <memory>

namespace ubistar
{
  class AStar
  {
  public:

    AStar(std::basic_string<TCHAR> mapPath, BYTE mapRows, BYTE mapCols, BOOL showmap);
    ~AStar() = default;

    BOOL FindPath(BYTE startX, BYTE startY, BYTE endX, BYTE endY);

    DOUBLE GetLastDuration() const { return m_Duration; }
    BOOL IsLastFound() const { return m_PathFound; }
    BOOL IsMapShown() const { return m_ShowMap; }
    Coordinate* GetLastStart() const { return m_Start; }
    Coordinate* GetLastEnd() const { return m_End; }
    DOUBLE GetLastCost() const { return m_Cost; }

    friend std::ostream& operator<<(std::ostream& os, const AStar& a);

  private:

    FLOAT CalcT(const Coordinate* const current, const DIRECTION& direction);
    FLOAT CalcH(const Coordinate* const start, const Coordinate* const end);
    FLOAT CalcG(const Coordinate* const current, const DIRECTION& direction);

    std::unique_ptr<World> m_World;

    FLOAT m_Weight;
    FLOAT m_DiagWeight;

    Coordinate* m_Start;
    Coordinate* m_End;
    BOOL m_ShowMap;

    DOUBLE m_Duration;
    DOUBLE m_Cost;
    BOOL m_PathFound;
  };
}