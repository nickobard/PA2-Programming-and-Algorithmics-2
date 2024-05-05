//
// Created by bardanik on 05/05/24.
//

#ifndef PA2_BIG_TASK_CRANGE_H
#define PA2_BIG_TASK_CRANGE_H

#include <unordered_map>
#include "CCell.h"

class CSpreadsheet;

using Range = unordered_map<pair<int, int>, shared_ptr<CCell>>;

class CRange {
public:
    CRange(CSpreadsheet &spreadsheet);

    void selectCells(const CPos &src, int w = 1, int h = 1);

    void selectCells(const CPos &from, const CPos &to);

    void pasteCells(const CPos &dst);

    CValue sum() const;

    CValue count() const;

    CValue min() const;

    CValue max() const;

    CValue countval(const CValue &reference_value) const;

private:

    void deleteCells(const CPos &dst);

    void shiftSelection(const pair<int, int> &offset);

    CSpreadsheet &m_spreadsheet;
    Range m_selection;
    CPos m_selection_position;
    int w, h;
};


#endif //PA2_BIG_TASK_CRANGE_H
