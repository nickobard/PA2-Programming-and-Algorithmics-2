//
// Created by bardanik on 28/04/24.
//

#include "../CSpreadsheet.h"

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

