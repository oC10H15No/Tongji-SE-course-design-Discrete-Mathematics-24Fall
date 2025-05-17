#include <iostream>
#include <vector> 
#include <climits>

using namespace std;

//显示欢迎消息 
void welcome()
{
    cout << "***************************************" << endl;  //标语
    cout << "**                                   **" << endl;;
    cout << "**        欢迎进入逻辑运算程序       **" << endl;
    cout << "**                                   **" << endl;
    cout << "***************************************\n" << endl;
    
}

//输入处理 
int getInput(char ch)
{
    while (true) {
        int input;
        cout << "请输入" << ch << "的值(1或0)，以回车结束："; 
        cin >> input;
        if ((input != 1 && input != 0) || cin.fail()) {
            cout << endl << ch << "的值输入有误，请重新输入！" << endl;
            cin.clear();				//清空缓冲区 
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        else {
            cin.ignore(INT_MAX, '\n');
            return input;
        }
    }
}

//继续计算？ 
bool continueCalc()
{
    bool flag;
    char input;
    cout << endl << "是否继续运算？(y/n) "; 
    while (true) {
        cin >> input; 
        if ((input != 'y' && input != 'n') || cin.fail()) {
            cout << "错误！请输入 y/n" << endl;
            cin.clear();				//清空缓冲区 
            cin.ignore(INT_MAX, '\n');
            continue;
        }
        else {
            if (input == 'y') {
                flag = true;
            }
            else {
                cout << "欢迎下次再次使用！" << endl;
                flag = false; 
            }
            cin.ignore(INT_MAX, '\n');
            return flag;
        }
    }
}

int main() {
    welcome();
    
    
    while (true) {
        int P = getInput('P');
        int Q = getInput('Q');
        
        vector<int> calc;
        calc.push_back(P && Q);                   //与运算 
        calc.push_back(P || Q);                   //或运算 
        calc.push_back(!P || Q);                  //蕴含运算，与与或非等价 
        calc.push_back((!P || Q) && (!Q || P));   //等值运算 
        
        cout << endl;
        cout << "合取：\n\tp /\\ q = " << calc[0] << endl;
        cout << "析取：\n\tp \\/ q = " << calc[1] << endl;
        cout << "条件：\n\tp -> q = " << calc[2] << endl;
        cout << "双条件：\n\tp <-> q = " << calc[3] << endl;

        if (!continueCalc()) {
            break;
        }
    }
    return 0;
}
