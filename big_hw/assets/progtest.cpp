#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "expression.h"

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;

constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif /* __PROGTEST__ */

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

class CCell;

/**
 * Exception that is thrown when cycle detection visitor detects a cycle in AST tree evaluation.
 */
class CCycleDetectedException : public exception {
};

/**
 * Class that visits each cell while AST evaluation. The evaluation is performed in DFS manner,
 * the visitor is passed recursively to next node and every time it arrives at a new cell,
 * it remembers that it was opened, so when the visitor arrives at already opened cell, it means
 * that there is oriented cycle, and throws a cycle detection exception. When evaluation of the cell
 * is done, the visitor leaves the node and marks it as closed/fresh. The node can be again opened from other
 * reference or range node.
 */
class CCycleDetectionVisitor {
public:
    /**
     * Visits cell and marks it as opened.
     * @param cell - cell that is being visited.
     */
    void visit(const CCell *cell);

    /**
     * Leaves cell and marks it as closed/fresh.
     * @param cell - cell that is being left.
     */
    void leave(const CCell *cell);

private:
    // Stores cells that is currently visited and marked as opened.
    unordered_set<const CCell *> m_opened;
};


class CSpreadsheet;

class CRange;

using namespace literals;

// Value type that is stored in each cell - double, string or monostate (undefined).
using CValue = variant<monostate, double, string>;

/**
 * Represents an abstract node in the AST tree.
 */
class CASTNode {
public:
    /**
     * Evaluates the node and returns the evaluation value.
     * @param visitor - cycle detection visitor that is propagated to check for cycles.
     * @return evaluation of the node.
     */
    virtual CValue evaluate(CCycleDetectionVisitor &visitor) = 0;

    /**
     * Evaluates range of nodes.
     * Is used by CRangeNode to evaluate ranges. In case of other nodes
     * returns just a vector with a single value.
     * @param visitor - cycle detection visitor that is propagated to check for cycles.
     * @return evaluations of each cell in range. In case of a non range node, returns
     * a vector with a single element in it.
     */
    virtual vector<CValue> evaluateRange(CCycleDetectionVisitor &visitor);

    /**
     * Returns the size of the rectangular selection of the range,
     * considering empty cells too.
     * @return the size of the rectangualr selection.
     */
    virtual size_t rangeCapacity() const;

    virtual ~CASTNode() = default;
};

/**
 * Represents a node that stores a string literal value.
 */
class CStringNode : public CASTNode {
public:
    /**
     * Constructs a string node which stores parsed string literal value.
     * @param parsed_value - parsed string literal, given by the parser.
     */
    CStringNode(const string &parsed_value);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    // Value of the node.
    CValue m_value;
};

/**
 * Represents a node that stores reference to another cell and also a spreadsheet,
 * where that cell is expected to be located. In evaluation finds that cell and gets value from the cell.
 */
class CReferenceNode : public CASTNode {
public:
    /**
     * Constructs a reference node.
     * @param pos - position of the referenced cell.
     * @param spreadsheet - spreadsheet where the referenced cell is expected.
     * @param offset - offset of the cell, that is used to construct reference node to point to
     * the correct place, considering the global shift of the cell. Look CCell documentation for details
     * of how it is implemented.
     */
    CReferenceNode(const string &pos, CSpreadsheet &spreadsheet, const pair<int, int> &offset);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    // Reference cell position.
    CPos m_reference_position;
    // Spreadsheet where the cell is expected to be.
    CSpreadsheet &m_spreadsheet;
};

/**
 * Represents a node that stores a range of cells. The selection and cells aggregation
 * is computed only in evaluation stage. The node stores only two positions - from position and
 * to position, which together are used to make a rectangular selection - CRange object.
 */
class CRangeNode : public CASTNode {
public:
    /**
     * Constucts a range node.
     * @param from - upper left corner position of the range.
     * @param to - bottom right corner position of the range.
     * @param spreadsheet - spreadsheet where selection is made.
     * @param offset - offset to consider shift if some cells were copied and shifted.
     * Look at CCell documentation for details how it works.
     */
    CRangeNode(const string &from, const string &to, CSpreadsheet &spreadsheet, const pair<int, int> &offset);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

    vector<CValue> evaluateRange(CCycleDetectionVisitor &visitor) override;

    size_t rangeCapacity() const override;

private:
    // Upper left corner position of the range.
    CPos m_from_position;
    // Bottom right corner position of the range.
    CPos m_to_position;
    // Spreadsheet where the selection of range will be made in evaluation.
    CSpreadsheet &m_spreadsheet;
};

/**
 * Represents a Number node.
 */
class CNumberNode : public CASTNode {
public:
    CNumberNode(double number);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    CValue m_number;
};


/**
 * Represents an abstract binary operation node class,
 * which stores two operands and applies binary operation defined in a derived class.
 */
class BinaryOperationNode : public CASTNode {
public:
    /**
     * Constructs binary operation node from two operands.
     * @param left_operand - left operand in operation.
     * @param right_operand - right operand in operation.
     */
    BinaryOperationNode(CASTNode *left_operand, CASTNode *right_operand);

    ~BinaryOperationNode();

    /**
     * Get values of specified type from the CValue.
     * Is used to extract certain types and values to make operations on them,
     * like double + double, etc.
     * @tparam L - left operand type.
     * @tparam R - right operand type.
     * @param values - CValue pair of values from which to extract values of specified types.
     * @return pair of values with specified type.
     */
    template<typename L, typename R>
    static pair<L, R> getValues(const pair<CValue, CValue> &values);

    /**
     * Same as getValues(const pair<CValue, CValue> &values), but with unpacked pair in params.
     */
    template<typename L, typename R>
    static pair<L, R> getValues(const CValue &first, const CValue &second);

    /**
     * Checks if CValues are of specified types. Is used to check types before extracting values
     * of specified types by getValues(...) functions.
     * @tparam L - left operand type.
     * @tparam R - right operand type.
     * @param values - pair of operands, first is left, second is right.
     * @return true if both values are of specified types, otherwise false.
     */
    template<typename L, typename R>
    static bool typesAre(const pair<CValue, CValue> &values);

    /**
     * Same as typesAre(const pair<CValue, CValue> &values), but with unpacked pair in params.
     */
    template<typename L, typename R>
    static bool typesAre(const CValue &first, const CValue &second);

protected:

    /**
     * Evaluates both operand nodes and returns a pair of their CValue evaluations.
     * @param visitor - to detect cycles in evaluation process.
     * @return pair of CValues, evaluated from both operands. First is left, second is right.
     */
    pair<CValue, CValue> evaluateValues(CCycleDetectionVisitor &visitor);

private:

    CASTNode *m_left_operand;
    CASTNode *m_right_operand;
};

/**
 * The node that performs addition operation on both operands.
 */
class AddNode : public BinaryOperationNode {
public:
    AddNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * The node that performs subtraction, where left operand is the minuend, right is subtrahend.
 */
class SubtractNode : public BinaryOperationNode {
public:
    SubtractNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * The node that performs multiplication on both operands.
 */
class MultiplicationNode : public BinaryOperationNode {
public:
    MultiplicationNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * The node that performs division, where left operand is the dividend, right operand is divisor.
 */
class DivisionNode : public BinaryOperationNode {
public:
    DivisionNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


/**
 * The node that performs power operation, where left operand is a base, right operand is an exponent.
 */
class PowerNode : public BinaryOperationNode {
public:
    PowerNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


/**
 * Represents an abstract relational binary operation class,
 * which stores two operands and compare them.
 */
class RelationalOperationNode : public BinaryOperationNode {
public:
    /**
     * Constructs relational operation node with two operands
     * @param left_operand - left operand in operation.
     * @param right_operand - right operand in operation.
     */
    RelationalOperationNode(CASTNode *left_operand, CASTNode *right_operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

protected:
    /**
     * Compares left operand and right operand.
     * @param lhs - double left operand.
     * @param rhs - double right operand.
     * @return true if comparison operation is true, for example if both operands are equal
     * in case of equals operation.
     */
    virtual bool compare(double lhs, double rhs) = 0;

    /**
     * Compares left operand and right operand.
     * @param lhs - string left operand.
     * @param rhs - string right operand.
     * @return true if comparison operation is true, for example if both operands are equal
     * in case of equals operation.
     */
    virtual bool compare(const string &lhs, const string &rhs) = 0;
};

/**
 * Represents equals (==) comparison between operands.
 */
class EqualNode : public RelationalOperationNode {
public:
    EqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents not equals (!=) comparison between operands.
 */
class NotEqualNode : public RelationalOperationNode {
public:
    NotEqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents less than (<) comparison between operands.
 */
class LessThanNode : public RelationalOperationNode {
public:
    LessThanNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents greater than (>) comparison between operands.
 */
class GreaterThanNode : public RelationalOperationNode {
public:
    GreaterThanNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents less than or equals (<=) comparison between operands.
 */
class LessThanOrEqualNode : public RelationalOperationNode {
public:
    LessThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};

/**
 * Represents greater than or equals (>=) comparison between operands.
 */
class GreaterThanOrEqualNode : public RelationalOperationNode {
public:
    GreaterThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand);

private:
    bool compare(double lhs, double rhs) override;

