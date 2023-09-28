#include <bits/stdc++.h>
using namespace std;

struct graph
{
    long vertexCount, edgeCount;
    vector<vector<pair<long, long>>> adjList;
    map<long, long> heuristic;
    map<pair<long, long>, long> tempWeight;
    vector<bool> expands;
    long costBFS = 0;
    long costDFS = 0;
    long costUCS = 0;
    long costGBFS = 0;
    void init(long v)
    {
        vertexCount = v;
        expands.resize(vertexCount);
        fill(expands.begin(), expands.end(), false);
        edgeCount = 0;
        for (int i = 0; i < vertexCount; i++)
        {
            adjList.push_back({});
        }
    }

    void add_heuristic(long vertex, long h)
    {
        heuristic[vertex] = h;
    }

    void add_edge(long vertex1, long vertex2, long weight)
    {
        tempWeight[make_pair(vertex1, vertex2)] = weight;
        tempWeight[make_pair(vertex2, vertex1)] = weight;
        adjList[vertex1].push_back(make_pair(vertex2, weight));
        adjList[vertex2].push_back(make_pair(vertex1, weight));
        edgeCount++;
    }

    void aStar(vector<pair<vector<long>, long>> &result, long start, long finish)
    {
        vector<bool> visited(adjList.size(), false);
        vector<long> intialPath;
        intialPath.push_back(start);
        priority_queue<pair<pair<long, vector<long>>, pair<long, long>>, vector<pair<pair<long, vector<long>>, pair<long, long>>>, greater<pair<pair<long, vector<long>>, pair<long, long>>>> urut;
        urut.push(make_pair(make_pair(0, intialPath), make_pair(INT_MAX, start)));

        while (!urut.empty())
        {
            vector<long> tempPath = urut.top().first.second;
            long temp = urut.top().second.second;
            long tempCost = urut.top().first.first;
            urut.pop();
            if (temp == finish)
            {
                costUCS = tempCost;
                result.push_back(make_pair(tempPath, tempCost));
                for (auto i : tempPath)
                {
                    cout << i;
                    if (i != tempPath.back())
                    {
                        cout << " -> ";
                    }
                }
                puts("");
                cout << "Total Cost : " << costUCS << endl;
                puts("----------------------------------");
                return;
            }
            if (tempPath.size() != 1)
            {
                tempCost -= heuristic[tempPath.back()];
            }
            if (!visited[temp])
            {
                visited[temp] = true;
                result.push_back(make_pair(tempPath, tempCost));
            }

            for (auto vertex : adjList[temp])
            {
                vector<long> newPath = tempPath;
                newPath.push_back(vertex.first);
                long neighbor = vertex.first;
                long cost = vertex.second;
                if (!visited[neighbor])
                {
                    urut.push(make_pair(make_pair((cost + tempCost + heuristic[neighbor]), newPath), make_pair(temp, neighbor)));
                }
            }
        }
    }

    void greedyBestFirstSearch(vector<vector<long>> &result, long start, long finish)
    {
        vector<bool> visited(adjList.size(), false);
        vector<long> intialPath;
        intialPath.push_back(start);
        priority_queue<pair<pair<long, vector<long>>, pair<long, long>>, vector<pair<pair<long, vector<long>>, pair<long, long>>>, greater<pair<pair<long, vector<long>>, pair<long, long>>>> urut;
        urut.push(make_pair(make_pair(0, intialPath), make_pair(INT_MAX, start)));

        while (!urut.empty())
        {
            vector<long> tempPath = urut.top().first.second;
            long temp = urut.top().second.second;
            long tempCost = urut.top().first.first;
            // cout << temp << " " << tempCost << endl;
            urut.pop();
            if (temp == finish)
            {
                long countTemp = start;
                result.push_back(tempPath);
                for (auto i : tempPath)
                {
                    costGBFS += tempWeight[make_pair(countTemp, i)];
                    cout << i;
                    if (i != tempPath.back())
                    {
                        cout << " -> ";
                    }
                    if (i == start)
                        continue;
                    countTemp = i;
                }
                puts("");
                cout << "Total Cost : " << costGBFS << endl;
                puts("----------------------------------");
                return;
            }

            if (!visited[temp])
            {
                visited[temp] = true;
                result.push_back(tempPath);
            }

            for (auto vertex : adjList[temp])
            {
                vector<long> newPath = tempPath;
                newPath.push_back(vertex.first);
                long neighbor = vertex.first;
                if (!visited[neighbor])
                {
                    urut.push(make_pair(make_pair(heuristic[neighbor], newPath), make_pair(temp, neighbor)));
                }
            }
        }
    }

