#include "huffman.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define N 256
#define Maxsize 80
#define SOME 1
#define Empty 0
#define FULL -1

typedef unsigned long int WeightType;
typedef unsigned char MyType;
typedef struct    //��������
{
    MyType ch;    //���ַ�
    WeightType weight;    /* ������Ÿ�������Ȩֵ */
    int parent, LChild, RChild;    /*ָ��˫�ס����ӽ���ָ�� */
} HTNode;

typedef struct    //����
{
    int tag;
    int front;
    int rear;
    MyType length;
    char elem[Maxsize];
} SeqQueue;

void writeFile();
void printHFM(HTNode * ht, int n);
void code(char **hc, int n, unsigned char *ch);

int InitQueue(SeqQueue * Q)
{
    if (!Q)
        return 1;
    Q->tag = Empty;
    Q->front = Q->rear = 0;
    Q->length = 0;

    return 0;
}

int In_seqQueue(SeqQueue * Q, char x)
{
    if (Q->front == Q->rear && Q->tag == SOME)
        return FULL;    //full

    Q->elem[Q->rear] = x;    // printf("in = %c",x);
    Q->rear = (Q->rear + 1) % Maxsize;
    Q->length++;
    Q->tag = SOME;
    return SOME;
}

int Out_Queue(SeqQueue * Q, char *x)
{
    if (Q->tag == Empty)
        return Empty;

    *x = Q->elem[Q->front];
    Q->length--;
    Q->front = (Q->front + 1) % Maxsize;

    if (Q->front == Q->rear)
        Q->tag = Empty;

    return SOME;
}

/* ------------------�����Ƕ��еĲ���------------------------- */

void SelectMinTree(HTNode * ht, int n, int *k)
{
    int i, temp;
    WeightType min;

    // printf(" Selecting����n= %d",n);
    for (i = 0; i <= n; i++) {
        if (0 == ht[i].parent) {
            min = ht[i].weight;    //init min
            temp = i;
            break;
        }
    }
    for (i++; i <= n; i++) {
        if (0 == ht[i].parent && ht[i].weight < min) {
            min = ht[i].weight;
            temp = i;
        }
    }
    *k = temp;
}

// �Թ����������򣬲�ͳ��Ҷ������
int SortTree(HTNode * ht)
{
    short i, j;
    HTNode tmp;

    for (i = N - 1; i >= 0; i--) {
        for (j = 0; j < i; j++)
            if (ht[j].weight < ht[j + 1].weight) {
                tmp = ht[j];
                ht[j] = ht[j + 1];
                ht[j + 1] = tmp;
            }
    }
    for (i = 0; i < N; i++)
        if (0 == ht[i].weight)
            return i;
    return i;    //����Ҷ�Ӹ���
}

//�������0-1�ַ������
char **CrtHuffmanCode(HTNode * ht, short LeafNum)
    /*��Ҷ�ӽ�㵽����������ÿ��Ҷ�ӽ���Ӧ�Ĺ���������*/
{
    char *cd, **hc;    //����
    int i, start, p, last;

    hc = (char **)malloc((LeafNum) * sizeof(char *));    /*����n�������ͷָ�� */

    if (1 == LeafNum)    //ֻ��һ��Ҷ�ӽڵ�ʱ
    {
        hc[0] = (char *)malloc((LeafNum + 1) * sizeof(char));
        strcpy(hc[0], "0");
        return hc;
    }

    cd = (char *)malloc((LeafNum + 1) * sizeof(char));    /*������ǰ����Ĺ����ռ� */
    cd[LeafNum] = '\0';    /*����������λ��ű��룬���ȴ�ű�������� */
    for (i = 0; i < LeafNum; i++) {    /*��n��Ҷ�ӽ���Ӧ�Ĺ��������� */
        start = LeafNum;    /*��ʼ��������ʼָ�� */
        last = i;
        for (p = ht[i].parent; p != 0; p = ht[p].parent) {    /*��Ҷ�ӵ����������� */
            if (ht[p].LChild == last)
                cd[--start] = '0';    /*���֧��0 */
            else
                cd[--start] = '1';    /*�ҷ�֧��1 */
            last = p;
        }
        hc[i] = (char *)malloc((LeafNum - start) * sizeof(char));    /*Ϊ��i���������ռ� */
        strcpy(hc[i], &cd[start]);
        //
        printf("%3d�� %3c �볤:%2d;����:%s\n", ht[i].ch, ht[i].ch,
                LeafNum - start, &cd[start]);
    }    //getchar();
    free(cd);    // Printcode(hc,n);
    return hc;
}

