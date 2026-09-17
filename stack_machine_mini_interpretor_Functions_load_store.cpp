#include<bits/stdc++.h>
using namespace std;
using ll = long long;

struct Value{
    bool isNumber;
    ll number;
    string s;
};

Value make_value_from_string(string demo)
{
    Value v;
    v.isNumber = false;
    v.number = -1;
    v.s = demo;
    return v;
}

Value make_value_from_number(ll demo)
{
    Value v;
    v.isNumber = true;
    v.number = demo;
    v.s = "";
    return v;
}

Value add(const Value &v1, const Value &v2)
{
    if(v1.isNumber && v2.isNumber) return make_value_from_number(v1.number + v2.number);
    if(!v1.isNumber && !v2.isNumber) return make_value_from_string(v1.s + v2.s);

    throw runtime_error("Cant add a string and a number together \n");
}

bool isNumeric(string demo)
{
    // assume it is numeric only
    bool check_numeric = true;
    int start = demo[0] =='-' ? 1:0;
    for(int i=start; i<demo.size(); i++)
    {
        if(!isdigit((unsigned char) demo[i]))
            check_numeric = false;
    }
    return check_numeric;
}

int main()
{
    vector<Value> stack; 
    // to mantain LIFO
    vector<int> callStack;
    // stores only return addresses, separate from data stack
    map<string, Value> variables;
    vector<string> instructions = {
        "PUSH 5","CALL addTen","PRINT","JMP end",
        "LABEL addTen","PUSH 10","ADD","RET",
        "LABEL end"};                                   // -> 15

    // O(n) pre pass to find labels
    map<string, int> label_name_to_pc;
    for(int i=0; i < instructions.size(); i++)
    {
        string instruction = instructions[i];
        string op = instruction.substr(0, instruction.find(' '));
        if(op == "LABEL")
        {
            string name = instruction.substr(instruction.find(' ') + 1);
            label_name_to_pc[name] = i;
        }
    }

    int steps = 0;
    constexpr int MAX_STEPS = 1e6;
    int program_counter = 0;
    while(program_counter < instructions.size())
    {
        steps++;
        if(steps >= MAX_STEPS)
            throw runtime_error("crossed MAX_STEPS \n");
        string instruction = instructions[program_counter];
        string op = instruction.substr(0, instruction.find(' '));

        if(op == "PUSH")
        {
            string operand = instruction.substr(instruction.find(' ') + 1);
            Value to_be_pushed;
            if(isNumeric(operand))
            {
                to_be_pushed = make_value_from_number(stoll(operand));
            }
            else{
                to_be_pushed = make_value_from_string(operand);
            }
            stack.push_back(to_be_pushed);
        }
        else if(op == "POP"){
            if(stack.size() == 0)
            {
                throw runtime_error("stack is empty, cant pop \n");
            }
            stack.pop_back();
        }
        else if(op == "ADD"){
            if(stack.size() < 2)
            {
                throw runtime_error("stack needs 2 members to add \n");
            }
            Value b = stack.back(); stack.pop_back();
            Value a = stack.back(); stack.pop_back();
            stack.push_back(add(a, b));
        }
        else if(op == "PRINT"){
            if(stack.size() == 0)
            {
                throw runtime_error("stack is empty, cant pop \n");
            }
            Value a = stack.back();
            if(a.isNumber) cout << a.number << endl;
            else cout << a.s << endl;
        }
        else if(op == "LABEL"){
            // do nothing, already pre-processed
        }
        else if(op == "JMP"){
            string label = instruction.substr(instruction.find(' ') + 1);
            
            if(label_name_to_pc.find(label) == label_name_to_pc.end())
                throw runtime_error("Dealing with an unkown label \n");

            int jmp_pc = label_name_to_pc[label];
            program_counter = jmp_pc;
            continue;
        }
        else if(op == "JZ"){
            string label = instruction.substr(instruction.find(' ') + 1);

            if(label_name_to_pc.find(label) == label_name_to_pc.end())
                throw runtime_error("Dealing with an unkown label \n");
            
            if(stack.size() == 0)
            {
                throw runtime_error("stack is empty, cant JZ \n");
            }

            Value top = stack.back();
            if(top.isNumber && top.number == 0)
            {
                program_counter = label_name_to_pc[label];
                continue;
                // jump taken
            }
            // jump not taken
        }
        else if(op == "STORE"){
            if(stack.empty())
                throw runtime_error("store a empty stack");
            string name = instruction.substr(instruction.find(" ")+1);
            variables[name] = stack.back();
            stack.pop_back(); 
        }
        else if(op == "LOAD"){
            string name = instruction.substr(instruction.find(" ")+1);
            if(variables.count(name) == 0)
                throw runtime_error("no info stored on this variable");
            stack.push_back(variables[name]);           
        }
        else if(op == "CALL"){
            string label = instruction.substr(instruction.find(" ")+1);
            if(label_name_to_pc.count(label) == 0)
                throw runtime_error("no info stored on this label");
            callStack.push_back(program_counter + 1); // to remember where to come back to 
            program_counter = label_name_to_pc[label];
            continue;
        }
        else if(op == "RET"){
            if(callStack.empty())
                throw runtime_error("Return to where? callStack is empty \n");
            program_counter = callStack.back();
            callStack.pop_back();
            continue;
        }
        else{
            throw runtime_error("Unknown instruction \n");
        }
        program_counter++;
    }
    return 0;
}