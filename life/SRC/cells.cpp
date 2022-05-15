#include "pch.h"

static
bool
ProcessCellStatus(
    bool cellIsAlive,
    span<bool>& neighborLifeStates)
{
    uint8_t liveNeighbors = 0; // at most 8 neighbors;
    for (bool neighborAlive : neighborLifeStates)
    {
        if (neighborAlive)
        {
            liveNeighbors += 1;
        }
    }

    if (cellIsAlive)
    {
        return liveNeighbors == 2 || liveNeighbors == 3;
    }
    else
    {
        return liveNeighbors == 3;
    }
}

void
UpdateCellGrids(
    CellGrid& active,
    CellGrid& scratch)
{
    assert(active.width == scratch.width);
    assert(active.height == scratch.height);

    const int numRows = active.height;
    const int numCols = active.width;

    // Check everything inside of the border
    ForRange(int, cellRow, 1, numRows - 1)
    {
        ForRange(int, cellCol, 1, numCols - 1)
        {
            int cellIndex = GetCellIndex(cellRow, cellCol, numCols);
            bool neighbors[] = {
                active.cells[GetCellIndex(cellRow - 1, cellCol - 1, numCols)], // TopLeft
                active.cells[GetCellIndex(cellRow - 1, cellCol    , numCols)], // TopCenter
                active.cells[GetCellIndex(cellRow - 1, cellCol + 1, numCols)], // TopRight
                active.cells[GetCellIndex(cellRow    , cellCol - 1, numCols)], // CenterLeft
                active.cells[GetCellIndex(cellRow    , cellCol + 1, numCols)], // CenterRight
                active.cells[GetCellIndex(cellRow + 1, cellCol - 1, numCols)], // BottomLeft
                active.cells[GetCellIndex(cellRow + 1, cellCol    , numCols)], // BottomCenter
                active.cells[GetCellIndex(cellRow + 1, cellCol + 1, numCols)], // BottomRight
            };
            scratch.cells[cellIndex] = ProcessCellStatus(active.cells[cellIndex], neighbors);
        }
    }

    // Check the corners: topleft
    int topLeftIndex = GetCellIndex(0, 0, numCols);
    bool topLeftNeighbors[] = {
        active.cells[GetCellIndex(0, 1, numCols)],
        active.cells[GetCellIndex(1, 0, numCols)],
        active.cells[GetCellIndex(1, 1, numCols)],
    };
    scratch.cells[topLeftIndex] = ProcessCellStatus(active.cells[topLeftIndex], topLeftNeighbors);

    // Check the corners: topright
    int topRightIndex = GetCellIndex(0, numCols - 1, numCols);
    bool topRightNeighbors[] = {
        active.cells[GetCellIndex(0, numCols - 2, numCols)],
        active.cells[GetCellIndex(1, numCols - 1, numCols)],
        active.cells[GetCellIndex(1, numCols - 2, numCols)],
    };
    scratch.cells[topRightIndex] = ProcessCellStatus(active.cells[topRightIndex], topRightNeighbors);

    // Check the corners: bottomleft
    int bottomLeftIndex = GetCellIndex(numRows - 1, 0, numCols);
    bool bottomLeftNeighbors[] = {
        active.cells[GetCellIndex(numRows - 1, 1, numCols)],
        active.cells[GetCellIndex(numRows - 2, 0, numCols)],
        active.cells[GetCellIndex(numRows - 2, 1, numCols)],
    };
    scratch.cells[bottomLeftIndex] = ProcessCellStatus(active.cells[bottomLeftIndex], bottomLeftNeighbors);

    // Check the corners: bottomright
    int bottomRightIndex = GetCellIndex(numRows - 1, numCols - 1, numCols);
    bool bottomRightNeighbors[] = {
        active.cells[GetCellIndex(numRows - 1, numCols - 2, numCols)],
        active.cells[GetCellIndex(numRows - 2, numCols - 1, numCols)],
        active.cells[GetCellIndex(numRows - 2, numCols - 2, numCols)],
    };
    scratch.cells[bottomRightIndex] = ProcessCellStatus(active.cells[bottomRightIndex], bottomRightNeighbors);

    // Check each side: left
    ForRange(int, cellRow, 1, numRows - 1)
    {
        int cellCol = 0;
        int cellIndex = GetCellIndex(cellRow, cellCol, numCols);
        bool neighbors[] = {
            active.cells[GetCellIndex(cellRow - 1, cellCol    , numCols)], // TopCenter
            active.cells[GetCellIndex(cellRow - 1, cellCol + 1, numCols)], // TopRight
            active.cells[GetCellIndex(cellRow    , cellCol + 1, numCols)], // CenterRight
            active.cells[GetCellIndex(cellRow + 1, cellCol    , numCols)], // BottomCenter
            active.cells[GetCellIndex(cellRow + 1, cellCol + 1, numCols)], // BottomRight
        };
        scratch.cells[cellIndex] = ProcessCellStatus(active.cells[cellIndex], neighbors);
    }

    // Check each side: right
    ForRange(int, cellRow, 1, numRows - 1)
    {
        int cellCol = numCols - 1;
        int cellIndex = GetCellIndex(cellRow, cellCol, numCols);
        bool neighbors[] = {
            active.cells[GetCellIndex(cellRow - 1, cellCol - 1, numCols)], // TopLeft
            active.cells[GetCellIndex(cellRow - 1, cellCol    , numCols)], // TopCenter
            active.cells[GetCellIndex(cellRow    , cellCol - 1, numCols)], // CenterLeft
            active.cells[GetCellIndex(cellRow + 1, cellCol - 1, numCols)], // BottomLeft
            active.cells[GetCellIndex(cellRow + 1, cellCol    , numCols)], // BottomCenter
        };
        scratch.cells[cellIndex] = ProcessCellStatus(active.cells[cellIndex], neighbors);
    }

    // Check each side: top
    ForRange(int, cellCol, 1, numCols - 1)
    {
        int cellRow = 0;
        int cellIndex = GetCellIndex(cellRow, cellCol, numCols);
        bool neighbors[] = {
            active.cells[GetCellIndex(cellRow    , cellCol - 1, numCols)], // CenterLeft
            active.cells[GetCellIndex(cellRow    , cellCol + 1, numCols)], // CenterRight
            active.cells[GetCellIndex(cellRow + 1, cellCol - 1, numCols)], // BottomLeft
            active.cells[GetCellIndex(cellRow + 1, cellCol    , numCols)], // BottomCenter
            active.cells[GetCellIndex(cellRow + 1, cellCol + 1, numCols)], // BottomRight
        };
        scratch.cells[cellIndex] = ProcessCellStatus(active.cells[cellIndex], neighbors);
    }

    // Check each side: bottom
    ForRange(int, cellCol, 1, numCols - 1)
    {
        int cellRow = numRows - 1;
        int cellIndex = GetCellIndex(cellRow, cellCol, numCols);
        bool neighbors[] = {
            active.cells[GetCellIndex(cellRow - 1, cellCol - 1, numCols)], // TopLeft
            active.cells[GetCellIndex(cellRow - 1, cellCol    , numCols)], // TopCenter
            active.cells[GetCellIndex(cellRow - 1, cellCol + 1, numCols)], // TopRight
            active.cells[GetCellIndex(cellRow    , cellCol - 1, numCols)], // CenterLeft
            active.cells[GetCellIndex(cellRow    , cellCol + 1, numCols)], // CenterRight
        };
        scratch.cells[cellIndex] = ProcessCellStatus(active.cells[cellIndex], neighbors);
    }

    std::swap(active, scratch);
}

void
FillCellGrid(
    CellGrid& grid,
    span<bool>& buffer)
{
    std::copy(buffer.data(), buffer.data() + buffer.size(), grid.cells.get());
}
