#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>

long multiply(long long x, long long y) {
    return  (long)x * y;
}

//http://winapi.freetechsecrets.com/win32/WIN32CryptDecrypt_New__Windows_NT_New.htm
//http://winapi.freetechsecrets.com/win32/WIN32CryptImportKey_New__Windows_NT_N.htm
//http://winapi.freetechsecrets.com/win32/WIN32CryptAcquireContext_New__Windows.htm


int main()
{
    int KEY_LENGTH = 8;

    long timestamp;
    long seed;
    timestamp = 1578088799;
    seed = timestamp;
    long key;
    char *hex_chunk;
    char result[16];

    for (int i = 0; i < KEY_LENGTH; i++)
    {
        hex_chunk = "";
        key = multiply(seed, 0x343FD);
        key = key + 0x269EC3;
        seed = key;
        key = key >> 0x10;
        key = key & 0x7FFF;
        sprintf(hex_chunk,"%x", key);
        int len = strlen(hex_chunk);
        const char* last_two = &hex_chunk[len - 2];
        printf("The key value is %s\n", last_two);
        strcat(result, last_two);
    }


    printf("The key value is %s\n", result);

    FILE* fp = fopen("c_program.obj", "r");
    DWORD numbytes;
    numbytes = ftell(fp);
    FILE* hSourceFile = NULL;
    HCRYPTPROV hProv = 0;
    HCRYPTKEY hKey = 0;
    BYTE* pbKeyBlob = NULL;
    DWORD dwBlobLen;

    
    int ret;

    ret=CryptAcquireContext(&hProv, "0", 1, PROV_RSA_FULL, 0);
    printf("The key value is %d\n", ret);
    ret=CryptImportKey(hProv, result, 8, 0, 0, &hKey);
    printf("The key value is %d\n", ret);
    ret=CryptDecrypt(hKey, 0, 1, 0, fp, &numbytes);
    printf("The key value is %d\n", ret);



    //CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
    





    /*
    key = multiply( timestamp , 0x343FD);
    key = key + 0x269EC3;
    key = key >> 0x10;
    key = key & 0x7FFF;

    printf("The timestamp you input is %d .\n", timestamp);
    printf("The key value is %d\n", key);
    printf("The key value is %x\n", key);*/
    return 0;
}