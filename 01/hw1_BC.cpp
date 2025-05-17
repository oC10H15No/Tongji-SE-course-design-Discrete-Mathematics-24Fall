#include <iostream>
#include <string>
#include <map>
#include <stack>
#include <cmath>
#include <limits> // for std::numeric_limits
using namespace std;

typedef map<char, int> Map_ci; // 定义字符到整数的映射，用于表示运算符及其优先级
typedef map<int, char> Map_ic; // 定义整数到字符的映射，用于命题变量及其索引
typedef map<int, int> Map_ii;  // 定义整数到整数的映射，用于存储命题变量的二进制值

Map_ci priority; // 全局变量，存储运算符的优先级

Map_ic getProposition(string formula);
int findProposition(Map_ic, char p);
int pow2(int n);
Map_ii toBinary(int n_proposition, int index);
int calculate(string formula, Map_ic pSet, Map_ii value);
void check(stack<int> &value, stack<char> &opter);
bool isOperator(const char ch);
void printMenu();

// 输出菜单
void printMenu()
{
    cout << "***************************************\n";
    cout << "**                                   **\n";
    cout << "**         欢迎进入逻辑运算软件      **\n";
    cout << "**   (可运算真值表,主范式,支持括号)  **\n";
    cout << "**                                   **\n";
    cout << "**              用!表示非            **\n";
    cout << "**              用&表示与            **\n";
    cout << "**              用|表示或            **\n";
    cout << "**             用^表示蕴含           **\n";
    cout << "**             用~表示等值           **\n";
    cout << "**                                   **\n";
    cout << "***************************************\n\n";
}

// 判断字符是否为运算符
bool isOperator(const char ch)
{
    return ch == '!' || ch == '&' || ch == '|' || ch == '^' || ch == '~' || ch == '(' || ch == ')';
}

bool isValidFormula(const string &str)
{
    //游标对象
    string::const_iterator it = str.begin();
    //记录括号情况的栈
    stack<char> brackets;  
    //操作符（单目、双目）个数，数字个数
    int op1Num = 0, op2Num = 0, numNum = 0;  

    while (it != str.end()) {
        //若扫描到字符表示命题，则进行计数
        if (((*it) >= 'a' && (*it) <= 'z') || ((*it) >= 'A' && (*it) <= 'Z'))
            //一个数字扫描完成，进行计数
            numNum++;  
        //若遇到左括号，压入栈中
        if (*it == '(')  
            brackets.push(*it);
        else if (*it == ')') 
        {
            /*若遇到右括号，栈不为空则弹出一个元素；
              栈为空则说明右括号多余，非法*/
            if (!brackets.empty())
                brackets.pop();

            else {
                cout << "括号不匹配" << endl;
                return false;
            }
        }
        else if (isOperator(*it)) 
        {
            //若遇到操作符，进行计数
            if (*it == '!')
                op1Num++;
            else
                op2Num++;
        }

        if (it != str.end())
            it++;
        else
            break;
    }

    if (!brackets.empty()) 
    {
        //若扫描完成栈仍不为空，则左括号多余，非法
        cout << "括号不匹配" << endl;
        return false;
    }

    if (numNum != op2Num + 1) 
    { 
        //若双目运算符个数不等于数字个数+1，则不匹配，非法
        cout << "操作符与操作数数量不匹配" << endl;
        return false;
    }
    //所有标准均符合，则表达式合法
    return true;  
}

