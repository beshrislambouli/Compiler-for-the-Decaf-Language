
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

    for (auto& field_decl : program -> field_decls) {
        for (auto& field: field_decl->fields ) {
            auto instr_declare = std::make_unique<Linear::Declare>();

            if (is_instance_of(field,AST::Id_Field_Decl)) {
                
                auto var = std::make_unique<Linear::Var>();
                var->type = T(field_decl->field_type->type->type);
                var-> id  = field->id->id;

                instr_declare->location = std::move(var);
            }
            else if (is_instance_of(field,AST::Array_Field_Decl)) {

                auto arr = std::make_unique<Linear::Arr>();
                arr->type = T(field_decl->field_type->type->type);
                arr->id   = field->id->id;
                

                auto index = std::make_unique<Linear::Literal>();
                index->type = Linear::Int;
                auto array_field_decl = dynamic_cast<AST::Array_Field_Decl*>(field.get());
                std::string size = array_field_decl -> size -> literal;
                index->id= size;

                arr -> index = std::move(index);
                instr_declare -> location = std::move(arr);
            }
            linear_program->globals.push_back(std::move(instr_declare));
        }
    }

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

    auto operand1 = dist->get_copy();

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

    std::string tmp = utils.get_tmp(dist->type);
    utils.push_instr (
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(dist->type,tmp),
            std::move(operand1),
            std::move(operand2),
            op
        )
    );
    utils.assign(std::move(dist), std::make_unique<Linear::Var>(dist->type,tmp));
}

void MethodBuilder::visit(AST::Location_Incr& node) {
    node.location -> accept(*this);
    auto dist = cast<Linear::Location>(std::move(utils.ret));


    auto operand1 = dist ->get_copy();

    
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

    std::string tmp = utils.get_tmp(dist->type);
    utils.push_instr (
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(dist->type,tmp),
            std::move(operand1),
            std::move(operand2),
            op
        )
    );
    utils.assign(std::move(dist), std::make_unique<Linear::Var>(dist->type,tmp));
}

void MethodBuilder::visit(AST::Method_Call_Stmt& node) {
    auto instr = std::make_unique<Linear::Method_Call>();

    instr->id = node.id->id;

    int arg_num = -1;
    for (auto& arg : node.extern_args) {
        arg_num ++ ;
        arg -> accept(*this);
        // if first 6 args -> 
        // ARG = arg, f (ARG), this will help precoloring the webs for the args
        // literals won't be in a web, but although arrays won't, their index might
        if ( arg_num >= 6 || is_instance_of (utils.ret, Linear::Literal)) {
            instr->args.push_back(std::move(utils.ret));
        } else {
            
            if (is_instance_of (utils.ret, Linear::Var)) {
                auto var_ptr = dynamic_cast<Linear::Var*> (utils.ret.get());
                // array ptr won't be in a web
                if (var_ptr->is_array_var) {
                    instr->args.push_back(std::move(utils.ret));
                    continue;
                }
            }
            

            // only for var args
            auto type = utils.ret->type;

            // ARG
            std::string tmp = utils.get_tmp_arg(type,arg_num);
            auto dist = std::make_unique<Linear::Var>(type,tmp);
            
            //arg
            auto operand = std::move (utils.ret);

            // ARG = arg
            auto instr_assign = std::make_unique<Linear::Assign>();
            instr_assign ->dist = std::move (dist);
            instr_assign ->operands .push_back (std::move(operand));
            utils.push_instr (std::move(instr_assign));

            // f (ARG)
            instr->args .push_back (std::make_unique<Linear::Var>(type,tmp));

        }
        
    }

    utils.push_instr(std::move(instr));
}

