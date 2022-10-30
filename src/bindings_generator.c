//
// LJBG © -- LuaJIT FFI bindings generator.
// Copyright (C) 2020 Stefan Pejic. See copyright notice in LICENSE file.
//

#include "bindings_generator.h"
#include <assert.h>
#include <clang-c/Index.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

static const char gLuaFileBoilerplatePrefix[] = "local ffi = require(\"ffi\")\n\nffi.cdef[[\n";
static const char gLuaFileBoilerplateSuffix[] = "]]\n";

static const char gUnnamedStructPrefix[] = "struct {";
static const char gUnnamedUnionPrefix[] = "union {";
static const char gUnnamedEnumPrefix[] = "enum {";
// In older versions of libclang, there are 2 spaces between 'enum' and '{'.
static const char gUnnamedEnumPrefixLegacy[] = "enum  {";

static bool IsUnnamedStructUnionOrEnum(const CXCursor c)
{
    if ((c.kind != CXCursor_StructDecl) && (c.kind != CXCursor_UnionDecl) && (c.kind != CXCursor_EnumDecl))
    {
        return false;
    }

    const CXString pretty_printed_cursor = clang_getCursorPrettyPrinted(c, (CXPrintingPolicy)NULL);
    const char* s = clang_getCString(pretty_printed_cursor);
    if ((strncmp(gUnnamedStructPrefix, s, strlen(gUnnamedStructPrefix)) != 0) &&
        (strncmp(gUnnamedUnionPrefix, s, strlen(gUnnamedUnionPrefix)) != 0) &&
        (strncmp(gUnnamedEnumPrefix, s, strlen(gUnnamedEnumPrefix)) != 0) &&
        (strncmp(gUnnamedEnumPrefixLegacy, s, strlen(gUnnamedEnumPrefixLegacy)) != 0))
    {
        clang_disposeString(pretty_printed_cursor);
        return false;
    }
    clang_disposeString(pretty_printed_cursor);
    return true;
}

static enum CXChildVisitResult FunctionVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data)
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
            FILE* f = *(FILE**)client_data;
            fprintf(f, "%s;\n", clang_getCString(pretty_printed_cursor));
        }

        clang_PrintingPolicy_dispose(policy);
        clang_disposeString(pretty_printed_cursor);
    }
    return CXChildVisit_Continue;
}

StatusCodes GenerateBindings(const Arguments args)
{
    FILE* in = fopen(args.input_file, "r");
    if (in == NULL)
    {
        fprintf(stderr, "Input file %s not found.\n", args.input_file);
        return StatusBadInputFile;
    }
    fclose(in);

    CXIndex index = clang_createIndex(0, 0);
    assert(index != NULL);

    CXTranslationUnit tu = clang_parseTranslationUnit(
        index, args.input_file, args.compilation_arguments, args.compilation_argument_count, NULL, 0, 0);
    assert(tu != NULL);

    unsigned num_diagnostics = clang_getNumDiagnostics(tu);
    bool has_error_occurred = false;
    for (unsigned i = 0; i < num_diagnostics; i++)
    {
        CXDiagnostic d = clang_getDiagnostic(tu, i);
        if (clang_getDiagnosticSeverity(d) > CXDiagnostic_Warning)
        {
            has_error_occurred = true;

            CXString s = clang_formatDiagnostic(d, CXDiagnostic_DisplaySourceLocation);
            const char* cs = clang_getCString(s);
            fprintf(stderr, "%s\n", cs);
            clang_disposeString(s);
        }
        clang_disposeDiagnostic(d);
    }
    if (has_error_occurred)
    {
        clang_disposeTranslationUnit(tu);
        clang_disposeIndex(index);

        return StatusCompilationError;
    }

    CXCursor cursor = clang_getTranslationUnitCursor(tu);

    FILE* out = args.output_file ? fopen(args.output_file, "w") : stdout;
    if (out == NULL)
    {
        fprintf(stderr, "Bad output file: %s\n", args.output_file ? args.output_file : "stdout");
        return StatusBadOutputFile;
    }

    fprintf(out, "%s", gLuaFileBoilerplatePrefix);
    clang_visitChildren(cursor, FunctionVisitor, (CXClientData)&out);
    fprintf(out, "%s", gLuaFileBoilerplateSuffix);

    if (out != stdout)
    {
        fclose(out);
    }

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);

    return StatusSuccess;
}
