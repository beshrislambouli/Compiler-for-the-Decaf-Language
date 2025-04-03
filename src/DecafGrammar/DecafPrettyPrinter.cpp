#include "DecafPrettyPrinter.h"

namespace AST {

// Helper methods implementation
void ASTPrinter::print_indent() {
    for (int i = 0; i < indent_level; ++i) {
        out << "| ";
    }
}

void ASTPrinter::increase_indent() {
    indent_level++;
}

void ASTPrinter::decrease_indent() {
    if (indent_level > 0) indent_level--;
}

// Template method to print node headers with location info
template<typename T>
void ASTPrinter::print_node_header(const T& node, const std::string& extra_info) {
    print_indent();
    out << "+" << typeid(node).name(); // Print the demangled type name
    
    // Remove the namespace prefix for cleaner output
    std::string type_name = typeid(node).name();
    size_t pos = type_name.find("AST::");
    if (pos != std::string::npos) {
        type_name = type_name.substr(pos + 5); // Remove "AST::"
    }
    
    out << " [" << node.row << ":" << node.col << "]";
    
    if (!extra_info.empty()) {
        out << " " << extra_info;
    }
    
    out << "\n";
}

// Constructor
ASTPrinter::ASTPrinter(std::ostream& out) : out(out) {}

// Visit methods implementation
void ASTPrinter::visit(Program& node) {
    print_node_header(node);
    increase_indent();
    
    // Print imports
    if (!node.import_decls.empty()) {
        print_indent();
        out << "Imports:\n";
        increase_indent();
        for (const auto& import : node.import_decls) {
            import->accept(*this);
        }
        decrease_indent();
    }
    
    // Print field declarations
    if (!node.field_decls.empty()) {
        print_indent();
        out << "Field Declarations:\n";
        increase_indent();
        for (const auto& field_decl : node.field_decls) {
            field_decl->accept(*this);
        }
        decrease_indent();
    }
    
    // Print method declarations
    if (!node.method_decls.empty()) {
        print_indent();
        out << "Method Declarations:\n";
        increase_indent();
        for (const auto& method_decl : node.method_decls) {
            method_decl->accept(*this);
        }
        decrease_indent();
    }
    
    decrease_indent();
}

void ASTPrinter::visit(Import_Decl& node) {
    print_node_header(node);
    increase_indent();
    node.id->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Field_Decl& node) {
    print_node_header(node);
    increase_indent();
    
    print_indent();
    out << "Type:\n";
    increase_indent();
    node.field_type->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Fields:\n";
    increase_indent();
    for (const auto& field : node.fields) {
        field->accept(*this);
    }
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Id_Field_Decl& node) {
    print_node_header(node);
    increase_indent();
    node.id->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Array_Field_Decl& node) {
    print_node_header(node);
    increase_indent();
    
    print_indent();
    out << "ID:\n";
    increase_indent();
    node.id->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Size:\n";
    increase_indent();
    node.size->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Method_Decl& node) {
    std::string method_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "Method: " + method_name);
    increase_indent();
    
    print_indent();
    out << "Return Type:\n";
    increase_indent();
    node.method_type->accept(*this);
    decrease_indent();
    
    if (!node.parameters.empty()) {
        print_indent();
        out << "Parameters:\n";
        increase_indent();
        for (const auto& param : node.parameters) {
            param->accept(*this);
        }
        decrease_indent();
    }
    
    print_indent();
    out << "Body:\n";
    increase_indent();
    node.block->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Parameter& node) {
    std::string param_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "Parameter: " + param_name);
    increase_indent();
    
    print_indent();
    out << "Type:\n";
    increase_indent();
    node.field_type->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Field_Type& node) {
    print_node_header(node);
    increase_indent();
    node.type->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Method_Type& node) {
    print_node_header(node);
    increase_indent();
    node.type->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Block& node) {
    print_node_header(node);
    increase_indent();
    
    // Print field declarations
    if (!node.field_decls.empty()) {
        print_indent();
        out << "Field Declarations:\n";
        increase_indent();
        for (const auto& field_decl : node.field_decls) {
            field_decl->accept(*this);
        }
        decrease_indent();
    }
    
    // Print statements
    if (!node.statements.empty()) {
        print_indent();
        out << "Statements:\n";
        increase_indent();
        for (const auto& stmt : node.statements) {
            stmt->accept(*this);
        }
        decrease_indent();
    }
    
    decrease_indent();
}

