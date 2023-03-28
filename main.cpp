//
// Created by 86151 on 3/18/2023.
//

#include <iostream>
#include <list>
#include <string>
#include <algorithm>
#include <queue>
#include <vector>
#include <map>
using namespace std;

enum OperandType { CHAR_OT, CHARSET_OT, REGULAR };
enum DriverType { NULL_DT, CHAR_DT, CHARSET_DT };
enum StateType { MATCH, UNMATCH };
enum LexemeCategory {
    INTEGER_CONST,		// 整数常量
    FLOAT_CONST,		// 实数常量
    SCIENTIFIC_CONST,	// 科学计数法常量
    NUMERIC_OPERATOR,	// 数值运算词
    NOTE,				// 注释
    STRING_CONST,		// 字符串常量
    SPACE_CONST,		// 空格常量
    COMPARE_OPERATOR,	// 比较运算词
    ID,					// 变量词
    LOGIC_OPERATOR		// 逻辑运算词
};

int greater1(int a, int b){
    if(a > b){
        return 1;
    }
    return 0;
}
class State {
public:
    int stateId;
    StateType type;
    LexemeCategory category;

    State();
    State(State *state);
};

class Edge {
public:
    int fromState;
    int nextState;
    int driverId;
    DriverType type;

    Edge();
    Edge(Edge *edge);
};

class Graph {
public:
    int graphId;
    int numOfStates;
    list <Edge *>*pEdgeTable;
    list <State *>*pStateTable;

    Graph();
    Graph(Graph *pNFA);
};

// 正则运算式
class regularExpression {
public:
    int regularId;
    string name;
    char operatorSymbol; 	//正则运算符，共有 7 种：‘=’, ‘~’，‘-’, ‘|’，‘.’, ‘*’, ‘+’, ‘?’
    int operandId1; 		//左操作数
    int operandId2; 		//右操作数
    OperandType type1; 		//左操作数的类型
    OperandType type2; 		//右操作数的类型
    OperandType resultType; //运算结果的类型
    LexemeCategory category; // 词的 category 属性值
    Graph *pNFA; 			//对应的 NFA
};

// 字符集
class CharSet {
public:
    int indexId; 	//字符集 id
    int segmentId; 	//字符集中的段 id。一个字符集可以包含多个段
    char fromChar; 	//段的起始字符
    char toChar; 	//段的结尾字符
};

int range(char fromChar, char toChar); 	// 字符的范围运算
int unionFunc(char c1, char c2); 		// 字符的并运算
int unionFunc(int charSetId, char c); 	// 字符集与字符之间的并运算
int unionFunc(int charSetId1,int charSetId2); //字符集与字符集的并运算
int difference(int charSetId, char c); 	// 字符集与字符之间的差运算
Graph * generateBasicNFA(DriverType driverType, int driverId);
Graph * unionFunc(Graph *pNFA1, Graph *pNFA2); 		// 并运算
Graph * product(Graph *pNFA1, Graph *pNFA2); 	// 连接运算
Graph * plusClosure(Graph *pNFA); 	// 正闭包运算
Graph * closure(Graph *pNFA); 		// 闭包运算
Graph * zeroOrOne(Graph *pNFA); 	// 0 或者 1 个运算

bool graphHasIn(Graph *pNFA); // 判断是否有入边
bool graphHasOut(Graph *pNFA); // 判断是否有出边
Graph * graphAddBeginState(Graph *pNFA);
Graph * graphAddEndState(Graph *pNFA);

list<int> *move(Graph *graph, list<int> *states, int driverId);
list<int> *eClosure(Graph *graph, list<int> *states);
Graph * NFA_to_DFA(Graph *pNFA);

int checkStateExisted(list<list<int> *> *existStates, list<int> *stateList, map<int, list<int> *> *m, int &id);
bool isStateListSame(list<int> *stateList1, list<int> *stateList2);
vector<int> *getAllDriver(Graph *pNFA);
StateType getType(Graph* pNFA, map<int, list<int> *> *m, int i);
DriverType getDriverType(Graph *pNFA, int id);

int serialCharSetId = 0;
int serialSegmentId = 0;
int serialGraphId = 0;
int graphId = 0;

// 正则运算表
list<regularExpression *> *pRegularTable;
// 字符集表
list<CharSet *> *pCharSetTable;

