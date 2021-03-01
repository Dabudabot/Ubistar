#pragma once

/*!
 *  \brief     World and cells container
 *  \details   Class that manages world elements
 *  \author    Daulet Tumbayev
 *  \date      2021
 */

 /************************************************
  *  Includes
  ***********************************************/

#include <Windows.h>
#include <tchar.h>
#include <string>
#include <vector>

  /************************************************
   *  class decl
   ***********************************************/

namespace ubistar
{

  /************************************************
   *  enums
   ***********************************************/

  /*!
  *  Directions relative from cell where we stand
  */
  enum class DIRECTION
  {
    N, E, S, W,
    NE, ES, SW, WN
  };

  /*!
  *  Different terrain types to match sym
  */
  enum class TERRAIN_TYPE
  {
    UNDEFINED,
    PLAIN,
    WATER,
    SWAMP,
    MOUNTAIN
  };

  /*!
  *  Global array to iterate enum
  */
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

  /*!
  *  One cell or tile in map
  */
  class Coordinate
  {
  public:

    /*!
    *  default ctor, it initializes all to 0
    */
    Coordinate();

    /*!
    *  default dtor, no need to free anything in this class by hand
    */
    ~Coordinate() = default;

    /*!
    *  simple getters
    */
    BYTE GetX() const { return m_x; }
    BYTE GetY() const { return m_y; }
    FLOAT GetG() const { return m_g; }
    FLOAT GetH() const { return m_h; }
    FLOAT GetTerrainCost() const { return m_TerrainCost; }
    TERRAIN_TYPE GetTerrainType() const { return m_TerrainType; }
    BOOL IsVisited() const { return m_Visited; }
    BOOL IsChoosen() const { return m_Choosen; }
    Coordinate* GetParent() const { return m_Parent; }
    FLOAT GetTravelCost() { return m_TravelCost; }

    /*!
    *  \return total cost (g + h)
    */
    FLOAT GetTotalCost() const;
    
    /*!
    *  \return symbolic representation of the terrain
    */
    CHAR GetTerrainTypeAsSym() const;
    
    /*!
    *  simple setters
    */
    VOID MarkAsVisited() { m_Visited = true; }
    VOID MarkAsChoosen() { m_Choosen = true; }
    VOID MarkAsPath() { m_Path = true; }
    VOID SetG(FLOAT g) { m_g = g; }
    VOID SetParent(Coordinate* parent) { m_Parent = parent; }
    VOID SetH(FLOAT h) { m_h = h; }
    VOID SetTravelCost(FLOAT cost) { m_TravelCost = cost; }

    /*!
    *  combining calls SetTerrain and SetCoords
    */
    VOID SetData(BYTE x, BYTE y, TCHAR type);

    /*!
    *  casting symbolic representation to enum
    */
    VOID SetTerrain(TCHAR type);

    /*!
    *  just setting x and y
    */
    VOID SetCoords(BYTE x, BYTE y);

    /*!
    *  setting all back to 0
    */
    VOID ResetValue();

  private:

    //
    // coordinates
    //
    BYTE m_x;
    BYTE m_y;

    //
    // cost from point to start
    //
    FLOAT m_g;

    //
    // cost from point to end (ignoring walls)
    //
    FLOAT m_h;

    //
    // flag that this tile may be the part of the path
    //
    BOOL m_Choosen;

    //
    // flag that this tile was calculated g and h
    //
    BOOL m_Visited;

    //
    // flag that this is part of the path
    //
    BOOL m_Path;

    //
    // type
    //
    TERRAIN_TYPE m_TerrainType;

    //
    // cost influenced by terrain
    //
    FLOAT m_TerrainCost;

    //
    // m_TerrainCost + move cost
    //
    FLOAT m_TravelCost;

    //
    // how do we reach this cell
    //
    Coordinate* m_Parent;
  };

  /*!
  *  container for all cells
  */
  class World
  {
  public:

    /*!
    *  ctor with param to init the world, read the file and compelete the vector
    *  \param mapPath path to file
    *  \param mapRows max amount of rows
    *  \param mapCols max amoutn of cols
    */
    World(const std::basic_string<TCHAR>& mapPath, size_t mapRows, size_t mapCols);

    /*!
    *  default dtor, no need to free anything in this class by hand
    */
    ~World() = default;

    /*!
    *  To get reference from container by coordinates
    *  Note coordinates also match position in container
    *  \param x col
    *  \param y row
    *  \return ref to tile
    */
    Coordinate* GetCoord(const BYTE& x, const BYTE& y);

    /*!
    *  To get reference from container. It goes for neighbour according to direction
    *  \param current whose neighbour we are looking for
    *  \param direction direction of the nieghbour relative to current
    *  \return ref to tile
    */
    Coordinate* GetNeighbour(const Coordinate* const current, DIRECTION direction);

    /*!
    *  Reset all tiles
    */
    VOID ResetValues();

    /*!
    *  Print all tiles
    */
    VOID Print();

  private:

    //
    // max amount of rows (y)
    //
    size_t m_MapRows;

    //
    // max amount of cols (x)
    //
    size_t m_MapCols;

    //
    // keeper of all tiles
    //
    std::vector<Coordinate> m_Coordinates;
  };
}