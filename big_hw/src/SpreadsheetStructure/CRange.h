//
// Created by bardanik on 05/05/24.
//

#ifndef PA2_BIG_TASK_CRANGE_H
#define PA2_BIG_TASK_CRANGE_H

#include <vector>
#include "CCell.h"

using Range = vector<pair<pair<int, int>, shared_ptr<CCell>>>;

class CRange {
public:
    explicit CRange(Cells &spreadsheet);

    void select(const CPos &src, int w = 1, int h = 1);

    void select(const CPos &from, const CPos &to);

    void paste(const CPos &dst);

    CValue sum() const;

    CValue count() const;

    CValue min() const;

    CValue max() const;

    CValue countval(const CValue &reference_value) const;

private:

    void deleteCells(const CPos &dst);

    void shiftSelection(const pair<int, int> &offset);

    void pasteCells();

    Cells &m_cells;
    Range m_selection;
    CPos m_selection_position;
    int m_w, m_h;
};


#endif //PA2_BIG_TASK_CRANGE_H