int main(int argc, char** argv){
    pRegularTable = new list<regularExpression *>();
    pCharSetTable = new list<CharSet *>();
    vector<Graph *> graphVec;

    char from, to, c;
    int charSetId1, charSetId2;

    from = 'a'; to = 'a';
    cout << "test func range" << endl;
    cout << "range: " << range(from, to)
         << " from " << from << " to " << to << endl;
    from = 'b'; to = 'b';
    cout << "range: " << range(from, to)
         << " from " << from << " to " << to << endl;

    cout << "CharSet List: " << endl;
    for (list<CharSet *>::iterator it = pCharSetTable->begin(); it != pCharSetTable->end(); ++it) {
        cout << (*it)->indexId << " "
             << (*it)->fromChar<< " "
             << (*it)->toChar << endl;
    }
    cout << endl;

    Graph *graph = generateBasicNFA(CHAR_DT, 1);
    cout << "graph a: " << graph->graphId << endl;
    graphVec.push_back(graph);

    graph = generateBasicNFA(CHAR_DT, 2);
    cout << "graph b: " << graph->graphId << endl;
    graphVec.push_back(graph);

    graph = unionFunc(graphVec[0], graphVec[1]);
    cout << "graph a|b: " << graph->graphId << endl;
    graphVec.push_back(graph);

    graph = closure(graphVec[2]);
    cout << "graph (a|b)*: " << graph->graphId << endl;
    graphVec.push_back(graph);

    graph = product(graphVec[3], graphVec[0]);
    cout << "graph (a|b)*a: " << graph->graphId << endl;
    graphVec.push_back(graph);

    graph = product(graphVec[4], graphVec[1]);
    graph = product(graph, graphVec[1]);
    cout << "graph (a|b)*abb: " << graph->graphId << endl;
    graphVec.push_back(graph);
    cout << endl;
    cout << "finish construct (a|b)*abb" << endl;
    //	NFA测试
    for (int i = graphVec.size() - 1; i < graphVec.size(); ++i) {
        graph = graphVec[i];
        cout << "graphId: " <<graph->graphId << endl << "States: "
             << graph->numOfStates << endl;
        cout << "state Lists: " << endl;
        for (list<State *>::iterator it = graph->pStateTable->begin(); it != graph->pStateTable->end(); ++it) {
            cout << "Id: " << (*it)->stateId << " type: "
                 << (*it)->type << " category: "
                 << (*it)->category << endl;
        }
        cout << "Edges: " << graph->pEdgeTable->size() << endl;
        cout << "edge lists: " << endl;
        for (list<Edge *>::iterator it = graph->pEdgeTable->begin(); it != graph->pEdgeTable->end(); ++it) {
            cout << "from " << (*it)->fromState << " to "
                 << (*it)->nextState << " driverId: "
                 << (*it)->driverId << " type: "
                 << (*it)->type << endl;
        }
        cout << endl;
    }

    // DFA测试
    graph = NFA_to_DFA(graph);
    cout << "DFA: " << graph->graphId << endl;
    cout << "graphId: " <<graph->graphId << endl << "States: "
         << graph->numOfStates << endl;
    cout << "state Lists: " << endl;
    for (list<State *>::iterator it = graph->pStateTable->begin(); it != graph->pStateTable->end(); ++it) {
        cout << "Id: " << (*it)->stateId << " type: "
             << (*it)->type << " category: "
             << (*it)->category << endl;
    }
    cout << "Edges: " << graph->pEdgeTable->size() << endl;
    cout << "edge lists: " << endl;
    for (list<Edge *>::iterator it = graph->pEdgeTable->begin(); it != graph->pEdgeTable->end(); ++it) {
        cout << "from " << (*it)->fromState << " to "
             << (*it)->nextState << " driverId: "
             << (*it)->driverId << " type: "
             << (*it)->type << endl;
    }
    cout << endl;
    return 0;
}

// 字符的范围运算
int range(char fromChar, char toChar) {
    CharSet *charSet = new CharSet();
    charSet->indexId = ++serialCharSetId;
    charSet->segmentId = ++serialSegmentId;
    charSet->fromChar = fromChar;
    charSet->toChar = toChar;
    pCharSetTable->push_back(charSet);
    return serialCharSetId;
}

// 字符的并运算
int unionFunc(char c1, char c2) {
    // 判断当前两个字符是否可以合并为一个字符集
    bool includeFlag = false;

    CharSet *charSet1 = new CharSet();
    charSet1->indexId = ++serialCharSetId;
    charSet1->segmentId = ++serialSegmentId;
    if (c2 == c1 - 1) {
        charSet1->fromChar = c2;
        includeFlag = true;
    } else {
        charSet1->fromChar = c1;
    }
    if (c2 == c1 + 1) {
        charSet1->toChar = c2;
        includeFlag = true;
    } else {
        charSet1->toChar = c1;
    }
    pCharSetTable->push_back(charSet1);

    if (c1 == c2) includeFlag = true;

    if (!includeFlag) {
        CharSet *charSet2 = new CharSet();
        charSet2->indexId = serialCharSetId;
        charSet2->segmentId = ++serialSegmentId;
        charSet2->fromChar = c2;
        charSet2->toChar = c2;
        pCharSetTable->push_back(charSet2);
    }
    return serialCharSetId;
}

