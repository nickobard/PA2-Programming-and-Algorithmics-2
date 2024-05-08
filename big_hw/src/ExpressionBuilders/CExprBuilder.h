//
// Created by bardanik on 14/04/24.
//

#ifndef PA2_BIG_TASK_CEXPRBUILDER_H
#define PA2_BIG_TASK_CEXPRBUILDER_H

#include <string>
#include <variant>

using namespace std;
using namespace literals;

// Value type that is stored in cells.
using CValue = variant<monostate, double, string>;

/**
 * Expression builder class that is using provided static library which has a function parseExpression(...)
 * to parse an expression and call CExprBuilder methods to evaluate the expression or construct some
 * data structure from the expression.
 */
class CExprBuilder {
public:
    /**
     * Is called when add operation is parsed by the parser.
     */
    virtual void opAdd() = 0;

    /**
     * Is called when      * Is called when  operation is parsed by the parser.
 operation is parsed by the parser.
     */
    virtual void opSub() = 0;

    /**
     * Is called when multiplication operation is parsed by the parser.
     */
    virtual void opMul() = 0;

    /**
     * Is called when division operation is parsed by the parser.
     */
    virtual void opDiv() = 0;

    /**
     * Is called when power operation is parsed by the parser.
     */
    virtual void opPow() = 0;

    /**
     * Is called when negation operation is parsed by the parser.
     */
    virtual void opNeg() = 0;

    /**
     * Is called when equals operation is parsed by the parser.
     */
    virtual void opEq() = 0;

    /**
     * Is called when not equals operation is parsed by the parser.
     */
    virtual void opNe() = 0;

    /**
     * Is called when less than operation is parsed by the parser.
     */
    virtual void opLt() = 0;

    /**
     * Is called when less that or equals operation is parsed by the parser.
     */
    virtual void opLe() = 0;

    /**
     * Is called when greater than operation is parsed by the parser.
     */
    virtual void opGt() = 0;

    /**
     * Is called when greater than or equal operation is parsed by the parser.
     */
    virtual void opGe() = 0;

    /**
     * Is called when parser evaluates some number in expression.
     * @param val - numeric value that is being evaluated by the parser.
     */
    virtual void valNumber(double val) = 0;

    /**
     * Is called when string literal is parsed.
     * @param val - parsed string literal, without double quoutes.
     */
    virtual void valString(std::string val) = 0;

    /**
     * Is called when reference to another cell is parsed.
     * @param val - returns string representation of another cell position.
     */
    virtual void valReference(std::string val) = 0;

    /**
     * Is called when range of cells is parsed.
     * @param val - string representation of cells range.
     */
    virtual void valRange(std::string val) = 0;

    /**
     * Is called when function is parsed.
     * @param fnName - name of the function.
     * @param paramCount - number of parameters in the function.
     */
    virtual void funcCall(std::string fnName,
                          int paramCount) = 0;

};

/**
 * Function provided by static library to parse an expression represented
 * as a string using and expression builder.
 * @param expr - string representation of expression.
 * @param builder - expression builder, which abstract methods are called by the parser to parse expression.
 */
void parseExpression(std::string expr,
                     CExprBuilder &builder);


#endif //PA2_BIG_TASK_CEXPRBUILDER_H