    bool compare(const string &lhs, const string &rhs) override;
};


/**
 * Represents an abstract unary operation node class, to apply unary operation on stored operands.
 */
class UnaryOperationNode : public CASTNode {
public:
    /**
     * Constructs unary operation node with some given operand.
     * @param operand - operand to which to apply unary operation.
     */
    explicit UnaryOperationNode(CASTNode *operand);

    ~UnaryOperationNode();

    /**
     * Evaluates the stored operand in the unary operation node.
     * @param visitor - cycle detection visitor to watch cycles in evaluation process.
     * @return value of the stored operand.
     */
    CValue evaluateValue(CCycleDetectionVisitor &visitor);

protected:

private:
    // Operand on which operation is applied.
    CASTNode *m_operand;
};

/**
 * Applies negation operation on the stored operand.
 */
class NegationNode : public UnaryOperationNode {
public:
    explicit NegationNode(CASTNode *operand);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


/**
 * Represents an abstract function node class to evaluate some function on given arguments.
 * Concrete functions are defined in derived classes.
 */
class FunctionNode : public CASTNode {
public:
    /**
     * Constructs function node with parameter pack.
     * @tparam Args parameters type, CASTNodes types are expected as arguments.
     * @param args arguments of the function
     */
    template<typename... Args>
    explicit FunctionNode(Args... args);

    ~FunctionNode();

protected:
    // Stores arguments passed to this function node.
    vector<CASTNode *> m_args;
};

/**
 * Represents sum operation on given range of cells. Sums numeric values across each cell in range.
 */
class SumNode : public FunctionNode {
public:
    /**
     * Constructs sum node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit SumNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * Represents count operation. Counts all numeric values in the given range.
 */
class CountNode : public FunctionNode {
public:
    /**
     * Constructs count node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit CountNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

};

/**
 * Represents min operation. Find the minimal numeric value in given range of values,
 * otherwise gives undefined value.
 */
class MinNode : public FunctionNode {
public:
    /**
     * Constructs min node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit MinNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * Represents max operation. Find the minimal numeric value in given range of values,
 * otherwise gives undefined value.
 */
class MaxNode : public FunctionNode {
public:
    /**
     * Constructs max node with a given range node.
     * @param m_range - node which evaluates range of cells.
     */
    explicit MaxNode(CASTNode *m_range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};

/**
 * Node that represents value count function, which counts number of values in a given range of cells
 * that equal to a give reference value. The reference value can be double, string or undefined.
 */
class CountValNode : public FunctionNode {
public:
    /**
     * Constructs the count value node.
     * @param value - reference value to which compare each value in a range of other values.
     * @param range - node that evaluates range of selected values.
     */
    explicit CountValNode(CASTNode *value, CASTNode *range);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;

private:
    /**
     * Performs counting based on given type, for example double, string or monostate (undefined).
     * @tparam T - type of the reference value.
     * @param range - vector of CValues (result of range node evaluation).
     * @param value - represents a value to which to compare each value in a range to count it in.
     * @return count of values from range that match the specified value argument.
     */
    template<typename T>
    double countValRange(const vector<CValue> &range, const T &value) const;

};

/**
 * Node that represents control flow function - the if statement expression. If condition
 * argument is evaluated as non zero - returns evaluation of true argument node, otherwise returns
 * evaluation of false argument node.
 */
class ConditionalNode : public FunctionNode {
public:
    /**
     * Constructs the conditional node.
     * @param cond - condition to be assessed.
     * @param if_true - node to be evaluated if condition is evaluated as a non zero value.
     * @param if_false - node to be evaluated if condition is evaluated as a zero value.
     */
    explicit ConditionalNode(CASTNode *cond, CASTNode *if_true, CASTNode *if_false);

    CValue evaluate(CCycleDetectionVisitor &visitor) override;
};


class CCell;

/**
 * Expression builder which constructs Abstract Syntactic Tree in expression parsing process.
 */
class CASTExpressionBuilder : public CExprBuilder {
public:
    /**
     * Constructs AST expression builder.
     * @param spreadsheet - reference to a spreadsheet.
     * @param current_cell - cell with expression that is being parsed.
     */
    explicit CASTExpressionBuilder(CSpreadsheet &spreadsheet, const CCell *current_cell);

    void opAdd() override;

    void opSub() override;

    void opMul() override;

    void opDiv() override;

    void opPow() override;

    void opNeg() override;

    void opEq() override;

    void opNe() override;

    void opLt() override;

    void opLe() override;

    void opGt() override;

    void opGe() override;

    void valNumber(double val) override;

    void valString(string val) override;

    void valReference(string val) override;

    void valRange(string val) override;

    void funcCall(std::string fnName,
                  int paramCount) override;

    /**
     * Returns the root of the AST tree as a result of the parsing process.
     * @return root of the AST tree.
     */
    CASTNode *getResult();

private:

    /**
     * Gets and removes top two AST nodes from the stack.
     * @return a pair of AST nodes stored on top of the stack.
     */
    pair<CASTNode *, CASTNode *> getNodesPairAndPop();

    /**
     * Get N number of AST nodes and removes them from top of the stack.
     * @tparam NArgs - number of nodes to extract.
     * @return vector with NArgs nodes.
     */
    template<size_t NArgs>
    vector<CASTNode *> getNodesAndPop();

    // Stack for storing intermediate nodes.
    stack<CASTNode *> m_stack;
    // Spreadsheet where the parsed cell is located.
    CSpreadsheet &m_spreadsheet;
    // Cell that is being under the parse process.
    const CCell *m_cell;
};




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




// Container to store selection - array of position and defined cells.
using Range = vector<pair<pair<int, int>, shared_ptr<CCell>>>;

/**
 * Represents rectangular selection of cells in the spreadsheet cells container.
 * Used for selecting rectangular selection of cells, copying and pasting the selection.
 */
class CRange {
public:

    /**
     * Construct empty range, associated with some spreadsheet.
     * @param spreadsheet - represents a spreadsheet where cells should be selected.
     */
    explicit CRange(CSpreadsheet &spreadsheet);

    /**
     * Select cells.
     * @param src - upper left corner of the rectangular selection.
     * @param w - w >= 1, width of the rectangular.
     * @param h - h >= 1, height of the rectangular.
     */
    void select(const CPos &src, int w = 1, int h = 1);

    /**
     * Select cells.
     * @param from - upper left corner of the rectangular selection.
     * @param to - bottom right corner of the rectangular selection.
     */
    void select(const CPos &from, const CPos &to);

    /**
     * Paste current selection to some another point.
     * Pasting without using select() - with empty selection - is undefined behaviour.
     * @param dst represents upper left corner from which to paste current selection.
     */
    void paste(const CPos &dst);

    /**
     * Evaluates cells in the selection.
     * @param visitor - cycle detection object for evaluation.
     * @return vector of evaluated cells in the selection.
     */
    vector<CValue> evaluate(CCycleDetectionVisitor &visitor);

    /**
     * Parses range of cells.
     * i.e. A1:F10 will be parsed to A1 and F10 tokens.
     * @param range - represents range literal to be parsed.
     * @return left upper and right bottom position tokens.
     */
    static pair<string, string> splitRange(const string &range);

private:

    /**
     * Delete cells stored in the rectangle form from some position in the spreadsheet.
     * @param dst upper left corner of the rectangular.
     */
    void deleteCells(const CPos &dst);

