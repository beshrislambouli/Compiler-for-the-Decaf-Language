#pragma once
#include "Linear.h"


namespace ConstantFolding {
class ConstantFolding {
    std::unique_ptr<Linear::Method>& method;
public:
    ConstantFolding (std::unique_ptr<Linear::Method>& method) : method(method) {}

    bool apply () {
        bool changed = false;

        for (int i = 0 ; i < method->instrs.size () ; i ++ ) {
            if ( ! is_instance_of(method->instrs[i], Linear::Binary) ) continue;

            Linear::Binary* binary_ptr = dynamic_cast<Linear::Binary*>(method->instrs[i].get());

            if ( ! is_instance_of (binary_ptr->operands [0], Linear::Literal)) continue;
            if ( ! is_instance_of (binary_ptr->operands [1], Linear::Literal)) continue;

            long long num1 = std::stoll(binary_ptr->operands [0]->id);
            long long num2 = std::stoll(binary_ptr->operands [1]->id);

            long long res;
            switch (binary_ptr->op) {
                // Add cases when Op enum is populated
                case Linear::Binary::Plus:
                    res = num1 + num2;
                    break;
                case Linear::Binary::Minus:
                    res = num1 - num2;
                    break;
                case Linear::Binary::Mul:
                    res = num1 * num2;
                    break;
                case Linear::Binary::Div:
                    res = num1 / num2;
                    break;
                case Linear::Binary::Mod:
                    res = num1 % num2;
                    break;
                case Linear::Binary::LT:
                    res = num1 < num2;
                    break;
                case Linear::Binary::GT:
                    res = num1 > num2;
                    break;
                case Linear::Binary::LE:
                    res = num1 <= num2;
                    break;
                case Linear::Binary::GE:
                    res = num1 >= num2;
                    break;
                case Linear::Binary::EQ:
                    res = num1 == num2;
                    break;
                case Linear::Binary::NEQ:
                    res = num1 != num2;
                    break;
                case Linear::Binary::OR:
                    res = num1 | num2;
                    break;
                case Linear::Binary::AND:
                    res = num1 & num2;
                    break;
                default: assert(false);
            }
            
            auto res_ptr = std::make_unique <Linear::Literal> (binary_ptr->dist->type, std::to_string (res));
            auto asn_ptr = std::make_unique <Linear::Assign > ();
            asn_ptr -> dist = std::move (binary_ptr->dist);
            asn_ptr -> operands .push_back (std::move(res_ptr));

            method->instrs[i] = std::move (asn_ptr);
            changed = true;
        }

        for (int i = 0 ; i < method->instrs.size () ; i ++ ) {
            if ( ! is_instance_of(method->instrs[i], Linear::Unary) ) continue;

            Linear::Unary* Unary_ptr = dynamic_cast<Linear::Unary*>(method->instrs[i].get());

            if ( ! is_instance_of (Unary_ptr->operands [0], Linear::Literal)) continue;
            if ( Unary_ptr -> op == Linear::Unary::LONG_CAST ) continue;
            if ( Unary_ptr->operands [0]->type == Linear::Type::Long ) continue;

            long long num1 = std::stoll(Unary_ptr->operands [0]->id);
            long long res;
            switch (Unary_ptr->op) {
                case Linear::Unary::Not:
                    res = ! num1;
                    break;
                case Linear::Unary::Minus:
                    res = - num1 ;
                    break;
                default: assert(false);
            }
            
            auto res_ptr = std::make_unique <Linear::Literal> (Unary_ptr->dist->type, std::to_string (res));
            auto asn_ptr = std::make_unique <Linear::Assign > ();
            asn_ptr -> dist = std::move (Unary_ptr->dist);
            asn_ptr -> operands .push_back (std::move(res_ptr));

            method->instrs[i] = std::move (asn_ptr);
            changed = true;
        }


        return changed;
    }
};
} 


