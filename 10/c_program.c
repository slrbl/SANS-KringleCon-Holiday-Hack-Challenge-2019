#include <stdio.h>
#include <windows.h>
#include <Wincrypt.h>
#include <strsafe.h>
#pragma comment( lib, "Advapi32.lib" )
#pragma comment( lib, "Rpcrt4.lib" )
#pragma comment( lib, "Ole32.lib" )
#pragma comment( lib, "Winhttp.lib" )
#pragma comment( lib, "Crypt32.lib" )

#pragma comment(lib, "crypt32.lib")

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <Wincrypt.h>


long multiply(long long x, long long y) {
    return  (long)x * y;
}


void MyHandleError(LPTSTR psz)
{
    _ftprintf(stderr, TEXT("An error occurred in the program. \n"));
    _ftprintf(stderr, TEXT("%s\n"), psz);
    _ftprintf(stderr, TEXT("Error number %x.\n"), GetLastError());
    //_ftprintf(stderr, TEXT("Program terminating. \n"));
  
} // End of MyHandleError

int main()
{


  
    int start_time, end_time;
    printf("Enter the start timestamp value:");
    scanf("%d", &start_time);
    printf("Enter the end timestamp value:");
    scanf("%d", &end_time);

    long timestamp = start_time;// 1575663640;
    int KEY_LENGTH = 8;
    int SUCCCCESS = 0;

    while (timestamp< end_time && SUCCCCESS==0)

    {

        printf("\nTrying the the seed value is %i\n", timestamp);
        long seed = timestamp;

    long key=0;
    char *hex_chunk="";
    char result[16]="";
    BYTE DesKeyBlob[] = {
    0x08,0x02,0x00,0x00,0x01,0x66,0x00,0x00, // BLOB header 
    0x08,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

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
        //printf("The key value is %s\n", last_two);
        strcat(result, last_two);
        int hex_v;// = hex2int(last_two);
        sscanf(last_two, "%x", &hex_v);
        //printf("%d\n", i + 12);
        DesKeyBlob[i+12] = hex_v;
    }




    printf("The key value is %s\n", result);

    FILE* fp = fopen("c_program.obj", "r");
    DWORD numbytes;
    numbytes = ftell(fp);
    FILE* hSourceFile = NULL;
    FILE* hDestinationFile = NULL;
    HCRYPTPROV hProv;
    HCRYPTKEY hKey = 0;
    BYTE* pbKeyBlob = NULL;
    DWORD dwBlobLen;

    
    int ret;
    LPCTSTR pszContainerName = TEXT("My Sample Key Container");



    if (CryptAcquireContext(
        &hProv,
        pszContainerName,
        NULL,
        PROV_RSA_FULL,
        0))
    {
        _tprintf(
            TEXT("A crypto context with the %s key container ")
            TEXT("has been acquired.\n"),
            pszContainerName);
    }
    else
    {
        //-----------------------------------------------------------
        // Some sort of error occurred in acquiring the context. 
        // This is most likely due to the specified container 
        // not existing. Create a new key container.
        if (GetLastError() == NTE_BAD_KEYSET)
        {
            if (CryptAcquireContext(
                &hProv,
                pszContainerName,
                NULL,
                PROV_RSA_FULL,
                CRYPT_NEWKEYSET))
            {
                _tprintf(TEXT("A new key container has been ")
                    TEXT("created.\n"));
            }
            else
            {
                MyHandleError(TEXT("Could not create a new key ")
                    TEXT("container.\n"));
            }
        }
        else
        {
            MyHandleError(TEXT("CryptAcquireContext failed.\n"));
        }
    }
    
    BYTE DesKeyBlobOld[20] = {
    0x08,0x02,0x00,0x00,0x01,0x66,0x00,0x00, // BLOB header 
    0x08,0x00,0x00,0x00                     // key length, in bytes  // DES key with parity
    };



    if (!CryptImportKey(
        hProv,
        DesKeyBlob,
        sizeof(DesKeyBlob),
        0,
        CRYPT_EXPORTABLE,
        &hKey))
    {
        printf("Error 0x%08x in importing the Des key \n",
            GetLastError());
    }


    //"ElfUResearchLabsSuperSledOMaticQuickStartGuideV1.2.pdf.enc",
    hSourceFile = CreateFile(
        
        "ElfUResearchLabsSuperSledOMaticQuickStartGuideV1.2.pdf.enc",
        FILE_READ_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (INVALID_HANDLE_VALUE != hSourceFile)
    {
        _tprintf(
            TEXT("The source encrypted file, %s, is open. \n"),
            "encrypted_hello");
    }
    else
    {
        MyHandleError(
            TEXT("Error opening source plaintext file!\n")
            //GetLastError()
            );
    }
    char* dest="";// = "result/decrypted_dest.txt";
    //strcat(dest, itoa(j));
    sprintf(dest, "result/decrypted_dest_%d.pdf", timestamp);
    //---------------------------------------------------------------
// Open the destination file. 
    hDestinationFile = CreateFile(
        dest,
        FILE_WRITE_DATA,
        FILE_SHARE_READ,
        NULL,
        OPEN_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (INVALID_HANDLE_VALUE != hDestinationFile)
    {
        _tprintf(
            TEXT("The destination file, %s, is open. \n"),
            "decrypted_dest.txt");
    }
    else
    {
        MyHandleError(
            TEXT("Error opening destination file!\n")
            //GetLastError()
            );
    }







#define ENCRYPT_BLOCK_SIZE 8 
    PBYTE pbBuffer = NULL;
    DWORD dwBlockLen = NULL;;
    DWORD dwCount=NULL;
    DWORD dwBufferLen=NULL;

    dwBlockLen =  10000000 - 10000000 % ENCRYPT_BLOCK_SIZE;
    dwBufferLen = dwBlockLen;

    if (!(pbBuffer = (PBYTE)malloc(dwBufferLen)))
    {
        MyHandleError(TEXT("Out of memory!\n"), E_OUTOFMEMORY);
        goto Exit_MyDecryptFile;
    }

    //---------------------------------------------------------------
    // Decrypt the source file, and write to the destination file. 
    boolean fEOF;
    fEOF = FALSE;
    do
    {
        
        //-----------------------------------------------------------
        // Read up to dwBlockLen bytes from the source file. 
        if (!ReadFile(
            hSourceFile,
            pbBuffer,
            dwBlockLen,
            &dwCount,
            NULL))
        {
            MyHandleError(
                TEXT("Error reading from source file!\n")
                //GetLastError()
            );
            goto Exit_MyDecryptFile;
        }

        if (dwCount <= dwBlockLen)
        {
            fEOF = TRUE;
        }

        //-----------------------------------------------------------
        // Decrypt the block of data. 
        if (!CryptDecrypt(
            hKey,
            0,
            fEOF,
            0,
            pbBuffer,
            &dwCount))
        {
            MyHandleError(
                TEXT("Error during CryptDecrypt!"),
                GetLastError()
            );
            goto Exit_MyDecryptFile;
        }
        else
        {
            printf("BINGO!");
            SUCCCCESS = 1;
        }

        //-----------------------------------------------------------
        // Write the decrypted data to the destination file. 
        if (!WriteFile(
            hDestinationFile,
            pbBuffer,
            dwCount,
            &dwCount,
            NULL))
        {
            MyHandleError(
                TEXT("Error writing ciphertext.\n")
                //GetLastError()
            );
            goto Exit_MyDecryptFile;
        }

        //-----------------------------------------------------------
        // End the do loop when the last block of the source file 
        // has been read, encrypted, and written to the destination 
        // file.
    } while (!fEOF);


Exit_MyDecryptFile:

    //---------------------------------------------------------------
    // Free the file read buffer.
    if (pbBuffer)
    {
        free(pbBuffer);
    }

    //---------------------------------------------------------------
    // Close files.
    if (hSourceFile)
    {
        CloseHandle(hSourceFile);
    }

    if (hDestinationFile)
    {
        CloseHandle(hDestinationFile);
    }

    //-----------------------------------------------------------
    // Release the hash object. 


    //---------------------------------------------------------------
    // Release the session key. 
    if (hKey)
    {
        if (!(CryptDestroyKey(hKey)))
        {
            MyHandleError(
                TEXT("Error during CryptDestroyKey!\n"),
                GetLastError());
        }
    }

    //---------------------------------------------------------------
    // Release the provider handle. 
    if (hProv)
    {
        if (!(CryptReleaseContext(hProv, 0)))
        {
            MyHandleError(
                TEXT("Error during CryptReleaseContext!\n"),
                GetLastError());
        }
    }

   // return fReturn;
    timestamp = timestamp + 1;

    }

    /*
    ret=CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, 0);
    printf("The key value is %d\n", ret);
   
    
    ret=CryptImportKey(hProv, result, 8, 0, 0, &hKey);
    printf("The key value is %d\n", ret);
    ret=CryptDecrypt(hKey, 0, 1, 0, fp, &numbytes);
    printf("The key value is %d\n", ret);*/

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