    /**
     * Shifts current cells positions in selection.
     * Is used for relative references to be shifted and to update selection positions.
     * @param offset - offset by which the rectangular selection is moved from one position to another.
     */
    void shiftSelection(const pair<int, int> &offset);

    /**
     * Assigns current selection non-empty cells to their current positions in the selection.
     */
    void pasteCells();

    // Reference to a spreadsheet.
    CSpreadsheet &m_spreadsheet;
    // Current selection container. Stored cells and their current positions.
    Range m_selection;
    // Upper left corner position - pivot -  where the last selection was made.
    CPos m_selection_position;
    // Width and height of the selection.
    int m_w, m_h;
};


/**
 * Class that is used to save/load spreadsheet cells to/from a file or any other stream.
 * Loader can be constructed for loading or saving, but not for both operations.
 * Uses hashing for checking if stream data are not damaged.
 */
class CLoader {
public:
    /**
     * Constructs loader for loading cells.
     * @param is - input stream from which to load cells.
     */
    explicit CLoader(istream &is);

    /**
     * Constructs loader for saving cells.
     * @param os - output stream to which to save cells.
     */
    explicit CLoader(ostream &os);

    /**
     * Save cells to current output stream.
     * @param cells - cells to save to output stream.
     * @return true if data were successfully saved.
     */
    bool save(const Cells &cells);

    /**
     * Load cells from current input stream.
     * @param cells - cells container where data will be loaded from input stream.
     * @return true if data were successfully loaded. If fails to load, original data are not touched.
     */
    bool load(Cells &cells);

private:

    /**
     * Used for verification if data are damaged by reading hash, the data, computing the data hash,
     * and comparing both hashes if they are the same.
     * @return true if data hashes are same.
     */
    bool verify();

    /**
     * Loads provided cells for saving into string data buffer,
     * which will be written to output stream later when all cells are saved in buffer.
     * @param cells - cells to be loaded into saving buffer.
     */
    void loadBuffer(const Cells &cells);

    /**
     * Computes hash of data stored currently in the buffer.
     * The computed hash is then padded with zeros from the left
     * to keep consistent hash length.
     * @return hash of the data.
     */
    string getHash() const;

    // Maximum hash length.
    static constexpr size_t HASH_SIZE = 20;

    // Input stream.
    istream *m_is;
    // Output stream.
    ostream *m_os;
    // Buffer where data are temporarily stored when loading or saved.
    string m_buffer;
    // Hash, loaded from a stream or stored while saving.
    string m_hash;
};


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

    // Container for storing cells.
    Cells m_cells;

};


CPos::CPos() : m_row(0), m_absolute_row(false), m_col(0), m_absolute_col(false) {

}


CPos::CPos(string_view str) : m_row(0), m_absolute_row(false), m_col(0), m_absolute_col(false) {
    string upper_cased_pos = toUpperCase(str);
    splitPositionAndParse(upper_cased_pos);
}

CPos::CPos(int row, int col) : m_row(row), m_absolute_row(false), m_col(col), m_absolute_col(false) {

}

string CPos::toUpperCase(const string_view &str) {
    string lower_cased;
    for (char c: str) {
        lower_cased.push_back((char) toupper(c));
    }
    return lower_cased;
}


void CPos::splitPositionAndParse(const string &position) {
    string label, number;

    bool readingColumnLabel = true;

    for (size_t i = 0; i < position.size(); i++) {
        char c = position[i];
        if (readingColumnLabel) {
            if (i == 0 && c == '$') {
                m_absolute_col = true;
            } else if (isalpha(c)) {
                label.push_back(c);
            } else if (isdigit(c)) {
                readingColumnLabel = false;
                number.push_back(c);
            } else if (!label.empty() && c == '$') {
                readingColumnLabel = false;
                m_absolute_row = true;
            } else {
                throw invalid_argument("Non valid character in column label.");
            }
        } else {
            if (!isdigit(c)) {
                throw invalid_argument("Non valid character in row number.");
            }
            number.push_back(c);
        }
    }

    m_col_label = label;
    m_row = stoi(number);
    m_col = convertColLabelToNumber(m_col_label);
}

int CPos::convertColLabelToNumber(const string &col_label) {
    int result = 0;
    for (char c: col_label) {
        result *= 26;
        result += (c - 'A' + 1);
    }
    result -= 1; // correction for indexing from 0
    return result;
}

pair<int, int> CPos::getCoords() const {
    return {m_row, m_col};
}

pair<int, int> CPos::getOffset(const CPos &src, const CPos &dst) {
    return {dst.m_row - src.m_row, dst.m_col - src.m_col};
}

void CPos::shift(const pair<int, int> &offset) {
    auto [row_shift, col_shift] = offset;
    if (!m_absolute_row) {
        m_row += row_shift;
    }
    if (!m_absolute_col) {
        m_col += col_shift;
    }
}


void CCycleDetectionVisitor::visit(const CCell *cell) {
    const auto &result = m_opened.insert(cell);
    bool inserted = result.second;
    if (!inserted) {
        throw CCycleDetectedException();
    }
}

void CCycleDetectionVisitor::leave(const CCell *cell) {
    m_opened.erase(cell);
}


CReferenceNode::CReferenceNode(const string &pos, CSpreadsheet &spreadsheet, const pair<int, int> &offset)
        : m_reference_position(CPos(pos)),
          m_spreadsheet(spreadsheet) {
    m_reference_position.shift(offset);
}

CValue CReferenceNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto value = m_spreadsheet.getValue(m_reference_position, visitor);
    return value;
}

CStringNode::CStringNode(const string &parsed_value) : m_value({parsed_value}) {

}

CValue CStringNode::evaluate(CCycleDetectionVisitor &visitor) {
    return m_value;
}


CNumberNode::CNumberNode(double number) : m_number({number}) {

}

CValue CNumberNode::evaluate(CCycleDetectionVisitor &visitor) {
    return m_number;
}


CRangeNode::CRangeNode(const string &from,
                       const string &to,
                       CSpreadsheet &spreadsheet,
                       const pair<int, int> &offset) : m_from_position(from), m_to_position(to),
                                                       m_spreadsheet(spreadsheet) {
    m_from_position.shift(offset);
    m_to_position.shift(offset);

}

CValue CRangeNode::evaluate(CCycleDetectionVisitor &visitor) {
    return {};
}

vector<CValue> CRangeNode::evaluateRange(CCycleDetectionVisitor &visitor) {
    CRange range(m_spreadsheet);
    range.select(m_from_position, m_to_position);
    return range.evaluate(visitor);
}

size_t CRangeNode::rangeCapacity() const {
    auto [row, col] = CPos::getOffset(m_from_position, m_to_position);
    int h = row + 1, w = col + 1;
    return h * w;
}

vector<CValue> CASTNode::evaluateRange(CCycleDetectionVisitor &visitor) {
    return {evaluate(visitor)};
}

size_t CASTNode::rangeCapacity() const {
    return 1;
}


BinaryOperationNode::BinaryOperationNode(CASTNode *left_operand, CASTNode *right_operand) : m_left_operand(
        left_operand), m_right_operand(right_operand) {

}

BinaryOperationNode::~BinaryOperationNode() {
    delete m_left_operand;
    delete m_right_operand;
}

pair<CValue, CValue> BinaryOperationNode::evaluateValues(CCycleDetectionVisitor &visitor) {
    return {m_left_operand->evaluate(visitor), m_right_operand->evaluate(visitor)};
}

template<typename L, typename R>
bool BinaryOperationNode::typesAre(const CValue &first, const CValue &second) {
    return holds_alternative<L>(first) && holds_alternative<R>(second);
}

template<typename L, typename R>
bool BinaryOperationNode::typesAre(const pair<CValue, CValue> &values) {
    return typesAre<L, R>(values.first, values.second);
}

template<typename L, typename R>
pair<L, R> BinaryOperationNode::getValues(const CValue &first, const CValue &second) {
    L left = get<L>(first);
    R right = get<R>(second);
    return {left, right};
}


template<typename L, typename R>
pair<L, R> BinaryOperationNode::getValues(const pair<CValue, CValue> &values) {
    return getValues<L, R>(values.first, values.second);
}


AddNode::AddNode(CASTNode *left_operand, CASTNode *right_operand) : BinaryOperationNode(left_operand, right_operand) {

}

CValue AddNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = left + right;
    } else if (typesAre<double, string>(values)) {
        auto [left, right] = getValues<double, string>(values);
        result = to_string(left) + right;
    } else if (typesAre<string, double>(values)) {
        auto [left, right] = getValues<string, double>(values);
        result = left + to_string(right);
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = left + right;
    }
    return result;
}


SubtractNode::SubtractNode(CASTNode *left_operand, CASTNode *right_operand) : BinaryOperationNode(left_operand,
                                                                                                  right_operand) {

}

CValue SubtractNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = left - right;
    }
    return result;
}


MultiplicationNode::MultiplicationNode(CASTNode *left_operand, CASTNode *right_operand) : BinaryOperationNode(
        left_operand,
        right_operand) {

}


CValue MultiplicationNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = left * right;
    }
    return result;
}


DivisionNode::DivisionNode(CASTNode *left_operand, CASTNode *right_operand) : BinaryOperationNode(left_operand,
                                                                                                  right_operand) {

}


CValue DivisionNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        if (right == 0) {
            return {};
        }
        result = left / right;
    }
    return result;
}


PowerNode::PowerNode(CASTNode *left_operand, CASTNode *right_operand) : BinaryOperationNode(left_operand,
                                                                                            right_operand) {

}

CValue PowerNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = pow(left, right);
    }
    return result;
}


RelationalOperationNode::RelationalOperationNode(CASTNode *left_operand, CASTNode *right_operand) : BinaryOperationNode(
        left_operand, right_operand) {

}

CValue RelationalOperationNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto values = evaluateValues(visitor);
    CValue result;
    if (typesAre<double, double>(values)) {
        auto [left, right] = getValues<double, double>(values);
        result = static_cast<double>(compare(left, right));
    } else if (typesAre<string, string>(values)) {
        auto [left, right] = getValues<string, string>(values);
        result = static_cast<double>(compare(left, right));
    }
    return result;
}


EqualNode::EqualNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand,
                                                                                                right_operand) {

}