HTNode *CreatHFM(FILE * fp, short *n, WeightType * FileLength)
{
    HTNode *ht = NULL;
    int i, m, s1, s2;
    MyType ch;

    ht = (HTNode *)malloc(2 * N * sizeof(HTNode));
    if (!ht)
        exit(1);

    for (i = 0; i < N; i++) {
        ht[i].weight = 0;
        ht[i].ch = (MyType)i;    /*1-n��ch Ϊ�ַ�����ʼ�� */
    }

    for (*FileLength = 0; !feof(fp); ++(*FileLength)) {
        ch = fgetc(fp);    //fread(&ch,1,1,fp);
        ht[ch].weight++;    //printf("ht[%c].weight= %ld\n",0,ht[0].weight);
    }
    --(*FileLength);    //ȥ���ļ�������ĳ���
    *n = SortTree(ht);
    m = *n * 2 - 1;    //free(&ht[m+1]);
    //printf("Ҷ�Ӹ���= %d\n",*n);//getchar();
    if (1 == *n) {
        ht[0].parent = 1;
        return ht;
    }
    else if (0 > *n)
        return NULL;

    for (i = m - 1; i >= 0; i--) {
        ht[i].LChild = 0;
        ht[i].parent = 0;
        ht[i].RChild = 0;
    }
    /* ------------��ʼ����ϣ���Ӧ�㷨����1--------- */
    for (i = *n; i < m; i++)    //������Ҷ�ӽ��,����������
    {    //��ht[0]~ht[i-1]�ķ�Χ��ѡ������parentΪ0��weight��С�Ľ�㣬����ŷֱ�ֵ��s1��s2����
        SelectMinTree(ht, i - 1, &s1);
        ht[s1].parent = i;
        ht[i].LChild = s1;

        SelectMinTree(ht, i - 1, &s2);
        ht[s2].parent = i;
        ht[i].RChild = s2;

        ht[i].weight = ht[s1].weight + ht[s2].weight;
    }    /*��������������� */// puts(" over^_^");

    return ht;
}

//�Ӷ�����ȡ8���ַ���0��1����ת����һ���ֽ�
MyType GetBits(SeqQueue * Q)
{
    MyType i, bits = 0;
    char x;

    for (i = 0; i < 8; i++) {
        if (Out_Queue(Q, &x) != Empty) {    //printf("%c",x);
            if ('0' == x)
                bits = bits << 1;
            else
                bits = (bits << 1) | 1;
        }
        else
            break;
    }    //printf(" bits=%d\n",bits);puts("");

    return bits;
}

//�������̣����볤��
    void
MaxMinLength(FILE * File, HTNode * ht, char **hc, short NLeaf, MyType * Max,
        MyType * Min)
{
    int i;
    MyType length;

    *Max = *Min = strlen(hc[0]);
    for (i = 0; i < NLeaf; i++) {
        length = strlen(hc[i]);
        fwrite(&ht[i].ch, sizeof(MyType), 1, File);    //�ַ��Ͷ�Ӧ��
        fwrite(&length, sizeof(MyType), 1, File);    //���볤��д���ļ�
        if (length > *Max)
            *Max = length;
        if (length < *Min)
            *Min = length;
    }
}

