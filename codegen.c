#include "9cc.h"

int rsp = 0;

static void gen_lval(Node *node) {
  if (node->kind != ND_LVAR)
    error("代入の左辺値が変数ではありません");

  printf("  mov rax, rbp\n");
  printf("  sub rax, %d\n", node->offset);
  printf("  push rax\n");
  push();
}

static int n_lend = 0;
static int n_lelse = 0;
static int n_lbegin = 0;

void gen(Node *node) {
  int lend, lelse, lbegin;
  int i;
  char *fname;


  if(node == NULL) return;

  switch (node->kind) {
  case ND_NUM:
    printf("  push %d\n", node->val);
    push();
    return;
  case ND_LVAR:
    gen_lval(node);
    printf("  pop rax\n");
    pop();
    printf("  mov rax, [rax]\n");
    printf("  push rax\n");
    push();
    return;
  case ND_FUNC:
    i = 0;
    for (Node *nod = node->lhs; nod != NULL && i < 7; nod = nod->bro) {
      gen(nod);
      i++;
    }
    for (; i > 0; i--) {
      switch (i) {
      case 1:
        printf("  pop rdi\n");
        break;
      case 2:
        printf("  pop rsi\n");
        break;
      case 3:
        printf("  pop rdx\n");
        break;
      case 4:
        printf("  pop rcx\n");
        break;
      case 5:
        printf("  pop r8\n");
        break;
      case 6:
        printf("  pop r9\n");
        break;
      }
      pop();
    }
    if (rsp % 16 != 0) {
      printf("  sub rsp, 8\n");
      rsp -= 8;
    }
    printf("  call %s\n", strncpy(fname, node->name, node->len));
    return;
  case ND_ASSIGN:
    gen_lval(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    pop();
    printf("  pop rax\n");
    pop();
    printf("  mov [rax], rdi\n");
    printf("  push rdi\n");
    push();
    return;
  case ND_RETURN:
    gen(node->lhs);
    printf("  pop rax\n");
    pop();
    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    pop();
    printf("  ret\n");
    return;
  case ND_IF:
    gen(node->lhs);
    printf("  pop rax\n");
    pop();
    printf("  cmp rax, 0\n");
    if (node->lhs->bro->bro == NULL) {
      n_lend++;
      lend = n_lend;
      printf("  je .Lend%d\n", lend);
      gen(node->lhs->bro);
    } else {
      n_lelse++;
      lelse = n_lelse;
      printf("  je .Lelse%d\n", lelse);
      gen(node->lhs->bro);
      n_lend++;
      lend = n_lend;
      printf("  jmp .Lend%d\n", lend);
      printf(".Lelse%d:\n", lelse);
      gen(node->lhs->bro->bro);
    }
    printf(".Lend%d:\n", lend);
    return;
  case ND_WHILE:
    n_lbegin++;
    lbegin = n_lbegin;
    printf(".Lbegin%d:\n", lbegin);
    gen(node->lhs);
    printf("  pop rax\n");
    pop();
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
    gen(node->lhs->bro);
    printf("  pop rax\n");
    pop();
    printf("  cmp rax, 0\n");
    n_lend++;
    lend = n_lend;
    printf(" je .Lend%d\n", lend);
    gen(node->lhs->bro->bro->bro);
    gen(node->lhs->bro->bro);
    printf(" jmp .Lbegin%d\n", lbegin);
    printf(".Lend%d:\n", lend);
    return;
  case ND_BLOCK:
    for (int i = 0; node->code[i] != NULL; i++) {
      gen(node->code[i]);
      if (node->code[i+1] != NULL) {
        printf("  pop rax\n");
        pop();
      }
    }
    return;
  }

  gen(node->lhs);
  gen(node->rhs);

  printf("  pop rdi\n");
  pop();
  printf("  pop rax\n");
  pop();

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
  push();
}