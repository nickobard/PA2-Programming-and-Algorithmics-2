//
// Created by bardanik on 28/04/24.
//

#include "CSpreadsheet.h"

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
            m_buffer.append(to_string(cell.m_shift.first) + ',' + to_string(cell.m_shift.second) + ',');
            string value;
            if (cell.m_type == CellType::DOUBLE) {
                value = to_string(get<double>(cell.m_value));
            } else {
                value = get<string>(cell.m_value);
            }
            m_buffer.append(to_string(value.size()) + ',' + value + ';');
        }
    }
}

bool CLoader::load(Cells &to_load) {
    if (!verify()) {
        return false;
    }
    istringstream iss(m_buffer);

    char sep;
    int row_pos, col_pos, row_shift, col_shift;
    streamsize cell_values_size;
    string cell_value;

    while (iss) {
        iss >> row_pos >> sep
            >> col_pos >> sep
            >> row_shift >> sep
            >> col_shift >> sep
            >> cell_values_size >> sep;
        cell_value.resize(cell_values_size);
        iss.read(cell_value.data(), cell_values_size);
        iss >> sep;

        CCell cell = CCell(cell_value);
        cell.m_shift = {row_shift, col_shift};
        CSpreadsheet::setCell(to_load, CPos(row_pos, col_pos), cell);

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

