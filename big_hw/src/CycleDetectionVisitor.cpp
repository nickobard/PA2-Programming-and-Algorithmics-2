//
// Created by bardanik on 03/05/24.
//

#include "CCycleDetectionVisitor.h"

void CCycleDetectionVisitor::visit(const CCell *cell) {
    const auto &result = m_opened.insert(cell);
    bool inserted = result.second;
    if (!inserted) {
        throw CCycleDetectedException();
    }
}

void CCycleDetectionVisitor::leave(const CCell *cell) {
    m_opened.erase(cell);
}
