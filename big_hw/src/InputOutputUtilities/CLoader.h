//
// Created by bardanik on 28/04/24.
//
#ifndef PA2_BIG_TASK_CLOADER_H
#define PA2_BIG_TASK_CLOADER_H

#include "../SpreadsheetStructure/CCell.h"

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


#endif //PA2_BIG_TASK_CLOADER_H
