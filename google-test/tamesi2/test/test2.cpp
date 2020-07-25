#include "gtest/gtest.h"
#include "torima1.h"
#include "torima2.h"
#include "torima3.h"

TEST(tamesi2_test, torima1){
  EXPECT_EQ(1,1);
  EXPECT_EQ(1,ret1());
  EXPECT_EQ(2,ret2());
  EXPECT_EQ(3,ret3());
}

TEST(tamesi2_test, torima2){
  EXPECT_EQ(1,2);
}
