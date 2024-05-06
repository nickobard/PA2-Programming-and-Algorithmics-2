//
// Created by bardanik on 06/05/24.
//

#include "FunctionNode.h"
#include "BinaryOperationNode.h"

template<typename... Args>
FunctionNode::FunctionNode(Args... args) : m_args{args...} {

}

FunctionNode::~FunctionNode() {
    for (auto *arg: m_args) {
        delete arg;
    }
}


SumNode::SumNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue SumNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double sum = 0.0;
    bool at_least_one_number = false;
    for (auto &value: range) {
        if (holds_alternative<double>(value)) {
            at_least_one_number = true;
            sum += get<double>(value);
        }
    }

    if (at_least_one_number) {
        return {sum};
    }
    return {};
}

CountNode::CountNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue CountNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double count = 0.0;
    for (auto &value: range) {
        if (holds_alternative<monostate>(value)) {
            continue;
        }
        count++;
    }
    return {count};
}

MinNode::MinNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue MinNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double min = 0.0;
    bool at_least_one_number = false;
    for (auto &value: range) {
        if (holds_alternative<double>(value)) {
            at_least_one_number = true;
            double number = get<double>(value);
            if (min > number) {
                min = number;
            }
        }
    }

    if (at_least_one_number) {
        return {min};
    }
    return {};
}

MaxNode::MaxNode(CASTNode *m_range) : FunctionNode(m_range) {

}

CValue MaxNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto range_node = m_args[0];
    auto range = range_node->evaluateRange(visitor);
    double max = 0.0;
    bool at_least_one_number = false;
    for (auto &value: range) {
        if (holds_alternative<double>(value)) {
            at_least_one_number = true;
            double number = get<double>(value);
            if (max < number) {
                max = number;
            }
        }
    }

    if (at_least_one_number) {
        return {max};
    }
    return {};
}

CountValNode::CountValNode(CASTNode *value, CASTNode *range) : FunctionNode(value, range) {

}

CValue CountValNode::evaluate(CCycleDetectionVisitor &visitor) {
    CValue value = m_args[0]->evaluate(visitor);
    auto range = m_args[1]->evaluateRange(visitor);
    double count = 0.0;
    if (holds_alternative<double>(value)) {
        for (auto &range_value: range) {
            if (holds_alternative<double>(range_value)) {
                count++;
            }
        }
    } else if (holds_alternative<string>(value)) {
        for (auto &range_value: range) {
            if (holds_alternative<string>(range_value)) {
                count++;
            }
        }
    } else {
        for (auto &range_value: range) {
            if (holds_alternative<monostate>(range_value)) {
                count++;
            }
        }
    }
    return count;
}

ConditionalNode::ConditionalNode(CASTNode *cond, CASTNode *if_true, CASTNode *if_false) : FunctionNode(cond, if_true,
                                                                                                       if_false) {

}

CValue ConditionalNode::evaluate(CCycleDetectionVisitor &visitor) {
    auto condition_result = m_args[0]->evaluate(visitor);
    if (holds_alternative<double>(condition_result)) {
        if (get<double>(condition_result) == 0.0) {
            return m_args[1]->evaluate(visitor);
        } else {
            return m_args[2]->evaluate(visitor);
        }
    } else {
        return {};
    }
}
