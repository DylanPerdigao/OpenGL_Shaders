//
//  reader.h
//  CG_Projeto_Shaders
//
//  Created by Dylan Perdigão on 26/12/2020.
//

#ifndef reader_h
#define reader_h
char* readShaderFile(char* name){
    char* DATA;
    long length = 0;
    FILE* fp = fopen(name, "r");
    if (fp) {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        rewind(fp);
        DATA = (char*)malloc(sizeof(char) * (length + 1));
        length = fread(DATA, sizeof(char), length, fp);
        DATA[length] = '\0';
        fclose(fp);
        return DATA;
    }
    printf("---> Error while reading  %s.\n", name);
    return NULL;
}

#endif /* reader_h */
