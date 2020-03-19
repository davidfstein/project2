#include "parser.h"

int main(int argc, char* argv[]) {
    FILE* file = fopen("./src/cli/data/data.sor", "r");
    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    SorParser parser{file, 0, file_size, file_size};
    parser.guessSchema();
    parser.parseFile();
    DataFrame* df = parser.getDataFrame();
    printf("from df: %d\n", df->get_int(1,0));
    printf("from df: %d\n", df->get_int(1,1));
    printf("from df: %d\n", df->get_int(1,2));
    printf("File successfully read to dataframe!\n");
    return 0;
}
