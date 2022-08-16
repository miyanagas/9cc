#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 "0;"
assert 42 "42;"
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 "5+6*7;"
assert 15 "5*(9-6);"
assert 4 "(3+5)/2;"
assert 10 "-10+20;"
assert 6 "-(+3*4/-2);"
assert 0 "1 == 0;"
assert 1 "3==3;"
assert 1 "-10+20*3/(3-1) == -((30+10)/-2);"
assert 0 "-9+20/4*5-25 == 12*4-(8/4*2);"
assert 1 "1 != 0;"
assert 0 "-10+20*3/(3-1) != -((30+10)/-2);"
assert 0 "1 < 0;"
assert 1 "-9+20/4*5-25 < 12*4-(8/4*2);"
assert 1 "1 > 0;"
assert 0 "-9+20/4*5-25 > 12*4-(8/4*2);"
assert 1 "3<=3;"
assert 0 "-9+20/4*5-25 >= 12*4-(8/4*2);"
assert 12 "a = 12;"
assert 13 "a = 7+5*(2+1*4*3-7); b = -(3+5+a/(-2));"
assert 20 "abc = 20;"
assert 33 "value = -3*9-(6/3*4-20)*5; result =  -(-value/11*(4-3+2))/(-3)*(-11);"

echo OK