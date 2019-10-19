#ifndef DSF_GRAPH_H //Это нужно чтобы заголовочный файл не включался больше 1 раза в какой либо файл
#define DSF_GRAPH_H
#include <vector>
#include <algorithm>

//перечисление цветов
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

//Ориентированный граф на основе матрицы инцендентности, реализующий интерфейс IGraph
class MatrixGraph : public IGraph {
public:
    //Конструктор принимает кол-во вершин в графе
    MatrixGraph(size_t verticesCount) : vertices(verticesCount, std::vector<bool>(verticesCount, false)) { //задаём матрицу verticesCount x verticesCount заполненую false
    }

    void AddEdge(size_t from, size_t to) override {
        vertices.at(from).at(to) = true;
        //Если бы граф был неориентированным то также было бы vertices.at(to).at(from) = true;
    }

    size_t VerticesCount() const override {
        return vertices.size();
    }

    //Внимание! эта функция работает за линейное от кол-ва вершин время
    std::vector<size_t> GetVertices(size_t vertex) const override {
        std::vector<size_t> result;
        for(size_t i = 0, n = vertices.size(); i < n; ++i) {
            if(vertices.at(vertex)[i]) {    //если в нашей матрице на позиции (vertex, i) стоит true то из vertex по ребру можно попасть в i
                result.push_back(i);        //добавляем i в результат. Амартизированное время работы будет константным
            }
        }
        return result;
    }
private:
    std::vector<std::vector<bool>> vertices;   //Матрицу будем хранить в виде вектора векторов, кол-во столбцов равео кол-ву строк  равно  кол-ву вершин
};

// Функция принимает указатель на интерфкйс графа, вершину, и ссылку на вектор цветов
bool dfs(const IGraph* const graph, size_t vertex, std::vector<Color>& colors) {
    colors[vertex] = GRAY;  //когда зашли в вершину закрасили её в серый цвет
    for(auto nextVertex : graph->GetVertices(vertex)) { //Перебираем все вершины в которые можно попасть по ребру из данной
        if(colors[nextVertex] == GRAY) {
            return true;    //Нашли что есть цикл, дальше можно не искать
        } else if(colors[nextVertex] == WHITE)  //надо проверить что цвет белый чтобы идти дальше, т к в орграфе можно попасть и в чёрную
            if(dfs(graph, nextVertex, colors)) {
                return true;
        }
    }
    colors[vertex] = BLACK; //когда все соседние вершмны обрабртаны закрашиваем данную вершину в чёрный
    return false;
}

bool hasCycle(const IGraph* const graph) {
    auto verticesColors = std::vector<Color>(graph->VerticesCount(), WHITE);    //Задаём вектор цветов, индекс - номер вершины
    //Ищим первую белую вершину, здесь тоже намеренное усложнение чтобы показать работу с find, быстрее было просто один раз пройти for
    auto it = std::find(verticesColors.begin(), verticesColors.end(), WHITE);
    while (it != verticesColors.end()) {    //В случае если белых вершин нет, find вернёт vertices.end()
        if(dfs(graph, it - verticesColors.begin(), verticesColors)) {   //запускаем dfs для белой вершины, индекс находим как разницу найденого итератора и begin
            return true;    //Нашли цикл дальше можно не идти
        }
        it = std::find(verticesColors.begin(), verticesColors.end(), WHITE);    //Ищем есть ли ещё белые вершины, если граф не очень связан
    }
    return false;
}
#endif //DSF_GRAPH_H