bool EqualNode::compare(double lhs, double rhs) {
    return lhs == rhs;
}

bool EqualNode::compare(const string &lhs, const string &rhs) {
    return lhs == rhs;
}


LessThanNode::LessThanNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand,
                                                                                                      right_operand) {

}

bool LessThanNode::compare(double lhs, double rhs) {
    return lhs < rhs;
}

bool LessThanNode::compare(const string &lhs, const string &rhs) {
    return lhs < rhs;
}


NotEqualNode::NotEqualNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(left_operand,
                                                                                                      right_operand) {

}

bool NotEqualNode::compare(double lhs, double rhs) {
    return lhs != rhs;
}

bool NotEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs != rhs;
}


GreaterThanNode::GreaterThanNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(
        left_operand,
        right_operand) {
}

bool GreaterThanNode::compare(double lhs, double rhs) {
    return lhs > rhs;
}

bool GreaterThanNode::compare(const string &lhs, const string &rhs) {
    return lhs > rhs;
}


LessThanOrEqualNode::LessThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand) : RelationalOperationNode(
        left_operand,
        right_operand) {

}

bool LessThanOrEqualNode::compare(double lhs, double rhs) {
    return lhs <= rhs;
}

bool LessThanOrEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs <= rhs;
}


GreaterThanOrEqualNode::GreaterThanOrEqualNode(CASTNode *left_operand, CASTNode *right_operand)
        : RelationalOperationNode(
        left_operand, right_operand) {

}

bool GreaterThanOrEqualNode::compare(double lhs, double rhs) {
    return lhs >= rhs;
}

bool GreaterThanOrEqualNode::compare(const string &lhs, const string &rhs) {
    return lhs >= rhs;
}


UnaryOperationNode::UnaryOperationNode(CASTNode *operand) : m_operand(operand) {

}

UnaryOperationNode::~UnaryOperationNode() {
    delete m_operand;
}

CValue UnaryOperationNode::evaluateValue(CCycleDetectionVisitor &visitor) {
    return m_operand->evaluate(visitor);
}


NegationNode::NegationNode(CASTNode *operand) : UnaryOperationNode(operand) {

}


CValue NegationNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto value = evaluateValue(visitor);
    if (holds_alternative<double>(value)) {
        auto operand = get<double>(value);
        auto result = -operand;
        return result;
    }
    return {};
}


template<typename... Args>
FunctionNode::FunctionNode(Args... args) : m_args{args...} {

}

FunctionNode::~FunctionNode() {
    for (auto *arg: m_args) {
        delete arg;
    }
}


SumNode::SumNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue SumNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double sum = 0.0;
    bool at_least_one_number = false;
    for (auto &value: range) {
        if (holds_alternative<double>(value)) {
            at_least_one_number = true;
            sum += get<double>(value);
        }
    }

    if (at_least_one_number) {
        return {sum};
    }
    return {};
}

CountNode::CountNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue CountNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double count = 0.0;
    for (auto &value: range) {
        if (holds_alternative<monostate>(value)) {
            continue;
        }
        count++;
    }
    return {count};
}

MinNode::MinNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue MinNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double min = 0.0;
    bool at_least_one_number = false;
    for (auto &value: range) {
        if (holds_alternative<double>(value)) {
            double number = get<double>(value);
            if (at_least_one_number) {
                if (min > number) {
                    min = number;
                }
            } else {
                at_least_one_number = true;
                min = number;
            }

        }
    }

    if (at_least_one_number) {
        return {min};
    }
    return {};
}

MaxNode::MaxNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue MaxNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double max = 0.0;
    bool at_least_one_number = false;
    for (auto &value: range) {
        if (holds_alternative<double>(value)) {
            double number = get<double>(value);
            if (at_least_one_number) {
                if (max < number) {
                    max = number;
                }
            } else {
                at_least_one_number = true;
                max = number;
            }

        }
    }

    if (at_least_one_number) {
        return {max};
    }
    return {};
}

CountValNode::CountValNode(CASTNode *value, CASTNode *range) : FunctionNode(value, range) {

}

CValue CountValNode::evaluate(CCycleDetectionVisitor &visitor) {
    CValue value = m_args[0]->evaluate(visitor);
    auto range = m_args[1]->evaluateRange(visitor);

    if (holds_alternative<double>(value)) {
        return countValRange<double>(range, get<double>(value));
    } else if (holds_alternative<string>(value)) {
        return countValRange<string>(range, get<string>(value));
    } else {
        auto empty_cells_count = static_cast<double>(m_args[1]->rangeCapacity() - range.size());
        return countValRange<monostate>(range, get<monostate>(value)) + empty_cells_count;
    }
}

template<typename T>
double CountValNode::countValRange(const vector<CValue> &range, const T &value) const {
    double count = 0.0;
    for (auto &range_value: range) {
        if (holds_alternative<T>(range_value) && get<T>(range_value) == value) {
            count++;
        }
    }
    return count;
}


ConditionalNode::ConditionalNode(CASTNode *cond, CASTNode *if_true, CASTNode *if_false) : FunctionNode(cond, if_true,
                                                                                                       if_false) {

}

CValue ConditionalNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto condition_result = m_args[0]->evaluate(visitor);
    if (holds_alternative<double>(condition_result)) {
        if (get<double>(condition_result) == 0.0) {
            return m_args[2]->evaluate(visitor);
        } else {
            return m_args[1]->evaluate(visitor);
        }
    } else {
        return {};
    }
}


CASTExpressionBuilder::CASTExpressionBuilder(CSpreadsheet &spreadsheet, const CCell *current_cell) :
        m_spreadsheet(
                spreadsheet), m_cell(current_cell) {
}

CASTNode *CASTExpressionBuilder::getResult() {
    auto root = m_stack.top();
    m_stack.pop();
    return root;
}

void CASTExpressionBuilder::opAdd() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new AddNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opSub() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new SubtractNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opMul() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new MultiplicationNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opDiv() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new DivisionNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opPow() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new PowerNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opNeg() {
    auto arg = m_stack.top();
    m_stack.pop();
    CASTNode *node = new NegationNode(arg);
    m_stack.push(node);
}

