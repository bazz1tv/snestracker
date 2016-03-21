/*
  Native File Dialog

  http://www.frogtoss.com/labs
 */

/* only locally define UNICODE in this compilation unit */
#ifndef UNICODE
#define UNICODE
#endif


#include <wchar.h>
#include <stdio.h>
#include <assert.h>
#include <windows.h>

#include "nfd_common.h"


// allocs the space in outPath -- call free()
static void CopyWCharToNFDChar( const wchar_t *inStr, nfdchar_t **outStr )
{
    int inStrCharacterCount = static_cast<int>(wcslen(inStr)); 
    int bytesNeeded = WideCharToMultiByte( CP_UTF8, 0,
                                           inStr, inStrCharacterCount,
                                           NULL, 0, NULL, NULL );    
    assert( bytesNeeded );
    bytesNeeded += 1;

    *outStr = (nfdchar_t*)NFDi_Malloc( bytesNeeded );
    if ( !*outStr )
        return;

    int bytesWritten = WideCharToMultiByte( CP_UTF8, 0,
                                            inStr, -1,
                                            *outStr, bytesNeeded,
                                            NULL, NULL );
    assert( bytesWritten ); _NFD_UNUSED( bytesWritten );
}

/* includes NULL terminator byte in return */
static size_t GetUTF8ByteCountForWChar( const wchar_t *str )
{
    int bytesNeeded = WideCharToMultiByte( CP_UTF8, 0,
                                           str, -1,
                                           NULL, 0, NULL, NULL );
    assert( bytesNeeded );
    return bytesNeeded+1;
}

// write to outPtr -- no free() necessary.  No memory stomp tests are done -- they must be done
// before entering this function.
static int CopyWCharToExistingNFDCharBuffer( const wchar_t *inStr, nfdchar_t *outPtr )
{
    int inStrCharacterCount = static_cast<int>(wcslen(inStr));
    int bytesNeeded = static_cast<int>(GetUTF8ByteCountForWChar( inStr ));

    /* invocation copies null term */
    int bytesWritten = WideCharToMultiByte( CP_UTF8, 0,
                                            inStr, -1,
                                            outPtr, bytesNeeded,
                                            NULL, 0 );
    assert( bytesWritten );

    return bytesWritten;

}


// allocs the space in outStr -- call free()
static void CopyNFDCharToWChar( const nfdchar_t *inStr, wchar_t **outStr )
{
    int inStrByteCount = static_cast<int>(strlen(inStr));
    int charsNeeded = MultiByteToWideChar(CP_UTF8, 0,
                                          inStr, inStrByteCount,
                                          NULL, 0 );    
    assert( charsNeeded );
    assert( !*outStr );
    charsNeeded += 1; // terminator
    
    *outStr = (wchar_t*)NFDi_Malloc( charsNeeded * sizeof(wchar_t) );    
    if ( !*outStr )
        return;        

    int ret = MultiByteToWideChar(CP_UTF8, 0,
                                  inStr, inStrByteCount,
                                  *outStr, charsNeeded);
    (*outStr)[charsNeeded-1] = '\0';

#ifdef _DEBUG
    int inStrCharacterCount = static_cast<int>(NFDi_UTF8_Strlen(inStr));
    assert( ret == inStrCharacterCount );
#else
    _NFD_UNUSED(ret);
#endif
}


/* ext is in format "jpg", no wildcards or separators */
static int AppendExtensionToSpecBuf( const char *ext, char *specBuf, size_t specBufLen )
{
    const char SEP[] = ";";
    assert( specBufLen > strlen(ext)+3 );
    
    if ( strlen(specBuf) > 0 )
    {
        strncat( specBuf, SEP, specBufLen - strlen(specBuf) - 1 );
        specBufLen += strlen(SEP);
    }

    char extWildcard[NFD_MAX_STRLEN];
    int bytesWritten = snprintf( extWildcard, NFD_MAX_STRLEN, "*.%s", ext );
    assert( bytesWritten == strlen(ext)+2 );
    
    strncat( specBuf, extWildcard, specBufLen - strlen(specBuf) - 1 );

    return NFD_OKAY;
}

