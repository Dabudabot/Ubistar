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
    m_Duration = duration_cast<milliseconds>(end - start).count();
    m_Cost = 0;

    return m_PathFound;
  }

  m_World->ResetValues();

  m_Start->SetTravelCost(0.0f);
  m_Start->MarkAsPath();

  priority_queue<Coordinate*> open;

  open.push(m_Start);
  Coordinate* current;

  while (!open.empty())
  {
    Coordinate* current = open.top();

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

      auto newG = CalcG(current, direction);

      if (!neighbour->IsVisited() || newG < neighbour->GetG())
      {
        neighbour->SetG(newG);
        neighbour->SetParent(current);
        neighbour->SetTravelCost(CalcT(neighbour, direction));
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
      current->MarkAsPath();
      m_Cost += current->GetTravelCost();
      current = current->GetParent();
    } while (current != m_Start);
  }

  auto end = std::chrono::high_resolution_clock::now();
  m_Duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();

  return m_PathFound;
}

FLOAT AStar::CalcH(const Coordinate* const start, const Coordinate* const end)
{
  auto x = abs(start->GetX() - end->GetX());
  auto y = abs(start->GetY() - end->GetY());
  return static_cast<FLOAT>(m_Weight * sqrt((x * x) + (y * y)));
}

FLOAT AStar::CalcT(const Coordinate* const current, const DIRECTION& direction)
{
  switch (direction)
  {
  case DIRECTION::N:
  case DIRECTION::E:
  case DIRECTION::S:
  case DIRECTION::W:
    return current->GetTerrainCost() + m_Weight;
    break;
  case DIRECTION::NE:
  case DIRECTION::ES:
  case DIRECTION::SW:
  case DIRECTION::WN:
    return current->GetTerrainCost() + m_DiagWeight;
    break;
  }
}

FLOAT AStar::CalcG(const Coordinate* const current, const DIRECTION& direction)
{
  switch (direction)
  {
  case DIRECTION::N:
  case DIRECTION::E:
  case DIRECTION::S:
  case DIRECTION::W:
    return current->GetG() + m_Weight;
    break;
  case DIRECTION::NE:
  case DIRECTION::ES:
  case DIRECTION::SW:
  case DIRECTION::WN:
    return current->GetG() + m_DiagWeight;
    break;
  }
}

std::ostream& ubistar::operator<<(std::ostream& os, const AStar& a)
{
  os << endl;

  os << "Start position: " << a.GetLastStart() << endl;
  os << "End position: " << a.GetLastEnd() << endl << endl;

  os << "Path found: " << (a.IsLastFound() ? "true" : "false") << endl;
  os << "Path cost: " << a.GetLastCost() << endl << endl;

  os << "Total duration: " << a.GetLastDuration() << endl;

  if (a.IsMapShown())
  {
    os << endl << a.m_World << endl;
  }

  return os;
}