void CASTExpressionBuilder::opEq() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new EqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opNe() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new NotEqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opLt() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new LessThanNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opLe() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new LessThanOrEqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opGt() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new GreaterThanNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::opGe() {
    auto [first, second] = getNodesPairAndPop();
    CASTNode *node = new GreaterThanOrEqualNode(first, second);
    m_stack.push(node);
}

void CASTExpressionBuilder::valNumber(double val) {
    CASTNode *node = new CNumberNode(val);
    m_stack.push(node);
}

void CASTExpressionBuilder::valString(string val) {
    CASTNode *node = new CStringNode(val);
    m_stack.push(node);
}

void CASTExpressionBuilder::valReference(string val) {
    CASTNode *node = new CReferenceNode(val, m_spreadsheet, m_cell->getShift());
    m_stack.push(node);
}

void CASTExpressionBuilder::valRange(string val) {
    auto [from, to] = CRange::splitRange(val);
    CASTNode *node = new CRangeNode(from, to, m_spreadsheet, m_cell->getShift());
    m_stack.push(node);
}

void CASTExpressionBuilder::funcCall(std::string fnName, int paramCount) {
    CASTNode *function_node = nullptr;
    if (fnName == "sum") {
        auto args = getNodesAndPop<1>();
        function_node = new SumNode(args[0]);
    } else if (fnName == "count") {
        auto args = getNodesAndPop<1>();
        function_node = new CountNode(args[0]);
    } else if (fnName == "min") {
        auto args = getNodesAndPop<1>();
        function_node = new MinNode(args[0]);
    } else if (fnName == "max") {
        auto args = getNodesAndPop<1>();
        function_node = new MaxNode(args[0]);
    } else if (fnName == "countval") {
        auto args = getNodesAndPop<2>();
        function_node = new CountValNode(args[1], args[0]);
    } else if (fnName == "if") {
        auto args = getNodesAndPop<3>();
        function_node = new ConditionalNode(args[2], args[1], args[0]);
    } else {
        throw invalid_argument("No matching function: " + fnName);
    }
    m_stack.push(function_node);

}

pair<CASTNode *, CASTNode *> CASTExpressionBuilder::getNodesPairAndPop() {
    auto *second_arg = m_stack.top();
    m_stack.pop();
    auto *first_arg = m_stack.top();
    m_stack.pop();
    return {first_arg, second_arg};
}

template<size_t NArgs>
vector<CASTNode *> CASTExpressionBuilder::getNodesAndPop() {
    vector<CASTNode *> nodes;
    for (size_t i = 0; i < NArgs; i++) {
        CASTNode *node = m_stack.top();
        m_stack.pop();
        nodes.push_back(node);
    }
    return nodes;
}


CCell::CCell(CValue value) : m_value(std::move(value)) {

}

CCell *CCell::createCell(const string &contents) {
    try {
        double number = stod(contents);
        return new CNumberCell(number);
    } catch (invalid_argument &e) {
        if (!contents.empty() && contents[0] == '=') {
            return new CExprCell(contents);
        } else {
            return new CStringCell(contents);
        }
    }
}


CNumberCell::CNumberCell(double value) : CCell(value) {
}

CNumberCell::CNumberCell() : CCell(0.0) {

}

CStringCell::CStringCell(const string &value) : CCell(value) {

}

CStringCell::CStringCell() : CCell("") {

}


CExprCell::CExprCell(const string &expression) : CCell(expression), m_shift({0, 0}) {

}


CExprCell::CExprCell() : CCell("="), m_shift({0, 0}) {

}


CCell *CStringCell::copy() const {
    return new CStringCell(get<string>(m_value));
}


CCell *CNumberCell::copy() const {
    return new CNumberCell(get<double>(m_value));
}

CCell *CExprCell::copy() const {
    auto *copy = new CExprCell(get<string>(m_value));
    copy->m_shift = m_shift;
    return copy;
}

string CStringCell::toString() const {
    string type = to_string(CCellType::STRING);
    string value = get<string>(m_value);
    string size = to_string(value.size());
    return type + ',' + size + ',' + value + ';';
}

istream &CStringCell::readCell(istream &is) {
    char sep;
    streamsize cell_value_size = 0;
    string cell_value;

    is >> cell_value_size >> sep;

    cell_value.resize(cell_value_size);
    is.read(cell_value.data(), cell_value_size);

    is >> sep;

    m_value = cell_value;
    return is;

}


string CNumberCell::toString() const {
    string type = to_string(CCellType::NUMBER);
    string value = to_string(get<double>(m_value));
    string size = to_string(value.size());
    return type + ',' + value + ';';
}

istream &CNumberCell::readCell(istream &is) {
    char sep;
    double value;
    is >> value >> sep;
    m_value = value;
    return is;

}


string CExprCell::toString() const {
    string type = to_string(CCellType::EXPRESSION);
    string shift = to_string(m_shift.first) + ',' + to_string(m_shift.second);
    string value = get<string>(m_value);
    string size = to_string(value.size());
    return type + ',' + shift + ',' + size + ',' + value + ';';
}


CValue CCell::getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor) {
    return m_value;
}


CValue CExprCell::getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor) {
    if (m_root == nullptr) {
        try {
            CASTExpressionBuilder builder(spreadsheet, this);
            parseExpression(get<string>(m_value), builder);
            m_root = unique_ptr<CASTNode>(builder.getResult());
        } catch (invalid_argument &e) {
            return m_value;
        }
    }
    visitor.visit(this);
    auto evaluation = m_root->evaluate(visitor);
    visitor.leave(this);
    return evaluation;
}

void CCell::shift(const pair<int, int> &offset) {
}

void CExprCell::shift(const pair<int, int> &offset) {
    if (m_root != nullptr) {
        m_root = nullptr;
    }
    m_shift.first += offset.first;
    m_shift.second += offset.second;
}

istream &CExprCell::readCell(istream &is) {

    char sep;
    int shift_row = 0, shift_col = 0;
    streamsize cell_value_size = 0;
    string cell_value;

    is >> shift_row >> sep
       >> shift_col >> sep
       >> cell_value_size >> sep;

    cell_value.resize(cell_value_size);
    is.read(cell_value.data(), cell_value_size);

    is >> sep;

    m_value = cell_value;
    m_shift = {shift_row, shift_col};

    return is;
}

pair<int, int> CCell::getShift() const {
    return {0, 0};
}

istream &operator>>(istream &is, CCell *cell) {
    return cell->readCell(is);
}

pair<int, int> CExprCell::getShift() const {
    return m_shift;
}


CRange::CRange(CSpreadsheet &spreadsheet) : m_spreadsheet(spreadsheet), m_selection({}), m_w(1), m_h(1) {

}

void CRange::select(const CPos &src, int w, int h) {
    m_selection_position = src;
    m_w = w, m_h = h;
    auto [row, col] = src.getCoords();

    auto row_beg = m_spreadsheet.getCells().lower_bound(row);
    auto row_end = m_spreadsheet.getCells().upper_bound(row + m_h - 1);

    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(col);
        auto col_end = row_beg->second.upper_bound(col + m_w - 1);
        while (col_beg != col_end) {
            pair<int, int> coords = {row_beg->first, col_beg->first};
            m_selection.emplace_back(coords, col_beg->second);
            col_beg++;
        }
        row_beg++;
    }
}

void CRange::select(const CPos &from, const CPos &to) {
    auto [row_offset, col_offset] = CPos::getOffset(from, to);
    int h = row_offset + 1, w = col_offset + 1;
    select(from, w, h);

}

void CRange::paste(const CPos &dst) {
    deleteCells(dst);
    auto offset = CPos::getOffset(m_selection_position, dst);
    shiftSelection(offset);
    pasteCells();
}

void CRange::deleteCells(const CPos &dst) {
    auto [dst_row, dst_col] = dst.getCoords();
    auto row_beg = m_spreadsheet.getCells().lower_bound(dst_row);
    auto row_end = m_spreadsheet.getCells().upper_bound(dst_row + m_h - 1);
    while (row_beg != row_end) {
        auto col_beg = row_beg->second.lower_bound(dst_col);
        auto col_end = row_beg->second.upper_bound(dst_col + m_w - 1);
        row_beg->second.erase(col_beg, col_end);
        if (row_beg->second.empty()) {
            row_beg = m_spreadsheet.getCells().erase(row_beg);
        } else {
            row_beg++;
        }
    }
}

