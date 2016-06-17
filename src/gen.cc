#include <clang-c/Index.h>
#include <stdio.h>
#include <libgen.h>
#include <cstdlib>
#include <libgen.h>
#include <cstring>
#include <ntsid.h>


CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data);

char *getFilename(char *path);

char *getType(CXType cString);

char * getArgs(const CXCursor &cursor, const CXType &type);

int main(int argc, const char *argv[]) {

    const CXIndex index = clang_createIndex(1, 0);
    const CXTranslationUnit translationUnit = clang_parseTranslationUnit(
            index,
            argv[0],
            argv + 1,
            argc - 1,
            0, 0, CXTranslationUnit_None);
    const CXCursor rootCursor = clang_getTranslationUnitCursor(translationUnit);
    printf("@extern object %s{\n", getFilename(
            (char *) argv[1]));
    const unsigned int res = clang_visitChildren(rootCursor, cursorVisitor, NULL);
    printf("}\n");
    clang_disposeTranslationUnit(translationUnit);
    clang_disposeIndex(index);
}


CXChildVisitResult cursorVisitor(CXCursor cursor, CXCursor parent, CXClientData client_data) {

    const CXCursorKind kind = clang_getCursorKind(cursor);
    const CXType type = clang_getCursorType(cursor);
    const CXType resultType = clang_getResultType(type);

    const CXString name = clang_getCursorSpelling(cursor);

    if (kind == CXCursor_FunctionDecl || kind == CXCursor_ObjCInstanceMethodDecl) {
        printf("def %s(%s) : %s = extern\n ", clang_getCString(name), getArgs(cursor, type), getType(resultType));
        return CXChildVisit_Continue;
    }
    return CXChildVisit_Recurse;
}
//TODO: make variable name
char *getArgs(const CXCursor &cursor, const CXType &type) {
    char *result;
    const int argsCount = clang_Cursor_getNumArguments(cursor);
    for (int i = 0; i < argsCount; i++) {
        if (i > 1)strcat(result, ",");
        CXType type1 = clang_getArgType(type, i);
        strcat(result, "a:");
        strcat(result, getType(type1));

    }
    return result;
}
//TODO: add all types
char *getType(CXType type) {
    switch (type.kind) {
        case CXType_Void:
        case CXType_Pointer:
            return (char *) "Prt[_]";
        case CXType_Int:
            return (char *) "CInt";
        case CXType_Long:
            return (char *) "CLong";
        case CXType_ULong:
            return (char *) "CUnsignedLong";
        default:
            return NULL;
    }
}

char *getFilename(char *path) {
    const char *chars = basename(path);
    int i = 0;
    while (*(chars + i) != '\0' && *(chars + i) != '.') {
        i++;
    }
    char *result = (char *) malloc(sizeof(char) * i);
    memcpy(result, chars, i);
    return result;
}