void MethodBuilder::visit(AST::If_Else_Stmt& node) {
    std::string if_then = "if_then" + utils.get_label();
    std::string if_else = "if_else" + utils.get_label();
    std::string if_end  = "end_if"  + utils.get_label();


    // jump condition
    node.expr_if->accept(*this);
    auto condition = std::move(utils.ret);
    utils.push_instr(std::make_unique<Linear::J_Cond>(if_else, "0", std::move(condition)));
    // utils.push_instr(std::make_unique<Linear::J_UnCond>(if_else));

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
void MethodBuilder::visit(AST::For_Stmt& node) {
    std::string for_condition = "for_condition" + utils.get_label();
    std::string for_body = "for_body" + utils.get_label();
    std::string for_update = "for_update" + utils.get_label();
    std::string for_end = "for_end" + utils.get_label();


    // init
    auto var_init = std::make_unique<Linear::Var>(T(node.id->type_t->type), node.id->id);
    node.expr_init -> accept(*this);
    auto operand_init = std::move(utils.ret);
    utils.assign(std::move(var_init), std::move(operand_init));

    // condition
    utils.label(for_condition);
    node.expr_cond->accept(*this);
    auto condition = std::move(utils.ret);
    utils.push_instr(std::make_unique<Linear::J_Cond>(for_end, "0", std::move(condition)));
    // utils.push_instr(std::make_unique<Linear::J_UnCond>(for_end));

    // for body
    utils.label(for_body);
    utils.push_loop_labels(for_update,for_end);
    node.block->accept(*this);
    utils.pop_loop_labels();

    // for update
    utils.label(for_update);
    node.for_update->accept(*this);
    utils.push_instr(std::make_unique<Linear::J_UnCond>(for_condition));

    // for end
    utils.label(for_end);
}

void MethodBuilder::visit(AST::While_Stmt& node) {
    std::string while_condition = "while_condition" + utils.get_label();
    std::string while_body = "while_body" + utils.get_label();
    std::string while_end = "while_end" + utils.get_label();

    //while condition
    utils.push_instr(std::make_unique<Linear::Label>(while_condition));
    node.expr_cond->accept(*this);
    auto condition = std::move(utils.ret);
    utils.push_instr(std::make_unique<Linear::J_Cond>(while_end, "0", std::move(condition)));
    // utils.push_instr(std::make_unique<Linear::J_UnCond>(while_end));

    // while body
    utils.push_instr(std::make_unique<Linear::Label>(while_body));
    utils.push_loop_labels(while_condition,while_end);
    node.block->accept(*this);
    utils.pop_loop_labels();
    utils.push_instr(std::make_unique<Linear::J_UnCond>(while_condition));


    utils.push_instr(std::make_unique<Linear::Label>(while_end));
}

void MethodBuilder::visit(AST::Return_Stmt& node) {
    auto return_instr = std::make_unique<Linear::Return>();

    if (node.expr) {
        node.expr->accept(*this);
        return_instr -> return_value = std::move(utils.ret);
    }

    utils.push_instr(std::move(return_instr));
}

void MethodBuilder::visit(AST::Break_Stmt& node) {
    utils.push_instr(std::make_unique<Linear::J_UnCond>(utils.break_label()));
}

void MethodBuilder::visit(AST::Continue_Stmt& node) {
    utils.push_instr(std::make_unique<Linear::J_UnCond>(utils.continue_label()));
}

void MethodBuilder::visit(AST::For_Upd_Assign_Op& node) {
    if (node.assign_Op->type == AST::Assign_Op::ASSIGN ) {

        node.location -> accept(*this);
        auto dist = cast<Linear::Location>(std::move(utils.ret));

        node.expr -> accept (*this);
        auto operand = cast<Linear::Operand>(std::move(utils.ret));

        utils.assign(std::move(dist), std::move(operand));
        return;
    }
    
    node.location -> accept(*this);
    auto dist = cast<Linear::Location>(std::move(utils.ret));

    auto operand1 = dist->get_copy();

    node.expr -> accept (*this);
    auto operand2 = cast<Linear::Operand>(std::move(utils.ret));

    Linear::Binary::Op op;
    switch (node.assign_Op->type)
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

    std::string tmp = utils.get_tmp(dist->type);
    utils.push_instr (
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(dist->type,tmp),
            std::move(operand1),
            std::move(operand2),
            op
        )
    );
    utils.assign(std::move(dist), std::make_unique<Linear::Var>(dist->type,tmp));
}
void MethodBuilder::visit(AST::For_Upd_Incr& node) {
    node.location -> accept(*this);
    auto dist = cast<Linear::Location>(std::move(utils.ret));


    auto operand1 = dist -> get_copy();

    
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

    std::string tmp = utils.get_tmp(dist->type);
    utils.push_instr (
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(dist->type,tmp),
            std::move(operand1),
            std::move(operand2),
            op
        )
    );
    utils.assign(std::move(dist), std::make_unique<Linear::Var>(dist->type,tmp));
}