void ASTPrinter::visit(Location_Assign_Op& node) {
    print_node_header(node, "Assignment");
    increase_indent();
    
    print_indent();
    out << "Location:\n";
    increase_indent();
    node.location->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.assign_op->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Expression:\n";
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Location_Incr& node) {
    print_node_header(node, "Increment/Decrement");
    increase_indent();
    
    print_indent();
    out << "Location:\n";
    increase_indent();
    node.location->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.increment->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Method_Call_Stmt& node) {
    std::string method_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "Method Call: " + method_name);
    increase_indent();
    
    if (!node.extern_args.empty()) {
        print_indent();
        out << "Arguments:\n";
        increase_indent();
        for (const auto& arg : node.extern_args) {
            arg->accept(*this);
        }
        decrease_indent();
    }
    
    decrease_indent();
}

void ASTPrinter::visit(If_Else_Stmt& node) {
    print_node_header(node, "If-Else Statement");
    increase_indent();
    
    print_indent();
    out << "Condition:\n";
    increase_indent();
    node.expr_if->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Then Block:\n";
    increase_indent();
    node.block_then->accept(*this);
    decrease_indent();
    
    if (node.block_else) {
        print_indent();
        out << "Else Block:\n";
        increase_indent();
        node.block_else->accept(*this);
        decrease_indent();
    }
    
    decrease_indent();
}

void ASTPrinter::visit(For_Stmt& node) {
    std::string var_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "For Loop: " + var_name);
    increase_indent();
    
    print_indent();
    out << "Initialization:\n";
    increase_indent();
    node.expr_init->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Condition:\n";
    increase_indent();
    node.expr_cond->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Update:\n";
    increase_indent();
    node.for_update->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(While_Stmt& node) {
    print_node_header(node, "While Loop");
    increase_indent();
    
    print_indent();
    out << "Condition:\n";
    increase_indent();
    node.expr_cond->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Body:\n";
    increase_indent();
    node.block->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Return_Stmt& node) {
    print_node_header(node, "Return Statement");
    increase_indent();
    
    if (node.expr) {
        print_indent();
        out << "Expression:\n";
        increase_indent();
        node.expr->accept(*this);
        decrease_indent();
    } else {
        print_indent();
        out << "<void return>\n";
    }
    
    decrease_indent();
}

void ASTPrinter::visit(Break_Stmt& node) {
    print_node_header(node, "Break Statement");
}

void ASTPrinter::visit(Continue_Stmt& node) {
    print_node_header(node, "Continue Statement");
}

