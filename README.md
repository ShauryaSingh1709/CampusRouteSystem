<div align="center">

# Campus Route Finder

## Table of Contents

1. [About the Project](#about-the-project)
2. [Project Structure](#project-structure)
3. [How to Compile and Run](#how-to-compile-and-run)
4. [The Campus Graph](#the-campus-graph)
5. [How the Graph is Stored](#how-the-graph-is-stored)
6. [Algorithms Used](#algorithms-used)
7. [Time and Space Complexity](#time-and-space-complexity)
8. [Code Walkthrough](#code-walkthrough)
9. [Sample Outputs](#sample-outputs)
10. [Test Cases](#test-cases)
11. [Author](#author)

## About the Project

A university campus has many places (Main Gate, Library, Labs and so on) joined by walking paths. This program stores the campus as a **graph**:

- every **location** is a **vertex**
- every **path** between two locations is an **edge**

All paths are two-way and have the same length, so the graph is **undirected and unweighted**. The program can:

1. Show the graph as an adjacency list and as an adjacency matrix
2. Find the shortest route between two places using **BFS**
3. Show every place reachable from a starting point using **DFS**, and list the places that are cut off
4. Check whether a destination can be reached using **DFS**
5. Compare the adjacency list with the adjacency matrix
6. Run built-in test cases with expected and actual outputs

### Assignment tasks covered

| Task | Where it is done |
| --- | --- |
| Graph with at least 8 locations | 10 locations in `names[]` and `buildGraph()` |
| Adjacency list | `adjList[][]` and `degree[]` |
| Adjacency matrix and comparison | `matrix[][]`, menu option 6 |
| BFS minimum-edge route | `bfs()` |
| DFS reachability and traversal | `dfs()` and `runDfs()` |
| User enters source and destination | `findRoute()` and `checkReachable()` |
| Reachable and disconnected locations | `showReachable()` |
| Complexity explanation | Sections below and menu option 6 |
| At least 3 test cases | `runTests()` has 4 |

## Project Structure

```text
campus-route-finder/
├── main.c        full source code
└── README.md     this file
```

## How to Compile and Run

You need a C compiler such as `gcc`.

```bash
gcc main.c -o route
./route
```

On Windows, run `route.exe` instead of `./route`.

The program shows a menu. Type the number of an option and press Enter. Locations are chosen by typing their number from the list shown on screen.

## The Campus Graph

| No. | Location | Directly connected to |
| --- | --- | --- |
| 1 | Main Gate | Library, Parking |
| 2 | Library | Main Gate, Block 1, Cafeteria |
| 3 | Block 1 | Library, Block 2, Lab |
| 4 | Block 2 | Block 1, Auditorium |
| 5 | Cafeteria | Library, Auditorium, Parking |
| 6 | Auditorium | Block 2, Cafeteria |
| 7 | Parking | Main Gate, Cafeteria |
| 8 | Lab | Block 1 |
| 9 | Hostel | Sports Ground |
| 10 | Sports Ground | Hostel |

Hostel and Sports Ground are joined only to each other. They are deliberately cut off from the rest of the campus so that the program has a **disconnected** part to detect and an **unreachable destination** to test.

## How the Graph is Stored

### Adjacency list

For every location we keep a list of its neighbours.

```c
int adjList[MAX][MAX];
int degree[MAX];
```

- `degree[i]` is how many neighbours location `i` has.
- `adjList[i][0]`, `adjList[i][1]` and so on are the numbers of those neighbours.

Example: Main Gate is location 0 and its neighbours are Library (1) and Parking (6), so `degree[0]` is 2, `adjList[0][0]` is 1 and `adjList[0][1]` is 6.

For simplicity the lists are stored in a fixed-size 2D array. A linked list would store exactly the same information.

### Adjacency matrix

A table where `matrix[i][j]` is 1 if locations `i` and `j` are joined by a path and 0 if they are not.

```c
int matrix[MAX][MAX];
```

### Adding a path

`addEdge(a, b)` is called once for every path. Because paths work both ways, it records the connection in both directions in the list and in the matrix.

## Algorithms Used

### BFS (Breadth-First Search) for the shortest route

BFS visits places in rings: first the start, then everything one path away, then everything two paths away, and so on. Because every path has equal length, the first time BFS reaches the destination it has used the **minimum number of paths**.

Steps in `bfs()`:

1. Mark the start as visited and put it in a **queue**.
2. Take the front place out of the queue.
3. For each unvisited neighbour: mark it visited, store the place we came from in `parent[]`, and add it to the queue.
4. Repeat steps 2 and 3 until the destination is reached or the queue is empty.
5. If the destination was never visited, there is no route.
6. Otherwise rebuild the route: start at the destination, follow `parent[]` back to the start, then reverse the list.

The queue is a plain array with a `front` and a `rear` index.

### DFS (Depth-First Search) for reachability

DFS goes as deep as possible along one path, then backs up and tries another. It is written as a **recursive** function: `dfs(u)` marks `u` as visited and calls `dfs(v)` for each unvisited neighbour `v`.

After `dfs(start)` finishes:

- every location with `visited[i] == 1` is **reachable** from the start
- every location with `visited[i] == 0` is **not reachable** (disconnected from it)

The same idea answers "can I get from A to B?": run DFS from A and look at `visited[B]`.

## Time and Space Complexity

`V` is the number of locations and `E` is the number of paths.

| Algorithm | Time | Extra space | Why |
| --- | --- | --- | --- |
| BFS with adjacency list | O(V + E) | O(V) | Each location enters the queue once and each neighbour entry is looked at once. The queue, `visited[]` and `parent[]` each need V spaces |
| DFS with adjacency list | O(V + E) | O(V) | Each location is visited once and each neighbour entry is looked at once. The recursion can go V levels deep |
| Rebuilding the route | O(V) | O(V) | Follows `parent[]` from the destination back to the start |
| BFS or DFS with adjacency matrix | O(V^2) | O(V) | Finding the neighbours of a location means scanning a whole row of V cells |

### Adjacency list compared with adjacency matrix

| | Adjacency list | Adjacency matrix |
| --- | --- | --- |
| Memory | O(V + E) | O(V^2) |
| List all neighbours of a location | O(degree of that location) | O(V) |
| Check if two locations are directly joined | O(degree of that location) | O(1) |
| Full BFS or DFS | O(V + E) | O(V^2) |
| Best for | Sparse graphs (few paths) | Dense graphs, or when quick "is there a path" checks matter |

For this campus V = 10 and E = 10. The list needs about V + 2E = 30 values while the matrix needs V x V = 100. A real campus has far fewer paths than possible pairs of places, so the adjacency list is the better choice. Menu option 6 prints this comparison.

## Code Walkthrough

| Function | What it does |
| --- | --- |
| `addEdge(a, b)` | Joins two locations in both the adjacency list and the matrix |
| `buildGraph()` | Calls `addEdge()` for all 10 campus paths |
| `clearVisited()` | Sets every `visited[]` value back to 0 before a new search |
| `readNumber()` | Asks for a number between a low and high value and keeps asking until the input is valid. Letters or out-of-range numbers do not crash the program |
| `printLocations()` | Prints the numbered list of locations |
| `printAdjacencyList()` | Prints each location with its neighbours |
| `printAdjacencyMatrix()` | Prints the 10 x 10 table of 0s and 1s |
| `bfs()` | Finds the shortest route and returns how many locations are on it, or 0 if there is no route |
| `dfs()` | Recursively visits every location reachable from a place |
| `runDfs()` | Resets the arrays and starts a DFS from a chosen location |
| `printRoute()` | Prints a route as `A -> B -> C` |
| `findRoute()` | Menu option 3: asks for source and destination, runs BFS, prints the route |
| `showReachable()` | Menu option 4: runs DFS and prints reachable and unreachable locations |
| `checkReachable()` | Menu option 5: runs DFS and says whether the destination can be reached |
| `compareRepresentations()` | Menu option 6: prints the list vs matrix comparison |
| `runTests()` | Menu option 7: runs 4 test cases and prints expected and actual results |
| `main()` | Builds the graph and shows the menu until the user exits |

## Sample Outputs

These are real outputs from the compiled program. The number after a prompt is what the user typed. In these samples the menu and the location list are printed only the first time to keep them short, but the real program prints the menu again after every action.

### 1. Graph input: adjacency list and adjacency matrix

```text
Campus Route Finder
1. Show campus locations and adjacency list
2. Show adjacency matrix
3. Find shortest route using BFS
4. Show reachable and disconnected locations using DFS
5. Check if a destination is reachable using DFS
6. Compare adjacency list and adjacency matrix
7. Run test cases
8. Exit
Enter your choice: 1

Campus locations
1. Main Gate
2. Library
3. Block 1
4. Block 2
5. Cafeteria
6. Auditorium
7. Parking
8. Lab
9. Hostel
10. Sports Ground

Adjacency list
Main Gate: Library, Parking
Library: Main Gate, Block 1, Cafeteria
Block 1: Library, Block 2, Lab
Block 2: Block 1, Auditorium
Cafeteria: Library, Auditorium, Parking
Auditorium: Block 2, Cafeteria
Parking: Main Gate, Cafeteria
Lab: Block 1
Hostel: Sports Ground
Sports Ground: Hostel
Enter your choice: 2

Adjacency matrix
      1  2  3  4  5  6  7  8  9 10
  1   0  1  0  0  0  0  1  0  0  0
  2   1  0  1  0  1  0  0  0  0  0
  3   0  1  0  1  0  0  0  1  0  0
  4   0  0  1  0  0  1  0  0  0  0
  5   0  1  0  0  0  1  1  0  0  0
  6   0  0  0  1  1  0  0  0  0  0
  7   1  0  0  0  1  0  0  0  0  0
  8   0  0  1  0  0  0  0  0  0  0
  9   0  0  0  0  0  0  0  0  0  1
 10   0  0  0  0  0  0  0  0  1  0
Enter your choice: 8

Thank you for using the Campus Route Finder.
```

### 2. BFS routes

A direct route, two multi-stop routes, and a destination with no route.

```text
Campus Route Finder
1. Show campus locations and adjacency list
2. Show adjacency matrix
3. Find shortest route using BFS
4. Show reachable and disconnected locations using DFS
5. Check if a destination is reachable using DFS
6. Compare adjacency list and adjacency matrix
7. Run test cases
8. Exit
Enter your choice: 3

Campus locations
1. Main Gate
2. Library
3. Block 1
4. Block 2
5. Cafeteria
6. Auditorium
7. Parking
8. Lab
9. Hostel
10. Sports Ground

Enter the number of the starting location: 1
Enter the number of the destination: 2

Shortest route found by BFS:
Main Gate -> Library
Number of paths used: 1
Enter your choice: 3

Enter the number of the starting location: 1
Enter the number of the destination: 6

Shortest route found by BFS:
Main Gate -> Library -> Cafeteria -> Auditorium
Number of paths used: 3
Enter your choice: 3

Enter the number of the starting location: 8
Enter the number of the destination: 6

Shortest route found by BFS:
Lab -> Block 1 -> Block 2 -> Auditorium
Number of paths used: 3
Enter your choice: 3

Enter the number of the starting location: 1
Enter the number of the destination: 9

No route exists from Main Gate to Hostel.
Enter your choice: 8

Thank you for using the Campus Route Finder.
```

### 3. DFS traversal, disconnected locations and reachability

```text
Campus Route Finder
1. Show campus locations and adjacency list
2. Show adjacency matrix
3. Find shortest route using BFS
4. Show reachable and disconnected locations using DFS
5. Check if a destination is reachable using DFS
6. Compare adjacency list and adjacency matrix
7. Run test cases
8. Exit
Enter your choice: 4

Campus locations
1. Main Gate
2. Library
3. Block 1
4. Block 2
5. Cafeteria
6. Auditorium
7. Parking
8. Lab
9. Hostel
10. Sports Ground

Enter the number of the starting location: 1

Locations reachable from Main Gate in DFS order:
1. Main Gate
2. Library
3. Block 1
4. Block 2
5. Auditorium
6. Cafeteria
7. Parking
8. Lab

Locations not reachable from Main Gate:
Hostel
Sports Ground
Enter your choice: 4

Enter the number of the starting location: 9

Locations reachable from Hostel in DFS order:
1. Hostel
2. Sports Ground

Locations not reachable from Hostel:
Main Gate
Library
Block 1
Block 2
Cafeteria
Auditorium
Parking
Lab
Enter your choice: 5

Enter the number of the starting location: 1
Enter the number of the destination: 8

Lab can be reached from Main Gate.
Enter your choice: 5

Enter the number of the starting location: 1
Enter the number of the destination: 9

Hostel cannot be reached from Main Gate.
Enter your choice: 8

Thank you for using the Campus Route Finder.
```

### 4. Comparison of the two representations

```text
Campus Route Finder
1. Show campus locations and adjacency list
2. Show adjacency matrix
3. Find shortest route using BFS
4. Show reachable and disconnected locations using DFS
5. Check if a destination is reachable using DFS
6. Compare adjacency list and adjacency matrix
7. Run test cases
8. Exit
Enter your choice: 6

Comparison of adjacency list and adjacency matrix
Locations (V) = 10, paths (E) = 10

Adjacency list
Memory used: about V + 2E = 30 values
Space complexity: O(V + E)
Finding all neighbours of a location: O(degree)
Checking if two locations are directly connected: O(degree)
BFS or DFS over the whole graph: O(V + E)

Adjacency matrix
Memory used: V x V = 100 values
Space complexity: O(V^2)
Finding all neighbours of a location: O(V)
Checking if two locations are directly connected: O(1)
BFS or DFS over the whole graph: O(V^2)

A campus has few paths compared to the number of possible pairs,
so the adjacency list saves memory and is faster for BFS and DFS.
Enter your choice: 8

Thank you for using the Campus Route Finder.
```

### 5. Invalid input

A menu number that is too small or too big, letters instead of numbers, and a location number that does not exist are all rejected and asked again.

```text
Campus Route Finder
1. Show campus locations and adjacency list
2. Show adjacency matrix
3. Find shortest route using BFS
4. Show reachable and disconnected locations using DFS
5. Check if a destination is reachable using DFS
6. Compare adjacency list and adjacency matrix
7. Run test cases
8. Exit
Enter your choice: 0
Please enter a number between 1 and 8.
Enter your choice: abc
That is not a number. Please try again.
Enter your choice: 12
Please enter a number between 1 and 8.
Enter your choice: 3

Campus locations
1. Main Gate
2. Library
3. Block 1
4. Block 2
5. Cafeteria
6. Auditorium
7. Parking
8. Lab
9. Hostel
10. Sports Ground

Enter the number of the starting location: 11
Please enter a number between 1 and 10.

Enter the number of the starting location: abc
That is not a number. Please try again.

Enter the number of the starting location: 2
Enter the number of the destination: 8

Shortest route found by BFS:
Library -> Block 1 -> Lab
Number of paths used: 2
Enter your choice: 8

Thank you for using the Campus Route Finder.
```

## Test Cases

| No. | Type | From | To | Expected route | Paths | Actual | Result |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 1 | Direct route | Main Gate | Library | Main Gate -> Library | 1 | Same | PASS |
| 2 | Multi-stop route | Main Gate | Auditorium | Main Gate -> Library -> Cafeteria -> Auditorium | 3 | Same | PASS |
| 3 | Multi-stop route | Lab | Auditorium | Lab -> Block 1 -> Block 2 -> Auditorium | 3 | Same | PASS |
| 4 | Unreachable | Main Gate | Hostel | No route | none | No route | PASS |

Each test also runs DFS and checks that its reachable or not-reachable answer agrees with BFS.

Output of menu option 7:

```text
Campus Route Finder
1. Show campus locations and adjacency list
2. Show adjacency matrix
3. Find shortest route using BFS
4. Show reachable and disconnected locations using DFS
5. Check if a destination is reachable using DFS
6. Compare adjacency list and adjacency matrix
7. Run test cases
8. Exit
Enter your choice: 7

Running 4 test cases

Test 1 (Direct route): Main Gate to Library
Expected: Main Gate -> Library
Actual:   Main Gate -> Library
Paths used: expected 1, actual 1
DFS reachability check agrees with BFS.
Result: PASS

Test 2 (Multi-stop route): Main Gate to Auditorium
Expected: Main Gate -> Library -> Cafeteria -> Auditorium
Actual:   Main Gate -> Library -> Cafeteria -> Auditorium
Paths used: expected 3, actual 3
DFS reachability check agrees with BFS.
Result: PASS

Test 3 (Multi-stop route): Lab to Auditorium
Expected: Lab -> Block 1 -> Block 2 -> Auditorium
Actual:   Lab -> Block 1 -> Block 2 -> Auditorium
Paths used: expected 3, actual 3
DFS reachability check agrees with BFS.
Result: PASS

Test 4 (Unreachable): Main Gate to Hostel
Expected: No route
Actual:   No route
Paths used: expected -1, actual -1
DFS reachability check agrees with BFS.
Result: PASS

4 out of 4 test cases passed.
Enter your choice: 8

Thank you for using the Campus Route Finder.
```

## Author

[**Shaurya**](https://github.com/Shaurya)
