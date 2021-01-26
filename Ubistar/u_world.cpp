#include "u_world.h"

#include <fstream>

using namespace ubistar;
using namespace std;

Coordinate::Coordinate()
  : m_x(0), m_y(0), m_g(0), m_h(0),
  m_TerrainType(TERRAIN_TYPE::UNDEFINED),
  m_TerrainCost(0), m_Parent(nullptr),
  m_Visited(false), m_Choosen(false) {}

FLOAT Coordinate::GetTotalCost() const
{
  return m_TerrainCost * (m_g + m_h);
}

VOID Coordinate::SetData(BYTE x, BYTE y, CHAR type)
{
  SetCoords(x, y);
  SetTerrain(type);
}

VOID Coordinate::SetCoords(BYTE x, BYTE y)
{
  m_x = x;
  m_y = y;
}

VOID Coordinate::SetTerrain(CHAR type)
{
  switch (type)
  {
  case '.':
    m_TerrainType = TERRAIN_TYPE::PLAIN;
    m_TerrainCost = 1.0f;
    break;
  case '*':
    m_TerrainType = TERRAIN_TYPE::WATER;
    m_TerrainCost = 0.0f;
    break;
  case '-':
    m_TerrainType = TERRAIN_TYPE::SWAMP;
    m_TerrainCost = 1.5f;
    break;
  case '^':
    m_TerrainType = TERRAIN_TYPE::MOUNTAIN;
    m_TerrainCost = 2.0f;
    break;
  default:
    m_TerrainType = TERRAIN_TYPE::UNDEFINED;
    m_TerrainCost = 0.0f;
    break;
  }
}

VOID Coordinate::ResetValue()
{
  m_g = 0.0f;
  m_h = 0.0f;
  m_Visited = false;
  m_Choosen = false;
  m_Path = false;
  m_Parent = nullptr;
}

VOID Coordinate::MarkAsVisited()
{
  m_Visited = true;
}

VOID Coordinate::MarkAsChoosen()
{
  m_Choosen = true;
}

VOID Coordinate::SetG(FLOAT g)
{
  m_g = g;
}

VOID Coordinate::SetParent(Coordinate* parent)
{
  m_Parent = parent;
}

VOID Coordinate::SetH(FLOAT h)
{
  m_h = h;
}

World::World(std::basic_string<TCHAR> mapPath, BYTE mapRows, BYTE mapCols)
  : m_MapRows(mapRows), m_MapCols(mapCols), m_Coordinates(mapRows* mapCols, Coordinate())
{
  // open file
  basic_fstream<TCHAR> infile(mapPath);

  // read it
  BYTE currentX = 0;
  BYTE currentY = 0;
  TCHAR symbol;

  while (infile >> symbol)
  {
    m_Coordinates[(mapRows * currentY) + currentX].SetTerrain(symbol);
    currentX++;
    if (currentX == mapCols)
    {
      currentX = 0;
      currentY++;
    }

    if (currentY == mapRows)
    {
      break;
    }
  }
}

Coordinate* World::GetCoord(const BYTE& x, const BYTE& y)
{
  return &m_Coordinates[(m_MapRows * y) + x];
}

Coordinate* World::GetNeighbour(const Coordinate* const current, DIRECTION direction)
{
  auto x = current->GetX();
  auto y = current->GetY();

  switch (direction)
  {
  case DIRECTION::N:
  {
    if (y == 0) return nullptr;
    y--;
  }
  break;
  case DIRECTION::E:
  {
    if (x == m_MapCols - 1) return nullptr;
    x++;
  }
  break;
  case DIRECTION::S:
  {
    if (y == m_MapRows - 1) return nullptr;
    y++;
  }
  break;
  case DIRECTION::W:
  {
    if (x == 0) return nullptr;
    x--;
  }
  break;
  case DIRECTION::NE:
  {
    if (y == 0) return nullptr;
    if (x == m_MapCols - 1) return nullptr;
    y--;
    x++;
  }
  break;
  case DIRECTION::ES:
  {
    if (x == m_MapCols - 1) return nullptr;
    if (y == m_MapRows - 1) return nullptr;
    y++;
    x++;
  }
  break;
  case DIRECTION::SW:
  {
    if (y == m_MapRows - 1) return nullptr;
    if (x == 0) return nullptr;
    x--;
    y++;
  }
  break;
  case DIRECTION::WN:
  {
    if (x == 0) return nullptr;
    if (y == 0) return nullptr;
    y--;
    x--;
  }
  break;
  }

  if (m_Coordinates[(m_MapRows * y) + x].GetTerrainType() == TERRAIN_TYPE::WATER ||
    m_Coordinates[(m_MapRows * y) + x].GetTerrainType() == TERRAIN_TYPE::UNDEFINED ||
    m_Coordinates[(m_MapRows * y) + x].IsChoosen())
  {
    return nullptr;
  }

  return &m_Coordinates[(m_MapRows * y) + x];
}

VOID World::ResetValues()
{
  for (auto& coord : m_Coordinates)
  {
    coord.ResetValue();
  }
}