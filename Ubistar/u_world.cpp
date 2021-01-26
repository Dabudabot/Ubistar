#include "u_world.h"

#include <fstream>
#include <iostream>

using namespace ubistar;
using namespace std;

Coordinate::Coordinate()
  : m_x(0), m_y(0), m_g(0), m_h(0),
  m_TerrainType(TERRAIN_TYPE::UNDEFINED),
  m_TerrainCost(0), m_Parent(nullptr),
  m_Visited(false), m_Choosen(false),
  m_Path(false), m_TravelCost(0) {}

FLOAT Coordinate::GetTotalCost() const
{
  return (m_g + m_h);
}

VOID Coordinate::SetData(BYTE x, BYTE y, TCHAR type)
{
  SetCoords(x, y);
  SetTerrain(type);
}

VOID Coordinate::SetCoords(BYTE x, BYTE y)
{
  m_x = x;
  m_y = y;
}

VOID Coordinate::SetTerrain(TCHAR type)
{
  switch (type)
  {
  case _T('.'):
    m_TerrainType = TERRAIN_TYPE::PLAIN;
    m_TerrainCost = 1.0f;
    break;
  case _T('*'):
    m_TerrainType = TERRAIN_TYPE::WATER;
    m_TerrainCost = 0.0f;
    break;
  case _T('-'):
    m_TerrainType = TERRAIN_TYPE::SWAMP;
    m_TerrainCost = 1.5f;
    break;
  case _T('^'):
    m_TerrainType = TERRAIN_TYPE::MOUNTAIN;
    m_TerrainCost = 2.0f;
    break;
  default:
    m_TerrainType = TERRAIN_TYPE::UNDEFINED;
    m_TerrainCost = 0.0f;
    break;
  }
}

CHAR Coordinate::GetTerrainTypeAsSym() const
{
  if (m_Path) return '+';

  switch (m_TerrainType)
  {
  case TERRAIN_TYPE::PLAIN:
    return '.';
  case TERRAIN_TYPE::WATER:
    return '*';
  case TERRAIN_TYPE::SWAMP:
    return '-';
  case TERRAIN_TYPE::MOUNTAIN:
    return '^';
  default:
    return ' ';
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

World::World(std::basic_string<TCHAR> mapPath, size_t mapRows, size_t mapCols)
  : m_MapRows(mapRows), m_MapCols(mapCols), m_Coordinates(mapRows * mapCols, Coordinate())
{
  // open file
  basic_fstream<TCHAR> infile(mapPath);

  // read it
  BYTE currentX = 0;
  BYTE currentY = 0;
  TCHAR symbol;

  while (infile >> symbol)
  {
    m_Coordinates[(mapRows * currentY) + currentX].SetData(currentX, currentY, symbol);
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
  return &m_Coordinates[(static_cast<size_t>(m_MapRows) * y) + x];
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

VOID World::Print()
{
  cout << endl;
  for (size_t y = 0; y < m_MapRows; y++)
  {
    for (size_t x = 0; x < m_MapCols; x++)
    {
      cout << m_Coordinates[(m_MapRows * y) + x].GetTerrainTypeAsSym();
    }
    cout << endl;
  }
}
