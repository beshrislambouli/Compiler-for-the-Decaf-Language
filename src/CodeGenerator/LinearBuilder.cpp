
#include "LinearBuilder.h"


#define is_instance_of(uptr, Type) (dynamic_cast<Type*>((uptr).get()) != nullptr)

template <typename T>
std::unique_ptr<T> cast (std::unique_ptr<Linear::Operand>&& ret) {
    if ( is_instance_of (ret, T) ) {
        T* raw_pointer = dynamic_cast<T*>(ret.get());
        ret.release();
        return std::unique_ptr<T>(raw_pointer);
    }

    std::cout << "Error cast" << std::endl;
    exit(1);
} 


std::unique_ptr<Linear::Program> LinearBuilder::build (std::unique_ptr<AST::Program> program) {
    auto linear_program = std::make_unique<Linear::Program> ();

    for (int i = 0 ; i < program->method_decls.size() ; i ++ ) {
        MethodBuilder method_builder;
        auto method = method_builder.build (std::move(program->method_decls[i]));
        linear_program->methods .push_back (std::move(method));
    }

    return linear_program;
}
std::unique_ptr<Linear::Method>  MethodBuilder::build (std::unique_ptr<AST::Method_Decl> method) {
    auto linear_method = std::make_unique<Linear::Method>();

    linear_method->type= T(method->method_type->type->type);
    linear_method->id  = method->id->id;
    utils.method_name = linear_method->id;

    for(auto& parameter : method->parameters) {
        parameter->accept (*this);

        auto var = cast<Linear::Var>(std::move(utils.ret));
        linear_method->params.push_back (std::move(var));
    }

    method->block->accept(*this);

    for (int i = 0 ; i < utils.instrs.size() ; i ++ ) {
        linear_method->instrs.push_back (std::move(utils.instrs[i]));
    }

    return linear_method;
} 
void MethodBuilder::visit(AST::Program& node) {}
void MethodBuilder::visit(AST::Import_Decl& node) {}
void MethodBuilder::visit(AST::Field_Decl& node) {

    for (auto& field : node.fields ) {
        if ( is_instance_of (field, AST::Id_Field_Decl) ) {
            utils.declare ( T (node.field_type->type->type), field->id->id);
        }
        else if ( is_instance_of (field, AST::Array_Field_Decl ) ) {

            auto array_field_decl = dynamic_cast<AST::Array_Field_Decl*>(field.get());
            std::string size = array_field_decl -> size -> literal;

            utils.declare ( T (node.field_type->type->type), field->id->id, size);
        } 
    }
    
}
void MethodBuilder::visit(AST::Id_Field_Decl& node) {}
void MethodBuilder::visit(AST::Array_Field_Decl& node) {}
void MethodBuilder::visit(AST::Method_Decl& node) {}
void MethodBuilder::visit(AST::Parameter& node) {
    auto var = std::make_unique<Linear::Var>();

    var->type = T (node.field_type->type->type); 
    var->id = node.id->id;
    
    utils.ret = std::move(var);
}
void MethodBuilder::visit(AST::Block& node) {
    utils.push_scope();

    for (auto& field_decl : node.field_decls) {
        field_decl -> accept (*this);
    }

    for (auto& statement : node.statements) {
        statement -> accept (*this);
    }

    utils.pop_scope();
}


void MethodBuilder::visit(AST::Field_Type& node) {}
void MethodBuilder::visit(AST::Method_Type& node) {}

void MethodBuilder::visit(AST::Location_Assign_Op& node) {
    if (node.assign_op->type == AST::Assign_Op::ASSIGN ) {

        node.location -> accept(*this);
        auto dist = cast<Linear::Location>(std::move(utils.ret));

        node.expr -> accept (*this);
        auto operand = cast<Linear::Operand>(std::move(utils.ret));

        utils.assign(std::move(dist), std::move(operand));
        return;
    }
    
    node.location -> accept(*this);
    auto dist = cast<Linear::Location>(std::move(utils.ret));


    node.location -> accept(*this);
    auto operand1 = cast<Linear::Operand>(std::move(utils.ret));

    node.expr -> accept (*this);
    auto operand2 = cast<Linear::Operand>(std::move(utils.ret));

    Linear::Binary::Op op;
    switch (node.assign_op->type)
    {
    case AST::Assign_Op::PLUS_ASSIGN:
        op = Linear::Binary::Plus;
        break;
    
    case AST::Assign_Op::MINUS_ASSIGN:
        op = Linear::Binary::Minus;
        break;
    
    case AST::Assign_Op::MUL_ASSIGN:
        op = Linear::Binary::Mul;
        break;
    
    case AST::Assign_Op::DIV_ASSIGN:
        op = Linear::Binary::Div;
        break;

    case AST::Assign_Op::MOD_ASSIGN:
        op = Linear::Binary::Mod;
        break;
    
    default:
        std::cout << "Error MethodBuilder::visit AST::Location_Assign_Op" << std::endl;
        break;
    }

    utils.binary(std::move(dist),std::move(operand1),std::move(operand2), op);
}

