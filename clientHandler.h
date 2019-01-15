//
// Created by ron on 13/01/19.
//

#ifndef SEMSTERPROJECT_CLIENTHANDLER_H
#define SEMSTERPROJECT_CLIENTHANDLER_H
#include <strings.h>
#include <sys/socket.h>
#include "cacheHandler.h"
#include "solver.h"
//#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#define BUFFER_SIZE 1024
#define END_OF_LINE_UNIX '\n'
#define END_OF_LINE_MAC '\r'
#define NUM_OF_END_LINE 4
#define INITIAL_STATE_INDEX 2
#define STATE_FIRST 0
#define STATE_SECOND 1
class ClientHandler {
public:
    virtual void handelClient(int sock_number) = 0;
    virtual MatrixProblem * createProblem(std::string& user_input )=0;
    virtual bool handels()=0;
};






template <class Solution ,class Problem >
    class MyClientHandler : public ClientHandler {
     private:
      CacheManager<Solution,Problem>* cache_manager;
      Solver<Problem,Solution>* solver;
      bool still_handle=true;
     public:
        MyClientHandler(CacheManager<Solution,Problem> *cache_manager ,Solver<Problem,Solution>* solver) {
          this->cache_manager = cache_manager;
          this->solver = solver;
      }
      // delete everthing before /n in the end
      std::vector<double> parseStringToInt(std::string& input ) {
        int i=0;
        std::string temp_num;
        std::vector<double > values;
        while (input[i] != END_OF_LINE_MAC &&
            input[i] != END_OF_LINE_UNIX) {
          while (input[i] != ',' && input[i] != END_OF_LINE_MAC && input[i] != END_OF_LINE_UNIX  ){
            if(input[i]!= ' ') {
              temp_num+= input[i];
            }
            ++i;
          }
          if(input[i] != END_OF_LINE_MAC && input[i] != END_OF_LINE_UNIX ) {
            i++;
          }
          values.push_back(stol(temp_num));
          temp_num.clear();
        }
        input.erase(0,i+1);
        return values;
      }
      MatrixProblem * createProblem(std::string& user_input ) {
        std::pair<int,int> initial;
        std::pair<int,int> goal;
        std::string input_end;
        state_pair* node;
        std::vector<std::vector<state_pair*>> matrix_graph;
        int  i = user_input.length()-1;
        int num_of_line_end=0;
        while (num_of_line_end != NUM_OF_END_LINE) {
          if(user_input[i] == END_OF_LINE_MAC ||
              user_input[i] == END_OF_LINE_UNIX) {
            ++num_of_line_end;
          }
          --i;
        }
        input_end = user_input.substr(i+INITIAL_STATE_INDEX,user_input.length()-i );
        user_input.erase(i+INITIAL_STATE_INDEX,user_input.length()-i);
        std::vector<double> values = parseStringToInt(input_end);
        initial.first= (int) values[STATE_FIRST];
        initial.second = (int)values[STATE_SECOND];
        values = parseStringToInt(input_end);
        goal.first = (int) values[STATE_FIRST];
        goal.second= (int) values[STATE_SECOND];
        input_end.erase();
        int row =0;
        while (!user_input.empty()) {
          values =  parseStringToInt(user_input);
          std::vector<state_pair*> state_row;
          for(int col=0 ; col<values.size();col++) {
            std::pair<int,int> indexes(row,col);
            node = new state_pair(indexes);
            node->setCost(values[col]);
            state_row.push_back(node);
          }
          matrix_graph.push_back(state_row);
          ++row;
        }
        return new MatrixProblem(initial,goal,row,(int)matrix_graph[0].size(),matrix_graph);
        }
      std::string readClient(int sock_number) {
          //std::cout<<"in socket :" + std::to_string(*n)<< std::endl;
          char buffer[BUFFER_SIZE];
          int n;
          std::string input;
          std::string curr_input;
          bool get_input=true;
          while (get_input) {
              bzero(buffer, BUFFER_SIZE);
              n = read(sock_number, buffer, BUFFER_SIZE - 1);
              if (n < 0) {
                  perror("ERROR reading from socket");
                  exit(1);
              }
              curr_input = buffer;
              if (curr_input.find("end") != std::string::npos) {
                  get_input = false;
              }
              input += curr_input;
          }
          return input;
      }
      bool handels() {
          return still_handle;
        }
      void handelClient(int sock_number){
          //std::cout<<"in handle c";
          // remmber to check if problem exists
          std::string input = this->readClient(sock_number);
          MatrixProblem *m = this->createProblem(input);
          Solution solution = this->solver->solve(m);
          this->cache_manager->save(m,solution);
         std::vector<State<std::pair<int,int>>*> solution_v =  solution->getSolution();
         std::string path;
         for(auto node : solution_v) {
           path += " ";
           path+= std::to_string(node->getCost());
         }
          send(sock_number,path.c_str(),input.size(),0);
          //this->still_handle= false;
      }



};


#endif //SEMSTERPROJECT_CLIENTHANDLER_H
