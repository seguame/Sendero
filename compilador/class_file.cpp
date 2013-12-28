#include "class_file.h"

ClassFile::ClassFile():
    cuerpo(NULL)
{
    cuerpo = (CuerpoClass*)malloc(sizeof(CuerpoClass));

    cuerpo->magic_number    = 0xCAFEBABE;
    cuerpo->minor_version   = 0x0000;
    cuerpo->major_version   = 0x0033;

    cuerpo->access_flags    = ACC_PUBLIC | ACC_SUPER | ACC_FINAL;
    cuerpo->super_class     = 0x0000;
    cuerpo->interfaces      = 0x0000;
}