void MethodBuilder::visit(AST::Loc_Var& node) {
    if (node.is_array_var) {
        utils.ret = std::make_unique<Linear::Var>(Linear::Type::Long, node.id->id, node.is_array_var);
    } else {
        utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type), node.id->id);
    }
    
}
void MethodBuilder::visit(AST::Loc_Array& node) {
    node.expr->accept(*this);
    auto index = std::move(utils.ret);

    utils.ret = std::make_unique<Linear::Arr>(T(node.type_t->type), node.id->id,std::move(index));
}

void MethodBuilder::visit(AST::Minus_Expr& node) {
    std::string tmp = utils.get_tmp(T(node.type_t->type));

    node.expr->accept(*this);
    utils.push_instr(
        std::make_unique<Linear::Unary>(
            std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
            std::move(utils.ret),
            Linear::Unary::Minus
        )
    );

    utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
}

void MethodBuilder::visit(AST::Not_Expr& node) {
    std::string tmp = utils.get_tmp(T(node.type_t->type));

    node.expr->accept(*this);
    utils.push_instr(
        std::make_unique<Linear::Unary>(
            std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
            std::move(utils.ret),
            Linear::Unary::Not
        )
    );

    utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
}
void MethodBuilder::visit(AST::INT_Expr& node) {
    node.expr->accept(*this);
}
void MethodBuilder::visit(AST::LONG_Expr& node) {
    if (node.expr->type_t->type != AST::Type::Long ) {

        std::string tmp = utils.get_tmp(T(node.type_t->type));

        node.expr->accept(*this);
        utils.push_instr(
            std::make_unique<Linear::Unary>(
                std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
                std::move(utils.ret),
                Linear::Unary::LONG_CAST
            )
        );

        utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);

    } else {

        node.expr->accept(*this);
        
    }  
}

void MethodBuilder::visit(AST::Mul_Op_Expr& node) {
    node.expr_lhs->accept(*this);
    auto operand1 = std::move(utils.ret);

    node.expr_rhs->accept(*this);
    auto operand2 = std::move(utils.ret);

    std::string tmp = utils.get_tmp(T(node.type_t->type));
    utils.push_instr(
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
            std::move(operand1),
            std::move(operand2),
            B(node.bin_op->type)
        )
    );
    utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
}
void MethodBuilder::visit(AST::Add_Op_Expr& node) {
    node.expr_lhs->accept(*this);
    auto operand1 = std::move(utils.ret);

    node.expr_rhs->accept(*this);
    auto operand2 = std::move(utils.ret);

    std::string tmp = utils.get_tmp(T(node.type_t->type));
    utils.push_instr(
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
            std::move(operand1),
            std::move(operand2),
            B(node.bin_op->type)
        )
    );
    utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
}
void MethodBuilder::visit(AST::Rel_Op_Expr& node) {
    node.expr_lhs->accept(*this);
    auto operand1 = std::move(utils.ret);

    node.expr_rhs->accept(*this);
    auto operand2 = std::move(utils.ret);

    std::string tmp = utils.get_tmp(T(node.type_t->type));
    utils.push_instr(
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
            std::move(operand1),
            std::move(operand2),
            B(node.bin_op->type)
        )
    );
    utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
}
void MethodBuilder::visit(AST::Eq_Op_Expr& node) {
    node.expr_lhs->accept(*this);
    auto operand1 = std::move(utils.ret);

    node.expr_rhs->accept(*this);
    auto operand2 = std::move(utils.ret);

    std::string tmp = utils.get_tmp(T(node.type_t->type));
    utils.push_instr(
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
            std::move(operand1),
            std::move(operand2),
            B(node.bin_op->type)
        )
    );
    utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
}
void MethodBuilder::visit(AST::Logic_Op_Expr& node) {
    std::string short_circuit = "short_circuit" + utils.get_label();
    std::string done_Logic_Op_Expr = "done_Logic_Op_Expr" + utils.get_label();
    std::string tmp = utils.get_tmp(T(node.type_t->type));
    
    node.expr_lhs->accept(*this);
    auto operand1 = std::move(utils.ret);

    auto operand1_copy = operand1->get_copy();
    std::string jump_on = ( node.bin_op->type == AST::Bin_Op::AND ) ? "0" : "1" ;
    utils.push_instr(std::make_unique<Linear::J_Cond>(short_circuit, jump_on, std::move(operand1_copy)));
    

    node.expr_rhs->accept(*this);
    auto operand2 = std::move(utils.ret);

    
    utils.push_instr(
        std::make_unique<Linear::Binary>(
            std::make_unique<Linear::Var>(T(node.type_t->type),tmp),
            std::move(operand1),
            std::move(operand2),
            B(node.bin_op->type)
        )
    );
    utils.push_instr(std::make_unique<Linear::J_UnCond>(done_Logic_Op_Expr));



    utils.push_instr(std::make_unique<Linear::Label>(short_circuit));

    if ( node.bin_op->type == AST::Bin_Op::AND ) {

        auto instr_assign = std::make_unique<Linear::Assign>();

        instr_assign -> dist = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
        instr_assign -> operands.push_back (
            std::make_unique<Linear::Literal>(T(node.type_t->type),"0")
        );

        utils.push_instr(std::move(instr_assign));

    } else if ( node.bin_op->type == AST::Bin_Op::OR ) {
        
        auto instr_assign = std::make_unique<Linear::Assign>();

        instr_assign -> dist = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
        instr_assign -> operands.push_back (
            std::make_unique<Linear::Literal>(T(node.type_t->type),"1")
        );

        utils.push_instr(std::move(instr_assign));

    }

    
    utils.push_instr(std::make_unique<Linear::Label>(done_Logic_Op_Expr));
    utils.ret = std::make_unique<Linear::Var>(T(node.type_t->type),tmp);
}