void MethodBuilder::visit(AST::Location_Incr& node) {
    node.location -> accept(*this);
    auto dist = cast<Linear::Location>(std::move(utils.ret));


    node.location -> accept(*this);
    auto operand1 = cast<Linear::Operand>(std::move(utils.ret));

    
    auto operand2 = std::make_unique<Linear::Literal>();
    operand2->type = T(node.location->type_t->type);
    operand2->id = "1";

    Linear::Binary::Op op;
    switch (node.increment->type)
    {
    case AST::Increment::INCREMENT:
        op = Linear::Binary::Plus;
        break;
    
    case AST::Increment::DECREMENT:
        op = Linear::Binary::Minus;
        break;
    
    default:
        break;
    }

    utils.binary(std::move(dist),std::move(operand1),std::move(operand2), op);
}
void MethodBuilder::visit(AST::Method_Call_Stmt& node) {}
void MethodBuilder::visit(AST::If_Else_Stmt& node) {
    std::string if_then = "if_then" + utils.get_label();
    std::string if_else = "if_else" + utils.get_label();
    std::string if_end  = "end_if"  + utils.get_label();


    // jump condition
    node.expr_if->accept(*this);
    auto condition = std::move(utils.ret);
    utils.push_instr(std::make_unique<Linear::J_Cond>(if_then, std::move(condition)));
    utils.push_instr(std::make_unique<Linear::J_UnCond>(if_else));

    //then
    utils.push_instr(std::make_unique<Linear::Label>(if_then));
    node.block_then->accept(*this);
    utils.push_instr(std::make_unique<Linear::J_UnCond>(if_end));


    //else
    utils.push_instr(std::make_unique<Linear::Label>(if_else));
    if (node.block_else) {
        node.block_else -> accept(*this);
    }

    utils.push_instr(std::make_unique<Linear::Label>(if_end));
    
}
void MethodBuilder::visit(AST::For_Stmt& node) {}
void MethodBuilder::visit(AST::While_Stmt& node) {}
void MethodBuilder::visit(AST::Return_Stmt& node) {}
void MethodBuilder::visit(AST::Break_Stmt& node) {}
void MethodBuilder::visit(AST::Continue_Stmt& node) {}
void MethodBuilder::visit(AST::For_Upd_Assign_Op& node) {}
void MethodBuilder::visit(AST::For_Upd_Incr& node) {}
void MethodBuilder::visit(AST::Loc_Var& node) {
    auto var = std::make_unique<Linear::Var>();

    var->type = T(node.type_t->type);
    var->id = node.id->id;

    utils.ret = std::move(var);
}
void MethodBuilder::visit(AST::Loc_Array& node) {}
void MethodBuilder::visit(AST::Minus_Expr& node) {}
void MethodBuilder::visit(AST::Not_Expr& node) {}
void MethodBuilder::visit(AST::INT_Expr& node) {}
void MethodBuilder::visit(AST::LONG_Expr& node) {}
void MethodBuilder::visit(AST::Mul_Op_Expr& node) {}
void MethodBuilder::visit(AST::Add_Op_Expr& node) {}
void MethodBuilder::visit(AST::Rel_Op_Expr& node) {}
void MethodBuilder::visit(AST::Eq_Op_Expr& node) {}
void MethodBuilder::visit(AST::Logic_Op_Expr& node) {}
void MethodBuilder::visit(AST::Loc_Expr& node) {
    node.location->accept(*this);
}
void MethodBuilder::visit(AST::Method_Call_Expr& node) {}
void MethodBuilder::visit(AST::Literal_Expr& node) {}
void MethodBuilder::visit(AST::Len_Expr& node) {}
void MethodBuilder::visit(AST::Paren_Expr& node) {}
void MethodBuilder::visit(AST::Expr_Arg& node) {}
void MethodBuilder::visit(AST::String_Arg& node) {}
void MethodBuilder::visit(AST::Assign_Op& node) {}
void MethodBuilder::visit(AST::Increment& node) {}
void MethodBuilder::visit(AST::Mul_Op& node) {}
void MethodBuilder::visit(AST::Add_Op& node) {}
void MethodBuilder::visit(AST::Rel_Op& node) {}
void MethodBuilder::visit(AST::Eq_Op& node) {}
void MethodBuilder::visit(AST::Logic_Op& node) {}
void MethodBuilder::visit(AST::Literal& node) {}
void MethodBuilder::visit(AST::Int_Lit& node) {}
void MethodBuilder::visit(AST::Long_Lit& node) {}
void MethodBuilder::visit(AST::Char_Lit& node) {}
void MethodBuilder::visit(AST::Bool_Lit& node) {}
void MethodBuilder::visit(AST::Type& node) {}
void MethodBuilder::visit(AST::Id& node) {}


Linear::Type T(AST::Type::Type_t AST_t) {
    switch (AST_t)
    {
    case AST::Type::Int:
        return Linear::Int;
        break;

    case AST::Type::Long:
        return Linear::Long;
        break;

    case AST::Type::Bool:
        return Linear::Int;
        break;
    
    case AST::Type::Void:
        return Linear::Void;
        break;
    
    default:
        std::cout << "ERROR T: Null_Type" << std::endl;
        exit(1);
        break;
    }
}