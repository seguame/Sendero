#include "class_file.h"

ClassFile::ClassFile():
    cuerpo(NULL)
{
    cuerpo = (CuerpoClass*)malloc(sizeof(CuerpoClass));

    cuerpo->magia           = 0xCAFEBABE;
    cuerpo->minor_version   = 0x0000;
    cuerpo->major_version   = 0x0033;
}