// 字符集与字符之间的并运算
int unionFunc(int charSetId, char c) {
    ++serialCharSetId;
    bool includeFlag = false;
    for (list<CharSet *>::iterator it = pCharSetTable->begin(); it != pCharSetTable->end(); ++it) {
        if ((*it)->indexId == charSetId) {
            // ？
            CharSet *tmpCharSet = new CharSet();
            tmpCharSet->indexId = serialCharSetId;
            tmpCharSet->segmentId = (*it)->segmentId;
            if (c == (*it)->fromChar - 1) {
                tmpCharSet->fromChar = c;
                includeFlag = true;
            } else {
                tmpCharSet->fromChar = (*it)->fromChar;
            }
            if (c == (*it)->toChar + 1) {
                tmpCharSet->toChar = c;
                includeFlag = true;
            } else {
                tmpCharSet->toChar = (*it)->toChar;
            }
            if (c >= tmpCharSet->fromChar && c <= tmpCharSet->toChar) includeFlag = true;
            pCharSetTable->push_back(tmpCharSet);
        }
    }
    if (!includeFlag) {
        CharSet *charSet = new CharSet();
        charSet->indexId = serialCharSetId;
        charSet->segmentId = ++serialSegmentId;
        charSet->fromChar = c;
        charSet->toChar = c;
        pCharSetTable->push_back(charSet);
    }
    return serialCharSetId;
}

//字符集与字符集的并运算 有问题，没有考虑段号相同的情况，多段
// TODO:此函数有很大的bug需要重写
int unionFunc(int charSetId1,int charSetId2) {
    ++serialCharSetId;
    // 已经存在的字符集
    map<char, int> existMap;
    int minChar = 127;
    int maxChar = 0;
    for (list<CharSet *>::iterator it = pCharSetTable->begin(); it != pCharSetTable->end(); ++it) {
        if ((*it)->indexId == charSetId1 || (*it)->indexId == charSetId2) {
            for (char i = (*it)->fromChar; i <= (*it)->toChar; ++i) {
                if (existMap.count(i) == 0) {
                    existMap[i] = 1;
                    if (i < minChar) minChar = i;
                    if (i > maxChar) maxChar = i;
                }
            }
        }
    }
    //minChar肯定存在，所以beginFlag初始值为true
    bool beginFlag = true;
    for (int i = minChar; i <= maxChar + 1; ++i) {
        // 判断当前字符是否存在
        if (!beginFlag && existMap.count(i)) {
            beginFlag = true;
            minChar = i;
        }
        if (beginFlag) {
            //找到第一个字符集与第二个字符集之间的差值
            if (existMap.count(i) == 0) {
                CharSet *tmpCharSet = new CharSet();
                tmpCharSet->indexId = serialCharSetId;
                tmpCharSet->segmentId = ++serialSegmentId;
                tmpCharSet->fromChar = minChar;
                tmpCharSet->toChar = i - 1;
                pCharSetTable->push_back(tmpCharSet);
                beginFlag = false;
            }
        }
    }
    return serialCharSetId;
}

// 字符集与字符之间的差运算
int difference(int charSetId, char c) {
    ++serialCharSetId;
    for (list<CharSet *>::iterator it = pCharSetTable->begin(); it != pCharSetTable->end(); ++it) {
        if ((*it)->indexId == charSetId) {
            if (c == (*it)->fromChar) {
                // c为头，取余下一段
                CharSet *tmpCharSet = new CharSet();
                tmpCharSet->indexId = serialCharSetId;
                tmpCharSet->segmentId = ++serialSegmentId;
                tmpCharSet->fromChar = (*it)->fromChar + 1;
                tmpCharSet->toChar = (*it)->toChar;
                pCharSetTable->push_back(tmpCharSet);
            } else if (c > (*it)->fromChar && c < (*it)->toChar) {
                // c 为中间，取前后两段
                CharSet *tmpCharSet1 = new CharSet();
                tmpCharSet1->indexId = serialCharSetId;
                tmpCharSet1->segmentId = ++serialSegmentId;
                tmpCharSet1->fromChar = (*it)->fromChar;
                tmpCharSet1->toChar = c - 1;
                pCharSetTable->push_back(tmpCharSet1);

                CharSet *tmpCharSet2 = new CharSet();
                tmpCharSet2->indexId = serialCharSetId;
                tmpCharSet2->segmentId = ++serialSegmentId;
                tmpCharSet2->fromChar = c + 1;
                tmpCharSet2->toChar = (*it)->toChar;
                pCharSetTable->push_back(tmpCharSet2);
            } else if (c == (*it)->toChar) {
                // c为尾，取前段
                CharSet *tmpCharSet = new CharSet();
                tmpCharSet->indexId = serialCharSetId;
                tmpCharSet->segmentId = ++serialSegmentId;
                tmpCharSet->fromChar = (*it)->fromChar;
                tmpCharSet->toChar = (*it)->toChar - 1;
                pCharSetTable->push_back(tmpCharSet);
            } else {
                // c在范围外，不管
                CharSet *tmpCharSet = new CharSet();
                tmpCharSet->indexId = serialCharSetId;
                tmpCharSet->segmentId = (*it)->segmentId;
                tmpCharSet->fromChar = (*it)->fromChar;
                tmpCharSet->toChar = (*it)->toChar;
                pCharSetTable->push_back(tmpCharSet);
            }
        }
    }
    return 0;
}

