#ifndef KYNOVA_CHAPIN_H
#define KYNOVA_CHAPIN_H

#include "ast.h"

/* Escribe el programa traducido a Chapín (archivo .gt). */
void chapin_emit(FILE *out, const AstNode *program);

#endif
