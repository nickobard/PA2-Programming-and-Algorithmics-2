//
// Created by bardanik on 28/04/24.
//
#ifndef PA2_BIG_TASK_CLOADER_H
#define PA2_BIG_TASK_CLOADER_H

#include <iostream>
#include <sstream>
#include <map>
#include "../SpreadsheetStructure/CCell.h"

using namespace std;
using Cells = map<int, map<int, shared_ptr<CCell>>>;

class CLoader {
public:
    explicit CLoader(istream &is);

    explicit CLoader(ostream &os);

    bool save(const Cells &cells);

    bool load(Cells &to_load);

private:

    bool verify();

    void loadBuffer(const Cells &cells);

    string getHash() const;

    static constexpr size_t HASH_SIZE = 20;

    istream *m_is;
    ostream *m_os;
    string m_buffer;
    string m_hash;
};


#endif //PA2_BIG_TASK_CLOADER_H