void MethodBuilder::visit(AST::Loc_Expr& node) {
    node.location->accept(*this);
}
void MethodBuilder::visit(AST::Method_Call_Expr& node) {
    auto instr = std::make_unique<Linear::Method_Call>();

    instr->id = node.id->id;

    int arg_num = -1;
    for (auto& arg : node.extern_args) {
        arg_num ++ ;
        arg -> accept(*this);
        // if first 6 args -> 
        // ARG = arg, f (ARG), this will help precoloring the webs for the args
        // literals won't be in a web, but although arrays won't, their index might
        if ( arg_num >= 6 || is_instance_of (utils.ret, Linear::Literal)) {
            instr->args.push_back(std::move(utils.ret));
        } else {
            
            if (is_instance_of (utils.ret, Linear::Var)) {
                auto var_ptr = dynamic_cast<Linear::Var*> (utils.ret.get());
                // array ptr won't be in a web
                if (var_ptr->is_array_var) {
                    instr->args.push_back(std::move(utils.ret));
                    continue;
                }
            }
            

            // only for var args
            auto type = utils.ret->type;

            // ARG
            std::string tmp = utils.get_tmp_arg(type,arg_num);
            auto dist = std::make_unique<Linear::Var>(type,tmp);
            
            //arg
            auto operand = std::move (utils.ret);

            // ARG = arg
            auto instr_assign = std::make_unique<Linear::Assign>();
            instr_assign ->dist = std::move (dist);
            instr_assign ->operands .push_back (std::move(operand));
            utils.push_instr (std::move(instr_assign));

            // f (ARG)
            instr->args .push_back (std::make_unique<Linear::Var>(type,tmp));

        }
        
    }

    std::string tmp = utils.get_tmp(T(node.id->type_t->type)); 
    auto return_location = std::make_unique<Linear::Var>(T(node.id->type_t->type),tmp);
    instr->return_location = std::move(return_location);

    utils.push_instr(std::move(instr));
    utils.ret = std::make_unique<Linear::Var>(T(node.id->type_t->type),tmp);
}

void MethodBuilder::visit(AST::Literal_Expr& node) {
    node.literal->accept(*this);
}