// TODO
static nfdresult_t AddFiltersToDialog( OPENFILENAME *oft, const char *filterList )
{
    const wchar_t EMPTY_WSTR[] = L"";
    const wchar_t WILDCARD[] = L"*.*";

    //if ( !filterList || strlen(filterList) == 0 )
        return NFD_OKAY;

    // Count rows to alloc
    UINT filterCount = 1; /* guaranteed to have one filter on a correct, non-empty parse */
    const char *p_filterList;
    char *p_diff;

    for ( p_filterList = filterList; *p_filterList; ++p_filterList )
    {
        if ( *p_filterList == ';' ) // todo: check if need && *(p_filterList+1) != 0
            ++filterCount;
    }    

    assert(filterCount);
    if ( !filterCount )
    {
        NFDi_SetError("Error parsing filters.");
        return NFD_ERROR;
    }

    /* filterCount plus 1 because we hardcode the *.* wildcard after the while loop */
    //COMDLG_FILTERSPEC *specList = (COMDLG_FILTERSPEC*)NFDi_Malloc( sizeof(COMDLG_FILTERSPEC) * (filterCount + 1) );
    char specList[NFD_MAX_STRLEN] = {0}; /* one per semicolon */
    char *specListp = specList;
    size_t specIdx = 0;
    p_filterList = filterList;
    p_diff = filterList;
    char typebuf[NFD_MAX_STRLEN] = {0};  /* one per comma or semicolon */
    char *p_typebuf = typebuf;
    char filterName[NFD_MAX_STRLEN] = {0};

    char specbuf[NFD_MAX_STRLEN] = {0}; /* one per semicolon */

    while ( 1 ) 
    {
        if ( NFDi_IsFilterSegmentChar(*p_filterList) )
        {
            /* append a type to the specbuf (pending filter) */
            AppendExtensionToSpecBuf( typebuf, specbuf, NFD_MAX_STRLEN );            

            p_typebuf = typebuf;
            memset( typebuf, 0, sizeof(char)*NFD_MAX_STRLEN );
        }

        if ( *p_filterList == ';' || *p_filterList == '\0' )
        {
            /* end of filter -- add it to specList */
            *(specListp++) = '0' + specIdx;
            *(specListp++) = 0;
            // Empty filter name -- Windows describes them by extension.            
            //specList[specIdx].pszName = EMPTY_WSTR;
            //CopyNFDCharToWChar( specbuf, (wchar_t**)&specList[specIdx].pszSpec );
                        
            memset( specbuf, 0, sizeof(char)*NFD_MAX_STRLEN );
            ++specIdx;
            if ( specIdx == filterCount )
                break;
        }

        if ( !NFDi_IsFilterSegmentChar( *p_filterList ))
        {
            *p_typebuf = *p_filterList;
            ++p_typebuf;
        }

        ++p_filterList;
    }

    /* Add wildcard */
    //specList[specIdx].pszSpec = WILDCARD;
    //specList[specIdx].pszName = EMPTY_WSTR;
    
    //OPENFILENAMEDialog->SetFileTypes( filterCount+1, specList );

    /* free speclist */
    // for ( size_t i = 0; i < filterCount; ++i )
    // {
    //     NFDi_Free( (void*)specList[i].pszSpec );
    // }
    // NFDi_Free( specList );    

    return NFD_OKAY;
}

