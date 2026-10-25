#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 10

char names[MAX][20] = {
    "Main Gate", "Library", "Block 1", "Block 2", "Cafeteria",
    "Auditorium", "Parking", "Lab", "Hostel", "Sports Ground"
};

int total = 10;
int edgeCount = 0;

int adjList[MAX][MAX];
int degree[MAX];
int matrix[MAX][MAX];

int visited[MAX];
int order[MAX];
int orderCount;

void addEdge(int a, int b)
{
    adjList[a][degree[a]] = b;
    degree[a]++;
    adjList[b][degree[b]] = a;
    degree[b]++;

    matrix[a][b] = 1;
    matrix[b][a] = 1;

    edgeCount++;
}

void buildGraph()
{
    addEdge(0, 1);
    addEdge(0, 6);
    addEdge(1, 2);
    addEdge(1, 4);
    addEdge(2, 3);
    addEdge(2, 7);
    addEdge(3, 5);
    addEdge(4, 5);
    addEdge(4, 6);
    addEdge(8, 9);
}

void clearVisited()
{
    int i;
    for (i = 0; i < total; i++)
        visited[i] = 0;
}

void clearLine()
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF)
        ;
}

int readNumber(char prompt[], int low, int high)
{
    int value;
    int result;

    while (1) {
        printf("%s", prompt);
        result = scanf("%d", &value);

        if (result == EOF) {
            printf("\nInput ended. Exiting.\n");
            exit(0);
        }

        if (result != 1)
            printf("That is not a number. Please try again.\n");
        else if (value < low || value > high)
            printf("Please enter a number between %d and %d.\n", low, high);
        else {
            clearLine();
            return value;
        }

        clearLine();
    }
}

void printLocations()
{
    int i;
    printf("\nCampus locations\n");
    for (i = 0; i < total; i++)
        printf("%d. %s\n", i + 1, names[i]);
}

void printAdjacencyList()
{
    int i, j;
    printf("\nAdjacency list\n");
    for (i = 0; i < total; i++) {
        printf("%s: ", names[i]);
        if (degree[i] == 0)
            printf("no paths");
        for (j = 0; j < degree[i]; j++) {
            printf("%s", names[adjList[i][j]]);
            if (j < degree[i] - 1)
                printf(", ");
        }
        printf("\n");
    }
}

void printAdjacencyMatrix()
{
    int i, j;
    printf("\nAdjacency matrix\n");
    printf("    ");
    for (i = 0; i < total; i++)
        printf("%3d", i + 1);
    printf("\n");
    for (i = 0; i < total; i++) {
        printf("%3d ", i + 1);
        for (j = 0; j < total; j++)
            printf("%3d", matrix[i][j]);
        printf("\n");
    }
}

int bfs(int src, int dst, int route[])
{
    int queue[MAX];
    int parent[MAX];
    int temp[MAX];
    int front = 0, rear = 0;
    int i, u, v, node, count;

    clearVisited();
    for (i = 0; i < total; i++)
        parent[i] = -1;

    visited[src] = 1;
    queue[rear] = src;
    rear++;

    while (front < rear) {
        u = queue[front];
        front++;

        if (u == dst)
            break;

        for (i = 0; i < degree[u]; i++) {
            v = adjList[u][i];
            if (visited[v] == 0) {
                visited[v] = 1;
                parent[v] = u;
                queue[rear] = v;
                rear++;
            }
        }
    }

    if (visited[dst] == 0)
        return 0;

    count = 0;
    node = dst;
    while (node != -1) {
        temp[count] = node;
        count++;
        node = parent[node];
    }

    for (i = 0; i < count; i++)
        route[i] = temp[count - 1 - i];

    return count;
}

void dfs(int u)
{
    int i, v;

    visited[u] = 1;
    order[orderCount] = u;
    orderCount++;

    for (i = 0; i < degree[u]; i++) {
        v = adjList[u][i];
        if (visited[v] == 0)
            dfs(v);
    }
}

void runDfs(int start)
{
    clearVisited();
    orderCount = 0;
    dfs(start);
}

void printRoute(int route[], int count)
{
    int i;
    for (i = 0; i < count; i++) {
        printf("%s", names[route[i]]);
        if (i < count - 1)
            printf(" -> ");
    }
    printf("\n");
}

void findRoute()
{
    int src, dst, count;
    int route[MAX];

    printLocations();
    src = readNumber("\nEnter the number of the starting location: ", 1, total) - 1;
    dst = readNumber("Enter the number of the destination: ", 1, total) - 1;

    count = bfs(src, dst, route);

    if (count == 0) {
        printf("\nNo route exists from %s to %s.\n", names[src], names[dst]);
        return;
    }

    printf("\nShortest route found by BFS:\n");
    printRoute(route, count);
    printf("Number of paths used: %d\n", count - 1);
}

