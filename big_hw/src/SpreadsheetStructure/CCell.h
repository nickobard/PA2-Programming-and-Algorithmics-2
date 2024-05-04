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

using Cells = map<int, map<int, CCell>>;


enum class CellType {
    DOUBLE,
    STRING,
    EXPRESSION
};

class CSpreadsheet;

class CLoader;

class CCell {
public:
    CCell(const CCell &src);

    CCell &operator=(const CCell &src);

    ~CCell();

    explicit CCell(const string &contents);

    CValue getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor);

    void shift(const pair<int, int> &shift);

    pair<int, int> getShift() const;

private:

    CValue m_value;
    CellType m_type;
    CASTNode *m_root;
    pair<int, int> m_shift;

    friend class CLoader;
};



#endif //BARDANIK_CCELL_H
