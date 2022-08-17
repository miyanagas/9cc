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
assert 6 "return 6;"
assert 12 "foo = 2;  boo = 5 * 6 - 8;  return (foo + boo) / 2;"
assert 6 "abc = 3; if ( abc == 3 ) abc = abc * 2;"
assert 1 "a = 5; b; if (a / 5 == 1) b = 1; else b = 0; return b;"
assert 0 "foo = 100; while (foo > 0) foo = foo - 1; return foo;"
assert 1 "i; x = 2; for (i = 0; i < 3; i = i + 1) x = x * x; return x / 256;"
assert 10 "a = 6; i = 0; while(i < 10) if(a / 2 == 3) i = i + 1; else i = 100; return i;"

echo OK