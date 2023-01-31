#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Print error and exit
// recieve same arguments of printf
void error(char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// 入力プログラム
char *user_input;

// Report error location
void error_at(char *location, char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);

  int position = location - user_input;
  fprintf(stderr, "%s\n", user_input);
  fprintf(stderr, "%*s", position, " ");  // position個分の空白を出力
  fprintf(stderr, "^ ");
  vfprintf(stderr, fmt, ap);
  fprintf(stderr, "\n");
  exit(1);
}

// Tokens
typedef enum {
  TK_RESERVED,  // operators
  TK_NUM,       // numbers
  TK_EOF,       // The end of file
} TokenKind;

typedef struct Token Token;

struct Token {
  TokenKind kind;
  Token *next;
  int val;    // If kind == TK_NUM, then the value
  char *str;  // If kind == TK_RESERVED, then the symbol
};

// Current token
Token *token;

// tokenが期待している記号のときはトークンを一つ読み進めてtrue
// そうでなければfalseを返す
bool consume(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op) return false;
  token = token->next;
  return true;
}

// tokenが期待している記号のときはtokenを一つ読み進める。
// otherwise report error
void expect(char op) {
  if (token->kind != TK_RESERVED || token->str[0] != op)
    error_at(token->str, "expected operator '%c' but got '%c'", op,
             token->str[0]);

  token = token->next;
}

// If token is number, then step to next token and return the current token
// value. Otherwise report error
int expect_number() {
  if (token->kind != TK_NUM) error_at(token->str, "It is not number");

  int val = token->val;
  token = token->next;
  return val;
}

bool at_eof() { return token->kind == TK_EOF; }

Token *new_token(TokenKind kind, Token *cur, char *str) {
  Token *tok = calloc(1, sizeof(Token));
  tok->kind = kind;
  tok->str = str;
  cur->next = tok;
  return tok;
}

Token *tokenize(char *p) {
  Token head;
  head.next = NULL;
  Token *cur = &head;

  while (*p) {
    // Skip whitespaces
    if (isblank(*p)) {
      p++;
      continue;
    }

    if (*p == '+' || *p == '-') {
      // hoge++はhogeを更新しつつもとの値を返す
      cur = new_token(TK_RESERVED, cur, p++);
      continue;
    }

    if (isdigit(*p)) {
      cur = new_token(TK_NUM, cur, p);
      cur->val = strtol(p, &p, 10);
      continue;
    }

    error_at(token->str, "cannot tokenize: '%c'", *p);
  }

  new_token(TK_EOF, cur, p);
  return head.next;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "The number of arguments is wrong\n");
    return 1;
  }

  user_input = argv[1];
  token = tokenize(argv[1]);

  // アセンブリの前半を出力
  printf(".intel_syntax noprefix\n");
  printf(".globl main\n");
  printf("main:\n");
  printf("  mov rax, %d\n", expect_number());

  while (!at_eof()) {
    if (consume('+')) {
      printf("  add rax, %d\n", expect_number());
      continue;
    }

    expect('-');
    printf("  sub rax, %d\n", expect_number());
  }

  printf("  ret\n");
  return 0;
}
