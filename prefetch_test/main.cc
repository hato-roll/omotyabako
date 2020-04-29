#include <stdio.h>
#include <time.h>
#include <random>
#include <iostream>
#include <string>
#include <chrono>
#include <xmmintrin.h>
#include <immintrin.h>
using namespace std;

bool is_correct(int n, float **a, float **b, float **c){
  float **d = new float*[n];
  for(int i=0;i<n;i++){
    d[i]=new float[n];
    fill(d[i], d[i]+n, 0);  
  }

  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      for(int k=0;k<n;k++){
        d[i][j]+=a[i][k]*b[k][j];
      }
    }
  }

  bool f=true;
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      if(c[i][j]!=d[i][j] && f){
        printf("(%d,%d) : diff=%e , true=%f , test=%f\n",i,j,c[i][j]-d[i][j], d[i][j], c[i][j]);
        f=false;
      }
    }
  }
  for(int i=0;i<n;i++){
    delete[] d[i];
  }
  delete[] d;
  return f;
}

void fill_zero(int n, float **x){
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      x[i][j]=0.0;
    }
  }
}

int main(){
  const int n = 1024;

  float **a = new float*[n];
  float **b = new float*[n];
  float **c = new float*[n];
  for(int i=0;i<n;i++){
    a[i]=new float[n];
    b[i]=new float[n];
    c[i]=new float[n];    
  }
  random_device rnd;
  
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      a[i][j]=rnd()%n;
      b[i][j]=rnd()%n;
      c[i][j]=0.0;
    }
  }
  
  chrono::high_resolution_clock::time_point  st,en;
  double e;

  //----------------------------------------------------------------------
  st = chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      for(int k=0;k<n;k++){
        c[i][j]+=a[i][k]*b[k][j];
      }
    }
  }  
  en = chrono::high_resolution_clock::now();
  e=chrono::duration_cast<std::chrono::milliseconds>(en-st).count();

  printf("%6.3f ms\n",e);
  if(!is_correct(n,a,b,c)) cout<<"incorrect 1"<<endl;
  //----------------------------------------------------------------------
  fill_zero(n,c);
  st = chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++){
    for(int k=0;k<n;k++){
      for(int j=0;j<n;j++){
        c[i][j]+=a[i][k]*b[k][j];
      }
    }
  }  
  en = chrono::high_resolution_clock::now();
  e=chrono::duration_cast<std::chrono::milliseconds>(en-st).count();

  printf("%6.3f ms\n",e);
  if(!is_correct(n,a,b,c)) cout<<"incorrect 2"<<endl;
  //----------------------------------------------------------------------
  fill_zero(n,c);
  st = chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++){
    for(int k=0;k<n;k++){
      _mm_prefetch((char const *)(&b[k+1][0]), _MM_HINT_T2);
      for(int j=0;j<n;j++){
        c[i][j]+=a[i][k]*b[k][j];
      }
    }
  }  
  en = chrono::high_resolution_clock::now();
  e=chrono::duration_cast<std::chrono::milliseconds>(en-st).count();

  printf("%6.3f ms\n",e);
  if(!is_correct(n,a,b,c)) cout<<"incorrect 2-1"<<endl;
  //----------------------------------------------------------------------
  fill_zero(n,c);
#define UNROLL_SIZE 32
  st = chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j+=UNROLL_SIZE){
      for(int k=0;k<n;k++){
#pragma unroll
        for(int l=0;l<UNROLL_SIZE;l++){
          c[i][j+l]+=a[i][k]*b[k][j+l];
        }
      }
    }
  }  
  en = chrono::high_resolution_clock::now();
  e=chrono::duration_cast<std::chrono::milliseconds>(en-st).count();

  printf("%6.3f ms\n",e);
  if(!is_correct(n,a,b,c)) cout<<"incorrect 3"<<endl;
  //----------------------------------------------------------------------
  fill_zero(n,c);
#define UNROLL_SIZE 32
  st = chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j+=UNROLL_SIZE){
      for(int k=0;k<n;k++){
        _mm_prefetch((char const *)(&b[k+16][j]), _MM_HINT_T2);
#pragma unroll
        for(int l=0;l<UNROLL_SIZE;l++){
          c[i][j+l]+=a[i][k]*b[k][j+l];
        }
      }
    }
  }  
  en = chrono::high_resolution_clock::now();
  e=chrono::duration_cast<std::chrono::milliseconds>(en-st).count();

  printf("%6.3f ms\n",e);
  if(!is_correct(n,a,b,c)) cout<<"incorrect 3-1"<<endl;
  //----------------------------------------------------------------------
  fill_zero(n,c);
  st = chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++){
    for(int k=0;k<n;k++){
      __m256 a_vec = _mm256_set1_ps(a[i][k]);
      for(int j=0;j<n;j+=8){
        //_mm_prefetch((char const *)(&b[k+16][j]), _MM_HINT_T2);
        // for(int jj=0;jj<8;jj++){
        //   c[i][j+jj]+=a[i][k]*b[k][j+jj];
        // }
        __m256 b_vec = _mm256_loadu_ps(&b[k][j]);
        __m256 c_vec = _mm256_loadu_ps(&c[i][j]);
        c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
        _mm256_storeu_ps(&c[i][j],c_vec);
      }
    }
  }
  en = chrono::high_resolution_clock::now();
  e=chrono::duration_cast<std::chrono::milliseconds>(en-st).count();
  
  printf("%6.3f ms\n",e);
  if(!is_correct(n,a,b,c)) cout<<"incorrect 4"<<endl;
  //----------------------------------------------------------------------
  fill_zero(n,c);
  st = chrono::high_resolution_clock::now();
  for(int i=0;i<n;i++){
    for(int k=0;k<n;k++){
      __m256 a_vec = _mm256_set1_ps(a[i][k]);
      _mm_prefetch((char const *)(&b[k+1][0]), _MM_HINT_T2);        
      for(int j=0;j<n;j+=8){
        // for(int jj=0;jj<8;jj++){
        //   c[i][j+jj]+=a[i][k]*b[k][j+jj];
        // }
        __m256 b_vec = _mm256_loadu_ps(&b[k][j]);
        __m256 c_vec = _mm256_loadu_ps(&c[i][j]);
        c_vec = _mm256_fmadd_ps(a_vec, b_vec, c_vec);
        _mm256_storeu_ps(&c[i][j],c_vec);
      }
    }
  }
  en = chrono::high_resolution_clock::now();
  e=chrono::duration_cast<std::chrono::milliseconds>(en-st).count();
  
  printf("%6.3f ms\n",e);
  if(!is_correct(n,a,b,c)) cout<<"incorrect 4-1"<<endl;
  //----------------------------------------------------------------------


  
  for(int i=0;i<n;i++){
    delete[] a[i];
    delete[] b[i];
    delete[] c[i];
  }
  delete[] a;
  delete[] b;
  delete[] c;
    
  return 0;
}