    void ucs(vector<pair<vector<long>, long>> &result, long start, long finish)
    {
        vector<bool> visited(adjList.size(), false);
        vector<long> intialPath;
        intialPath.push_back(start);
        priority_queue<pair<pair<long, vector<long>>, pair<long, long>>, vector<pair<pair<long, vector<long>>, pair<long, long>>>, greater<pair<pair<long, vector<long>>, pair<long, long>>>> urut;
        urut.push(make_pair(make_pair(0, intialPath), make_pair(INT_MAX, start)));

        while (!urut.empty())
        {
            vector<long> tempPath = urut.top().first.second;
            long tempCost = urut.top().first.first;
            long temp = urut.top().second.second;
            // cout << start << "->" << temp << ":" << tempCost << endl;
            urut.pop();
            if (temp == finish)
            {
                costUCS = tempCost;
                result.push_back(make_pair(tempPath, tempCost));
                for (auto i : tempPath)
                {
                    cout << i;
                    if (i != tempPath.back())
                    {
                        cout << " -> ";
                    }
                }
                puts("");
                cout << "Total Cost : " << costUCS << endl;
                puts("----------------------------------");
                return;
            }

            if (!visited[temp])
            {
                visited[temp] = true;
                result.push_back(make_pair(tempPath, tempCost));
            }

            for (auto vertex : adjList[temp])
            {
                vector<long> newPath = tempPath;
                newPath.push_back(vertex.first);
                long neighbor = vertex.first;
                long cost = vertex.second;
                if (!visited[neighbor])
                {
                    urut.push(make_pair(make_pair(cost + tempCost, newPath), make_pair(temp, neighbor)));
                }
            }
        }
    }

    void dfs(vector<vector<long>> &result, long start, long finish)
    {
        for (auto &inner_vec : adjList)
        {
            sort(inner_vec.begin(), inner_vec.end(), [](const pair<long, long> &a, const pair<long, long> &b)
                 { return a.first < b.first; });
        }
        vector<bool> visited(vertexCount, false);
        vector<long> initialPath;
        initialPath.push_back(start);
        stack<pair<long, vector<long>>> st;
        st.push(make_pair(start, initialPath));
        visited[start] = true;
        result.push_back(initialPath);

        while (!st.empty())
        {
            vector<long> tempPath = st.top().second;
            long temp = st.top().first;
            st.pop();
            if (temp == finish)
            {
                for (auto i : tempPath)
                {
                    cout << i;
                    if (i != finish)
                    {
                        cout << " -> ";
                    }
                    for (auto j : adjList[start])
                    {
                        if (j.first == i)
                        {
                            costDFS += j.second;
                            break;
                        }
                    }
                    start = i;
                }
                puts("");
                result.push_back(tempPath);
                cout << "Total Cost : " << costDFS << endl;
                puts("----------------------------------");
                return;
            }
            if (!visited[temp])
            {
                result.push_back(tempPath);
                visited[temp] = true;
            }

            for (auto vertex : adjList[temp])
            {
                vector<long> newPath = tempPath;
                newPath.push_back(vertex.first);
                if (!visited[vertex.first])
                    st.push(make_pair(vertex.first, newPath));
            }
        }
    }

    void bfs(vector<vector<long>> &result, long start, long finish)
    {
        for (auto &inner_vec : adjList)
        {
            sort(inner_vec.begin(), inner_vec.end(), [](const pair<long, long> &a, const pair<long, long> &b)
                 { return a.first < b.first; });
        }
        vector<bool> visited(vertexCount, false);
        queue<vector<long>> q;
        vector<long> initialPath;
        initialPath.push_back(start);
        q.push(initialPath);
        visited[start] = true;
        result.push_back(initialPath);
        while (!q.empty())
        {
            vector<long> currentPath = q.front();
            long temp = currentPath.back();
            q.pop();
            for (auto vertex : adjList[temp])
            {
                if (!visited[vertex.first])
                {
                    vector<long> newPath = currentPath;
                    newPath.push_back(vertex.first);
                    if (vertex.first == finish)
                    {
                        for (auto i : newPath)
                        {
                            cout << i;
                            if (i != finish)
                            {
                                cout << " -> ";
                            }
                            if (start == i)
                                continue;
                            for (auto j : adjList[start])
                            {
                                if (j.first == i)
                                {
                                    costBFS += j.second;
                                    break;
                                }
                            }
                            start = i;
                        }
                        puts("");
                        cout << "Total Cost : " << costBFS << endl;
                        puts("----------------------------------");
                    }
                    q.push(newPath);
                    visited[vertex.first] = true;
                    result.push_back(newPath);
                }
            }
        }
    }

