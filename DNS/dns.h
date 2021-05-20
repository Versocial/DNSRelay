#ifndef DNSH
#define DNSH

struct FLAG
{
    unsigned qr : 1;     /* response flag */
    unsigned opcode : 4; /* purpose of message */
    unsigned aa : 1;     /* authoritive answer */
    unsigned tc : 1;     /* truncated message */
    unsigned rd : 1;     /* recursion desired */
    unsigned ra : 1;     /* recursion available */
    unsigned z : 1;      /* unused bits, must be ZERO */
    unsigned cd : 1;     /* checking disabled by resolver */
    unsigned ad : 1;     /* authentic data from named */
    unsigned rcode : 4;  /* response code */
};

struct HEADER 
{
    unsigned id : 16;    /* query identification number */
    struct FLAG flag;   /* flag of 16 bits */
    unsigned  qdcount : 16;       /* number of question entries */
    unsigned  ancount : 16;       /* number of answer entries */
    unsigned  nscount : 16;       /* number of authority entries */
    unsigned  arcount : 16;       /* number of resource entries */
};

struct DNS
{
    struct HEADER head;


};





#endif // !DNSH