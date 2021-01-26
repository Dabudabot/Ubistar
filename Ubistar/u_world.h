#pragma once

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>

namespace ubistar
{
  enum class DIRECTION
  {
    N,
    E,
    S,
    W,
    NE,
    ES,
    SW,
    WN
  };

  enum class TERRAIN_TYPE
  {
    UNDEFINED,
    PLAIN,
    WATER,
    SWAMP,
    MOUNTAIN
  };

  const DIRECTION DIRECTIONS[] =
  {
    DIRECTION::N,
    DIRECTION::E,
    DIRECTION::S,
    DIRECTION::W,
    DIRECTION::NE,
    DIRECTION::ES,
    DIRECTION::SW,
    DIRECTION::WN
  };

  class Coordinate
  {
  public:

    Coordinate();
    ~Coordinate() = default;

    BYTE GetX() const { return m_x; }
    BYTE GetY() const { return m_y; }
    FLOAT GetG() const { return m_g; }
    FLOAT GetH() const { return m_h; }

    FLOAT GetTotalCost() const;
    FLOAT GetTerrainCost() const { return m_TerrainCost; }
    TERRAIN_TYPE GetTerrainType() const { return m_TerrainType; }
    CHAR GetTerrainTypeAsSym() const;
    BOOL IsVisited() const { return m_Visited; }
    BOOL IsChoosen() const { return m_Choosen; }
    Coordinate* GetParent() const { return m_Parent; }

    VOID SetData(BYTE x, BYTE y, TCHAR type);
    VOID SetTerrain(TCHAR type);
    VOID SetCoords(BYTE x, BYTE y);
    VOID ResetValue();
    VOID MarkAsVisited() { m_Visited = true; }
    VOID MarkAsChoosen() { m_Choosen = true; }
    VOID SetG(FLOAT g) { m_g = g; }
    VOID SetParent(Coordinate* parent) { m_Parent = parent; }
    VOID SetH(FLOAT h) { m_h = h; }

    VOID SetTravelCost(FLOAT cost) { m_TravelCost = cost; }
    FLOAT GetTravelCost() { return m_TravelCost; }

    VOID MarkAsPath() { m_Path = true; }

  private:

    BYTE m_x;
    BYTE m_y;

    FLOAT m_g;
    FLOAT m_h;

    BOOL m_Choosen;
    BOOL m_Visited;
    BOOL m_Path;

    TERRAIN_TYPE m_TerrainType;
    FLOAT m_TerrainCost;
    FLOAT m_TravelCost;

    Coordinate* m_Parent;
  };

  class World
  {
  public:

    World(std::basic_string<TCHAR> mapPath, size_t mapRows, size_t mapCols);

    Coordinate* GetCoord(const BYTE& x, const BYTE& y);
    Coordinate* GetNeighbour(const Coordinate* const current, DIRECTION direction);
    VOID ResetValues();
    VOID Print();

  private:
    size_t m_MapRows;
    size_t m_MapCols;
    std::vector<Coordinate> m_Coordinates;
  };
}