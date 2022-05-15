#pragma once

struct CellGrid
{
    CellGrid() :
        width(0),
        height(0),
        cells(nullptr)
    {
    }

    CellGrid(
        int width,
        int height) :
        width(width),
        height(height),
        cells(new bool[width * height])
    {
    }

    CellGrid(
        CellGrid&& cg) :
        width (cg.width),
        height (cg.height),
        cells (cg.cells.release())
    {
        cg.width = 0;
        cg.height = 0;
    }

    CellGrid&
    operator=(
        CellGrid&& cg)
    {
        if (this == &cg)
        {
            return *this;
        }

        cells.reset(cg.cells.release());
        width = cg.width;
        cg.width = 0;
        height = cg.height;
        cg.height = 0;
        return *this;
    }

    void
    Zero()
    {
        std::fill(cells.get(), cells.get() + (width * height), false);
    }

    span<bool>
    GetCellSpan()
    {
        return {cells.get(), width * height};
    }

    int width;
    int height;
    std::unique_ptr<bool[]> cells;
};

static
inline
int
GetCellIndex(
    int row,
    int col,
    int width)
{
    return row * width + col;
}

void
UpdateCellGrids(
    CellGrid& active,
    CellGrid& scratch);

void
FillCellGrid(
    CellGrid& grid,
    span<bool>& buffer);
