#ifndef CLASS_FILE_H
#define CLASS_FILE_H

#include <cstdlib>

#define ACC_PUBLIC          0x0001
#define ACC_PRIVATE         0x0002
#define ACC_PROTECTED       0x0004
#define ACC_STATIC          0x0008
#define ACC_FINAL           0x0010
#define ACC_SYNCHRONIZED    0x0020
#define ACC_NATIVE          0x0100
#define ACC_ABSTRACT        0x0400
#define ACC_STRICT          0x0800

typedef unsigned char   u1;
typedef unsigned short  u2;
typedef unsigned int    u4;

typedef struct
{
    u1   tag;
    u1*  info;
}cp_info;

typedef struct
{
    u1 tag;
    u2 name_index;
}CONSTANT_Class_info;

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