void CRange::shiftSelection(const pair<int, int> &offset) {
    for (auto &[coords, cell]: m_selection) {
        if (cell.unique()) {
            cell->shift(offset);
        } else {
            cell = shared_ptr<CCell>(cell->copy());
            cell->shift(offset);
        }
        coords.first += offset.first;
        coords.second += offset.second;
    }
}

void CRange::pasteCells() {
    for (auto &[coords, cell]: m_selection) {
        auto [row, col] = coords;
        CSpreadsheet::setCell(m_spreadsheet.getCells(), CPos(row, col), cell);
    }
}

vector<CValue> CRange::evaluate(CCycleDetectionVisitor &visitor) {
    vector<CValue> values;
    for (auto &[coords, cell]: m_selection) {
        auto value = cell->getValue(m_spreadsheet, visitor);
        values.emplace_back(value);
    }
    return values;
}

pair<string, string> CRange::splitRange(const string &range) {
    string first, second;
    bool is_first_current = true;
    for (char c: range) {
        if (is_first_current) {
            if (c == ':') {
                is_first_current = false;
                continue;
            }
            first.push_back(c);
        } else {
            second.push_back(c);
        }
    }
    return {first, second};
}


CLoader::CLoader(istream &is) : m_is(&is), m_os(nullptr) {

}

CLoader::CLoader(ostream &os) : m_is(nullptr), m_os(&os) {

}

bool CLoader::save(const Cells &cells) {
    loadBuffer(cells);

    m_hash = getHash();
    m_os->write(m_hash.data(), static_cast<long>(HASH_SIZE));
    m_os->write(m_buffer.data(), static_cast<long>(m_buffer.size()));
    if (m_os->fail()) {
        return false;
    }
    return true;
}

string CLoader::getHash() const {
    unsigned long hash = 5381;
    for (char c: m_buffer) {
        hash = ((hash << 5) + hash) + c;
    };
    string string_hash = to_string(hash);
    size_t to_pad_size = HASH_SIZE - string_hash.size();
    return string("0", to_pad_size) + string_hash;
}

void CLoader::loadBuffer(const Cells &cells) {
    for (auto &[row_pos, column]: cells) {
        for (auto &[col_pos, cell]: column) {
            m_buffer.append(to_string(row_pos) + ',' + to_string(col_pos) + ',');
            m_buffer.append(cell->toString());
        }
    }
}

bool CLoader::load(Cells &cells) {
    if (!verify()) {
        return false;
    }
    istringstream iss(m_buffer);

    char sep;
    int row_pos, col_pos, cell_type;

    while (iss.peek() != EOF) {
        iss >> row_pos >> sep
            >> col_pos >> sep
            >> cell_type >> sep;

        CCell *cell = nullptr;
        if (cell_type == CCellType::NUMBER) {
            cell = new CNumberCell();
        } else if (cell_type == CCellType::STRING) {
            cell = new CStringCell();
        } else {
            cell = new CExprCell();
        }
        iss >> cell;
        auto shared_ptr_cell = shared_ptr<CCell>(cell);
        CSpreadsheet::setCell(cells, CPos(row_pos, col_pos), shared_ptr_cell);

    }
    return true;
}

bool CLoader::verify() {
    m_hash.resize(HASH_SIZE);
    m_is->read(m_hash.data(), HASH_SIZE);

    m_buffer.assign(istreambuf_iterator<char>(*m_is), istreambuf_iterator<char>());
    if (!m_is->good() && (m_is->bad() || !m_is->eof())) {
        return false;
    }
    if (m_hash != getHash()) {
        return false;
    }
    return true;
}


CSpreadsheet::CSpreadsheet(const CSpreadsheet &src) {
    for (const auto &row_element: src.m_cells) {
        int row = row_element.first;
        for (const auto &col_element: row_element.second) {
            int col = col_element.first;
            shared_ptr<CCell> copy = shared_ptr<CCell>(col_element.second->copy());
            setCell(m_cells, {row, col}, copy);
        }
    }
}

CSpreadsheet &CSpreadsheet::operator=(CSpreadsheet src) {
    swap(m_cells, src.m_cells);
    return *this;
}


bool CSpreadsheet::load(istream &is) {
    CLoader loader(is);
    Cells loaded;
    bool ok = loader.load(loaded);
    if (!ok) {
        return false;
    }
    m_cells = loaded;
    return true;
}

bool CSpreadsheet::save(ostream &os) const {
    CLoader loader(os);
    return loader.save(m_cells);
}

bool CSpreadsheet::setCell(CPos pos, string contents) {
    CCell *cell = CCell::createCell(contents);
    auto to_set = shared_ptr<CCell>(cell);
    return setCell(m_cells, pos, to_set);
}


bool CSpreadsheet::setCell(Cells &cells, const CPos &pos, shared_ptr<CCell> &cell) {
    auto [row, col] = pos.getCoords();
    auto row_element = cells.find(row);
    if (row_element == cells.end()) {
        cells.insert({row, {{col, cell}}});
        return true;
    }
    auto col_element = row_element->second.find(col);
    if (col_element == row_element->second.end()) {
        row_element->second.insert({col, cell});
        return true;
    }
    col_element->second = cell;
    return true;
}


CValue CSpreadsheet::getValue(CPos pos) {
    auto [row, col] = pos.getCoords();
    auto row_element = m_cells.find(row);
    if (row_element == m_cells.end()) {
        return {};
    }
    auto col_element = row_element->second.find(col);
    if (col_element == row_element->second.end()) {
        return {};
    }
    try {
        CCycleDetectionVisitor visitor;
        return col_element->second->getValue(*this, visitor);
    } catch (CCycleDetectedException &e) {
        return {};
    }
}


CValue CSpreadsheet::getValue(CPos pos, CCycleDetectionVisitor &visitor) {
    auto [row, col] = pos.getCoords();
    auto row_element = m_cells.find(row);
    if (row_element == m_cells.end()) {
        return {};
    }
    auto col_element = row_element->second.find(col);
    if (col_element == row_element->second.end()) {
        return {};
    }
    return col_element->second->getValue(*this, visitor);
}


void CSpreadsheet::copyRect(CPos dst, CPos src, int w, int h) {
    CRange range(*this);
    range.select(src, w, h);
    range.paste(dst);
}

Cells &CSpreadsheet::getCells() {
    return m_cells;
}


#ifndef __PROGTEST__

bool valueMatch(const CValue &r,
                const CValue &s) {
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0)
               || (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}


/**
 * Tester, which is used to structure tests and run them in more organized manner.
 */
struct Tester {

    /**
     * Layout for every test function - shows how each test function should be structured.
     */
    static void layout() {
        cout << '\n' << __func__ << " -> START" << endl;
        // tests...
        cout << __func__ << " ->    OK" << '\n' << endl;
    }

    /**
     * Run all selected function tests.
     */
    static void runAll() {
        basicTest();
        cycleDetectionTest();
        loaderTest();
        functionsTest();
    }