State::State(){
    this->stateId = 0;
    this->type = UNMATCH;
    this->category = INTEGER_CONST;
}

State::State(State *state) {
    this->stateId = state->stateId;
    this->type = state->type;
    this->category = state->category;
}

Edge::Edge() {
    this->driverId = 0;
    this->fromState = 0;
    this->nextState = 0;
    this->type = NULL_DT;
}

Edge::Edge(Edge *edge) {
    this->driverId = edge->driverId;
    this->fromState = edge->fromState;
    this->nextState = edge->nextState;
    this->type = edge->type;
}

Graph::Graph() {
    this->graphId = 0;
    this->numOfStates = 0;
    this->pEdgeTable = nullptr;
    this->pStateTable = nullptr;
}

Graph::Graph(Graph *pNFA) {
    this->graphId = ++serialGraphId;
    this->numOfStates = pNFA->numOfStates;

    // 导入状态列表
    list<State *> *stateTable = new list<State *>();
    for (list<State *>::iterator it = pNFA->pStateTable->begin(); it != pNFA->pStateTable->end(); ++it) {
        State *tmpState = new State(*it);
        stateTable->push_back(tmpState);
    }
    this->pStateTable = stateTable;

    // 导入边列表
    list<Edge *> *edgeTable = new list<Edge *>();
    for (list<Edge *>::iterator it = pNFA->pEdgeTable->begin(); it != pNFA->pEdgeTable->end(); ++it) {
        Edge *tmpEdge = new Edge(*it);
        edgeTable->push_back(tmpEdge);
    }
    this->pEdgeTable = edgeTable;
}

Graph * generateBasicNFA(DriverType driverType, int driverId) {
    Graph *graph = new Graph();
    graph->graphId = ++serialGraphId;
    graph->numOfStates = 2;

    // 创建首尾状态
    int serialStateId = -1;
    State *state1 = new State();
    state1->stateId = ++serialStateId;
    state1->type = UNMATCH;
    //尾部状态
    State *state2 = new State();
    state2->stateId = ++serialStateId;
    state2->type = MATCH;

    // 添加状态列表
    list<State *> *stateTable = new list<State *>();
    stateTable->push_back(state1);
    stateTable->push_back(state2);
    graph->pStateTable = stateTable;

    // 创建边
    Edge *edge = new Edge();
    edge->fromState = state1->stateId;
    edge->nextState = state2->stateId;
    edge->driverId = driverId;
    edge->type = driverType;

    // 添加边列表
    list<Edge *> *edgeTable = new list<Edge *>();
    edgeTable->push_back(edge);
    graph->pEdgeTable = edgeTable;

    return graph;
}

