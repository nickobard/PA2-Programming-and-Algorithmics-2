//
// Created by bardanik on 11/04/24.
//
#ifndef BARDANIK_CPOS_H
#define BARDANIK_CPOS_H


#include <string>
#include <stdexcept>

using namespace std;

class CPos {
public:
    explicit CPos(string_view str);

    CPos(int row, int col);

    /**
     * @return row, column pair
     */
    pair<int, int> getCoords() const;

    static pair<int, int> getOffset(const CPos &src, const CPos &dst);

    void shift(const pair<int, int> &offset);

    static string toUpperCase(const string_view &str);

private:

    void splitPositionAndParse(const string &position);

    static int convertColLabelToNumber(const string &col_label);

    int m_row;
    bool m_absolute_row;
    int m_col;
    bool m_absolute_col;

    string m_col_label;

};

#endif //BARDANIK_CPOS_H
