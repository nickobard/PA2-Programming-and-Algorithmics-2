#!/bin/bash

cd ../src || exit
grep -vh '^#include' \
  SpreadsheetStructure/CPos.h \
  ExpressionBuilders/CycleDetectionVisitor/CCycleDetectionVisitor.h \
  ExpressionBuilders/ASTNodes/CASTNode.h \
  ExpressionBuilders/ASTNodes/BinaryOperationNode.h \
  ExpressionBuilders/ASTNodes/RelationalOperationNode.h \
  ExpressionBuilders/ASTNodes/UnaryOperationNode.h \
  ExpressionBuilders/CASTExpressionBuilder.h \
  SpreadsheetStructure/CCell.h \
  InputOutputUtilities/CLoader.h \
  CSpreadsheet.h >| ../assets/all_in_one.cpp

grep -vh '^#include' \
  SpreadsheetStructure/CPos.cpp \
  ExpressionBuilders/CycleDetectionVisitor/CCycleDetectionVisitor.cpp \
  ExpressionBuilders/ASTNodes/CASTNode.cpp \
  ExpressionBuilders/ASTNodes/BinaryOperationNode.cpp \
  ExpressionBuilders/ASTNodes/RelationalOperationNode.cpp \
  ExpressionBuilders/ASTNodes/UnaryOperationNode.cpp \
  ExpressionBuilders/CASTExpressionBuilder.cpp \
  SpreadsheetStructure/CCell.cpp \
  InputOutputUtilities/CLoader.cpp \
  CSpreadsheet.cpp >> ../assets/all_in_one.cpp