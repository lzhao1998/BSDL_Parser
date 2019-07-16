#ifndef Error_H
#define Error_H

//TOKEN ERROR
#define ERR_SYSTEM_ERROR          10
#define ERR_INVALID_INSTRUCTION   20
#define ERR_INVALID_OPERAND       21
#define ERR_INVALID_STRING		    30
#define ERR_INVALID_CHAR	   	    32
#define ERR_INVALID_INTEGER       41
#define ERR_INVALID_FLOAT         42
#define ERR_INVALID_IDENTIFIER    50
#define ERR_INVALID_OPERATOR      51


//FILE ERROR
#define ERR_FILE_NOT_EXISTS       80
#define ERR_FILE_INVALID          81
#define ERR_INVALID_COMMEND_LINE  82
#define ERR_INVALID_LINE          83 //temporary name only
#define ERR_VHDL_IDENTIFIER       85
#define ERR_COMPONENT_NAME_FORMAT 90
#define ERR_GENERIC_PARAMETER     91
#define ERR_USE_STATEMENT         92
#define ERR_PORT_DESCRIPTION      93

#endif // Error_H