// 并运算
Graph * unionFunc(Graph *pNFA1, Graph *pNFA2) {
    Graph *newGraph1 = pNFA1;
    Graph *newGraph2 = pNFA2;
    // 预处理，处理成为初始无入，最终无出，可能产生垃圾
    if (graphHasIn(pNFA1)) newGraph1 = graphAddBeginState(newGraph1);
    if (graphHasOut(pNFA1)) newGraph1 = graphAddEndState(newGraph1);
    if (graphHasIn(pNFA2)) newGraph2 = graphAddBeginState(newGraph2);
    if (graphHasOut(pNFA2)) newGraph2 = graphAddEndState(newGraph2);

    Graph *graph = new Graph();
    graph->graphId = ++serialGraphId;
    graph->numOfStates = newGraph1->numOfStates + newGraph2->numOfStates - 2;

    // 添加状态
    list<State *> *stateTable = new list<State *>();
    // 起始状态
    State *beginState = new State();
    beginState->stateId = 0;
    beginState->type = UNMATCH;
    stateTable->push_back(beginState);
    // newGraph1状态
    for (list<State *>::iterator it = newGraph1->pStateTable->begin(); it != newGraph1->pStateTable->end(); ++it) {
        if ((*it)->stateId == 0) continue;	// 不添加第一个状态
        if ((*it)->stateId == newGraph1->numOfStates - 1) continue; // 不添加最后一个状态
        State *state = new State();
        state->stateId = (*it)->stateId;
        state->type = UNMATCH;
        state->category = (*it)->category;
        stateTable->push_back(state);
    }
    // newGraph2状态
    for (list<State *>::iterator it = newGraph2->pStateTable->begin(); it != newGraph2->pStateTable->end(); ++it) {
        if ((*it)->stateId == 0) continue;	// 不添加第一个状态
        if ((*it)->stateId == newGraph2->numOfStates - 1) continue; // 不添加最后一个状态
        State *state = new State();
        state->stateId = (*it)->stateId + newGraph1->numOfStates - 1;
        state->type = UNMATCH;
        state->category = (*it)->category;
        stateTable->push_back(state);
    }
    // 最终状态
    State *endState = new State();
    endState->stateId = newGraph1->numOfStates + newGraph2->numOfStates - 3;
    endState->type = MATCH;
    stateTable->push_back(endState);
    graph->pStateTable = stateTable;

    // 添加边
    list<Edge *> *edgeTable = new list<Edge *>();
    // 第一个图，仅更改末尾边
    for (list<Edge *>::iterator it = newGraph1->pEdgeTable->begin(); it != newGraph1->pEdgeTable->end(); ++it) {
        Edge *tmpEdge = new Edge();
        tmpEdge->driverId = (*it)->driverId;
        tmpEdge->fromState = (*it)->fromState;
        if ((*it)->nextState == newGraph1->numOfStates - 1) {	// 最终状态入边下一状态改变
            tmpEdge->nextState = newGraph1->numOfStates + newGraph2->numOfStates - 3;
        } else {
            tmpEdge->nextState = (*it)->nextState;
        }
        tmpEdge->type = (*it)->type;
        edgeTable->push_back(tmpEdge);
    }
    // 第二个图，更改所有边的首尾状态
    int baseStateId = newGraph1->numOfStates - 2;
    for (list<Edge *>::iterator it = newGraph2->pEdgeTable->begin(); it != newGraph2->pEdgeTable->end(); ++it) {
        Edge *tmpEdge = new Edge();
        tmpEdge->driverId = (*it)->driverId;
        if ((*it)->fromState == 0) {	// 最初状态出边，从0出，连接到id + baseId
            tmpEdge->fromState = 0;
        } else {
            tmpEdge->fromState = (*it)->fromState + baseStateId;
        }
        tmpEdge->nextState = (*it)->nextState + baseStateId;
        tmpEdge->type = (*it)->type;
        edgeTable->push_back(tmpEdge);
    }
    graph->pEdgeTable = edgeTable;
    return graph;
}

// 连接运算
Graph * product(Graph *pNFA1, Graph *pNFA2) {
    Graph *newGraph1 = pNFA1;
    Graph *newGraph2 = pNFA2;
    if (graphHasOut(pNFA1) && graphHasIn(pNFA2)) {
        newGraph1 = graphAddEndState(pNFA1);
        // 将图1末尾状态当作图2开始状态
        State *endState = newGraph1->pStateTable->back();
        State *beginState = newGraph2->pStateTable->front();
        endState->type = UNMATCH;
        endState->category = beginState->category;
    }

    Graph *graph = new Graph();
    graph->graphId = ++serialGraphId;
    graph->numOfStates = newGraph1->numOfStates + newGraph2->numOfStates - 1;

    // 添加状态
    list<State *> *stateTable = new list<State *>();
    // 添加图1所有状态
    for (list<State *>::iterator it = newGraph1->pStateTable->begin(); it != newGraph1->pStateTable->end(); ++it) {
        State *tmpState = new State();
        tmpState->stateId = (*it)->stateId;
        tmpState->type = UNMATCH;	// 图一所有状态均为unmatch
        tmpState->category = (*it)->category;
        stateTable->push_back(tmpState);
    }
    // 添加图2除初始状态外所有状态，状态ID增加
    int baseStateId = newGraph1->numOfStates - 1;
    for (list<State *>::iterator it = newGraph2->pStateTable->begin(); it != newGraph2->pStateTable->end(); ++it) {
        State *tmpState = new State();
        if ((*it)->stateId == 0) continue;
        tmpState->stateId = (*it)->stateId + baseStateId;
        tmpState->type = (*it)->type;
        tmpState->category = (*it)->category;
        stateTable->push_back(tmpState);
    }
    graph->pStateTable = stateTable;

    // 添加边
    list<Edge *> *edgeTable = new list<Edge *>();
    // 添加图1所有边
    for (list<Edge *>::iterator it = newGraph1->pEdgeTable->begin(); it != newGraph1->pEdgeTable->end(); ++it) {
        Edge *tmpEdge = new Edge();
        tmpEdge->driverId = (*it)->driverId;
        tmpEdge->fromState = (*it)->fromState;
        tmpEdge->nextState = (*it)->nextState;
        tmpEdge->type = (*it)->type;
        edgeTable->push_back(tmpEdge);
    }
    // 添加图2所有边
    for (list<Edge *>::iterator it = newGraph2->pEdgeTable->begin(); it != newGraph2->pEdgeTable->end(); ++it) {
        Edge *tmpEdge = new Edge();
        tmpEdge->driverId = (*it)->driverId;
        tmpEdge->fromState = (*it)->fromState + baseStateId;
        tmpEdge->nextState = (*it)->nextState + baseStateId;
        tmpEdge->type = (*it)->type;
        edgeTable->push_back(tmpEdge);
    }
    graph->pEdgeTable = edgeTable;
    return graph;
}