static nfdresult_t AllocPathSet( OPENFILENAME *ofn, nfdpathset_t *pathSet )
{
    wchar_t* str;
    wchar_t* directory;
    size_t dir_bufsize;
    const char ERRORMSG[] = "Error allocating pathset.";

    assert(ofn);
    assert(pathSet);
    
    /* count the total bytes needed for buf */
    size_t bufSize = 0;
    // How many items in shellItems?
    DWORD numShellItems = 0;
    // first test if we have single file
    if (ofn->nFileExtension)    // single file
    {
        numShellItems = 1;
        bufSize += GetUTF8ByteCountForWChar( ofn->lpstrFile );
    }
    else
    {
        directory = ofn->lpstrFile;
        dir_bufsize = GetUTF8ByteCountForWChar( directory );
        str = &ofn->lpstrFile[ofn->nFileOffset];
        while ( *str ) {
          // use the filename, e.g. add it to a vector
          // Calculate length of name with UTF-8 encoding
          bufSize += dir_bufsize + GetUTF8ByteCountForWChar( str );
          numShellItems++;
          str += ( wcslen(str) + 1 + 1 );
        }
    }
    


    pathSet->count = static_cast<size_t>(numShellItems);
    assert( pathSet->count > 0 );

    pathSet->indices = (size_t*)NFDi_Malloc( sizeof(size_t)*pathSet->count );
    if ( !pathSet->indices )
    {
        return NFD_ERROR;
    }

    assert(bufSize);

    pathSet->buf = (nfdchar_t*)NFDi_Malloc( sizeof(nfdchar_t) * bufSize );
    memset( pathSet->buf, 0, sizeof(nfdchar_t) * bufSize );

    /* fill buf */
    nfdchar_t *p_buf = pathSet->buf;

    if (numShellItems == 1)
    {
        int bytesWritten = CopyWCharToExistingNFDCharBuffer(ofn->lpstrFile, p_buf);

        ptrdiff_t index = p_buf - pathSet->buf;
        assert( index >= 0 );
        pathSet->indices[0] = static_cast<size_t>(index);
        
        p_buf += bytesWritten; 
    }
    else
    {
        str = &ofn->lpstrFile[ofn->nFileOffset];
        for (DWORD i = 0; i < numShellItems; ++i )
        {
            LPWSTR name;
            //shellItem->GetDisplayName(SIGDN_FILESYSPATH, &name);

            wchar_t *dir_filename = (wchar_t *) NFDi_Malloc(wcslen(directory) + wcslen(L"/") + wcslen(str));

            int bytesWritten = CopyWCharToExistingNFDCharBuffer(dir_filename, p_buf);

            ptrdiff_t index = p_buf - pathSet->buf;
            assert( index >= 0 );
            pathSet->indices[i] = static_cast<size_t>(index);
            
            p_buf += bytesWritten;
            str += ( wcslen(str) + 1 + 1 );
        }
    }
    return NFD_OKAY;
}


static nfdresult_t SetDefaultPath( OPENFILENAME *ofn, const char *defaultPath )
{
    if ( !defaultPath || strlen(defaultPath) == 0 )
        return NFD_OKAY;

    wchar_t *defaultPathW = {0};
    CopyNFDCharToWChar( defaultPath, &defaultPathW );

    // Todo - check if dir EXISTS
    ofn->lpstrInitialDir = defaultPathW;

    NFDi_Free( defaultPathW );
    
    return NFD_OKAY;
}

// internal
void NFD_init_ofn(OPENFILENAME *ofn,  wchar_t *buf, size_t len)
{
    ZeroMemory(ofn, sizeof(OPENFILENAME));
    ofn->lStructSize = sizeof(OPENFILENAME);
    ofn->hwndOwner = NULL;
    ofn->lpstrFile = buf;
    ofn->lpstrFile[0] = '\0';
    ofn->nMaxFile = len;
    
    //AddFiltersToDialog
    ofn->lpstrFilter = NULL; //"All\0*.*\0Text\0*.TXT\0";
    ofn->nFilterIndex = 0;
    
    ofn->lpstrFileTitle = NULL;
    ofn->nMaxFileTitle = 0;

    // SetDefaultPath
    ofn->lpstrInitialDir=NULL;

    ofn->Flags = OFN_PATHMUSTEXIST|OFN_FILEMUSTEXIST|OFN_EXPLORER;
}
void NFD_ofn_free(OPENFILENAME *ofn)
{
    if(ofn->lpstrFilter)
        NFDi_Free((void*)ofn->lpstrFilter);
    if(ofn->lpstrInitialDir)
        NFDi_Free((void*)ofn->lpstrInitialDir);
}

/* public */


