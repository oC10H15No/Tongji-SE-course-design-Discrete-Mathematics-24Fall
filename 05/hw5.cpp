#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>

using namespace std;

int total_cost = 0;

// 霍夫曼树节点
struct HuffmanNode {
    int freq;
    HuffmanNode *left, *right;

    HuffmanNode(int freq) : freq(freq), left(nullptr), right(nullptr) {}
};

// 比较器，用于优先队列
struct compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

// 递归前序遍历打印霍夫曼编码
void printCodes(HuffmanNode* root, string str) {
    if (!root)
        return;

    if (!root->left && !root->right) // 叶子节点
        cout << setw(2) << root->freq << ": " << str << "\n";

    printCodes(root->left, str + "0");
    printCodes(root->right, str + "1");
}

// 初始化霍夫曼树节点
priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare> initializeTree(int freq[], int size) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare> minHeap;
    for (int i = 0; i < size; ++i)
        minHeap.push(new HuffmanNode(freq[i]));
    return minHeap;
}

// 构建霍夫曼树
HuffmanNode* buildHuffmanTree(priority_queue<HuffmanNode*, vector<HuffmanNode*>, compare>& minHeap) {
    HuffmanNode *left, *right, *top;

    while (minHeap.size() != 1) {
        left = minHeap.top();
        minHeap.pop();

        right = minHeap.top();
        minHeap.pop();

        top = new HuffmanNode(left->freq + right->freq);
        top->left = left;
        top->right = right;
        total_cost += top->freq;
        minHeap.push(top);
    }

    return minHeap.top();
}

// 打印霍夫曼编码
void printHuffmanCodes(HuffmanNode* root) {
    printCodes(root, "");
}

int main() {
    int n;
    cout << "请输入叶节点的数量: ";
    while (true)
    {
        cin >> n;
        if (cin.fail() || n <= 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "输入错误，请重新输入: ";
            continue;
        } else {
            break;
        }
    }

    int* freq = new int[n];

    cout << "请依次输入叶节点: ";
    for (int i = 0; i < n; ++i) {
        cin >> freq[i];
        if (cin.fail() || freq[i] <= 0) {
            cerr << "输入错误，请重新输入第 " << i + 1 << " 个节点及以后的节点: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --i;
        }
    }

    auto minHeap = initializeTree(freq, n);
    HuffmanNode* root = buildHuffmanTree(minHeap);
    cout << "霍夫曼编码: \n";
    printHuffmanCodes(root);
    // cout << "总花销为：" << total_cost << endl;
    delete[] freq;

    return 0;
}