//正闭包运算 新建一条边由尾部状态指向开始状态 driverType = NULL_DT
Graph * plusClosure(Graph *pNFA) {
    // 增加一条边即可
    Graph *graph = new Graph(pNFA);
    Edge *edge = new Edge();
    edge->driverId = 0;
    edge->type = NULL_DT;
    edge->fromState = pNFA->numOfStates - 1;
    edge->nextState = 0;
    graph->pEdgeTable->push_back(edge);
    return graph;
}

// 闭包运算
Graph * closure(Graph *pNFA) {
    Graph *graph = new Graph(pNFA);

    // 是否可以化简
    bool hasIn = graphHasIn(graph);
    bool hasOut = graphHasOut(graph);
    if (!hasIn && !hasOut && graph->numOfStates == 2) {
        // 保留唯一状态
        graph->numOfStates = 1;
        list<State *>::iterator itState = graph->pStateTable->begin();
        State *beginState = new State(*itState);
        beginState->type = MATCH;
        graph->pStateTable->clear();
        graph->pStateTable->push_back(beginState);
        // 处理边
        for (list<Edge *>::iterator it = graph->pEdgeTable->begin(); it != graph->pEdgeTable->end();) {
            if ((*it)->type != NULL_DT) {
                (*it)->nextState = 0;
                ++it;
            } else {
                graph->pEdgeTable->erase(it);
            }
        }
    } else {
        // 添加返回边
        Edge *edge = new Edge();
        edge->driverId = 0;
        edge->type = NULL_DT;
        edge->fromState = pNFA->numOfStates - 1;
        edge->nextState = 0;
        graph->pEdgeTable->push_back(edge);
        // 预处理图
        if (graphHasIn(graph)) graph = graphAddBeginState(graph);
        if (graphHasOut(graph)) graph = graphAddEndState(graph);
        // 添加跳过边
        edge = new Edge();
        edge->driverId = 0;
        edge->type = NULL_DT;
        edge->fromState = 0;
        edge->nextState = pNFA->numOfStates - 1;
        graph->pEdgeTable->push_back(edge);
    }


    return graph;
}

// 0 或者 1 个运算
Graph * zeroOrOne(Graph *pNFA) {
    Graph *graph = new Graph(pNFA);
    if (graphHasIn(graph)) graph = graphAddBeginState(graph);
    if (graphHasOut(graph)) graph = graphAddEndState(graph);
    // 增加从开始状态到最终状态的边
    Edge *edge = new Edge();
    edge->driverId = 0;
    edge->type = NULL_DT;
    edge->fromState = 0;
    edge->nextState = pNFA->numOfStates - 1;
    graph->pEdgeTable->push_back(edge);

    return graph;
}

bool graphHasIn(Graph *pNFA) {
    for (list<Edge *>::iterator it = pNFA->pEdgeTable->begin(); it != pNFA->pEdgeTable->end(); ++it) {
        if ((*it)->nextState == 0) {
            return true;
        }
    }
    return false;
}

bool graphHasOut(Graph *pNFA) {
    for (list<Edge *>::iterator it = pNFA->pEdgeTable->begin(); it != pNFA->pEdgeTable->end(); ++it) {
        if ((*it)->fromState == pNFA->numOfStates - 1) {
            return true;
        }
    }
    return false;
}

