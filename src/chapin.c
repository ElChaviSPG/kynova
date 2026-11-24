#include "chapin.h"

/* Traductor Kynova -> Chapín (grupo 4): recorre el AST y escribe Chapin.gt.
   va pisto/quetzal = splice adam/eve | cabal/sino = atlas/ryan | brega = vita
   clavar = radio (entrada) | shute = diary (salida) */

static void indent(FILE *o, int n) { fprintf(o, "%*s", n * 4, ""); }

static int prec(const AstNode *e) { return e->kind == NODE_RELOP ? 0 : e->kind != NODE_BINOP ? 3 : (e->op == OP_MUL || e->op == OP_DIV) ? 2 : 1; }

static void expr(FILE *o, const AstNode *e)
{
    switch (e->kind) {
    case NODE_INT: case NODE_REAL: case NODE_STR: case NODE_ID: fputs(e->text, o); break;
    case NODE_BINOP:
    case NODE_RELOP: {
        /* El AST no guarda paréntesis: se reponen según precedencia y asociatividad izquierda. */
        int p = prec(e);
        int pl = prec(e->child[0]) < p, pr = prec(e->child[1]) <= p;
        if (pl) fputc('(', o);
        expr(o, e->child[0]);
        if (pl) fputc(')', o);
        fprintf(o, " %s ", op_symbol(e->op));
        if (pr) fputc('(', o);
        expr(o, e->child[1]);
        if (pr) fputc(')', o);
        break;
    }
    default: break;
    }
}

static void block(FILE *o, const AstNode *b, int lvl);

static void stmt(FILE *o, const AstNode *s, int lvl)
{
    indent(o, lvl);
    switch (s->kind) {
    case NODE_DECL:
        fprintf(o, "va %s %s", s->type == TYPE_ADAM ? "pisto" : "quetzal", s->text);
        if (s->child[0]) { fputs(" = ", o); expr(o, s->child[0]); }
        fputs(";\n", o);
        break;
    case NODE_ASSIGN: fprintf(o, "%s = ", s->text); expr(o, s->child[0]); fputs(";\n", o); break;
    case NODE_READ:   fprintf(o, "clavar(%s);\n", s->text); break;
    case NODE_WRITE:  fputs("shute(", o); expr(o, s->child[0]); fputs(");\n", o); break;
    case NODE_IF:
        fputs("cabal (", o); expr(o, s->child[0]); fputs(") ", o);
        block(o, s->child[1], lvl);
        fputs(" sino ", o);
        block(o, s->child[2], lvl);
        fputc('\n', o);
        break;
    case NODE_WHILE:
        fputs("brega (", o); expr(o, s->child[0]); fputs(") ", o);
        block(o, s->child[1], lvl);
        fputc('\n', o);
        break;
    default: break;
    }
}

static void block(FILE *o, const AstNode *b, int lvl)
{
    fputs("{\n", o);
    for (int i = 0; i < b->count; i++) stmt(o, b->items[i], lvl + 1);
    indent(o, lvl);
    fputc('}', o);
}

void chapin_emit(FILE *out, const AstNode *program)
{
    for (int i = 0; i < program->count; i++) stmt(out, program->items[i], 0);
}
