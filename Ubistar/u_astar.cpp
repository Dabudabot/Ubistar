#include "u_astar.h"

#include <chrono>
#include <queue>
#include <iostream>

using namespace ubistar;
using namespace std;
using namespace chrono;

AStar::AStar(std::basic_string<TCHAR> mapPath, BYTE mapRows, BYTE mapCols, BOOL showmap)
  : m_Weight(1.0f), m_ShowMap(showmap), m_Duration(0), m_Cost(0), m_PathFound(false)
{
  m_World = make_unique<World>(mapPath, mapRows, mapCols);
  m_DiagWeight = sqrt(2 * m_Weight * m_Weight);
}

BOOL AStar::FindPath(BYTE startX, BYTE startY, BYTE endX, BYTE endY)
{
  auto start = high_resolution_clock::now();

  m_Start = m_World->GetCoord(startX, startY);
  m_End = m_World->GetCoord(endX, endY);

  if (m_Start->GetTerrainType() == TERRAIN_TYPE::UNDEFINED || 
    m_Start->GetTerrainType() == TERRAIN_TYPE::WATER ||
    m_End->GetTerrainType() == TERRAIN_TYPE::UNDEFINED ||
    m_End->GetTerrainType() == TERRAIN_TYPE::WATER)
  {
    m_PathFound = false;

    auto end = high_resolution_clock::now();
    m_Duration = duration_cast<microseconds>(end - start).count() / 1000.0;
    m_Cost = 0;

    return m_PathFound;
  }

  m_World->ResetValues();

  m_Start->SetTravelCost(0.0f);
  m_Start->MarkAsPath();

  auto cmp = [](Coordinate* l, Coordinate* r)
  {
    if (l->GetTotalCost() == r->GetTotalCost())
    {
      return l->GetH() > r->GetH();
    }

    return l->GetTotalCost() > r->GetTotalCost();
  };

  priority_queue<Coordinate*, vector<Coordinate*>, decltype(cmp)> open(cmp);

  open.push(m_Start);
  Coordinate* current = nullptr;

  while (!open.empty())
  {
    current = open.top();

    if (current == m_End)
    {
      m_PathFound = true;
      break;
    }

    open.pop();
    current->MarkAsChoosen();

    for (const auto& direction : DIRECTIONS)
    {
      auto neighbour = m_World->GetNeighbour(current, direction);

      if (!neighbour) continue;

      auto newG = CalcG(current->GetG(), neighbour->GetTerrainCost(), direction);

      if (!neighbour->IsVisited() || newG < neighbour->GetG())
      {
        neighbour->SetG(newG);
        neighbour->SetParent(current);
        neighbour->SetTravelCost(newG - current->GetG());
      }

      if (!neighbour->IsVisited())
      {
        neighbour->SetH(CalcH(neighbour, m_End));
        neighbour->MarkAsVisited();
        open.push(neighbour);
      }
    }
  }

  if (m_PathFound)
  {
    do
    {
      if (nullptr == current) break;
      current->MarkAsPath();
      m_Cost += current->GetTravelCost();
      current = current->GetParent();
    } while (current != m_Start);
  }

  auto end = high_resolution_clock::now();
  m_Duration = duration_cast<microseconds>(end - start).count() / 1000.0;

  return m_PathFound;
}

FLOAT AStar::CalcH(const Coordinate* const start, const Coordinate* const end)
{
  auto x = abs(start->GetX() - end->GetX());
  auto y = abs(start->GetY() - end->GetY());
  return static_cast<FLOAT>(m_Weight * sqrt((x * x) + (y * y)));
}

FLOAT AStar::CalcG(const FLOAT& currentG, const FLOAT& neighbourT, const DIRECTION& direction)
{
  switch (direction)
  {
  case DIRECTION::N:
  case DIRECTION::E:
  case DIRECTION::S:
  case DIRECTION::W:
    return currentG + (neighbourT * m_Weight);
    break;
  case DIRECTION::NE:
  case DIRECTION::ES:
  case DIRECTION::SW:
  case DIRECTION::WN:
    return currentG + (neighbourT * m_DiagWeight);
    break;
  }

  return 0;
}

VOID AStar::Print()
{
  cout << endl;

  cout << "Start position: (" << static_cast<INT>(GetLastStart()->GetX()) 
    << ", " << static_cast<INT>(GetLastStart()->GetY()) << ")" << endl;
  cout << "End position: (" << static_cast<INT>(GetLastEnd()->GetX())
    << ", " << static_cast<INT>(GetLastEnd()->GetY()) << ")" << endl << endl;

  cout << "Path found: " << (IsLastFound() ? "true" : "false") << endl;
  cout << "Path cost: " << GetLastCost() << endl << endl;

  cout << "Total duration: " << GetLastDuration() << " ms" << endl;

  if (IsMapShown())
  {
    m_World->Print();
  }
}
