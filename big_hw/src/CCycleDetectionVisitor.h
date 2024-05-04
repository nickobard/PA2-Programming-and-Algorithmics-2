//
// Created by bardanik on 03/05/24.
//
#ifndef PA2_BIG_TASK_CCYCLEDETECTIONVISITOR_H
#define PA2_BIG_TASK_CCYCLEDETECTIONVISITOR_H

#include <unordered_set>
#include <exception>

using namespace std;

class CCell;

class CCycleDetectedException : public exception {
};

class CCycleDetectionVisitor {
public:
    void visit(const CCell *cell);

    void leave(const CCell *cell);

private:
    unordered_set<const CCell *> m_opened;
};

#endif //PA2_BIG_TASK_CCYCLEDETECTIONVISITOR_H