gtestを単一のファイルくらいでやりたい

googletestのビルド
```
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
cd ../../
```

環境変数
```
export GTEST_ROOT=/path/to/googletest
export GTEST_INCLUDEDIR=$GTEST_ROOT/googletest/include
export GMOCK_INCLUDEDIR=$GTEST_ROOT/googlemock/include
export GMOCK_LIBDIR=$GTEST_ROOT/build/lib
export GTEST_LIBDIR=$GTEST_ROOT/build/lib
```

テストの実行
```
make
./test
```