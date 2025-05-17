#include <iostream>
#include <limits>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <map>

using namespace std;

// 定义模板类 Matrix，可适用于任意类型的矩阵操作
template <class T>
class Matrix
{
private:
    T **matrix; // 矩阵数据，使用二维指针数组
    int rows;   // 行数
    int cols;   // 列数
public:
    // 构造函数
    Matrix(int rows, int cols)
    {
        this->rows = rows;
        this->cols = cols;
        matrix = new T *[rows]; // 分配行指针数组
        for (int i = 0; i < rows; ++i)
        {
            matrix[i] = new T[cols]; // 分配每一行的列数组
            for (int j = 0; j < cols; ++j)
            {
                matrix[i][j] = T(); // 初始化为默认值
            }
        }
    }

    // 拷贝构造函数，确保深拷贝
    Matrix(const Matrix<T> &m)
    {
        rows = m.rows;
        cols = m.cols;
        matrix = new T *[rows];
        for (int i = 0; i < rows; ++i)
        {
            matrix[i] = new T[cols];
            for (int j = 0; j < cols; ++j)
            {
                matrix[i][j] = m.matrix[i][j];
            }
        }
    }

    // 默认构造函数，初始化为空矩阵
    Matrix()
    {
        rows = 0;
        cols = 0;
        matrix = nullptr;
    }

    // 赋值运算符，防止自我赋值，并保证深拷贝
    Matrix<T> &operator=(const Matrix<T> &m)
    {
        if (this == &m)
            return *this; // 防止自我赋值
        // 释放原有矩阵内存
        for (int i = 0; i < rows; ++i)
            delete[] matrix[i];
        delete[] matrix;
        // 重新分配并复制新矩阵
        rows = m.rows;
        cols = m.cols;
        matrix = new T *[rows];
        for (int i = 0; i < rows; ++i)
        {
            matrix[i] = new T[cols];
            for (int j = 0; j < cols; ++j)
            {
                matrix[i][j] = m.matrix[i][j];
            }
        }
        return *this;
    }

    // 析构函数，释放内存并置空指针
    ~Matrix()
    {
        for (int i = 0; i < rows; ++i)
        {
            delete[] matrix[i];
        }
        delete[] matrix;
        matrix = nullptr; // 防止悬空指针
    }

    void set(int i, int j, T value) // 设置矩阵元素
    {
        matrix[i][j] = value;
    }

    T get(int i, int j) const // 获取矩阵元素
    {
        return matrix[i][j];
    }

    int getRows() const // 获取行数
    {
        return rows;
    }

    int getCols() const // 获取列数
    {
        return cols;
    }

