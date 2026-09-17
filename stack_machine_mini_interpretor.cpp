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
    for(int i=0; i<demo.size(); i++)
    {
        if(!isdigit((unsigned char) demo[i]))
            check_numeric = false;
    }
    return check_numeric;
}

int main()
{
    vector<Value> stack; // to mantain LIFO

    vector<string> instructions = {
        "PUSH 5",
        "PUSH 3",
        "ADD",
        "PRINT",
        "POP",
        "PUSH ARC",
        "PUSH HIT",
        "ADD",
        "PRINT",
        "PUSH 7a",
        "ADD",
        "PRINT"
    };
    for(auto instruction: instructions)
    {
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
    }
    return 0;
}