Graph * graphAddBeginState(Graph *pNFA) {
    Graph *graph = new Graph();
    graph->graphId = ++serialGraphId;
    graph->numOfStates = pNFA->numOfStates + 1;

    list<State *> *stateTable = new list<State *>();
    list<Edge *> *edgeTable = new list<Edge *>();

    // 添加起始状态
    State *beginState = new State();
    beginState->stateId = 0;
    beginState->type = UNMATCH;
    stateTable->push_back(beginState);
    for (list<State *>::iterator it = pNFA->pStateTable->begin(); it != pNFA->pStateTable->end(); ++it) {
        State *state = new State();
        state->stateId = (*it)->stateId + 1;
        state->type = (*it)->type;
        state->category = (*it)->category;
        stateTable->push_back(state);
    }
    graph->pStateTable = stateTable;

    // 添加起始边
    Edge *beginEdge = new Edge();
    beginEdge->driverId = 0;
    beginEdge->fromState = 0;
    beginEdge->nextState = 1;
    beginEdge->type = NULL_DT;
    edgeTable->push_back(beginEdge);
    for (list<Edge *>::iterator it = pNFA->pEdgeTable->begin(); it != pNFA->pEdgeTable->end(); ++it) {
        Edge *edge = new Edge();
        edge->driverId = (*it)->driverId;
        edge->fromState = (*it)->fromState + 1;
        edge->nextState = (*it)->nextState + 1;
        edge->type = (*it)->type;
        edgeTable->push_back(edge);
    }
    graph->pEdgeTable = edgeTable;
    return graph;
}

Graph * graphAddEndState(Graph *pNFA) {
    Graph *graph = new Graph();
    graph->graphId = ++serialGraphId;
    graph->numOfStates = pNFA->numOfStates + 1;

    list<State *> *stateTable = new list<State *>();
    list<Edge *> *edgeTable = new list<Edge *>();

    for (list<State *>::iterator it = pNFA->pStateTable->begin(); it != pNFA->pStateTable->end(); ++it) {
        State *state = new State();
        state->stateId = (*it)->stateId;
        state->type = UNMATCH;
        state->category = (*it)->category;
        stateTable->push_back(state);
    }
    // 添加最后状态
    State *endState = new State();
    endState->stateId = pNFA->numOfStates;
    endState->type = MATCH;
    stateTable->push_back(endState);
    graph->pStateTable = stateTable;

    for (list<Edge *>::iterator it = pNFA->pEdgeTable->begin(); it != pNFA->pEdgeTable->end(); ++it) {
        Edge *edge = new Edge();
        edge->driverId = (*it)->driverId;
        edge->fromState = (*it)->fromState;
        edge->nextState = (*it)->nextState;
        edge->type = (*it)->type;
        edgeTable->push_back(edge);
    }
    // 添加最后边
    Edge *endEdge = new Edge();
    endEdge->driverId = 0;
    endEdge->fromState = pNFA->numOfStates - 1;
    endEdge->nextState = pNFA->numOfStates;
    endEdge->type = NULL_DT;
    edgeTable->push_back(endEdge);
    graph->pEdgeTable = edgeTable;

    return graph;
}

list<int> *move(Graph *graph, list<int> *states, int driverId) {
    list<int> *nextStates = new list<int>();
    for (list<int>::iterator itState = states->begin(); itState != states->end(); ++itState) {
        for (list<Edge *>::iterator itEdge = graph->pEdgeTable->begin(); itEdge != graph->pEdgeTable->end(); ++itEdge) {
            if (*itState == (*itEdge)->fromState && driverId == (*itEdge)->driverId) {
                // 查重 是否已经添加进入集合 nextStates
                if (find(nextStates->begin(), nextStates->end(), (*itEdge)->nextState) == nextStates->end()) {
                    nextStates->push_back((*itEdge)->nextState);
                }
            }
        }
    }
    return nextStates;
}

list<int> *eClosure(Graph *graph, list<int> *states) {
    list<int> *closureStates = new list<int>();
    queue<int> qStates;
    for (list<int>::iterator it = states->begin(); it != states->end(); ++it) {
        closureStates->push_back(*it);
        qStates.push(*it);
    }
    while (!qStates.empty()) {
        int state = qStates.front();
        qStates.pop();
        for (list<Edge *>::iterator itEdge = graph->pEdgeTable->begin(); itEdge != graph->pEdgeTable->end(); ++itEdge) {
            if (state == (*itEdge)->fromState && (*itEdge)->type == NULL_DT) {
                // 查重
                if (find(closureStates->begin(), closureStates->end(), (*itEdge)->nextState) == closureStates->end()) {
                    closureStates->push_back((*itEdge)->nextState);
                    qStates.push((*itEdge)->nextState);
                }
            }
        }
    }
    return closureStates;
}

