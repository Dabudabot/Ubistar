#pragma once

/*!
 *  \brief     A* algorithm
 *  \details   Class that manages algorithm itself
 *  \author    Daulet Tumbayev
 *  \date      2021
 */

 /************************************************
  *  Includes
  ***********************************************/

#include "u_world.h"

#include <Windows.h>
#include <string>
#include <memory>

  /************************************************
   *  class decl
   ***********************************************/

namespace ubistar
{
  /*!
  *  Class wrapping algorithm
  */
  class AStar
  {
  public:

    /*!
    *  ctor with params to init algo
    *  \param mapPath path to file with map
    *  \param mapRows maximum amount of rows in map
    *  \param mapCols maximum amount of cols in map
    *  \param showmap should map finally printed
    * 
    *  \details also bunch of internal params initialized with default values
    */
    AStar(const std::basic_string<TCHAR>& mapPath, BYTE mapRows, BYTE mapCols, BOOL showmap);

    /*!
    *  default dtor, no need to remove anything here by hand
    */
    ~AStar() = default;

    /*!
    *  Algo itself run here, each start resets the map and run search
    *  \param startX x coordinate (col) of start pos
    *  \param startY y coordinate (row) of start pos
    *  \param endX x coordinate (col) of end pos
    *  \param endY y coordinate (row) of end pos
    * 
    *  \details algorithm first checks if start and end pos is valid
    *           then we go for general implementation of A*
    *           good explanation of the algorithm (https://youtu.be/-L-WgKMFuhE)
    * 
    *  \return true if path is found, false otherwise
    */
    BOOL FindPath(BYTE startX, BYTE startY, BYTE endX, BYTE endY);

    /*!
    *  \return duration of the last call to FindPath
    */
    DOUBLE GetLastDuration() const { return m_Duration; }

    /*!
    *  \return last status of the FindPath call
    */
    BOOL IsLastFound() const { return m_PathFound; }

    /*!
    *  \return current flag to show the map or not
    */
    BOOL IsMapShown() const { return m_ShowMap; }

    /*!
    *  \return reference to start tile in the world
    *          all values are in World class
    *          anyone else has only references
    *          it helps to avoid additional copying
    */
    Coordinate* GetLastStart() const { return m_Start; }

    /*!
    *  \return reference to end tile in the world
    */
    Coordinate* GetLastEnd() const { return m_End; }

    /*!
    *  \return last calculated cost of found path
    */
    DOUBLE GetLastCost() const { return m_Cost; }

    /*!
    *  printing to std out. TODO need to replace by << operator overload
    */
    VOID Print();

  private:

    /*!
    *  calculating heuristic value. It is value from start coodinate to end
    *  ignoring walls. Actually it is just euclidian diff between to points
    *  \param start point from which to calc
    *  \param end point to which we calc
    *  \return value (note: it is 0 if we reach the end)
    */
    FLOAT CalcH(const Coordinate* const start, const Coordinate* const end);

    /*!
    *  calculating distance * terrain specificy. It is value how costly is it to go
    *  \param currentG current const that we have on specific tile
    *  \param neighbourT cost of terrain where we planning to go
    *  \param direction actually need to determine diagonal or straight movement it is
    *  \return value cost
    */
    FLOAT CalcG(const FLOAT& currentG, const FLOAT& neighbourT, const DIRECTION& direction);

    //
    // World that contains all the cells
    //
    std::unique_ptr<World> m_World;

    //
    // Multipler to vert or horizontal movement
    //
    FLOAT m_Weight;

    //
    // Multipler to diagonal movement
    //
    FLOAT m_DiagWeight;

    //
    // reference to cell that is starting point
    //
    Coordinate* m_Start;

    //
    // reference to cell that is end point
    //
    Coordinate* m_End;

    //
    // flag to show or hide map on print
    //
    BOOL m_ShowMap;

    //
    // result duration of the algo
    //
    DOUBLE m_Duration;

    //
    // cost of the found path (if not found it is 0)
    //
    DOUBLE m_Cost;

    //
    // indicates status of search
    //
    BOOL m_PathFound;
  };
}