    void print() // 打印矩阵
    {
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                cout << matrix[i][j] << " ";
            }
            cout << endl;
        }
    }

    // 重载运算符 + (矩阵加法)
    Matrix<T> operator+(const Matrix<T> &m) const
    {
        if (rows != m.rows || cols != m.cols)
        {
            throw invalid_argument("矩阵维度不匹配");
        }
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if constexpr (is_same<T, bool>::value)
                {
                    result.set(i, j, matrix[i][j] || m.get(i, j)); // 布尔矩阵使用逻辑或
                }
                else
                {
                    result.set(i, j, matrix[i][j] + m.get(i, j)); // 其他类型使用加法
                }
            }
        }
        return result;
    }

    // 重载运算符 * (矩阵乘法)
    Matrix<T> operator*(const Matrix<T> &m) const
    {
        if (cols != m.rows)
        {
            throw invalid_argument("矩阵维度不匹配");
        }
        Matrix<T> result(rows, m.cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
            {
                if constexpr (is_same<T, bool>::value)
                {
                    bool sum = false;
                    for (int k = 0; k < cols; k++)
                    {
                        sum = sum || (matrix[i][k] && m.get(k, j)); // 布尔矩阵使用逻辑与
                    }
                    result.set(i, j, sum);
                }
                else
                {
                    T sum = 0;
                    for (int k = 0; k < cols; k++)
                    {
                        sum += matrix[i][k] * m.get(k, j); // 其他类型使用乘法
                    }
                    result.set(i, j, sum);
                }
            }
        }
        return result;
    }

    Matrix<T> transpose() // 矩阵转置
    {
        Matrix<T> result(cols, rows);
        for (int i = 0; i < cols; i++)
        {
            for (int j = 0; j < rows; j++)
            {
                result.set(i, j, matrix[j][i]);
            }
        }
        return result;
    }

    T det() // 矩阵行列式
    {
        if (rows != cols)
        {
            cout << "Error: Matrix is not square." << endl;
            return 0;
        }
        if (rows == 1)
        {
            return matrix[0][0];
        }
        if (rows == 2)
        {
            return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
        }
        T result = 0;
        for (int i = 0; i < cols; i++)
        {
            Matrix<T> sub(rows - 1, cols - 1);
            for (int j = 1; j < rows; j++)
            {
                for (int k = 0; k < cols; k++)
                {
                    if (k < i)
                    {
                        sub.set(j - 1, k, matrix[j][k]);
                    }
                    else if (k > i)
                    {
                        sub.set(j - 1, k - 1, matrix[j][k]);
                    }
                }
            }
            result += matrix[0][i] * pow(-1, i) * sub.det();
        }
        return result;
    }

    Matrix<T> inv() // 矩阵逆
    {
        if (rows != cols)
        {
            cout << "Error: Matrix is not square." << endl;
            return *this;
        }
        T d = det();
        if (d == 0)
        {
            cout << "Error: Matrix is singular." << endl;
            return *this;
        }
        Matrix<T> result(rows, cols);
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Matrix<T> sub(rows - 1, cols - 1);
                for (int k = 0; k < rows; k++)
                {
                    for (int l = 0; l < cols; l++)
                    {
                        if (k < i && l < j)
                        {
                            sub.set(k, l, matrix[k][l]);
                        }
                        else if (k < i && l > j)
                        {
                            sub.set(k, l - 1, matrix[k][l]);
                        }
                        else if (k > i && l < j)
                        {
                            sub.set(k - 1, l, matrix[k][l]);
                        }
                        else if (k > i && l > j)
                        {
                            sub.set(k - 1, l - 1, matrix[k][l]);
                        }
                    }
                }
                result.set(i, j, pow(-1, i + j) * sub.det() / d);
            }
        }
        return result.transpose();
    }

    // 重载运算符 ==
    bool operator==(Matrix<T> &m)
    {
        if (rows != m.rows || cols != m.cols)
        {
            return false;
        }
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                if (matrix[i][j] != m.get(i, j))
                {
                    return false;
                }
            }
        }
        return true;
    }

    // 重载输出运算符
    friend ostream &operator<<(ostream &os, Matrix<T> &m)
    {
        for (int i = 0; i < m.rows; i++)
        {
            for (int j = 0; j < m.cols; j++)
            {
                os << m.get(i, j) << " ";
            }
            os << endl;
        }
        return os;
    }

    // 重载幂运算符
    Matrix<T> operator^(int n)
    {
        if (rows != cols)
        {
            cout << "Error: Matrix is not square." << endl;
            return *this;
        }
        if (n == 0)
        {
            Matrix<T> result(rows, cols);
            for (int i = 0; i < rows; i++)
            {
                result.set(i, i, 1);
            }
            return result;
        }
        if (n < 0)
        {
            return inv() ^ (-n);
        }
        Matrix<T> result = *this;
        for (int i = 1; i < n; i++)
        {
            result = result * (*this);
        }
        return result;
    }
};

