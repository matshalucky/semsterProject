#include <iostream>
#include <fstream>
#include "utils.h"
#include "parser.h"
#include "searchable.h"
#include "searcher.h"
#include "queue"
#include "server.h"
#include "clientHandler.h"
#include "cacheHandler.h"
#include "solver.h"
void changeFile(string & file_name);
#define NUM_OF_ARGV 2
#define FILE_NAME_INDEX 1
int getPathCost(Solution<std::pair<int,int>>* sol);
int main(int argc, char *argv[]) {

  //auto cmp = [](int left, int right) { return left  > right ;};
  //std::priority_queue<int, std::vector<int>, decltype(cmp)> q3(cmp);

  //for(int n : {1,8,5,6,3,4,0,9,7,2}) {
   // q3.push(n);
  //}

  //while (!q3.empty()) {
   // std::cout << q3.top() << " ";
   // q3.pop();
  //}
  //std::cout << '\n';
typedef std::pair<int,int> pair;
  server_side::Server* s = new MyParallelServer();
  Searcher<pair,pair> * searcher = new Astar<pair,pair>();
  Solver<Searchable<pair>*,Solution<pair>*> *solver = new SolverSearcher<Searchable<pair>*,Solution<pair>* ,pair,pair>(searcher);
  CacheManager<std::string ,std::string>* cacheManager = new FileCacheManager();
  ClientHandler *c = new MyClientHandler<Solution<pair>* ,Searchable<pair>*,std::string,std::string >(cacheManager,solver);
  s->open(5401,c);
  //string string_1 = "1,2\n 4,5\n 0,0\n 1,1\n end \n";
  //c->handelClient(1,string_1);
  //MatrixProblem* m =c->createProblem(string_1);
  string file_name= "matrix.txt";
  //changeFile(file_name);
  vector<MatrixProblem*> mp = createMatrixProblem(file_name);
  //Searcher<std::pair<int,int>,std::pair<int,int>>* best_first = new BestFirstSearch<std::pair<int,int>,std::pair<int,int>>;
  //Searcher<std::pair<int,int>,std::pair<int,int>>* dfs = new DFS<std::pair<int,int>,std::pair<int,int>>;
  //Searcher<std::pair<int,int>,std::pair<int,int>>* bfs = new BFS<std::pair<int,int>,std::pair<int,int>>;
  //Searcher<std::pair<int,int>,std::pair<int,int>>* astar = new Astar<std::pair<int,int>,std::pair<int,int>>;
  //std::vector<Solution<std::pair<int,int>>*> solutions;

//  for(auto problem : mp) {
  //  solutions.push_back(best_first->search(problem));
 // }
  // for(auto problem : mp) {
   //   solutions.push_back(astar->search(problem));
   //  std::cout<<astar->getNumberOfNodesEvaluated()<<std::endl;
    // }

 //    for(auto sol : solutions) {
     //  int a = getPathCost(sol);
   //    std::cout<<a<<std::endl;
    // }



  //Solution<std::pair<int,int>> sol = s->search((mp[0]));
  //cout<<"the number of nodes :" + to_string(s->getNumberOfNodesEvaluated())<<endl;
  //vector<State<std::pair<int,int>>*> v = sol .getSolution();
  //Ser
 // state_pair intial_state = mp[0]->getInitialState();
  //vector<state_pair> all_state = mp[0]->getAllPossibleStates(intial_state);
  //for(MatrixProblem* problem : mp) {
    //  delete(problem);
  //}
  //State<string> a("b");
  //State<string> b("b");
  //State<string> c("c");
  //cout<< to_string((a == b)) + " excpect 1" << endl;
  //cout<< to_string((b==c)) + " excpect 0" << endl;
 // string to_split = "3,4";
 // string del =",";
 // vector<string> result = split(del,to_split);

//     if(argc != NUM_OF_ARGV) {
//    cout<< "invalid num of CLI"<<endl;
//  }
//  string file_name = argv[FILE_NAME_INDEX];
//  Lexer lex(file_name);
//  vector<string> string_command =lex.get_string_command();
//  Parser p(string_command);
//  try {
 //   p.run();
 // } catch (...) {
 //  cout<< "failed"<<endl;
 // }
  return 0;
}
int getPathCost(Solution<std::pair<int,int>>* sol) {
  int total_cost=0;
  auto solutions = sol->getSolution();
  for(auto solution : solutions){
    total_cost +=solution->getCost();
  }
  return total_cost;
}

void changeFile(string & file_name) {
  ifstream myFile(file_name);
  ofstream fileWrite("matrix.txt");
  if(!(fileWrite.is_open())) {
    throw "could not open file";
  }
  if(!(myFile.is_open())) {
    throw "could not open file";
  }
  string line;
  string copy;
  size_t size ;
  size_t change;
  while (getline(myFile,line)) {
    size = line.length();
    copy=line;
    change =0;
    for(size_t i=0 ; i< size;i++) {

      if(line[i] == '-') {
        copy.insert(i +change," ");
        change++;
      } else if(i != size-2 && line[i]=='1' && line[i+1] == '0'){
        copy.insert(i+ change," ");
        change++;
      }
    }
    copy.append("\n");
    fileWrite<< copy;
  }
  myFile.close();
  fileWrite.close();
}