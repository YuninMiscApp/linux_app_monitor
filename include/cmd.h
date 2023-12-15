
#ifndef CMD_H
#define CMD_H 


#ifdef __cplusplus
extern "C"
{
#endif


#define CMD_EXEC(args...) do { \
    char _cmd_[1024*4] = {0,}; \
    FILE *_fp_ = NULL; \
    memset(_cmd_,0,sizeof(_cmd_));\
    snprintf(_cmd_,sizeof(_cmd_),args); \
    _fp_ = popen (_cmd_, "r"); \
    assert(_fp_);\
    fclose(_fp_);\
    _fp_ = NULL;\
} while(0)


#define CMD_PRINT(args...) do { \
    char _cmd_[1024*4] = {0,}; \
    char _buf_[1024*16] = {0,}; \
    FILE *_fp_ = NULL; \
    char *_result_ = NULL; \
    memset(_cmd_,0,sizeof(_cmd_));\
    snprintf(_cmd_,sizeof(_cmd_),args); \
    _fp_ = popen (_cmd_, "r"); \
    assert(_fp_);\
    memset(_buf_,0,sizeof(_buf_));\
    while ((_result_ = fgets(_buf_, sizeof (_buf_), _fp_)) != NULL) \
        printf("%s",_buf_);\
    fclose(_fp_);\
    _fp_ = NULL;\
} while(0)

#define CMD_GET_BUF(dst_buf,args...) do { \
    char _cmd_[1024*4] = {0,}; \
    FILE *_fp_ = NULL; \
    char *_result_ = NULL; \
    memset(_cmd_,0,sizeof(_cmd_));\
    snprintf(_cmd_,sizeof(_cmd_),args); \
    _fp_ = popen (_cmd_, "r"); \
    assert(_fp_);\
    memset(dst_buf,0,sizeof(dst_buf));\
    _result_ = fgets (dst_buf, sizeof (dst_buf), _fp_);\
    fclose(_fp_);\
    _fp_ = NULL;\
} while(0)



#ifdef __cplusplus
}
#endif

#endif   //end of CMD_H