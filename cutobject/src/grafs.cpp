// поиск в глубину
// v - vertex вершины

void dfs(vector<vector<int>>& graph, int v, vector<int>& visited) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (!visited[to])
			dfs(graph, to, visited);
	}	
}
//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8
//
// 7 5
// возможно ли пройти от вершины 7 к 5
void dfs(vector<vector<int>>& graph, int v, vector<int>& visited) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (!visited[to])
			dfs(graph, to, visited);
	}	
}
int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a); // неориентированный граф
	}
	int start, finish;
	cin >> start >> finish;
	
	vector<int> visited(vertexCount);
	dfs(graph, start, visited);
	if(visited[finish])
		cout << "Yes";
	else
		cout << "No";
}


//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8

// связанный ли граф
void dfs(vector<vector<int>>& graph, int v, vector<int>& visited) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (!visited[to])
			dfs(graph, to, visited);
	}	
}
int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a); // неориентированный граф
	}
	vector<int> visited(vertexCount);
	dfs(graph, 0, visited);
	
	if(find(visited.begin(), visited(end), 0) == visited.end())
		cout << "Yes";
	else
		cout << "No";
}


//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8

// подсчёт компонент связанности
void dfs(vector<vector<int>>& graph, int v, vector<int>& visited) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (!visited[to])
			dfs(graph, to, visited);
	}	
}
int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a); // неориентированный граф
	}
	vector<int> visited(vertexCount);
	int componentCount = 0;
	for(int v = 0; v < vertexCount; v++) {
		if (!visited[v]) {
			componentCount++;
			dfs(graph, v, visited);
		}
	}
	cout << componentCount;
}



//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8

// раскраска компонент связанности
void dfs(vector<vector<int>>& graph, int v, vector<int>& visited, int component) {
	visited[v] = component;
	for (int to : graph[v]) {
		if (!visited[to])
			dfs(graph, to, visited, component);
	}	
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a); // неориентированный граф
	}
	vector<int> visited(vertexCount);
	int componentCount = 0;
	for(int v = 0; v < vertexCount; v++) {
		if (!visited[v]) {
			componentCount++;
			dfs(graph, v, componentCount);
		}
	}
	for (int color : visited)
		cout << color << " ";
}



//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8

// поиск циклов, в ориентированном графе

void dfs(vector<vector<int>>& graph, int v, vector<int>& visited, bool& hasCycle) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (!visited[to]) {
			dfs(graph, to, visited, hasCycle);
		} else if (visited[to] == 1) {
			// в графе есть цикл
			hasCycle = true;
		}
	}	
	visited[v] = 2;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
	}
	vector<int> visited(vertexCount);
	bool hasCycle = false;
	
	for(int v = 0; v < vertexCount; v++) {
		if (!visited[v]) {
			dfs(graph, v, visited, hasCycle);
		}
	}
	
	if(hasCycle)
		cout << "Yes";
	else
		cout << "No";
}



//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8

// восстановление цикла в ореинтированном графе
vector<int> getCycle(vector<int>& from, int lastVertex) {
	vector<int> cycle = {lastVertex};
	for (int v = from[lastVertex]; v != lastVertex; v = from[v])
		cycle.push_back(v);
	reverse(cycle.begin(), cycle.end());
	return cycle;
}

void dfs(vector<vector<int>>& graph, int v, vector<int>& visited, 
		vector<int>& from, vector<int>& cycle) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (!visited[to]) {
			from[to] = v;
			dfs(graph, to, visited, from);
			if (!cycle.empty())
				return;
		} else if (visited[to] == 1) {
			// в графе есть цикл
			from[to] = v;
			cycle = getCycle(from, to);
			return;
		}
	}
	visited[v] = 2;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
	}
	
	vector<int> visited(vertexCount);
	vector<int> from(vertexCount-1);
	vector<int> cycle;
	
	for(int v = 0; cycle.empty() && v < vertexCount; v++) {
		if (!visited[v]) {
			dfs(graph, v, visited, from, cycle);
		}
	}
	
	if(!cycle.empty()) {
		cout << "Yes\n";
		for (int v : cycle)
			cout << v + 1 << " ";
	} else {
		cout << "No";
	}
}




//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8

// поиск циклов, в Неориентированном графе

void dfs(vector<vector<int>>& graph, int v, vector<int>& visited,
		int from, bool& hasCycle) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (to == from) {
			continue;
		} else if (!visited[to]) {
			dfs(graph, to, visited, v, hasCycle);
		} else if (visited[to] == 1) {
			// в графе есть цикл
			hasCycle = true;
		}
	}
	visited[v] = 2;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a); // неориентированный граф
	}
	vector<int> visited(vertexCount);
	bool hasCycle = false;
	
	for(int v = 0; v < vertexCount; v++) {
		if (!visited[v]) {
			dfs(graph, v, visited, -1, hasCycle);
		}
	}
	
	if(hasCycle)
		cout << "Yes";
	else
		cout << "No";
}


//---------------------------------------------------------------------//
// input.txt
// 8 8
// 1 5
// 5 2
// 3 2
// 6 3
// 5 6
// 5 7
// 8 6
// 7 8

// восстановление цикла в Неореинтированном графе
vector<int> getCycle(vector<int>& from, int lastVertex) {
	vector<int> cycle = {lastVertex};
	for (int v = from[lastVertex]; v != lastVertex; v = from[v])
		cycle.push_back(v);
	reverse(cycle.begin(), cycle.end());
	return cycle;
}

void dfs(vector<vector<int>>& graph, int v, vector<int>& visited, 
		vector<int>& from, vector<int>& cycle) {
	visited[v] = 1;
	for (int to : graph[v]) {
		if (to == from[v]) {
			continue
		} else if (!visited[to]) {
			from[to] = v;
			dfs(graph, to, visited, from);
			if (!cycle.empty())
				return;
		} else if (visited[to] == 1) {
			// в графе есть цикл
			from[to] = v;
			cycle = getCycle(from, to);
			return;
		}
	}
	visited[v] = 2;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "r", stdout);
	int vertexCount, edgeCount;
	cin >> vertexCount >> edgeCount;
	vectro<vector<int>> graph(vertexCount);
	for (int i = 0; i < edgeCount; i++) 
	{
		int a, b;
		cin >> a >> b;
		a--;
		b--;
		graph[a].push_back(b);
		graph[b].push_back(a); // неориентированный граф
	}
	
	vector<int> visited(vertexCount);
	vector<int> from(vertexCount-1);
	vector<int> cycle;
	bool hasCycle = false;
	
	for(int v = 0; cycle.empty() && v < vertexCount; v++) {
		if (!visited[v]) {
			dfs(graph, v, visited, from, cycle);
		}
	}
	
	if(!cycle.empty()) {
		cout << "Yes\n";
		for (int v : cycle)
			cout << v + 1 << " ";
	} else {
		cout << "No";
	}
}
