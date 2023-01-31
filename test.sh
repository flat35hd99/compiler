#!/bin/bash

assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual=$?

  if [ "$actual" = "$expected" ];then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

# Step1
assert 0 0
assert 42 42

# Step2
assert 21 '5+20-4'
assert 42 '100-50-8'

echo OK
