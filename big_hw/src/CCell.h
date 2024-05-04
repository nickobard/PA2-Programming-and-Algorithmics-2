//
// Created by bardanik on 11/04/24.
//

#ifndef BARDANIK_CCELL_H
#define BARDANIK_CCELL_H

#include "CASTExpressionBuilder.h"

using namespace literals;
using CValue = variant<monostate, double, string>;


class CSpreadsheet;

class CLoader;

enum CCellType {
    NUMBER,
    STRING,
    EXPRESSION
};

class CCell {
public:
    explicit CCell(CValue value);

    virtual ~CCell() = default;

    static CCell *createCell(const string &contents);

    virtual CValue getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor);

    virtual CCell *copy() const = 0;

    virtual void shift(const pair<int, int> &shift);

    virtual string toString() const = 0;

    virtual void readCell(istream &is) = 0;

    virtual pair<int, int> getShift() const;


protected:
    CValue m_value;
};


class CNumberCell : public CCell {
public:
    CNumberCell();

    explicit CNumberCell(double value);

    void readCell(std::istream &is) override;

    string toString() const override;

    CCell *copy() const override;

};

class CStringCell : public CCell {
public:
    CStringCell();

    explicit CStringCell(const string &value);

    string toString() const override;

    CCell *copy() const override;

    void readCell(std::istream &is) override;
};


class CExprCell : public CCell {
public:
    CExprCell();

    explicit CExprCell(const string &expression);

    CValue getValue(CSpreadsheet &spreadsheet, CCycleDetectionVisitor &visitor) override;

    CCell *copy() const override;

    string toString() const override;

    void readCell(std::istream &is) override;

    void shift(const pair<int, int> &shift) override;

    pair<int, int> getShift() const override;

private:
    unique_ptr<CASTNode> m_root;
    pair<int, int> m_shift;

};


#endif //BARDANIK_CCELL_H