void ASTPrinter::visit(For_Upd_Assign_Op& node) {
    print_node_header(node, "For Update Assignment");
    increase_indent();
    
    print_indent();
    out << "Location:\n";
    increase_indent();
    node.location->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.assign_Op->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Expression:\n";
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(For_Upd_Incr& node) {
    print_node_header(node, "For Update Increment");
    increase_indent();
    
    print_indent();
    out << "Location:\n";
    increase_indent();
    node.location->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.increment->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Loc_Var& node) {
    std::string var_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "Variable: " + var_name);
    increase_indent();
    node.id->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Loc_Array& node) {
    std::string array_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "Array Access: " + array_name);
    increase_indent();
    
    print_indent();
    out << "ID:\n";
    increase_indent();
    node.id->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Index:\n";
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Minus_Expr& node) {
    print_node_header(node, "Unary Minus");
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Not_Expr& node) {
    print_node_header(node, "Logical Not");
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(INT_Expr& node) {
    print_node_header(node, "INT Cast");
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(LONG_Expr& node) {
    print_node_header(node, "LONG Cast");
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Paren_Expr& node) {
    print_node_header(node, "Parenthesized Expression");
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Mul_Op_Expr& node) {
    print_node_header(node, "Multiplication Operation");
    increase_indent();
    
    print_indent();
    out << "Left:\n";
    increase_indent();
    node.expr_lhs->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.bin_op->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Right:\n";
    increase_indent();
    node.expr_rhs->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Add_Op_Expr& node) {
    print_node_header(node, "Addition Operation");
    increase_indent();
    
    print_indent();
    out << "Left:\n";
    increase_indent();
    node.expr_lhs->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.bin_op->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Right:\n";
    increase_indent();
    node.expr_rhs->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Rel_Op_Expr& node) {
    print_node_header(node, "Relational Operation");
    increase_indent();
    
    print_indent();
    out << "Left:\n";
    increase_indent();
    node.expr_lhs->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.bin_op->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Right:\n";
    increase_indent();
    node.expr_rhs->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Eq_Op_Expr& node) {
    print_node_header(node, "Equality Operation");
    increase_indent();
    
    print_indent();
    out << "Left:\n";
    increase_indent();
    node.expr_lhs->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.bin_op->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Right:\n";
    increase_indent();
    node.expr_rhs->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Logic_Op_Expr& node) {
    print_node_header(node, "Logical Operation");
    increase_indent();
    
    print_indent();
    out << "Left:\n";
    increase_indent();
    node.expr_lhs->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Operator:\n";
    increase_indent();
    node.bin_op->accept(*this);
    decrease_indent();
    
    print_indent();
    out << "Right:\n";
    increase_indent();
    node.expr_rhs->accept(*this);
    decrease_indent();
    
    decrease_indent();
}

void ASTPrinter::visit(Loc_Expr& node) {
    print_node_header(node, "Location Expression");
    increase_indent();
    node.location->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Method_Call_Expr& node) {
    std::string method_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "Method Call Expression: " + method_name);
    increase_indent();
    
    if (!node.extern_args.empty()) {
        print_indent();
        out << "Arguments:\n";
        increase_indent();
        for (const auto& arg : node.extern_args) {
            arg->accept(*this);
        }
        decrease_indent();
    }
    
    decrease_indent();
}

void ASTPrinter::visit(Literal_Expr& node) {
    print_node_header(node, "Literal Expression");
    increase_indent();
    node.literal->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Len_Expr& node) {
    std::string id_name = node.id ? node.id->id : "<unnamed>";
    print_node_header(node, "Length Expression: " + id_name);
    increase_indent();
    node.id->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(Expr_Arg& node) {
    print_node_header(node, "Expression Argument");
    increase_indent();
    node.expr->accept(*this);
    decrease_indent();
}

void ASTPrinter::visit(String_Arg& node) {
    print_node_header(node, "String Argument: \"" + node.string_arg + "\"");
}

void ASTPrinter::visit(Assign_Op& node) {
    std::string op_str;
    switch (node.type) {
        case Assign_Op::ASSIGN: op_str = "="; break;
        case Assign_Op::PLUS_ASSIGN: op_str = "+="; break;
        case Assign_Op::MINUS_ASSIGN: op_str = "-="; break;
        case Assign_Op::MUL_ASSIGN: op_str = "*="; break;
        case Assign_Op::DIV_ASSIGN: op_str = "/="; break;
        case Assign_Op::MOD_ASSIGN: op_str = "%="; break;
    }
    print_node_header(node, "Operator: " + op_str);
}

void ASTPrinter::visit(Increment& node) {
    std::string op_str = (node.type == Increment::INCREMENT) ? "++" : "--";
    print_node_header(node, "Operator: " + op_str);
}

void ASTPrinter::visit(Mul_Op& node) {
    std::string op_str;
    switch (node.type) {
        case Mul_Op::STAR: op_str = "*"; break;
        case Mul_Op::DIV: op_str = "/"; break;
        case Mul_Op::MOD: op_str = "%"; break;
    }
    print_node_header(node, "Operator: " + op_str);
}

void ASTPrinter::visit(Add_Op& node) {
    std::string op_str = (node.type == Add_Op::PLUS) ? "+" : "-";
    print_node_header(node, "Operator: " + op_str);
}

void ASTPrinter::visit(Rel_Op& node) {
    std::string op_str;
    switch (node.type) {
        case Rel_Op::LT: op_str = "<"; break;
        case Rel_Op::GT: op_str = ">"; break;
        case Rel_Op::LE: op_str = "<="; break;
        case Rel_Op::GE: op_str = ">="; break;
    }
    print_node_header(node, "Operator: " + op_str);
}

void ASTPrinter::visit(Eq_Op& node) {
    std::string op_str = (node.type == Eq_Op::EQ) ? "==" : "!=";
    print_node_header(node, "Operator: " + op_str);
}

void ASTPrinter::visit(Logic_Op& node) {
    std::string op_str = (node.type == Logic_Op::OR) ? "||" : "&&";
    print_node_header(node, "Operator: " + op_str);
}

void ASTPrinter::visit(Literal& node) {
    print_node_header(node, "Value: " + node.literal);
}

void ASTPrinter::visit(Int_Lit& node) {
    std::string value = (node.minus ? "-" : "") + node.literal;
    print_node_header(node, "Integer Literal: " + value);
}

void ASTPrinter::visit(Long_Lit& node) {
    print_node_header(node, "Long Literal: " + node.literal + "L");
}

void ASTPrinter::visit(Char_Lit& node) {
    print_node_header(node, "Character Literal: '" + node.literal + "'");
}

void ASTPrinter::visit(Bool_Lit& node) {
    print_node_header(node, "Boolean Literal: " + node.literal);
}

void ASTPrinter::visit(Type& node) {
    std::string type_str;
    switch (node.type) {
        case Type::Int: type_str = "int"; break;
        case Type::Long: type_str = "long"; break;
        case Type::Char: type_str = "char"; break;
        case Type::Bool: type_str = "bool"; break;
        case Type::Void: type_str = "void"; break;
        case Type::Null_Type: type_str = "null"; break;
    }
    print_node_header(node, "Type: " + type_str);
}

void ASTPrinter::visit(Id& node) {
    print_node_header(node, "Identifier: " + node.id);
}

} // namespace AST