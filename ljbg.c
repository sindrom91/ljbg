//
// LJBG © -- LuaJIT FFI bindings generator.
//
// Copyright (C) 2020 Stefan Pejic
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
// documentation files (the  "Software"), to deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the
// Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
// WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
// OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "argtable3.h"
#include <assert.h>
#include <clang-c/Index.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char gLuaFileBoilerplatePrefix[] = "local ffi = require(\"ffi\")\n\nffi.cdef[[\n";
static const char gLuaFileBoilerplateSuffix[] = "]]\n";

static const char gUnnamedStructPrefix[] = "struct {";
static const char gUnnamedUnionPrefix[] = "union {";
// TODO: There are 2 spaces between enum and {. This is probably a bug in libclang.
static const char gUnnamedEnumPrefix[] = "enum  {";

bool IsUnnamedStructUnionOrEnum(const CXCursor c)
{
    if ((c.kind != CXCursor_StructDecl) && (c.kind != CXCursor_UnionDecl) && (c.kind != CXCursor_EnumDecl))
    {
        return false;
    }

    const CXString pretty_printed_cursor = clang_getCursorPrettyPrinted(c, (CXPrintingPolicy)NULL);
    const char *s = clang_getCString(pretty_printed_cursor);
    if ((strncmp(gUnnamedStructPrefix, s, strlen(gUnnamedStructPrefix)) != 0) &&
        (strncmp(gUnnamedUnionPrefix, s, strlen(gUnnamedUnionPrefix)) != 0) &&
        (strncmp(gUnnamedEnumPrefix, s, strlen(gUnnamedEnumPrefix)) != 0))
    {
        clang_disposeString(pretty_printed_cursor);
        return false;
    }
    clang_disposeString(pretty_printed_cursor);
    return true;
}

enum CXChildVisitResult FunctionVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
{
    (void)parent;
    if ((clang_isDeclaration(cursor.kind) && (clang_getCursorLinkage(cursor) == CXLinkage_External)) ||
        ((cursor.kind == CXCursor_TypedefDecl) && (clang_getCursorLinkage(cursor) == CXLinkage_NoLinkage)))
    {
        CXPrintingPolicy policy = clang_getCursorPrintingPolicy(cursor);

        if (cursor.kind == CXCursor_TypedefDecl)
        {
            CXCursor c = clang_getTypeDeclaration(clang_getTypedefDeclUnderlyingType(cursor));
            if (IsUnnamedStructUnionOrEnum(c))
            {
                clang_PrintingPolicy_setProperty(policy, CXPrintingPolicy_IncludeTagDefinition, 1);
            }
        }
        const CXString pretty_printed_cursor = clang_getCursorPrettyPrinted(cursor, policy);
        if (!IsUnnamedStructUnionOrEnum(cursor))
        {
            FILE *f = *(FILE **)client_data;
            fprintf(f, "%s;\n", clang_getCString(pretty_printed_cursor));
        }

        clang_PrintingPolicy_dispose(policy);
        clang_disposeString(pretty_printed_cursor);
    }
    return CXChildVisit_Continue;
}

typedef struct
{
    struct arg_file *ifile;
    struct arg_file *ofile;
    struct arg_str *args;
    struct arg_end *end;
} Arguments;

Arguments ParseArguments(const int argc, char **argv)
{
    Arguments a = {arg_filen(NULL, NULL, NULL, 1, 1, NULL), arg_filen("o", "output", NULL, 1, 1, NULL),
                   arg_strn(NULL, NULL, NULL, 0, 100, NULL), arg_end(20)};
    void *argtable[] = {a.ifile, a.ofile, a.args, a.end};
    arg_parse(argc, argv, argtable);
    return a;
}

int main(int argc, char **argv)
{
    assert(argc > 2);

    CXIndex index = clang_createIndex(0, 0);
    assert(index != NULL);

    Arguments a = ParseArguments(argc, argv);

    if (a.end->count > 0)
    {
        arg_print_errors(stderr, a.end, "ljbg");
        return 1;
    }

    CXTranslationUnit tu =
        clang_parseTranslationUnit(index, a.ifile->filename[0], a.args->sval, a.args->count, NULL, 0, 0);
    assert(tu != NULL);

    CXCursor cursor = clang_getTranslationUnitCursor(tu);

    FILE *f = fopen(a.ofile->filename[0], "w");
    assert(f != NULL);

    fprintf(f, "%s", gLuaFileBoilerplatePrefix);
    clang_visitChildren(cursor, FunctionVisitor, (CXClientData)&f);
    fprintf(f, "%s", gLuaFileBoilerplateSuffix);

    fclose(f);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return 0;
}