void MethodBuilder::visit(AST::Len_Expr& node) {
    utils.ret = std::make_unique<Linear::Literal>(
        Linear::Int,
        node.size
    );
}
void MethodBuilder::visit(AST::Paren_Expr& node) {
    node.expr->accept(*this);
}

void MethodBuilder::visit(AST::Expr_Arg& node) {
    node.expr->accept(*this);
}
void MethodBuilder::visit(AST::String_Arg& node) {
    std::string literal = node.string_arg.substr(1,node.string_arg.length()-2);
    auto string_literal = std::make_unique<Linear::Literal>(
        Linear::Long,
        literal
    );
    string_literal->is_string = true;

    utils.ret = std::move(string_literal);
}

void MethodBuilder::visit(AST::Assign_Op& node) {}
void MethodBuilder::visit(AST::Increment& node) {}
void MethodBuilder::visit(AST::Mul_Op& node) {}
void MethodBuilder::visit(AST::Add_Op& node) {}
void MethodBuilder::visit(AST::Rel_Op& node) {}
void MethodBuilder::visit(AST::Eq_Op& node) {}
void MethodBuilder::visit(AST::Logic_Op& node) {}
void MethodBuilder::visit(AST::Literal& node) {}

void MethodBuilder::visit(AST::Int_Lit& node) {
    std::string num = node.literal;
    int base = 10;
    if (num.size() > 2 && (num[0] == '0') && num[1] == 'x') {
        base = 16;
    }
    long long val = std::stoll(num, nullptr, base);
    std::string literal = std::to_string(val);

    utils.ret = std::make_unique<Linear::Literal>(
        Linear::Int,
        literal
    );
}
void MethodBuilder::visit(AST::Long_Lit& node) {
    
    std::string num = node.literal;
    num.pop_back();

    int base = 10;
    if (num.size() > 2 && (num[0] == '0') && num[1] == 'x') {
        base = 16;
    }
    long long val = std::stoll(num, nullptr, base);
    std::string literal = std::to_string(val);

    utils.ret = std::make_unique<Linear::Literal>(
        Linear::Long,
        literal
    );
}
void MethodBuilder::visit(AST::Char_Lit& node) {
    std::string literal = std::to_string((int)node.literal[1]);
    utils.ret = std::make_unique<Linear::Literal>(
        Linear::Int,
        literal
    );
}
void MethodBuilder::visit(AST::Bool_Lit& node) {
    std::string literal = "";
    if (node.literal == "true"){
        literal = "1";
    } else {
        literal = "0";
    }
    utils.ret = std::make_unique<Linear::Literal>(
        Linear::Int,
        literal
    );
}

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

Linear::Binary::Op B(AST::Bin_Op::Type from) {
    switch (from)
    {
    case AST::Bin_Op::Type::STAR:
        return Linear::Binary::Mul;
        break;

    case AST::Bin_Op::Type::DIV:
        return Linear::Binary::Div;
        break;

    case AST::Bin_Op::Type::MOD:
        return Linear::Binary::Mod;
        break;
    
    case AST::Bin_Op::Type::PLUS:
        return Linear::Binary::Plus;
        break;

    case AST::Bin_Op::Type::MINUS:
        return Linear::Binary::Minus;
        break;
    
    case AST::Bin_Op::Type::LT:
        return Linear::Binary::LT;
        break;
    
    case AST::Bin_Op::Type::GT:
        return Linear::Binary::GT;
        break;
    
    case AST::Bin_Op::Type::LE:
        return Linear::Binary::LE;
        break;
    
    case AST::Bin_Op::Type::GE:
        return Linear::Binary::GE;
        break;
    
    case AST::Bin_Op::Type::EQ:
        return Linear::Binary::EQ;
        break;
    
    case AST::Bin_Op::Type::NEQ:
        return Linear::Binary::NEQ;
        break;
    
    case AST::Bin_Op::Type::OR:
        return Linear::Binary::OR;
        break;
    
    case AST::Bin_Op::Type::AND:
        return Linear::Binary::AND;
        break;
    
    default:
        std::cout << "ERROR B: Null_Type" << std::endl;
        exit(1);
        break;
    }
}