//
// Created by bardanik on 11/04/24.
//
#ifndef BARDANIK_CPOS_H
#define BARDANIK_CPOS_H


#include <string>
#include <stdexcept>

using namespace std;

/**
 * Class that represents position in the spreadsheet.
 * Is used to parse string representation of position and operate with it.
 */
class CPos {
public:
    /**
     * Constructs default relative position with coordinates 0, 0.
     */
    CPos();

    /**
     * Constructs position through string representation.
     * @param str - string representation of position.
     */
    explicit CPos(string_view str);

    /**
     * Constructs position with integer representation.
     * @param row - row position.
     * @param col - column position.
     */
    CPos(int row, int col);

    /**
     * Extracts pair of position coordinates.
     * @return row and column positions pair.
     */
    pair<int, int> getCoords() const;

    /**
     * Get offset between two positions - like vector.
     * Is used to apply offset when copying cells and moving them into another place.
     * @param src - start position point.
     * @param dst - end position point.
     * @return row offset and column offset pair.
     */
    static pair<int, int> getOffset(const CPos &src, const CPos &dst);

    /**
     * Shifts the position given offset.
     * Is used to shift cell position when copying and moving the cell.
     * @param offset - offset to apply to this position.
     */
    void shift(const pair<int, int> &offset);

    /**
     * Static function to convert string to upper case.
     * @param str - string to convert to upper case.
     * @return upper cased string.
     */
    static string toUpperCase(const string_view &str);

private:

    /**
     * Splits position represented in a string into row element and column element.
     * Also considers if the element is relative or is absolute.
     * @param position - string representation of position.
     */
    void splitPositionAndParse(const string &position);

    /**
     * Converts column string label, which consists of alphabet characters,
     * into number for internal storage and work.
     * @param col_label - column label to convert into a number
     * @return integer equivalent of the alphabetic representation.
     */
    static int convertColLabelToNumber(const string &col_label);

    // Row position.
    int m_row;
    // If row position is absolute or relative.
    bool m_absolute_row;
    // Column position.
    int m_col;
    // If column position is absolute or relative.
    bool m_absolute_col;

    // Original alphabetic representation of the column position.
    string m_col_label;

};

#endif //BARDANIK_CPOS_H