nfdresult_t NFD_OpenDialog( const char *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath )
{
    nfdresult_t nfdResult = NFD_ERROR;
    BOOL bresult;
    
    // Init COM library.
    HRESULT result = ::CoInitializeEx(NULL,
                                      ::COINIT_APARTMENTTHREADED |
                                      ::COINIT_DISABLE_OLE1DDE );
    
    wchar_t buf[5000];
    // Step 0 - Create the OPENFILENAME thing
    // open a file name
    OPENFILENAME ofn ;
    NFD_init_ofn(&ofn, buf, 5000);

    // Step 1 -- rewrite the AddFiltersToDialog!!
    // Build the filter list
    if ( !AddFiltersToDialog( &ofn, filterList ) )
    {
        goto end;
    }

    // Set the default path
    if ( !SetDefaultPath( &ofn, defaultPath ) )
    {
        goto end;
    }

    // Set a flag for multiple options
    ofn.Flags = OFN_OVERWRITEPROMPT;
 
    // Show the dialog.
    bresult = GetSaveFileName( &ofn );
    if ( bresult )
    {
        CopyWCharToNFDChar( ofn.lpstrFile, outPath );
        if ( !*outPath )
        {
            /* error is malloc-based, error message would be redundant */
            goto end;
        }
        nfdResult = NFD_OKAY;
    }
    else
    {
        // Check if cancel or ERROR
        DWORD err = CommDlgExtendedError();

        if (err)
        {
            NFDi_SetError("File dialog box show failed.");
            nfdResult = NFD_ERROR;
        }
        else
            nfdResult = NFD_CANCEL;
    }

 end:
    NFD_ofn_free(&ofn);
    ::CoUninitialize();
    
    return nfdResult;
}



// this is the only one that needs updating for now
nfdresult_t NFD_OpenDialogMultiple( const nfdchar_t *filterList,    // ie "spc,rsn,rar,7z" or "spc;rsn;rar;7z"
                                    const nfdchar_t *defaultPath,   // NULL
                                    nfdpathset_t *outPaths )        // 
{
    nfdresult_t nfdResult = NFD_ERROR;
    BOOL bresult;
    // Init COM library.
    HRESULT result = ::CoInitializeEx(NULL,
                                      ::COINIT_APARTMENTTHREADED |
                                      ::COINIT_DISABLE_OLE1DDE );
    if ( !SUCCEEDED(result))
    {
        NFDi_SetError("Could not initialize COM.");
        return NFD_ERROR;
    }

    wchar_t buf[5000];
    // Step 0 - Create the OPENFILENAME thing
    // open a file name
    OPENFILENAME ofn ;
    NFD_init_ofn(&ofn, buf, 5000);

    // Step 1 -- rewrite the AddFiltersToDialog!!
    // Build the filter list
    if ( !AddFiltersToDialog( &ofn, filterList ) )
    {
        goto end;
    }

    // Set the default path
    if ( !SetDefaultPath( &ofn, defaultPath ) )
    {
        goto end;
    }

    // Set a flag for multiple options
    ofn.Flags |= OFN_ALLOWMULTISELECT;
 
    // Show the dialog.
    bresult = GetOpenFileName( &ofn );
    if ( bresult )
    {
        if ( AllocPathSet( &ofn, outPaths ) == NFD_ERROR )
        {
            goto end;
        }
        nfdResult = NFD_OKAY;
    }
    else
    {
        // Check if cancel or ERROR
        DWORD err = CommDlgExtendedError();

        if (err)
        {
            NFDi_SetError("File dialog box show failed.");
            nfdResult = NFD_ERROR;
        }
        else
            nfdResult = NFD_CANCEL;
    }

 end:
    NFD_ofn_free(&ofn);
    ::CoUninitialize();
    
    return nfdResult;
}


nfdresult_t NFD_SaveDialog( const nfdchar_t *filterList,
                            const nfdchar_t *defaultPath,
                            nfdchar_t **outPath )
{
    nfdresult_t nfdResult = NFD_ERROR;
    
    // Init COM library.
    HRESULT result = ::CoInitializeEx(NULL,
                                      ::COINIT_APARTMENTTHREADED |
                                      ::COINIT_DISABLE_OLE1DDE );
    if ( !SUCCEEDED(result))
    {
        NFDi_SetError("Could not initialize COM.");
        return NFD_ERROR;
    }

    
}