void showReachable()
{
    int start, i, found;

    printLocations();
    start = readNumber("\nEnter the number of the starting location: ", 1, total) - 1;

    runDfs(start);

    printf("\nLocations reachable from %s in DFS order:\n", names[start]);
    for (i = 0; i < orderCount; i++)
        printf("%d. %s\n", i + 1, names[order[i]]);

    printf("\nLocations not reachable from %s:\n", names[start]);
    found = 0;
    for (i = 0; i < total; i++) {
        if (visited[i] == 0) {
            printf("%s\n", names[i]);
            found = 1;
        }
    }
    if (found == 0)
        printf("None. Every location is connected to this one.\n");
}

void checkReachable()
{
    int src, dst;

    printLocations();
    src = readNumber("\nEnter the number of the starting location: ", 1, total) - 1;
    dst = readNumber("Enter the number of the destination: ", 1, total) - 1;

    runDfs(src);

    if (visited[dst] == 1)
        printf("\n%s can be reached from %s.\n", names[dst], names[src]);
    else
        printf("\n%s cannot be reached from %s.\n", names[dst], names[src]);
}

void compareRepresentations()
{
    printf("\nComparison of adjacency list and adjacency matrix\n");
    printf("Locations (V) = %d, paths (E) = %d\n\n", total, edgeCount);

    printf("Adjacency list\n");
    printf("Memory used: about V + 2E = %d values\n", total + 2 * edgeCount);
    printf("Space complexity: O(V + E)\n");
    printf("Finding all neighbours of a location: O(degree)\n");
    printf("Checking if two locations are directly connected: O(degree)\n");
    printf("BFS or DFS over the whole graph: O(V + E)\n\n");

    printf("Adjacency matrix\n");
    printf("Memory used: V x V = %d values\n", total * total);
    printf("Space complexity: O(V^2)\n");
    printf("Finding all neighbours of a location: O(V)\n");
    printf("Checking if two locations are directly connected: O(1)\n");
    printf("BFS or DFS over the whole graph: O(V^2)\n\n");

    printf("A campus has few paths compared to the number of possible pairs,\n");
    printf("so the adjacency list saves memory and is faster for BFS and DFS.\n");
}

void runTests()
{
    int srcs[4] = {0, 0, 7, 0};
    int dsts[4] = {1, 5, 5, 8};
    int expectedEdges[4] = {1, 3, 3, -1};
    char expectedRoute[4][100] = {
        "Main Gate -> Library",
        "Main Gate -> Library -> Cafeteria -> Auditorium",
        "Lab -> Block 1 -> Block 2 -> Auditorium",
        "No route"
    };
    char testType[4][20] = {"Direct route", "Multi-stop route", "Multi-stop route", "Unreachable"};

    int route[MAX];
    char actual[100];
    int t, i, count, edges, bfsOk, dfsOk, passed;

    passed = 0;
    printf("\nRunning 4 test cases\n");

    for (t = 0; t < 4; t++) {
        count = bfs(srcs[t], dsts[t], route);

        actual[0] = '\0';
        if (count == 0) {
            strcpy(actual, "No route");
            edges = -1;
        } else {
            for (i = 0; i < count; i++) {
                strcat(actual, names[route[i]]);
                if (i < count - 1)
                    strcat(actual, " -> ");
            }
            edges = count - 1;
        }

        runDfs(srcs[t]);
        dfsOk = (visited[dsts[t]] == 1) == (count > 0);
        bfsOk = (strcmp(actual, expectedRoute[t]) == 0 && edges == expectedEdges[t]);

        printf("\nTest %d (%s): %s to %s\n", t + 1, testType[t], names[srcs[t]], names[dsts[t]]);
        printf("Expected: %s\n", expectedRoute[t]);
        printf("Actual:   %s\n", actual);
        printf("Paths used: expected %d, actual %d\n", expectedEdges[t], edges);
        if (dfsOk)
            printf("DFS reachability check agrees with BFS.\n");
        else
            printf("DFS reachability check does not agree with BFS.\n");

        if (bfsOk && dfsOk) {
            printf("Result: PASS\n");
            passed++;
        } else {
            printf("Result: FAIL\n");
        }
    }

    printf("\n%d out of 4 test cases passed.\n", passed);
}

void showMenu()
{
    printf("\nCampus Route Finder\n");
    printf("1. Show campus locations and adjacency list\n");
    printf("2. Show adjacency matrix\n");
    printf("3. Find shortest route using BFS\n");
    printf("4. Show reachable and disconnected locations using DFS\n");
    printf("5. Check if a destination is reachable using DFS\n");
    printf("6. Compare adjacency list and adjacency matrix\n");
    printf("7. Run test cases\n");
    printf("8. Exit\n");
}

int main()
{
    int choice;

    buildGraph();

    do {
        showMenu();
        choice = readNumber("Enter your choice: ", 1, 8);

        if (choice == 1) {
            printLocations();
            printAdjacencyList();
        } else if (choice == 2) {
            printAdjacencyMatrix();
        } else if (choice == 3) {
            findRoute();
        } else if (choice == 4) {
            showReachable();
        } else if (choice == 5) {
            checkReachable();
        } else if (choice == 6) {
            compareRepresentations();
        } else if (choice == 7) {
            runTests();
        }
    } while (choice != 8);

    printf("\nThank you for using the Campus Route Finder.\n");
    return 0;
}
