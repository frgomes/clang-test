#include <clang-c/Index.h>
#include <stdio.h>


int main(int argc, const char *argv[]) {

    CXIndex index = clang_createIndex(1, 0);

    CXTranslationUnit translationUnit = clang_parseTranslationUnit(
            index, argv[1], argv+2, 0,
            0, 0, CXTranslationUnit_None);

    printf("Result of translation unit %d", translationUnit!=0);
}
