#include <vector>
#include <algorithm>
#include <queue>
#include <iostream>

enum Color {
    WHITE,
    GRAY,
    BLACK
};

class IGraph {  //Интерфейс графа
public:
    virtual ~IGraph() {}    //Виртуальный деструктор чтобы при удалении по указателю не было утечки памяти

    //Добавить ребро выходящее из вершины from и ведущее в вершину to
    virtual void AddEdge(size_t from, size_t to) = 0;   //чисто виртуальная функция (не имеет реализации в этом классе

    //получить кол-во вершин в графе
    virtual size_t VerticesCount() const  = 0;

    //Получить вектор всех вершин в которые можно попасть по ребру из вершины vertex
    virtual std::vector<size_t> GetVertices(size_t vertex) const = 0;
};

//невзвешенный неорграф
class ListGraph : public IGraph {
public:
    ListGraph(size_t verticesNumber) : vertices(verticesNumber){}

    void AddEdge(size_t from, size_t to) override {
        vertices.at(from).push_back(to);
        vertices.at(to).push_back(from);
    }

    size_t VerticesCount() const override{
        return vertices.size();
    }

    std::vector<size_t> GetVertices(size_t vertex) const override {
        return vertices.at(vertex);
    }
private:
    std::vector<std::vector<size_t>> vertices;
};

int bfs(const IGraph* const graph, size_t vertex) {
    std::queue<size_t> q;
    q.push (vertex);
    std::vector<bool> used (graph->VerticesCount(), false);
    std::vector<int> length(graph->VerticesCount()), parent(graph->VerticesCount());
    used[vertex] = true;
    parent[vertex] = -1;
    while (!q.empty()) {
        size_t nextVertex = q.front();
        q.pop();
        for (size_t i=0; i<graph->GetVertices(nextVertex).size(); ++i) {
            size_t to = graph->GetVertices(nextVertex)[i];

            if (!used[to]) {
            used[to] = true;
            q.push (to);
            length[to] = length[nextVertex] + 1;
            parent[to] = static_cast<int>(nextVertex);
            }
            else {
            }
        }
    }
    return 0;
}


size_t kShortestPaths(const IGraph* const graph, size_t start, size_t finish){
    std::vector<size_t> shortest_paths(graph->VerticesCount());
    std::vector<bool> next_ring(graph->VerticesCount());
    std::queue<size_t> q;
    q.push (start);
    shortest_paths[start] = 1;
    while (!q.empty()) {
        size_t curVertex = q.front();
        q.pop();
        if(next_ring[curVertex] == true){
            if(shortest_paths[finish] != 0){
                return shortest_paths[finish];
            }
            next_ring.assign(next_ring.size(), false);
        }
        for (size_t nextVertex : graph->GetVertices(curVertex)) {

            if (shortest_paths[nextVertex] == 0) {
                shortest_paths[nextVertex] = shortest_paths[curVertex];
                q.push(nextVertex);
                next_ring[nextVertex] = true;
            }
            else if(next_ring[nextVertex]) {
                shortest_paths[nextVertex] += shortest_paths[curVertex];
            }

        }
    }

}


int main() {
    size_t V, N;
    std::cin >> V;
    std::cin >> N;
    ListGraph graph(V);
    for(size_t i = 0; i < N; i++) {
        size_t A, B;
        std::cin >> A >> B;
        graph.AddEdge(A, B);
    }
    size_t A, B;
    std::cin >> A >> B;
    std::cout << kShortestPaths(&graph, A, B);
}