//�ѳ��ֹ����ַ��������ѹ��д���ļ�
short CodeToFile(FILE * fp, char **hc, short n, SeqQueue * Q, MyType * length)
{
    int i;
    char *p;
    MyType j, bits;
    short count = 0;

    for (i = 0; i < n; i++)    // ��n��Ҷ��ѹ����д���ļ�
    {
        for (p = hc[i]; '\0' != *p; p++)
            In_seqQueue(Q, *p);

        while (Q->length > 8) {    // puts("����");
            bits = GetBits(Q);    //����8��Ԫ��
            fputc(bits, fp);    //fwrite(&bits,sizeof(char),1,fp);
            //printf("ѹ�ַ� %c\n",bits);
            count++;
        }
    }    // printf("�������ʣ: %d\n",Q->length);

    *length = Q->length;
    i = 8 - *length;
    bits = GetBits(Q);    //ȡ8������Ӳ���
    for (j = 0; j < i; j++)
        bits = bits << 1;    //printf("ѹ �ַ� %c\n",bits);
    fputc(bits, fp);    //fwrite(&bits,sizeof(char),1,fp);
    count++;    //printf(" ָ �� ��%d \n",ftell(fp));

    InitQueue(Q);
    return count;
}

//ѹ��
void Compress(const char *desFile, const char *rename)
{
    MyType maxLen, minLen, ch, bits, n, finalLength;
    int i;
    short LeafNum, codeNum;
    WeightType count = 0, Length = 0, FileLength;
    FILE *fp, *compressFile;
    SeqQueue *Q;
    HTNode *ht = NULL;
    char **hc = NULL, **Map = NULL, *p;

#if 0
    printf("filename to be compressed:");
    scanf("%s", desFile);
    //getchar();//fflush(stdin);//������뻺������

    printf("filename after compressed:");
    scanf("%s", rename);
    if ('\0' == rename[0]) {
        printf("���벻��Ϊ�գ�����������:");
        scanf("%s", rename);
    }
#endif

    compressFile = fopen(rename, "wb");
    fp = fopen(desFile, "rb");    //ԭ�ļ�

    if (!fp || !compressFile) {
        puts("Cannot open file.");
        return;
    }

    ht = CreatHFM(fp, &LeafNum, &FileLength);    //������������,ͳ��Ҷ�Ӹ�����ԭ�ļ�����
    if (!FileLength) {    // printf("�ļ�Ϊ�գ�����ѹ��...");
        fclose(fp);
        fclose(compressFile);
        free(ht);
        return;
    }
    Q = (SeqQueue *)malloc(sizeof(SeqQueue));
    InitQueue(Q);    //SEEK_SET:�ļ���ͷ SEEK_CUR:��ǰλ�� SEEK_END:�ļ���β
    hc = CrtHuffmanCode(ht, LeafNum);    //ȡ�ù�����0��1����,hc�ĳ���ΪLeafNum
    //MapΪ��ȡ����ö�λ���ٽ���ȫ��(256��)//
    Map = (char **)malloc(N * sizeof(char *));    //�ַ������
    if (!Map) {
        puts("����ռ�ʧ��");
        return;
    }

    for (i = 0; i < N; i++)    //��ʼ��
        Map[i] = NULL;

    for (i = 0; i < LeafNum; i++)    // ��λ������ָ������Map[256]
        Map[(int)(ht[i].ch)] = hc[i];

    fseek(compressFile, sizeof(WeightType) + sizeof(short) + 6 * sizeof(MyType), SEEK_SET);    //��ռ��λ��
    //��ռ��λ��,������ѹ��Ҷ�ӱ���ʣ����������볤//getchar();

    MaxMinLength(compressFile, ht, hc, LeafNum, &maxLen, &minLen);    //�����봮����,˳����д�ַ���Ӧ���볤
    // for(i = 0;i < LeafNum;i++)
    // if(ht[i].ch == (MyType)1)
    // printf("char %c %s\n",ht[i].ch,hc[i]);

    free(ht);    //getchar();printf("��봮����: %d\n",MaxCode);printf("ftell = %d\n",ftell(compressFile));
    codeNum = CodeToFile(compressFile, hc, LeafNum, Q, &finalLength);    //���ַ�ת��������Ʊ���д���ļ�,����ѹ�ɶ��ٸ�

    rewind(compressFile);    //ʹ�ļ�ָ���Ƶ���ʼprintf("ftelll = %d\n",ftell(compressFile));
    fseek(compressFile, sizeof(WeightType) + sizeof(MyType), SEEK_SET);
    fwrite(&LeafNum, sizeof(short), 1, compressFile);    //д��Ҷ�Ӹ���
    fwrite(&maxLen, sizeof(MyType), 1, compressFile);    //��봮����
    fwrite(&minLen, sizeof(MyType), 1, compressFile);    //����봮����
    //printf("MaxLen %d Min %d ftell = %d\n",maxLen,minLen,ftell(compressFile));getchar();
    fwrite(&codeNum, sizeof(short), 1, compressFile);    //��дҶ�ӱ���ѹ���ٸ�
    fwrite(&finalLength, sizeof(MyType), 1, compressFile);    //���ʣ
    //printf("Ҷ�ӹ�ѹ��%d��,���ʣ%d��\n\n",codeNum,finalLength);

    fseek(compressFile, 2 * LeafNum * sizeof(MyType) + codeNum, SEEK_CUR);
    //printf("��ʼ����ftell= %d ԭ�ĳ�%d\n",ftell(compressFile),FileLength);getchar();
    fseek(fp, 0, SEEK_SET);
    printf("Please wait a minute,compressing...");

    while (count < FileLength) {
        ch = fgetc(fp);    //fread(&ch,sizeof(MyType),1,fp);
        ++count;    // printf("Read�� : %c ֵ %d %s\n",ch,ch,Map[ch]);
        for (p = Map[ch]; *p != '\0'; p++)
            In_seqQueue(Q, *p);
        //printf("\nQlength: %d\n",Q->length);

        while (Q->length > 8)    // printf("OutQueue: ");
        {
            bits = GetBits(Q);    //����8��Ԫ��,�ϳ�һ���ֽ�
            fputc(bits, compressFile);    //fwrite(&bits,1,1,compressFile);
            Length++;    // printf("ѹ��%c\n",bits);
        }
    }    //end of while
    //���һ��bits ;
    finalLength = Q->length;    //printf("���ʣQlength:%d\n",Q->length);
    n = 8 - finalLength;
    bits = GetBits(Q);
    //printf("���Qlength:%d\n",Q->length);
    for (i = 0; i < n; i++)
        bits = bits << 1;    //�ԡ�0����
    //fputc(bits,compressFile);
    fwrite(&bits, sizeof(MyType), 1, compressFile);    //printf("\nfinalѹ��%c\n",bits);
    Length++;

    rewind(compressFile);    //ԭ�ļ���fwrite(&FileLength,sizeof(WeightType),1,compressFile);
    fwrite(&Length, sizeof(WeightType), 1, compressFile);    //ѹ����ĳ�
    fwrite(&finalLength, sizeof(char), 1, compressFile);    //���Ĵ���

    Length = Length + 12 + codeNum;    //printf("ԭ�ļ��� %ld;ѹ���� %ld\n",FileLength,Length);

    if (Length >= FileLength)
        puts("\nCompression rate: 0.0%");
    else
        printf("\nCompression rate: %.2lf%c\n",
                (double)((FileLength -
                        Length) / (double)FileLength) * 100.0, '%');

    fclose(fp);
    fclose(compressFile);
    free(Q);
    free(hc);
    free(Map);
}