Matrix<bool> readMatrix()
{
    int n;

    while (true)
    {
        cout << "请输入矩阵的阶数 : ";
        cin >> n;
        if (cin.fail() || n <= 0)
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "请输入正整数！" << endl;
        }
        else
        {
            break;
        }
    }

    int rows = n, cols = n;
    cout << "请输入关系矩阵 : " << endl;

    Matrix<bool> m(rows, cols);
    for (int i = 0; i < rows; i++)
    {
        cout << "请输入矩阵的第 " << i + 1 << " 行元素(元素以空格分割) : ";

        for (int j = 0; j < cols; j++)
        {
            while (true)
            {
                int value;
                cin >> value;
                if (cin.fail() || (value != 0 && value != 1))
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "矩阵的第 " << i + 1 << " 行输入错误，请重新以合法的布尔值输入该行 : " << endl;
                    j = 0;
                }
                else
                {
                    m.set(i, j, value);
                    break;
                }
            }
        }
    }
    return m;
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
            return true;
        }
        else if (ch == 'N' || ch == 'n')
        {
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

// 传递闭包 Warshall 算法
Matrix<bool> transitiveWarshall(Matrix<bool> &m)
{
    Matrix<bool> result = m;
    int n = m.getRows();
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                // // 更新 result[i][j]，如果存在从 i 到 k 和从 k 到 j 的路径，则存在从 i 到 j 的路径
                result.set(i, j, result.get(i, j) || (result.get(i, k) && result.get(k, j)));
            }
        }
    }
    return result;
}

int main()
{
    while (true)
    {
        cout << "请选择求解传递闭包的操作 : (1.集合形式; 2.关系矩阵形式)" << endl;
        int choice;
        while (true)
        {
            cin >> choice;
            if (cin.fail() || choice != 1 && choice != 2)
            {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "输入错误，请重新输入" << endl;
            }
            else
            {
                break;
            }
        }
        if (choice == 1)
        {
            int numElements;
            cout << "请输入集合 A 中元素个数: ";
            while (true)
            {
                cin >> numElements;
                if (cin.fail() || numElements <= 0)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "请输入正整数！" << endl;
                }
                else
                {
                    break;
                }
            }

            vector<char> elements(numElements);
            map<char, int> elementIndex;
            cout << "请输入集合 A 中的各个元素: ";
            for (int i = 0; i < numElements; ++i)
            {
                cin >> elements[i];
                elementIndex[elements[i]] = i;
            }

            int numRelations;
            cout << "请输入二元关系 R 中元素个数: ";
            while (true)
            {
                cin >> numRelations;
                if (cin.fail() || numRelations <= 0)
                {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "请输入正整数！" << endl;
                }
                else
                {
                    break;
                }
            }

            Matrix<bool> relationshipMatrix(numElements, numElements);
            cout << "请依次输入二元关系 R 中的各个元素 (例如 a b 表示 <a, b>):" << endl;
            for (int i = 0; i < numRelations; ++i)
            {
                char a, b;
                cin >> a >> b;
                relationshipMatrix.set(elementIndex[a], elementIndex[b], true);
            }

            Matrix<bool> transitiveClosureWarshall = transitiveWarshall(relationshipMatrix);

            cout << "传递闭包 t(R) = { ";
            bool first = true;
            for (int i = 0; i < numElements; ++i)
            {
                for (int j = 0; j < numElements; ++j)
                {
                    if (transitiveClosureWarshall.get(i, j))
                    {
                        if (!first)
                        {
                            cout << ", ";
                        }
                        cout << "<" << elements[i] << ", " << elements[j] << ">";
                        first = false;
                    }
                }
            }
            cout << " }" << endl;

            if (!askToContinue())
            {
                break;
            }
        }
        if (choice == 2)
        {

            Matrix<bool> relationshipMatrix = readMatrix();
            cout << "输入的关系矩阵为 : " << endl
                 << relationshipMatrix << endl;

            cout << "传递闭包 (Warshall 算法) : " << endl;
            Matrix<bool> transitiveClosureWarshall = transitiveWarshall(relationshipMatrix);
            cout << transitiveClosureWarshall << endl;

            if (!askToContinue())
            {
                break;
            }
        }
    }
    return 0;
}

/*
0 0 1
1 0 1
0 1 1

1 0 1
0 0 1
0 1 0

1 0 1 1
0 1 0 1
0 0 0 0
1 0 0 0

0 1 0 0 1
1 0 1 0 0
0 1 0 1 0
0 0 1 0 1
1 0 0 1 0

*/