    void printChild(string mode, long parent, long start, long cp)
    {
        if (mode == "DFS")
        {
            for (auto &inner_vec : adjList)
            {
                sort(inner_vec.begin(), inner_vec.end(), [](const pair<long, long> &a, const pair<long, long> &b)
                     { return a.first > b.first; });
            }
        }

        else if (mode == "UCS" || mode == "aStar")
        {
            for (auto i : adjList[start])
            {
                if (!expands[i.first] && mode == "UCS")
                {
                    cout << "Child : " << i.first << " [Cost : " << cp + i.second << "]" << endl;
                }
                else if (!expands[i.first] && mode == "aStar")
                {
                    cout << "Child : " << i.first << " [Cost : " << cp + i.second + heuristic[i.first] << "]" << endl;
                }

                else
                {
                    cout << "Child : " << i.first << " [Expanded]" << endl;
                }
                expands[parent] = true;
            }
            return;
        }

        for (auto i : adjList[start])
        {
            if (!expands[i.first] && mode == "gBFS")
            {
                cout << "Child : " << i.first << " [Cost : " << heuristic[i.first] << "]" << endl;
            }
            else if (!expands[i.first])
            {
                cout << "Child : " << i.first << endl;
            }
            else
            {
                cout << "Child : " << i.first << " [Expanded]" << endl;
            }
            expands[parent] = true;
        }
    }

    void resetExpand()
    {
        fill(expands.begin(), expands.end(), false);
    }
};

int main()
{
    graph g;
    g.init(6);
    g.add_edge(3, 2, 1);
    g.add_edge(2, 4, 3);
    g.add_edge(2, 0, 2);
    g.add_edge(0, 5, 3);
    g.add_edge(0, 4, 4);
    g.add_edge(4, 5, 3);
    g.add_edge(1, 5, 5);
    g.add_edge(1, 4, 9);
    g.add_heuristic(0, 6);
    g.add_heuristic(1, 0);
    g.add_heuristic(2, 10);
    g.add_heuristic(3, 9);
    g.add_heuristic(4, 9);
    g.add_heuristic(5, 4);
    vector<pair<vector<long>, long>> ucs_result, aStar_result;
    vector<vector<long>> bfs_result, dfs_result, gBFS_result;

    puts("A*");
    g.aStar(aStar_result, 3, 1);
    g.resetExpand();
    for (auto it : aStar_result)
    {
        cout << "Expands : ";
        for (auto at : it.first)
        {
            cout << at;
            if (at != it.first.back())
            {
                cout << " -> ";
            }
            else
            {
                puts("");
                g.printChild("aStar", it.first.back(), at, it.second);
            }
        }
        puts("");
    }
    puts("");

    puts("Greedy BFS");
    g.greedyBestFirstSearch(gBFS_result, 3, 1);
    g.resetExpand();
    for (auto it : gBFS_result)
    {
        cout << "Expands : ";
        for (auto at : it)
        {
            cout << at;
            if (at != it.back())
            {
                cout << " -> ";
            }
            else
            {
                puts("");
                g.printChild("gBFS", it.back(), at, 0);
            }
        }
        puts("");
    }
    puts("");

    puts("UCS");
    g.ucs(ucs_result, 3, 1);
    g.resetExpand();
    for (auto it : ucs_result)
    {
        cout << "Expands : ";
        for (auto at : it.first)
        {
            cout << at;
            if (at != it.first.back())
            {
                cout << " -> ";
            }
            else
            {
                puts("");
                g.printChild("UCS", it.first.back(), at, it.second);
            }
        }
        puts("");
    }
    puts("");

    puts("DFS");
    g.dfs(dfs_result, 3, 1);
    g.resetExpand();
    for (auto it : dfs_result)
    {
        cout << "Expands : ";
        for (auto at : it)
        {
            cout << at;
            if (at != it.back())
            {
                cout << " -> ";
            }
            else
            {
                puts("");
                g.printChild("DFS", it.back(), at, 0);
            }
        }
        puts("");
    }
    cout << endl;

    puts("BFS");
    g.bfs(bfs_result, 3, 1);
    g.resetExpand();
    for (auto it : bfs_result)
    {
        cout << "Expands : ";
        for (auto at : it)
        {
            cout << at;
            if (at != it.back())
            {
                cout << " -> ";
            }
            else
            {
                puts("");
                g.printChild("BFS", it.back(), at, 0);
            }
        }
        puts("");
    }
    cout << endl;

    return 0;
}