#ifndef CLASS_FILE_H
#define CLASS_FILE_H

#include <cstdlib>

//Para clases y metodos
#define ACC_PUBLIC          0x0001
#define ACC_PRIVATE         0x0002
#define ACC_PROTECTED       0x0004
#define ACC_STATIC          0x0008
#define ACC_FINAL           0x0010
#define ACC_SUPER           0x0020


//Para usar en el tag de cp_info
#define CONSTANT_Class 7
#define CONSTANT_Fieldref 9
#define CONSTANT_Methodref 10
#define CONSTANT_InterfaceMethodref 11
#define CONSTANT_String 8
#define CONSTANT_Integer 3
#define CONSTANT_Float 4
#define CONSTANT_Long 5
#define CONSTANT_Double 6
#define CONSTANT_NameAndType 12
#define CONSTANT_Utf8 1
#define CONSTANT_MethodHandle 15
#define CONSTANT_MethodType 16
#define CONSTANT_InvokeDynamic 18

typedef unsigned char   u1;
typedef unsigned short  u2;
typedef unsigned int    u4;

typedef struct
{
    u2 name_index;
}CONSTANT_Class_info;

typedef struct
{
     u2 class_index;
     u2 name_and_type_index;
}CONSTANT_Fieldref_info;

typedef struct
{
    u2 class_index;
    u2 name_and_type_index;
}CONSTANT_Methodref_info;

typedef struct
{
    u2 class_index;
    u2 name_and_type_index;
}CONSTANT_InterfaceMethodref_info;

typedef struct
{
    u2 string_index;
}CONSTANT_String_info;

typedef struct
{
    u4 bytes;
}CONSTANT_Integer_info;

typedef struct
{
    u4 bytes;
}CONSTANT_Float_info;

typedef struct
{
    u4 high_bytes;
    u4 low_bytes;
}CONSTANT_Long_info;

typedef struct
{
    u4 high_bytes;
    u4 low_bytes;
}CONSTANT_Double_info;

typedef struct
{
    u2 name_index;
    u2 descriptor_index;
}CONSTANT_NameAndType_info;

typedef struct
{
    u2  length;
    u1* bytes;
}CONSTANT_Utf8_info;

typedef struct
{
    u1 reference_kind;
    u2 reference_index;
}CONSTANT_MethodHandle_info;

typedef struct
{
    u2 descriptor_index;
}CONSTANT_MethodType_info;

typedef struct
{
    u2 bootstrap_method_attr_index;
    u2 name_and_type_index;
}CONSTANT_InvokeDynamic_info;

typedef struct
{
    u1   tag;
    CONSTANT_Class_info                 CC;
    CONSTANT_Fieldref_info              CF;
    CONSTANT_Methodref_info             CM;
    CONSTANT_InterfaceMethodref_info    CI;
    CONSTANT_String_info                CS;
    CONSTANT_Integer_info               Ci;
    CONSTANT_Float_info                 Cf;
    CONSTANT_Long_info                  Cl;
    CONSTANT_Double_info                Cd;
    CONSTANT_NameAndType_info           CNT;
    CONSTANT_Utf8_info                  CU;
    CONSTANT_MethodHandle_info          CH;
    CONSTANT_MethodType_info            CT;
    CONSTANT_InvokeDynamic_info         CID;
}cp_info;


typedef struct
{
    u2 attribute_name_index;
    u4 attribute_length;  // Fijado en 2 para este atributo
    u2 constantvalue_index;
}ConstantValue_attribute;


typedef struct
{
    u2  attribute_name_index;
    u4  attribute_length;
    u1* info; //attribute_length
}attribute_info;

typedef struct
{
    u2              access_flags;
    u2              name_index;
    u2              descriptor_index;
    u2              attributes_count;
    attribute_info* attributes; //attributes_count
}field_info;

typedef struct
{
    u2              access_flags;
    u2              name_index;
    u2              descriptor_index;
    u2              attributes_count;
    attribute_info* attributes; //attributes_count
}method_info;

typedef struct
{
    u4              magic_number;
    u2              minor_version;
    u2              major_version;
    u2              constant_pool_count;
    cp_info*        constant_pool;  //constant_pool_count-1
    u2              access_flags;
    u2              this_class;
    u2              super_class;
    u2              interfaces_count;
    u2*             interfaces;     //interfaces_count
    u2              fields_count;
    field_info*     fields; //fields_count
    u2              methods_count;
    method_info*    methods; //methods_count
    u2              attributes_count;
    attribute_info* attributes; //attributes_count
}CuerpoClass;



class ClassFile
{
public:
    ClassFile();

private:
    CuerpoClass* cuerpo;
};

#endif // CLASS_FILE_H
