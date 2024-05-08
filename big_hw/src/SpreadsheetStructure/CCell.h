//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CCELL_H
#define BARDANIK_CCELL_H

#include <memory>
#include <iostream>
#include <sstream>
#include <map>
#include "../ExpressionBuilders/CASTExpressionBuilder.h"

// Container to store cells - sparse matrix.
using Cells = map<int, map<int, shared_ptr<CCell>>>;

/**
 * Cells type - used for Loader to save information about cell type.
 */
enum CCellType {
    NUMBER,
    STRING,
    EXPRESSION
};

/**
 * Class which represents a cell in a spreadsheet, stores its value and evaluates it.
 *
 * When the cell is moved, expression stored in the cell is not changed, but the offset from
 * its original position is stored. In evaluation the offset is applied on relative references
 * in the expression. So the expression don't have to be parsed and changed, only offset is always
 * updated on each move (shift). This design is weird, but allows to not construct AST tree everytime
 * it is copied, which is much faster  (and the given expression parser is a static library and
 * and its use is limited, if its source code was given, it could be used to reconstruct the expression).
 */
class CCell {
public:
    /**
     * Constructs cell with specified value.
     * @param value to store in the cell - number, string or expression.
     */
    explicit CCell(CValue value);

    virtual ~CCell() = default;

    /**
     * Factory method, creates specific cells type based on the contents,
     * if it is a number, string or expression.
     * @param contents - stores string representation of value.
     * @return new cell with stored and parses contents.
     */
    static CCell *createCell(const string &contents);

    /**
     * Calculates value of the cell as CValue object - double, string or monostate (undefined).
     * @param spreadsheet - reference to spreadsheet where the cell is stored.
     * @param visitor - cycle detection visitor object, which watches cycles while evaluation of the cell.
     * @return CValue object - double, string or monostate (undefined).
     */
    virtual CValue getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor);

    /**
     * Copies this cell and returns new cell of the same type.
     * @return new copy of this cell.
     */
    virtual CCell *copy() const = 0;

    /**
     * Shifts cell's position. Is used when copying this cell and changing relative references
     * to other cells in the spreadsheet when it is moved to another position.
     * @param offset
     */
    virtual void shift(const pair<int, int> &offset);

    /**
     * Converts cell to string representation.
     * Is used by Loader to save cell in the output stream.
     * @return string representation of the cell.
     */
    virtual string toString() const = 0;

    /**
     * Reads cell from any input stream.
     * @param is input stream to read cell.
     * @param cell - cell which is reading input.
     */
    friend istream &operator>>(istream &is, CCell *cell);

    /**
     * Reads the data for this cell from input stream.
     * Is used when loading cell.
     * @param is - input stream from which to read the cell.
     * @return input stream back for further reading.
     */
    virtual istream &readCell(istream &is) = 0;

    /**
     * Gets current shift from the original epxression position.
     * @return row shift and col shift pair.
     */
    virtual pair<int, int> getShift() const;


protected:
    // Value stored in the cell - double, string or monostate (undefined).
    CValue m_value;
};


/**
 * Cell that represents a number.
 */
class CNumberCell : public CCell {
public:
    /**
     * Constructs cell with 0.0 value.
     */
    CNumberCell();

    /**
     * Constructs a number cell.
     * @param value - value of the cell.
     */
    explicit CNumberCell(double value);

    istream &readCell(istream &is) override;

    string toString() const override;

    CCell *copy() const override;

};

/**
 * Represents a string literal cell.
 */
class CStringCell : public CCell {
public:
    /**
     * Constructs empty string literal cell.
     */
    CStringCell();

    /**
     * Constructs string literal with a given string.
     * @param value - string literal stored in the cell.
     */
    explicit CStringCell(const string &value);

    string toString() const override;

    CCell *copy() const override;

    istream &readCell(istream &is) override;
};

/**
 * Cell to store expressions.
 */
class CExprCell : public CCell {
public:
    /**
     * Constructs empty expression cell.
     */
    CExprCell();

    /**
     * Constructs expression cell with a give expression.
     * @param expression - expression to be stored in the cell.
     */
    explicit CExprCell(const string &expression);

    CValue getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor) override;

    CCell *copy() const override;

    string toString() const override;

    istream &readCell(istream &is) override;

    void shift(const pair<int, int> &offset) override;

    pair<int, int> getShift() const override;

private:
    // Root of the constructed AST tree when getting the cell value.
    unique_ptr<CASTNode> m_root;
    // Offset from the original position of the cell to shift expression when building the AST tree.
    pair<int, int> m_shift;

};


#endif //BARDANIK_CCELL_H
