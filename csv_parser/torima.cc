#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

template<typename T>
T str2type(std::string str){
  // if T is numerical type, maybe return 0
  if(str=="") str="-";
  std::stringstream sts(str);
  T conv;
  sts>>conv;
  
  return conv;
}

std::string delete_space(std::string str){
  size_t pos;
  while((pos = str.find_first_of(' ')) != std::string::npos) str.erase(pos,1);
  return str;
}

template<typename T>
std::vector<std::vector<T> > parse(std::string filename, int skip_head_line=0){
  char delm = ','; // if you want to parse with other than comma, change delimiter
  std::vector<std::vector<T> > ret_vv;
  std::ifstream ifs(filename);
  
  if(ifs.fail()){
    std::cerr<<"failed open "<<filename<<std::endl;
    exit(-1);
  }
  
  std::string line;
  while(std::getline(ifs,line)){
    if(skip_head_line > 0){
      skip_head_line--;
      continue;
    }
    //std::cout<<"befole "<<line<<std::endl;
    line = delete_space(line); // ignore space
    
    std::stringstream sts(line);
    
    std::vector<T> tmp_v;
    std::string elem;
    while(std::getline(sts, elem, delm)){
      T conv_elem = str2type<T>(elem);
      //std::cout<<conv_elem<<std::endl;
      tmp_v.push_back(conv_elem);
    }
    ret_vv.push_back(tmp_v);
  }

  return ret_vv;
}

template<typename T>
void print_vv(std::vector<std::vector<T> > &v){
  for(auto x : v){
    for(size_t i=0;i<x.size()-1;i++){
      std::cout<<x[i]<<", ";
    }
    if(x.size()!=0) std::cout<<x[x.size()-1]<<std::endl;
  }
}


int main(){

  std::string filename="nanika.csv";
  
  //#define type_t double
  #define type_t std::string
  std::vector<std::vector<type_t> > vvd;
  vvd = parse<type_t>(filename, 0);

  print_vv<type_t>(vvd);
  
  return 0;
}