    /**
     * Basic tests - spreadsheet creating, copying, selection, saving and loading,
     * pasting and evaluation of numbers, literals and non function expressions.
     */
    static void basicTest() {
        cout << '\n' << __func__ << " -> START" << endl;

        CSpreadsheet x0, x1;
        std::ostringstream oss;
        std::istringstream iss;
        std::string data;
        assert(x0.setCell(CPos("A1"), "10"));
        assert(x0.setCell(CPos("A2"), "20.5"));
        assert(x0.setCell(CPos("A3"), "3e1"));
        assert(x0.setCell(CPos("A4"), "=40"));
        assert(x0.setCell(CPos("A5"), "=5e+1"));
        assert(x0.setCell(CPos("A6"), "raw text with any characters, including a quote \" or a newline\n"));
        assert(x0.setCell(CPos("A7"),
                          "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));
        assert(valueMatch(x0.getValue(CPos("A1")), CValue(10.0)));
        assert(valueMatch(x0.getValue(CPos("A2")), CValue(20.5)));
        assert(valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
        assert(valueMatch(x0.getValue(CPos("A4")), CValue(40.0)));
        assert(valueMatch(x0.getValue(CPos("A5")), CValue(50.0)));
        assert(valueMatch(x0.getValue(CPos("A6")),
                          CValue("raw text with any characters, including a quote \" or a newline\n")));
        assert(valueMatch(x0.getValue(CPos("A7")),
                          CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
        assert(valueMatch(x0.getValue(CPos("A8")), CValue()));
        assert(valueMatch(x0.getValue(CPos("AAAA9999")), CValue()));
        assert(x0.setCell(CPos("B1"), "=A1+A2*A3"));
        assert(x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
        assert(x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
        assert(x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
        assert(x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
        assert(x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
        assert(valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
        assert(valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));
        assert(x0.setCell(CPos("A1"), "12"));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
        assert(valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
        assert(valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));
        x1 = x0;
        assert(x0.setCell(CPos("A2"), "100"));
        assert(x1.setCell(CPos("A2"), "=A3+A5+A4"));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
        assert(valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
        assert(valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
        assert(valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
        assert(valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));
        oss.clear();
        oss.str("");
        assert(x0.save(oss));
        data = oss.str();
        iss.clear();
        iss.str(data);
        assert(x1.load(iss));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
        assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
        assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
        assert(x0.setCell(CPos("A3"), "4e1"));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
        assert(valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
        assert(valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
        oss.clear();
        oss.str("");
        assert(x0.save(oss));
        data = oss.str();
        for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
            data[i] ^= 0x5a;
        iss.clear();
        iss.str(data);
        assert(!x1.load(iss));
        assert(x0.setCell(CPos("D0"), "10"));
        assert(x0.setCell(CPos("D1"), "20"));
        assert(x0.setCell(CPos("D2"), "30"));
        assert(x0.setCell(CPos("D3"), "40"));
        assert(x0.setCell(CPos("D4"), "50"));
        assert(x0.setCell(CPos("E0"), "60"));
        assert(x0.setCell(CPos("E1"), "70"));
        assert(x0.setCell(CPos("E2"), "80"));
        assert(x0.setCell(CPos("E3"), "90"));
        assert(x0.setCell(CPos("E4"), "100"));
        assert(x0.setCell(CPos("F10"), "=D0+5"));
        assert(x0.setCell(CPos("F11"), "=$D0+5"));
        assert(x0.setCell(CPos("F12"), "=D$0+5"));
        assert(x0.setCell(CPos("F13"), "=$D$0+5"));
        x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);
        assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
        assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
        assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
        assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
        x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
        assert(valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("F14")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G10")), CValue()));
        assert(valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
        assert(valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
        assert(valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
        assert(valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
        assert(valueMatch(x0.getValue(CPos("H10")), CValue()));
        assert(valueMatch(x0.getValue(CPos("H11")), CValue()));
        auto res = x0.getValue(CPos("H12"));
        assert(valueMatch(x0.getValue(CPos("H12")), CValue()));
        assert(valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
        assert(valueMatch(x0.getValue(CPos("H14")), CValue()));
        assert(x0.setCell(CPos("F0"), "-27"));
        assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
        x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
        assert(valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
        assert(valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
        assert(valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));

        cout << __func__ << " ->    OK" << '\n' << endl;
    }

    /**
     * Tests cycle detection of spreadsheet capabilities.
     */
    static void cycleDetectionTest() {
        cout << '\n' << __func__ << " -> START" << endl;

        CSpreadsheet x3;
        assert(x3.setCell(CPos("A1"), "=5+A2"));
        assert(x3.setCell(CPos("A2"), "=A1"));
        assert(valueMatch(x3.getValue(CPos("A1")), CValue()));
        assert(x3.setCell(CPos("B1"), "42"));
        assert(x3.setCell(CPos("B2"), "42"));

        cout << __func__ << " ->    OK" << '\n' << endl;
    }

    /**
     * Advanced tests of saving and loading.
     */
    static void loaderTest() {
        cout << '\n' << __func__ << " -> START" << endl;


        std::ostringstream oss;
        std::istringstream iss;
        std::string data;

        CSpreadsheet x0;
        assert(x0.setCell(CPos("A0"), ",s,;tr,,;ing\n\n A0,;"));
        assert(x0.setCell(CPos("A1"), ",s,;tr,,;ing\n\n A1,;"));
        assert(x0.setCell(CPos("A2"), ",s,;tr,,;ing\n\n A2,;"));
        assert(x0.setCell(CPos("A3"), ",s,;tr,,;ing\n\n A3,;"));
        assert(x0.setCell(CPos("A4"), ",s,;tr,,;ing\n\n A4,;"));
        assert(x0.setCell(CPos("B0"), ",s,;tr,,;ing\n\n B0,;"));
        assert(x0.setCell(CPos("B1"), ",s,;tr,,;ing\n\n B1,;"));
        assert(x0.setCell(CPos("B2"), ",s,;tr,,;ing\n\n B2,;"));
        assert(x0.setCell(CPos("B3"), ",s,;tr,,;ing\n\n B3,;"));
        assert(x0.setCell(CPos("B4"), ",s,;tr,,;ing\n\n B4,;"));
        assert(x0.setCell(CPos("C0"), ",s,;tr,,;ing\n\n C0,;"));
        assert(x0.setCell(CPos("C1"), ",s,;tr,,;ing\n\n C1,;"));
        assert(x0.setCell(CPos("C2"), ",s,;tr,,;ing\n\n C2,;"));
        assert(x0.setCell(CPos("C3"), ",s,;tr,,;ing\n\n C3,;"));
        assert(x0.setCell(CPos("C4"), ",s,;tr,,;ing\n\n C4,;"));

        CSpreadsheet x1;
        assert(x1.setCell(CPos("B0"), "=\",s,;tr,,;ing\n\n B0,;\""));
        assert(x1.setCell(CPos("B1"), "=\",s,;tr,,;ing\n\n B1,;\""));
        assert(x1.setCell(CPos("B2"), "=\",s,;tr,,;ing\n\n B2,;\""));
        assert(x1.setCell(CPos("B3"), "=\",s,;tr,,;ing\n\n B3,;\""));
        assert(x1.setCell(CPos("B4"), "=\",s,;tr,,;ing\n\n B4,;\""));
        assert(x1.setCell(CPos("C0"), "=\",s,;tr,,;ing\n\n C0,;\""));
        assert(x1.setCell(CPos("C1"), "=\",s,;tr,,;ing\n\n C1,;\""));
        assert(x1.setCell(CPos("C2"), "=\",s,;tr,,;ing\n\n C2,;\""));
        assert(x1.setCell(CPos("C3"), "=\",s,;tr,,;ing\n\n C3,;\""));
        assert(x1.setCell(CPos("C4"), "=\",s,;tr,,;ing\n\n C4,;\""));
        assert(x1.setCell(CPos("D0"), "=\",s,;tr,,;ing\n\n D0,;\""));
        assert(x1.setCell(CPos("D1"), "=\",s,;tr,,;ing\n\n D1,;\""));
        assert(x1.setCell(CPos("D2"), "=\",s,;tr,,;ing\n\n D2,;\""));
        assert(x1.setCell(CPos("D3"), "=\",s,;tr,,;ing\n\n D3,;\""));
        assert(x1.setCell(CPos("D4"), "=\",s,;tr,,;ing\n\n D4,;\""));

        oss.clear();
        oss.str("");
        assert(x0.save(oss));
        data = oss.str();
        iss.clear();
        iss.str(data);
        assert(x1.load(iss));

        assert(valueMatch(x0.getValue(CPos("A0")), CValue(",s,;tr,,;ing\n\n A0,;")));
        assert(valueMatch(x0.getValue(CPos("A1")), CValue(",s,;tr,,;ing\n\n A1,;")));
        assert(valueMatch(x0.getValue(CPos("A2")), CValue(",s,;tr,,;ing\n\n A2,;")));
        assert(valueMatch(x0.getValue(CPos("A3")), CValue(",s,;tr,,;ing\n\n A3,;")));
        assert(valueMatch(x0.getValue(CPos("A4")), CValue(",s,;tr,,;ing\n\n A4,;")));
        assert(valueMatch(x0.getValue(CPos("B0")), CValue(",s,;tr,,;ing\n\n B0,;")));
        assert(valueMatch(x0.getValue(CPos("B1")), CValue(",s,;tr,,;ing\n\n B1,;")));
        assert(valueMatch(x0.getValue(CPos("B2")), CValue(",s,;tr,,;ing\n\n B2,;")));
        assert(valueMatch(x0.getValue(CPos("B3")), CValue(",s,;tr,,;ing\n\n B3,;")));
        assert(valueMatch(x0.getValue(CPos("B4")), CValue(",s,;tr,,;ing\n\n B4,;")));
        assert(valueMatch(x0.getValue(CPos("C0")), CValue(",s,;tr,,;ing\n\n C0,;")));
        assert(valueMatch(x0.getValue(CPos("C1")), CValue(",s,;tr,,;ing\n\n C1,;")));
        assert(valueMatch(x0.getValue(CPos("C2")), CValue(",s,;tr,,;ing\n\n C2,;")));
        assert(valueMatch(x0.getValue(CPos("C3")), CValue(",s,;tr,,;ing\n\n C3,;")));
        assert(valueMatch(x0.getValue(CPos("C4")), CValue(",s,;tr,,;ing\n\n C4,;")));

        assert(valueMatch(x1.getValue(CPos("A0")), CValue(",s,;tr,,;ing\n\n A0,;")));
        assert(valueMatch(x1.getValue(CPos("A1")), CValue(",s,;tr,,;ing\n\n A1,;")));
        assert(valueMatch(x1.getValue(CPos("A2")), CValue(",s,;tr,,;ing\n\n A2,;")));
        assert(valueMatch(x1.getValue(CPos("A3")), CValue(",s,;tr,,;ing\n\n A3,;")));
        assert(valueMatch(x1.getValue(CPos("A4")), CValue(",s,;tr,,;ing\n\n A4,;")));
        assert(valueMatch(x1.getValue(CPos("B0")), CValue(",s,;tr,,;ing\n\n B0,;")));
        assert(valueMatch(x1.getValue(CPos("B1")), CValue(",s,;tr,,;ing\n\n B1,;")));
        assert(valueMatch(x1.getValue(CPos("B2")), CValue(",s,;tr,,;ing\n\n B2,;")));
        assert(valueMatch(x1.getValue(CPos("B3")), CValue(",s,;tr,,;ing\n\n B3,;")));
        assert(valueMatch(x1.getValue(CPos("B4")), CValue(",s,;tr,,;ing\n\n B4,;")));
        assert(valueMatch(x1.getValue(CPos("C0")), CValue(",s,;tr,,;ing\n\n C0,;")));
        assert(valueMatch(x1.getValue(CPos("C1")), CValue(",s,;tr,,;ing\n\n C1,;")));
        assert(valueMatch(x1.getValue(CPos("C2")), CValue(",s,;tr,,;ing\n\n C2,;")));
        assert(valueMatch(x1.getValue(CPos("C3")), CValue(",s,;tr,,;ing\n\n C3,;")));
        assert(valueMatch(x1.getValue(CPos("C4")), CValue(",s,;tr,,;ing\n\n C4,;")));

        assert(valueMatch(x1.getValue(CPos("D0")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D1")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D2")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D3")), CValue()));
        assert(valueMatch(x1.getValue(CPos("D4")), CValue()));

        cout << __func__ << " ->    OK" << '\n' << endl;

    }

    /**
     * Tests function expressions.
     */
    static void functionsTest() {
        cout << '\n' << __func__ << " -> START" << endl;

        CSpreadsheet x0;
        assert(x0.setCell(CPos("A0"), "42"));
        assert(x0.setCell(CPos("A1"), "42"));
        assert(x0.setCell(CPos("A3"), "just a string"));
        assert(x0.setCell(CPos("A4"), "=\"just an expression string\""));
        assert(x0.setCell(CPos("A5"), "=1"));
        assert(x0.setCell(CPos("A6"), "=A0 + A1"));
        assert(x0.setCell(CPos("A7"), "=S69"));

        assert(valueMatch(x0.getValue(CPos("A4")), CValue("just an expression string")));

        assert(x0.setCell(CPos("A20"), "=sum(A0:A7)"));
        CValue result = x0.getValue(CPos("A20"));
        assert(valueMatch(result, CValue(169.0)));

        assert(x0.setCell(CPos("B0"), "42"));
        assert(x0.setCell(CPos("B1"), "42"));
        assert(x0.setCell(CPos("B3"), "just a string"));
        assert(x0.setCell(CPos("B4"), "=\"just an expression string\""));
        assert(x0.setCell(CPos("B5"), "=1"));
        assert(x0.setCell(CPos("B6"), "=A0 + A1"));
        assert(x0.setCell(CPos("B7"), "=S69"));

        result = x0.getValue(CPos("A20"));
        assert(valueMatch(result, CValue(169.0)));

        assert(x0.setCell(CPos("A8"), "42"));
        assert(x0.setCell(CPos("A9"), "42"));
        assert(x0.setCell(CPos("A11"), "just a string"));
        assert(x0.setCell(CPos("A12"), "=\"just an expression string\""));
        assert(x0.setCell(CPos("A13"), "=1"));
        assert(x0.setCell(CPos("A14"), "=A0 + A1"));
        assert(x0.setCell(CPos("A15"), "=S69"));
        assert(x0.setCell(CPos("B8"), "42"));
        assert(x0.setCell(CPos("B9"), "42"));
        assert(x0.setCell(CPos("B11"), "just a string"));
        assert(x0.setCell(CPos("B12"), "=\"just an expression string\""));
        assert(x0.setCell(CPos("B13"), "=1"));
        assert(x0.setCell(CPos("B14"), "=A0 + A1"));
        assert(x0.setCell(CPos("B15"), "=S69"));


        result = x0.getValue(CPos("A20"));
        assert(valueMatch(result, CValue(169.0)));

        assert(x0.setCell(CPos("A21"), "=sum(A0:B7)"));
        result = x0.getValue(CPos("A21"));
        assert(valueMatch(result, CValue(338.0)));

        assert(x0.setCell(CPos("A22"), "=count(A0:A7)"));
        result = x0.getValue(CPos("A22"));
        assert(valueMatch(result, CValue(6.0)));

        assert(x0.setCell(CPos("A22"), "=count(A0:B7)"));
        result = x0.getValue(CPos("A22"));
        assert(valueMatch(result, CValue(12.0)));

        assert(x0.setCell(CPos("A23"), "=count(A0:B15)"));
        result = x0.getValue(CPos("A23"));
        assert(valueMatch(result, CValue(24.0)));

        assert(x0.setCell(CPos("A24"), "=max(A0:B15)"));
        result = x0.getValue(CPos("A24"));
        assert(valueMatch(result, CValue(84.0)));

        assert(x0.setCell(CPos("A25"), "=min(A0:B15)"));
        result = x0.getValue(CPos("A25"));
        assert(valueMatch(result, CValue(1.0)));

        assert(x0.setCell(CPos("A26"), "=min(A2:B4)"));
        result = x0.getValue(CPos("A26"));
        assert(valueMatch(result, CValue()));

        assert(x0.setCell(CPos("A27"), "=max(A2:B4)"));
        result = x0.getValue(CPos("A27"));
        assert(valueMatch(result, CValue()));

        CPos pos = CPos("A28");
        assert(x0.setCell(pos, "=countval(A0, A2:B4)"));
        result = x0.getValue(pos);
        assert(valueMatch(result, CValue(0.0)));

        pos = CPos("A29");
        assert(x0.setCell(pos, "=countval(A0, A0:B15)"));
        result = x0.getValue(pos);
        assert(valueMatch(result, CValue(8.0)));

        assert(x0.setCell(pos, "=countval(2, A0:B15)"));
        result = x0.getValue(pos);
        assert(valueMatch(result, CValue(0.0)));

        assert(x0.setCell(pos, "=countval(\"just a string\", A0:B15)"));
        result = x0.getValue(pos);
        assert(valueMatch(result, CValue(4.0)));

        assert(x0.setCell(pos, "=countval(A15, A0:B15)"));
        result = x0.getValue(pos);
        assert(valueMatch(result, CValue(8.0)));

        assert(x0.setCell(pos, "=if(\"string\", 42, 69)"));
        result = x0.getValue(pos);
        assert(valueMatch(result, CValue()));

        assert(x0.setCell(pos, "=if(1, 42, 69)"));
        result = x0.getValue(pos);
        assert(valueMatch(result, CValue(42.0)));

        cout << __func__ << " ->    OK" << '\n' << endl;
    }

};


int main() {
    Tester::runAll();
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