bool askToContinue()
{
    char ch;
    while (true)
    {
        cout << "是否继续运算(Y/N)？" << endl;
        cin >> ch;
        if (ch == 'Y' || ch == 'y')
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            #if defined(_WIN32) || defined(_WIN64) 
            system("cls");
            #else 
            system("clear");
            #endif
            return true;
        }
        else if (ch == 'N' || ch == 'n') {
            return false;
        }
        else
        {
            cout << "输入错误，请重新输入" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

int main()
{
    priority['('] = 6;
    priority[')'] = 6;
    priority['!'] = 5;
    priority['&'] = 4;
    priority['|'] = 3;
    priority['^'] = 2;
    priority['~'] = 1;
    priority['#'] = 0; // 结束符
    // 运算符优先级

    while (true)
    {
        printMenu();

        string formula;
        while (true)
        {
            cout << "请输入合法的命题公式：" << endl;
            cin >> formula;
            if (isValidFormula(formula))
                break;
            else
            {
                cout << "输入的公式不合法，请重新输入！" << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        Map_ic proposition_set = getProposition(formula); // 获取公式中的命题变项（即命题字母），并返回它们的集合
        cout << "该式子中的变量个数为：" << proposition_set.size() << endl;

        cout << "输出真值表如下：" << endl;
        for (unsigned int i = 0; i < proposition_set.size(); i++)
        {
            cout << proposition_set[i] << "\t";
        }
        cout << formula << endl;

        int *m = new int[pow2(proposition_set.size())];   // 动态分配内存用于保存每行真值表计算的结果，依次存放命题公式的各行(1或0)的运算结果的值

        for (int i = 0; i < pow2(proposition_set.size()); i++)
        {
            // 将当前行数转换为命题变量的二进制组合
            Map_ii bina_set = toBinary(proposition_set.size(), i);
            // 输出当前行的二进制组合（即命题变项的真假值）
            for (unsigned int j = 0; j < bina_set.size(); j++)
            {
                cout << bina_set[j] << "\t";
            }
            // 计算当前行下，公式在对应命题变量取值下的结果
            int result = calculate(formula, proposition_set, bina_set);
            *(m + i) = result;
            cout << result << endl;
        }

        // 输出该逻辑公式的主析取范式（Disjunctive Normal Form）
        int n_m = 0;
        cout << "该命题公式的主析取范式：" << endl;
        for (int i = 0; i < pow2(proposition_set.size()); i++)
        {
            if (*(m + i) == 1)
            {
                if (n_m == 0)
                {
                    cout << "m<" << i << ">";
                }
                else
                {
                    cout << " \\/ m<" << i << "> ";
                }
                n_m++;
            }
        }
        if (n_m == 0)
        { // 如果没有一个组合使公式为真，输出 0
            cout << "0";
        }
        cout << endl;

        // 输出该逻辑公式的主合取范式（Conjunctive Normal Form）
        int n_M = 0;
        cout << "该命题公式的主合取范式：" << endl;
        for (int i = 0; i < pow2(proposition_set.size()); i++)
        {
            if (*(m + i) == 0)
            {
                if (n_M == 0)
                {
                    cout << "M<" << i << ">";
                }
                else
                {
                    cout << " /\\ M<" << i << "> ";
                }
                n_M++;
            }
        }
        if (n_M == 0)
        {
            cout << "0";
        }
        cout << endl
             << endl;

        delete[] m;

        if (!askToContinue())
            break;
    }
    return 0;
}

int findProposition(Map_ic pSet, char p) // 返回-1，表示该命题变项尚未被遍历过，可计数；否则说明该命题变项已被遍历过，则不重复计数。另外，还可以返回指定命题变项的下标
{
    // 遍历命题集合，查找命题变量 p 的下标
    Map_ic::iterator it = pSet.begin();
    while (it != pSet.end())
    {
        if (it->second == p)
        {
            return it->first; // 返回该命题变量的下标
        }
        it++;
    }
    return -1; // 未找到，返回 -1，表示该命题变量未被记录
}

Map_ic getProposition(string formula) // 该函数返回所输入公式中的命题变项(不包括运算符)
{
    Map_ic proposition;	   // 用于存储命题变项
    int n_proposition = 0; // 记录当前命题变量的数量

    for (unsigned int i = 0; i < formula.length(); i++)
    {
        char c = formula[i];
        if (isalpha(c))
        {
            // 遍历所有命题变项
            int r = findProposition(proposition, c);
            if (r == -1)
            {
                // 说明该命题变项尚未被遍历过
                proposition[n_proposition] = c; // 将命题变量添加到集合中
                n_proposition++;				// 命题变量计数加 1
            }
        }
        else if (!priority.count(c))
        { // 如果当前字符不是命题变量且不在优先级表中
            cerr << c << " is undefined!" << endl;
            exit(2);
        }
    }
    return proposition;
}

Map_ii toBinary(int n_proposition, int index) // 该函数返回命题变项的二进制(1或0)取值
{
    Map_ii result;
    // 将整数 index 转换为二进制表示，并存储到 result 中
    for (int i = 0; i < n_proposition; ++i)
    {
        int r = index % 2;
        result[n_proposition - 1 - i] = r;
        index = index / 2;
    }
    return result;
}

int pow2(int n) // 该函数返回指定数字的二次方的值
{
    if (n == 0)
        return 1;
    else
        return 2 * pow2(n - 1);
}

int calculate(string formula, Map_ic pSet, Map_ii value) // 该函数返回给定命题变项(值取1或0，可含括号)组合的运算结果
{
    stack<char> opter; // 运算符栈
    stack<int> pvalue; // 操作数栈

    opter.push('#');		 // 将 # 作为结束符入栈
    formula = formula + "#"; // 在公式末尾添加结束符

    for (unsigned int i = 0; i < formula.length(); i++)
    {
        char c = formula[i];
        if (isalpha(c))
        {										// 如果是命题变项
            int pos = findProposition(pSet, c); // 查找命题变项的下标
            pvalue.push(value[pos]);			// 将对应的真值（0 或 1）入栈
        }
        else
        {
            // 此时遍历的是运算符
            char tmp = opter.top(); // 获取栈顶运算符
            // 如果当前栈顶运算符优先级高于当前运算符
            if (priority[tmp] > priority[c])
            {
                while (priority[tmp] > priority[c] && tmp != '(')
                {
                    check(pvalue, opter); // 执行运算并更新栈
                    tmp = opter.top();	  // 更新栈顶运算符
                    if (tmp == '#' && c == '#')
                    {						 // 如果是结束符，返回最终结果
                        return pvalue.top(); // 返回运算结果
                    }
                }
                opter.push(c); // 否则，将当前运算符入栈
            }
            else
                opter.push(c);
        }
    }
    return -1;
}

void check(stack<int> &value, stack<char> &opter) // 该函数返回两个命题变项(取值1或0)的各种运算结果(0或1)
{
    // 该函数用于执行栈顶两个命题变量的运算，并将结果存入栈中。
    // value 栈存储命题变量的真假值，opter 栈存储运算符。

    int p, q, result;		// 定义两个命题变量的值 p 和 q，以及存储结果的 result
    char opt = opter.top(); // 获取栈顶的运算符

    switch (opt)
    {
    case '&':				// 与运算符处理
        p = value.top();	// 取出栈顶命题变量 p 的值
        value.pop();		// 将 p 从栈中弹出
        q = value.top();	// 取出下一个栈顶命题变量 q 的值
        value.pop();		// 将 q 从栈中弹出
        result = p && q;	// 计算 p 与 q 的逻辑与运算
        value.push(result); // 将计算结果压入栈中
        opter.pop();		// 将与运算符从运算符栈中弹出
        break;

    case '|':				// 或运算符处理
        p = value.top();	// 取出栈顶命题变量 p 的值
        value.pop();		// 将 p 从栈中弹出
        q = value.top();	// 取出下一个栈顶命题变量 q 的值
        value.pop();		// 将 q 从栈中弹出
        result = p || q;	// 计算 p 与 q 的逻辑或运算
        value.push(result); // 将计算结果压入栈中
        opter.pop();		// 将或运算符从运算符栈中弹出
        break;

    case '!':				// 非运算符处理
        p = value.top();	// 取出栈顶命题变量 p 的值
        value.pop();		// 将 p 从栈中弹出
        result = !p;		// 计算 p 的逻辑非运算
        value.push(result); // 将计算结果压入栈中
        opter.pop();		// 将非运算符从运算符栈中弹出
        break;

    case '^':				// 蕴含运算符处理 (p -> q)
        q = value.top();	// 取出栈顶命题变量 q 的值
        value.pop();		// 将 q 从栈中弹出
        p = value.top();	// 取出下一个栈顶命题变量 p 的值
        value.pop();		// 将 p 从栈中弹出
        result = !p || q;	// 计算 p 蕴含 q 的逻辑运算 (!p 或 q)
        value.push(result); // 将计算结果压入栈中
        opter.pop();		// 将蕴含运算符从运算符栈中弹出
        break;

    case '~':			 // 等值运算符处理 (p <-> q)
        p = value.top(); // 取出栈顶命题变量 p 的值
        value.pop();	 // 将 p 从栈中弹出
        q = value.top(); // 取出下一个栈顶命题变量 q 的值
        value.pop();	 // 将 q 从栈中弹出
        // 计算 p 和 q 的逻辑等值运算：(!p 或 q) 且 (p 或 !q)
        result = (!p || q) && (p || !q);
        value.push(result); // 将计算结果压入栈中
        opter.pop();		// 将等值运算符从运算符栈中弹出
        break;

    case '#': // 结束符处理
        // 结束符不做处理，直接跳过
        break;

    case '(': // 左括号处理
        // 左括号不做处理，直接跳过
        break;

    case ')':		 // 右括号处理
        opter.pop(); // 弹出右括号
        // 一直执行运算直到遇到左括号
        while (opter.top() != '(')
        {
            check(value, opter); // 递归调用 check 函数，计算括号内的表达式
        }
        if (opter.top() == '(')
        {				 // 如果遇到左括号
            opter.pop(); // 弹出左括号
        }
        break;

    default: // 默认处理，处理其他情况
        break;
    }
}