// TODO:NFA_to_DFA 函数有问题 需要修改
Graph * NFA_to_DFA(Graph *pNFA) {
    // 已有的状态集合列表
    int stateListId = 0;
    list<list<int> *> *existStates = new list<list<int> *>();
    map<int, list<int> *> *existStatesMap = new map<int, list<int> *>();

    // DFA表
    vector<vector<int>> DFAtable;
    vector<int> *drivers = getAllDriver(pNFA);

    list<int> *zero = new list<int>();
    zero->push_back(0); // 初始状态
    list<int> *stateList = eClosure(pNFA, zero);
    existStates->push_back(stateList);
    existStatesMap->insert(pair<int, list<int> *>(0, stateList));
    free(zero);

    queue<int> qStateList;
    qStateList.push(0);
    int row = 0;
    while (!qStateList.empty()) {
        qStateList.pop();
        vector<int> rowVector;
        int len = drivers->size();
        for (int i = 0; i < len; ++i) {
            list<int> *tmpStateList = move(pNFA, existStatesMap->at(row), drivers->at(i));
            if (tmpStateList->size() == 0) rowVector.push_back(-1);
            else {
                int preId = stateListId;
                int num = checkStateExisted(existStates, tmpStateList, existStatesMap, stateListId);
                // 添加了显得DFA状态
                if (preId + 1 == stateListId) {
                    qStateList.push(num);
                }
                rowVector.push_back(num);
            }
        }
        DFAtable.push_back(rowVector);
        ++row;
    }

    // 表转换成图
    Graph *graph = new Graph();
    graph->graphId = ++serialGraphId;
    graph->numOfStates = DFAtable.size();

    // 添加状态
    list<State *> *stateTable = new list<State *>();
    for (int i = 0; i < graph->numOfStates; ++i) {
        State *tmpState = new State();
        tmpState->stateId = i;
        tmpState->type = getType(pNFA, existStatesMap, i);
        stateTable->push_back(tmpState);
    }
    graph->pStateTable = stateTable;

    // 添加边
    list<Edge *> *edgeTable = new list<Edge *>();
    for (int i = 0; i < graph->numOfStates; ++i) {
        for (int j = 0; j < drivers->size(); ++j) {
            if (DFAtable[i][j] != -1) {
                Edge *tmpEdge = new Edge();
                tmpEdge->fromState = i;
                tmpEdge->nextState = DFAtable[i][j];
                tmpEdge->driverId = drivers->at(j);
                tmpEdge->type = getDriverType(pNFA, tmpEdge->driverId);
                edgeTable->push_back(tmpEdge);
            }
        }
    }
    graph->pEdgeTable = edgeTable;
    return graph;
}

int checkStateExisted(list<list<int> *> *existStates, list<int> *stateList, map<int, list<int> *> *m, int &id) {
    int index = 0;
    for (list<list<int> *>::iterator it = existStates->begin(); it != existStates->end(); ++it) {
        if ( isStateListSame(stateList, (*it)) ) return index;
        ++index;
    }
    existStates->push_back(stateList);
    m->insert(pair<int, list<int> *>(++id, stateList));
    return id;
}

bool isStateListSame(list<int> *stateList1, list<int> *stateList2) {
    if (stateList1->size() != stateList2->size()) return false;
    for (list<int>::iterator it = stateList1->begin(); it != stateList1->end(); ++it) {
        if (find(stateList2->begin(), stateList2->end(), *it) == stateList2->end()) {
            return false;
        }
    }
    return true;
}

// 返回NFA中所有正则式，转移状态的输入
vector<int> *getAllDriver(Graph *pNFA) {
    vector<int> *res = new vector<int>();
    map<int, int> m;
    for (list<Edge *>::iterator it = pNFA->pEdgeTable->begin(); it != pNFA->pEdgeTable->end(); ++it) {
        if (m.count((*it)->driverId) == 0) {
            res->push_back((*it)->driverId);
            m[(*it)->driverId] = 1;
        }
    }
    return res;
}

StateType getType(Graph* pNFA, map<int, list<int> *> *m, int i) {
    list<int> *stateList = m->at(i);
    for (int j = 0; j < stateList->size(); ++j) {
        for (list<State *>::iterator it = pNFA->pStateTable->begin(); it != pNFA->pStateTable->end(); ++it) {
            if ((*it)->stateId == i && (*it)->type == MATCH) {
                return MATCH;
            }
        }
    }
    return UNMATCH;
}

DriverType getDriverType(Graph *pNFA, int id) {
    for (list<Edge *>::iterator it = pNFA->pEdgeTable->begin(); it != pNFA->pEdgeTable->end(); ++it) {
        if ((*it)->driverId == id) {
            return (*it)->type;
        }
    }
    return NULL_DT;
}
