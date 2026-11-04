#include "json_data.h"
#include <string.h>
#include <limits.h>
static void ws(TJ_DOC *d){while(d->pos<d->length&&(d->bytes[d->pos]==' '||d->bytes[d->pos]=='\t'||d->bytes[d->pos]=='\r'||d->bytes[d->pos]=='\n'))d->pos++;}
static int value(TJ_DOC *d,unsigned depth);
static int string(TJ_DOC *d,int key){unsigned i,j;if(d->bytes[d->pos++]!='"')return 0;
 while(d->pos<d->length){unsigned char c=(unsigned char)d->bytes[d->pos++];if(c=='"')return 1;if(c<32||c>126)return 0;
 if(c=='\\'){if(key||d->pos>=d->length)return 0;c=(unsigned char)d->bytes[d->pos++];if(c=='u'){for(j=0;j<4;j++){if(d->pos>=d->length)return 0;i=(unsigned char)d->bytes[d->pos++];if(!((i>='0'&&i<='9')||(i>='a'&&i<='f')||(i>='A'&&i<='F')))return 0;}}else if(!strchr("\"\\/bfnrt",c))return 0;}}
 return 0;
}
static int add(TJ_DOC *d,char type){unsigned i;if(d->count==TJ_MAX)return -1;i=d->count++;memset(&d->node[i],0,sizeof(d->node[i]));d->node[i].type=type;d->node[i].start=d->pos;d->node[i].first=d->count;return (int)i;}
static int value(TJ_DOC *d,unsigned depth){int id,k,v;unsigned past,j;char c;TJ_NODE *n;if(depth>16)return -1;ws(d);if(d->pos>=d->length)return -1;c=d->bytes[d->pos];
 id=add(d,c=='{'?'o':c=='['?'a':c=='"'?'s':c>='0'&&c<='9'?'n':'l');if(id<0)return -1;n=&d->node[id];
 if(c=='{'||c=='['){d->pos++;ws(d);if(d->pos<d->length&&d->bytes[d->pos]==(c=='{'?'}':']'))d->pos++;
 else for(;;){ws(d);if(c=='{'){
 if(d->pos>=d->length||d->bytes[d->pos]!='"')return -1;k=add(d,'s');if(k<0||!string(d,1))return -1;d->node[k].end=d->pos;d->node[k].next=d->count;
 for(j=n->first;j<(unsigned)k;){past=d->node[j].end-d->node[j].start;if(past==d->node[k].end-d->node[k].start&&!memcmp(d->bytes+d->node[j].start,d->bytes+d->node[k].start,past))return -1;j=d->node[j+1].next;}
 ws(d);if(d->pos>=d->length||d->bytes[d->pos++]!=':')return -1;}
 v=value(d,depth+1);if(v<0)return -1;n->count++;ws(d);if(d->pos>=d->length)return -1;
 if(d->bytes[d->pos]==(c=='{'?'}':']')){d->pos++;break;}if(d->bytes[d->pos++]!=',')return -1;}
 }else if(c=='"'){if(!string(d,0))return -1;}
 else if(c>='0'&&c<='9'){if(c=='0')d->pos++;else while(d->pos<d->length&&d->bytes[d->pos]>='0'&&d->bytes[d->pos]<='9')d->pos++;}
 else if(d->length-d->pos>=4&&!memcmp(d->bytes+d->pos,"true",4))d->pos+=4;
 else if(d->length-d->pos>=5&&!memcmp(d->bytes+d->pos,"false",5))d->pos+=5;
 else if(d->length-d->pos>=4&&!memcmp(d->bytes+d->pos,"null",4))d->pos+=4;
 else return -1;n->end=d->pos;n->next=d->count;return id;
}
int tj_parse(TJ_DOC *d,size_t length){d->count=0;d->pos=0;d->length=length;if(!length||length>=TJ_BYTES)return 0;if(value(d,0)!=0||d->node[0].type!='o')return 0;ws(d);return d->pos==d->length;}
int tj_string(const TJ_DOC *d,int id,char *out,size_t cap){unsigned i,a,n;if(id<0||(unsigned)id>=d->count||d->node[id].type!='s'||!cap)return 0;a=d->node[id].start+1;n=d->node[id].end-a-1;if(n>=cap)return 0;
 for(i=0;i<n;i++)if(d->bytes[a+i]=='\\'||(unsigned char)d->bytes[a+i]<32||(unsigned char)d->bytes[a+i]>126)return 0;memcpy(out,d->bytes+a,n);out[n]=0;return 1;
}
int tj_get(const TJ_DOC *d,int object,const char *key){unsigned i;char b[128];if(object<0||(unsigned)object>=d->count||d->node[object].type!='o')return -1;
 for(i=d->node[object].first;i<d->node[object].next;){if(!tj_string(d,(int)i,b,sizeof(b)))return -1;if(!strcmp(b,key))return (int)i+1;i=d->node[i+1].next;}return -1;
}
int tj_is(const TJ_DOC *d,int id,const char *s){char b[512];return tj_string(d,id,b,sizeof(b))&&!strcmp(b,s);}
int tj_true(const TJ_DOC *d,int id){return id>=0&&(unsigned)id<d->count&&d->node[id].type=='l'&&d->node[id].end-d->node[id].start==4&&!memcmp(d->bytes+d->node[id].start,"true",4);}
int tj_u64(const TJ_DOC *d,int id,unsigned long long *out){unsigned i;unsigned long long n=0;if(id<0||(unsigned)id>=d->count||d->node[id].type!='n')return 0;
 for(i=d->node[id].start;i<d->node[id].end;i++){unsigned x=(unsigned)(d->bytes[i]-'0');if(n>(ULLONG_MAX-x)/10)return 0;n=n*10+x;}*out=n;return 1;
}
