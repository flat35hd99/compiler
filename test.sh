#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  if [ "$?" != "0" ];then
    echo "9cc process finished in fail"
    exit 1;
  fi
  cc -o tmp tmp.s
  if [ "$?" != "0" ];then
    echo "cc process finished in fail"
    exit 1;
  fi

  ./tmp
  actual=$?

  if [ "$actual" = "$expected" ];then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

# Step1: 整数
assert 0 0
assert 42 42

# Step2: 加減算
assert 21 '5+20-4'
assert 42 '100-50-8'

# Step3: Tokenizer
assert 2 '5 - 3'
assert 42 ' 51   - 12+3 '

echo OK
