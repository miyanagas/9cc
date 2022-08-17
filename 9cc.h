#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// トークンの種類
typedef enum {
  TK_RESERVED, // 記号と予約語
  TK_IDENT,    // 識別子
  TK_NUM,      // 整数トークン
  TK_EOF,      // 入力の終わりを表すトークン
} TokenKind;

typedef struct Token Token;
// トークン型
struct Token {
  TokenKind kind; // トークンの型
  Token *next;    // 次の入力トークン
  int val;        // kindがTK_NUMの場合、その数値
  char *str;      // トークン文字列
  int len;        // トークンの長さ
};

// 抽象構文木のノードの種類
typedef enum {
  ND_ADD,     // +
  ND_SUB,     // -
  ND_MUL,     // *
  ND_DIV,     // /
  ND_EQ,      // ==
  ND_NE,      // !=
  ND_LT,      // <
  ND_LE,      // <=
  ND_ASSIGN,  // =
  ND_LVAR,    // ローカル変数
  ND_FUNC,    // 関数
  ND_RETURN,  // return文
  ND_IF,      // if文
  ND_WHILE,   // while文
  ND_FOR,     // for文
  ND_BLOCK,   // ブロック
  ND_NUM,     // 整数
} NodeKind;

typedef struct Node Node;
// 抽象構文木のノードの型
struct Node {
  NodeKind kind;    // ノードの型
  Node *lhs;        // 左辺（子ノード1）
  Node *rhs;        // 右辺（子ノード2）
  Node *bro;        // 兄弟ノード
  int val;          // kindがND_NUMの場合のみ使う
  int offset;       // kindがND_LVARの場合のみ使う
  char *name;       // kindがND_FUNCの場合のみ使う
  int len;          // kindがND_FUNCの場合のみ使う
  Node *code[100];  // kindがND_BLOCKの場合のみ使う
};

typedef struct LVar LVar;
// ローカル変数の型
struct LVar {
  LVar *next; // 次の変数かNULL
  char *name; // 変数の名前
  int len;    // 名前の長さ
  int offset; // RBPからのオフセット
};

extern int rsp;

#define push() rsp - 8
#define pop() rsp + 8

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void program();
void gen(Node *node);