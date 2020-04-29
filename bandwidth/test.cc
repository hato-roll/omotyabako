#include <iostream>
#include <cstdlib>
#include <chrono>
#include <random>
#include <omp.h>
using namespace std;
using namespace std::chrono;

int main(int argc, char *argv[]){
  if(argc!=4){
    cerr<<"usage : (data size) (num threads) (loop count)"<<endl;
    exit(1);
  }

  long long int dsize=atol(argv[1]);
  int nthreads=atoi(argv[2]);
  long long int lcount=atol(argv[3]);
  int max_threads=omp_get_max_threads();

  if(dsize%sizeof(int)!=0){
    cerr<<"data size must be a multiple of sizeof(int)"<<endl;
    exit(2);
  }
  if(nthreads>max_threads){
    cerr<<"nthreads must be under omp max threads"<<endl;
    cerr<<"omp max threads is "<<max_threads<<endl;
    exit(3);
  }
  int *mem1,*mem2;
  mem1 = (int *)malloc(dsize);
  if(mem1==NULL){
    cerr<<"bad malloc"<<endl;
    exit(4);
  }
  
  printf("data size : %lld Byte (%lld KiB)\n",dsize,dsize/1024);
  printf("num threads : %d (max num threads : %d)\n",nthreads, max_threads);  
  
  //mem2 = (int *)malloc(dsize);
  int num=dsize/sizeof(int);

  high_resolution_clock::time_point begin,end;
  double e; // for elapsed time
  
  random_device rnd;
  mt19937 mt(rnd());
  for(int i=0;i<num;i++){
    mem1[i]=mt();
    //mem2[i]=mt();
  }

  int hako=0;

  /* ---------------------------------------------------------------------------- */
  
  begin = high_resolution_clock::now();
  for(int l=0;l<lcount;l++){
    
#pragma omp parallel for num_threads(nthreads) private(hako)
    for(int o=0;o<nthreads;o++){
      for(int i=0;i<num;i++){
        hako+=mem1[i];
      }
    }
  }
  end = high_resolution_clock::now();
  
  e=duration_cast<nanoseconds>(end - begin).count();
  e/=1e9d;
  printf("sum time : %.3f s\n",e);
  printf("sum instruction per core: %.3f M instruction/s\n",lcount*num/e/1e6d);

  /* ---------------------------------------------------------------------------- */

  printf("read1 : no pragma (regardless of num_threads)\n");
  printf("read2 : use pragma inner for loop\n");
  printf("read3 : use pragma outer for loop (loop_count)\n");
  
  /* ---------------------------------------------------------------------------- */
  
  begin = high_resolution_clock::now();
  for(int l=0;l<lcount;l++){
    for(int i=0;i<num;i++){
      int a=mem1[i];
    }
  }
  end = high_resolution_clock::now();

  e=duration_cast<nanoseconds>(end - begin).count();
  e/=1e9d;
  printf("read1 time : %.3f s\n",e);
  printf("read1 bandwidth : %.3f MiB/s\n",(double((dsize/1024.)*lcount)/1024.)/e);
  printf("read1 instruction : %.3f M instruction/s\n",lcount*num/e/1e6d);

  /* ---------------------------------------------------------------------------- */

  begin = high_resolution_clock::now();
  for(int l=0;l<lcount;l++){
#pragma omp parallel for num_threads(nthreads)
    for(int i=0;i<num;i++){
      int a=mem1[i];
    }
  }
  end = high_resolution_clock::now();

  e=duration_cast<nanoseconds>(end - begin).count();
  e/=1e9d;
  printf("read2 time : %.3f s\n",e);
  printf("read2 bandwidth : %.3f MiB/s\n",(double((dsize/1024.)*lcount)/1024.)/e);
  printf("read2 instruction : %.3f M instruction/s\n",lcount*num/e/1e6d);

  /* ---------------------------------------------------------------------------- */

  begin = high_resolution_clock::now();
#pragma omp parallel for num_threads(nthreads)
  for(int l=0;l<lcount;l++){
    for(int i=0;i<num;i++){
      int a=mem1[i];
    }
  }
  end = high_resolution_clock::now();

  e=duration_cast<nanoseconds>(end - begin).count();
  e/=1e9d;
  printf("read3 time : %.3f s\n",e);
  printf("read3 bandwidth : %.3f MiB/s\n",(double((dsize/1024.)*lcount)/1024.)/e);
  printf("read3 instruction : %.3f M instruction/s\n",lcount*num/e/1e6d);

  /* ---------------------------------------------------------------------------- */

  begin = high_resolution_clock::now();
  for(int l=0;l<lcount;l++){
#pragma omp parallel for num_threads(nthreads)
    for(int i=0;i<num;i++){
      mem1[i]=i;
    }
  }
  end = high_resolution_clock::now();
  
  e=duration_cast<nanoseconds>(end - begin).count();
  e/=1e9d;
  printf("write time : %.3f s\n",e);
  printf("write bandwidth : %.3f MiB/s\n",(double((dsize/1024.)*lcount)/1024.)/e);
  printf("write instruction : %.3f M instruction/s\n",lcount*num/e/1e6d);

  /* ---------------------------------------------------------------------------- */
  
  begin = high_resolution_clock::now();
  for(int l=0;l<lcount;l++){
#pragma omp parallel for num_threads(nthreads)
    for(int i=0;i<num;i++){
      mem1[i]+=1;
    }
  }
  end = high_resolution_clock::now();
  
  e=duration_cast<nanoseconds>(end - begin).count();
  e/=1e9d;
  printf("increment time : %.3f s\n",e);
  printf("increment bandwidth : %.3f MiB/s\n",(double((dsize/1024.)*lcount)/1024.)/e);
  printf("increment instruction : %.3f M instruction/s\n",lcount*num/e/1e6d);
  
  free(mem1);
  //free(mem2);
  return 0;
}
