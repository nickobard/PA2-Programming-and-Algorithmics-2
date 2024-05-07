//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CSPREADSHEET_H
#define BARDANIK_CSPREADSHEET_H


#include "SpreadsheetStructure/CRange.h"
#include "InputOutputUtilities/CLoader.h"

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;

/**
 * Class that represents a spreadsheet.
 */
class CSpreadsheet {
public:
    /**
     * What functions the spreadsheet supports.
     */
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FILE_IO | SPREADSHEET_FUNCTIONS;
    }

    /**
     * Default constructor.
     */
    CSpreadsheet() = default;

    /**
     * Copy constructor - makes deep copy of cells.
     * @param src - spreadsheet to make deep copy from.
     */
    CSpreadsheet(const CSpreadsheet &src);

    /**
     * Copy-assignment operator - makes deep copy of cells.
     * @param src - spreadsheet to make deep copy from.
     * @return reference to the object where data was copied.
     */
    CSpreadsheet &operator=(CSpreadsheet src);

    /**
     * Loads spreadsheet from any input stream.
     * @param is - input stream to load this spreadsheet from.
     * @return true if successfully loaded data to this spreadsheet, in case of error this data are not rewritten.
     */
    bool load(istream &is);

    /**
     * Saves this spreadsheet to any output stream.
     * @param os - output stream to save this spreadsheet data.
     * @return true if successfully saved data, in case of error this data are not rewritten.
     */
    bool save(ostream &os) const;

    /**
     * Set cell in spreadsheet with content. The content can be a number, string literal, or expression.
     * @param pos - where to set content.
     * @param contents - contents of the cell.
     * @return true if cell is successfully set.
     */
    bool setCell(CPos pos,
                 string contents);

    /**
     * Calculate value from some cell.
     * @param pos - position of the cell to evaluate.
     * @return value evaluated from the cell.
     */
    CValue getValue(CPos pos);

    /**
     * Calculate value from position, but using provided cycle detection visitor.
     * @param pos - position of the cell to evaluate.
     * @param visitor - cycle detection visitor.
     * @return value evaluated from the cell.
     */
    CValue getValue(CPos pos, CCycleDetectionVisitor &visitor);

    /**
     * Copy rectangular portion of the spreadsheet and paste it to the another place,
     * rewriting previous values.
     * @param dst - upper left corner where to paste copied rectangular selection.
     * @param src - upper left corner from which to copy rectangular selection.
     * @param w - width of the rectangular selection, w >= 1.
     * @param h - height of the rectangular selection, h>= 1.
     */
    void copyRect(CPos dst,
                  CPos src,
                  int w = 1,
                  int h = 1);

    /**
     * Set cell in provided cells container.
     * @param cells - container in which to set a cell.
     * @param pos - position of the cell to evaluate.
     * @param cell - cell to assign.
     * @return true if the cell is successfully set.
     */
    static bool setCell(Cells &cells, const CPos &pos, shared_ptr<CCell> &cell);

    /**
     * Get the container with cells of this spreadsheet.
     * @return container with cells.
     */
    Cells &getCells();

private:

    Cells m_cells;

};


#endif //BARDANIK_CSPREADSHEET_H
