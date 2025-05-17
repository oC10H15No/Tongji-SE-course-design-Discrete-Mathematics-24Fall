#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// 边
class Edge
{
public:
    int u, v;   // 顶点
    int weight; // 权重
    Edge(int u, int v, int weight) : u(u), v(v), weight(weight) {}
    bool operator<(const Edge &other) const { return weight < other.weight; }
};

// 并查集，用于 Kruskal 算法检测环
class UnionFind
{
private:
    vector<int> parent;
public:
    // 初始化，每个节点的父节点指向自己
    UnionFind(int n)
    {
        parent.resize(n);
        for (int i = 0; i < n; ++i) { parent[i] = i; }
    }
    // 查找，带路径压缩
    int find(int x)
    {
        if (parent[x] != x) { parent[x] = find(parent[x]); }
        return parent[x];
    }
    // 合并
    void unite(int x, int y) { parent[find(x)] = find(y); }
    // 检查两个节点是否联通
    bool connected(int x, int y) { return find(x) == find(y); }
};

// 图
class Graph
{
private:
    int V;                                  // 顶点数
    vector<Edge> edges;                     // 边，用于 Kruskal 算法
    vector<vector<pair<int, int>>> adjList; // 邻接表，用于 Prim 算法
public:
    Graph(int V) : V(V) { adjList.resize(V); }

    // 添加边，u 和 v 为定点编号，weight 为权值
    void addEdge(int u, int v, int weight)
    {
        edges.push_back(Edge(u, v, weight));
        adjList[u].push_back({v, weight});
        adjList[v].push_back({u, weight});
    }

    void kruskalMST()
    {
        // 将边按权值排序
        sort(edges.begin(), edges.end());
        UnionFind uf(V);        // 初始化并查集
        int mst_weight = 0;     // 最小生成树的总权值
        vector<Edge> mst_edges; // 存储最小生成树的边

        for (const auto &edge : edges)
        {
            // 如果边的两个端点不连通，则加入最小生成树
            if (!uf.connected(edge.u, edge.v))
            {
                uf.unite(edge.u, edge.v); // 合并
                mst_weight += edge.weight;
                mst_edges.push_back(edge);
            }
        }

        cout << "\nKruskal 最小生成树的总权值(最小成本)：" << mst_weight << endl;
        cout << "构成最小生成树的边：" << endl;
        for (const auto &edge : mst_edges)
        {
            cout << edge.u + 1 << " - " << edge.v + 1 << " : " << edge.weight << endl;
        }
        cout << endl;
    }

    void primMST()
    {
        vector<int> key(V, numeric_limits<int>::max());  // 保存连接每个顶点的最小权值
        vector<int> parent(V, -1);    // 保存每个顶点的父节点
        vector<bool> inMST(V, false); // 标记顶点是否在最小生成树中

        // 优先队列，按权值从小到大选择边
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        key[0] = 0;      // 从顶点 0 开始
        pq.push({0, 0}); // {权值，顶点}

        while (!pq.empty())
        {
            int u = pq.top().second; // 选择权值最小的顶点
            pq.pop();

            if (inMST[u])
                continue;    // 如果已在最小生成树中，跳过
            inMST[u] = true; // 标记为已访问

            // 遍历 u 的邻接边
            for (auto &adj : adjList[u])
            {
                int v = adj.first;
                int weight = adj.second;
                // 如果 v 不在最小生成树中且权值小于 key[v]，则更新 key[v] 和 parent[v]
                if (!inMST[v] && weight < key[v])
                {
                    key[v] = weight;
                    parent[v] = u;
                    pq.push({key[v], v});
                }
            }
        }

        int mst_weight = 0; // 最小生成树的总权值
        for (int i = 0; i < V; ++i)
        {
            if (parent[i] != -1) mst_weight += key[i];
        }

        cout << "\nPrim 最小生成树的总权值(最小成本)：" << mst_weight << endl;
        cout << "构成最小生成树的边：" << endl;
        for (int i = 1; i < V; ++i)
        {
            if (parent[i] != -1) {
                cout << parent[i] + 1 << " - " << i + 1 << " : " << key[i] << endl;
            }
        }
        cout << endl;
    }
};

int main()
{
    int V, E;

    while (true)
    {
        cout << "请输入顶点数和边数：(输入 0 0 退出)" << endl;
        cin >> V >> E;
        if (V == 0 && E == 0) return 0;
        else if (cin.fail() || V < 2 || E < V - 1 || E > V * (V - 1) / 2)
        {
            cerr << "输入错误，请重新输入" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        else
        {
            break;
        }
    }

    Graph graph(V);
    cout << "请一次输入每条边的起点、终点和权值（以空格分割）：" << endl;
    for (int i = 0; i < E; ++i)
    {
        cout << "第 " << i + 1 << " 条边：";
        int u, v, weight;
        cin >> u >> v >> weight;

        if (cin.fail() || u < 1 || u > V || v < 1 || v > V || weight < 0)
        {
            cerr << "输入错误，请重试!" << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            --i; // 重复此次输入
            continue;
        }

        --u, --v;
        graph.addEdge(u, v, weight);
    }

    graph.kruskalMST();
    graph.primMST();

    return 0;
}