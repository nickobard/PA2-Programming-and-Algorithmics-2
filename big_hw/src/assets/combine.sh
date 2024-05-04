#!/bin/bash

cd ..
grep -vh '^#include' CPos.h CCycleDetectionVisitor.h CASTNode.h CASTExpressionBuilder.h CCell.h CLoader.h CSpreadsheet.h CPos.cpp CCycleDetectionVisitor.cpp CASTNode.cpp CASTExpressionBuilder.cpp CCell.cpp CLoader.cpp CSpreadsheet.cpp >| assets/all_in_one.cpp