//�Ѷ������ַ���ת����8��0��1�ַ��������
void ToQueue(SeqQueue * Q, MyType ch)
{
    int i;
    MyType temp;

    for (i = 0; i < 8; i++) {
        temp = ch << i;
        temp = temp >> 7;
        if (1 == temp)
            In_seqQueue(Q, '1');    //printf("1");
        else
            In_seqQueue(Q, '0');    //printf("0");
    }    //puts("");
}

//��ѹ��
void UnCompress(const char *CName, const char *UName)
{
    MyType *str, MaxLength, MinLength, ch, *num, finalLength = 0, final = 0;
    FILE *cf, *uf;
    short NLeaf, Ncom;
    char **hc, *p, x, *buf;
    SeqQueue *Q = NULL;
    int i, j;
    WeightType srcLen = 0, thisFile = 0;

#if 0
    fflush(stdin);    //������뻺������
    printf("filename to be uncompressed:");
    scanf("%s", CName);
    if (CName[0] == '\0') {
        printf("try again:");
        scanf("%s", CName);
        fflush(stdin);
    }
    fflush(stdin);    //������뻺������
    printf("filename after uncompressed:");
    scanf("%s", UName);
    if (UName[0] == '\0') {
        printf("try again:");
        scanf("%s", UName);
    }
#endif

    cf = fopen(CName, "rb");
    uf = fopen(UName, "wb");
    if (!cf || !uf) {
        puts("Cannot open files.");
        return;
    }

    fread(&srcLen, sizeof(WeightType), 1, cf);    //printf("ѹ���ļ�����%ld\n",srcLen);
    fread(&finalLength, 1, 1, cf);    // printf("ԭ�ļ�ѹʣ:%d��\n",finalLength);
    fread(&NLeaf, sizeof(short), 1, cf);    //printf("Ҷ��:%d��\n",NLeaf);
    fread(&MaxLength, sizeof(MyType), 1, cf);
    fread(&MinLength, sizeof(MyType), 1, cf);
    //printf("MaxLength = %d;minLen= %d,Leaf= %d\n",MaxLength,MinLength,NLeaf);getchar();
    Q = (SeqQueue *)malloc(sizeof(SeqQueue));

    buf = (char *)malloc((2 + MaxLength * sizeof(char)));
    str = (MyType *)malloc(NLeaf * sizeof(MyType));
    num = (MyType *)malloc(NLeaf * sizeof(MyType));
    //ѹ��Ҷ������x�����ʣ��
    if (!Q || !str || !num || !buf) {
        puts("Memery error.");
        exit(1);
    }
    InitQueue(Q);    //��ʼ��

    fread(&Ncom, sizeof(short), 1, cf);
    fread(&final, sizeof(MyType), 1, cf);

    for (i = 0; i < NLeaf; i++)    //��ȡҶ�Ӽ����볤
    {
        fread(&str[i], sizeof(MyType), 1, cf);
        fread(&num[i], sizeof(MyType), 1, cf);
        //printf("chars= %c;num= %d\n",Chars[i],CHlength[i]);
    }
    //printf("read ftell= %d\n",ftell(cf));//getchar();
    // printf("\nҶ������ѹ����%d��,���ʣ%d��\n",Ncom,final);

    hc = (char **)malloc((NLeaf) * sizeof(char *));    //hcΪ������ָ������
    if (!hc)
        exit(1);

    --Ncom;    //printf("��ʼȡҶ�ӱ���ftell = %d Ncom=%d\n",ftell(cf),Ncom);
    for (j = i = 0; i < Ncom; i++) {
        ch = fgetc(cf);    //fread(&ch,1,1,cf);
        ToQueue(Q, ch);    //printf("Q->Len= %d\n",Q->length);
        while (Q->length > MaxLength) {
            hc[j] = p = (char *)malloc(1 + num[j]);
            for (ch = 0; ch < num[j]; ch++) {
                Out_Queue(Q, &x);
                *p++ = x;
            }
            *p = '\0';    //printf("%c�� ��%s\n",str[j],hc[j]);
            j++;
        }
    }
    ch = fgetc(cf);    //fread(&ch,1,1,cf);//���һ��
    ToQueue(Q, ch);
    final = 8 - final;
    while (Q->length > final) {
        p = hc[j] = (char *)malloc(1 + num[j]);
        for (ch = 0; ch < num[j]; ch++) {
            Out_Queue(Q, &x);
            *p++ = x;
        }
        *p = '\0';    //printf("%c����%s\n",str[j],hc[j]);getchar();
        j++;
    }

    InitQueue(Q);
    --srcLen;
    --MinLength;
    //printf("��ʼ��ѹ����%d ,ѹ��= %d\n",ftell(cf),srcLen);getchar();
    printf("Please wait a minute,uncompressing...");
    while (thisFile < srcLen) {
        // printf("QLength= %d\n",Q->length);
        ch = fgetc(cf);    //fread(&ch,sizeof(MyType),1,cf);
        ToQueue(Q, ch);
        thisFile++;    // printf("char:%c",ch);
        //���˺���г����봮�����ֵ

        while (Q->length > MaxLength) {
            for (i = 0; i < MinLength; i++) {
                Out_Queue(Q, &x);
                buf[i] = x;
            }
            for (; i < MaxLength; i++) {
                Out_Queue(Q, &x);
                buf[i] = x;
                buf[i + 1] = '\0';
                for (j = 0; j < NLeaf; j++) {
                    if (i + 1 == num[j]
                            && 0 == strcmp(hc[j], buf)) {
                        ch = str[j];    //printf("���� %s\n",buf);
                        fputc(ch, uf);    //fwrite(&ch,1,1,uf);////printf("�� ����%c\n",ch);
                        break;
                    }
                }
                if (j < NLeaf)
                    break;
            }
        }    //while MaxLength
    }

    ch = fgetc(cf);    //fread(&ch,1,1,cf);//��ȡ���һ��ѹ����getchar();
    ToQueue(Q, ch);

    finalLength = 8 - finalLength;
    while (Q->length > finalLength) {
        for (i = 0; i < MinLength; i++) {
            Out_Queue(Q, &x);
            buf[i] = x;
        }
        for (; i < MaxLength; i++) {
            Out_Queue(Q, &x);
            buf[i] = x;
            buf[i + 1] = '\0';
            for (j = 0; j < NLeaf; j++) {
                if (i + 1 == num[j] && 0 == strcmp(hc[j], buf)) {
                    ch = str[j];    //printf("�� %s\n",buf);
                    fputc(ch, uf);    //fwrite(&ch,1,1,uf);//printf("�� ����%c\n",ch);
                    break;
                }
            }
            if (j < NLeaf)
                break;
        }
    }
    //printf("Q->length= %d\n",Q->length);printf("ftell= %d\n",ftell(uf));
    free(Q);
    free(str);
    free(num);
    free(hc);
    fclose(uf);
    fclose(cf);
}

#if 0
int HFM_main(int argc, char *argv[])
{
    char choice, blank[] = " ";
    // writeFile();
    system("color 8a");
    while (1) {
        //system("clear");    //����
        puts(" * * * *Welcome use huffman encoder\\decoder* * *");
        puts(" **********************^_^***********************");
        puts(" * *");
        printf(" * %s 1 ]. Compress %s*\n", blank, blank);
        puts(" * *");
        printf(" * %s 2 ]. Uncompress%s *\n", blank, blank);
        puts(" * *");
        printf(" * %s 3 ]. Exit ^_^ %s *\n", blank, blank);
        puts(" * *");
        puts(" ************************************************");
        printf
            (" (Apply to text file) Copyright 2011 By Bocai\n");
        printf(" Choose (1 to 3):");
        choice = getchar();

        puts("");
        getchar();
        fflush(stdin);    //������뻺�����򣬷������Ļس�������Ϊ�����β����scanf���룬�˺�����stdio.h��
        switch (choice) {
            case '1':
                Compress();
                printf("Press Enter to continue...");
                getchar();
                break;
            case '2':
                UnCompress();
                printf("\nPress Enter to continue...");
                getchar();
                break;
            case '3':
                return 0;
                break;
        }
    }

    return 0;
}
#endif

