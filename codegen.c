#include "9cc.h"

static void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("代入の左辺値が変数ではありません");

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
}

static int n_lend = 0;
static int n_lelse = 0;
static int n_lbegin = 0;

void gen(Node *node) {
  int lend;
  int lelse;
  int lbegin;

  if(node == NULL) return;

  switch (node->kind) {
  case ND_NUM:
    printf("  push %d\n", node->val);
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    return;
  case ND_RETURN:
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return;
  case ND_IF:
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    if (node->chd3 == NULL) {
      n_lend++;
      lend = n_lend;
      printf("  je .Lend%d\n", lend);
      gen(node->rhs);
    } else {
      n_lelse++;
      lelse = n_lelse;
      printf("  je .Lelse%d\n", lelse);
      gen(node->rhs);
      n_lend++;
      lend = n_lend;
      printf("  jmp .Lend%d\n", lend);
      printf(".Lelse%d:\n", lelse);
      gen(node->chd3);
    }
    printf(".Lend%d:\n", lend);
    return;
  case ND_WHILE:
    n_lbegin++;
    lbegin = n_lbegin;
    printf(".Lbegin%d:\n", lbegin);
    gen(node->lhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    n_lend++;
    lend = n_lend;
    printf("  je .Lend%d\n", lend);
    gen(node->rhs);
    printf("  jmp .Lbegin%d\n", lbegin);
    printf(".Lend%d:\n", lend);
    return;
  case ND_FOR:
    gen(node->lhs);
    n_lbegin++;
    lbegin = n_lbegin;
    printf(".Lbegin%d:\n", lbegin);
    gen(node->rhs);
    printf("  pop rax\n");
    printf("  cmp rax, 0\n");
    n_lend++;
    lend = n_lend;
    printf(" je .Lend%d\n", lend);
    gen(node->chd4);
    gen(node->chd3);
    printf(" jmp .Lbegin%d\n", lbegin);
    printf(".Lend%d:\n", lend);
    return;
  case ND_BLOCK:
    for (int i = 0; node->code[i] != NULL; i++) {
      gen(node->code[i]);
      if (node->code[i+1] != NULL)
        printf("  pop rax\n");
    }
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  printf("  pop rax\n");

  switch (node->kind) {
  case ND_ADD:
    printf("  add rax, rdi\n");
    break;
  case ND_SUB:
    printf("  sub rax, rdi\n");
    break;
  case ND_MUL:
    printf("  imul rax, rdi\n");
    break;
  case ND_DIV:
    printf("  cqo\n");
    printf("  idiv rdi\n");
    break;
  case ND_EQ:
    printf("  cmp rax, rdi\n");
    printf("  sete al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_NE:
    printf("  cmp rax, rdi\n");
    printf("  setne al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LT:
    printf("  cmp rax, rdi\n");
    printf("  setl al\n");
    printf("  movzb rax, al\n");
    break;
  case ND_LE:
    printf("  cmp rax, rdi\n");
    printf("  setle al\n");
    printf("  movzb rax, al\n");
    break;
  }

  printf("  push rax\n");
}