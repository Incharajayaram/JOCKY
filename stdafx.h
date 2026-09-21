#pragma once
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRACE printf
#define DEBUG_NEW new

class CString {
    char buf[1024];
public:
    CString() { buf[0] = 0; }
    CString(const char* s) { strcpy(buf, s); }
    void Format(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vsprintf(buf, fmt, args);
        va_end(args);
    }
    void Empty() { buf[0] = 0; }
    operator const char*() const { return buf; }
};

template<class TYPE, class ARG_TYPE>
class CArray {
    TYPE* m_pData;
    int m_nSize;
    int m_nMaxSize;
public:
    CArray() : m_pData(NULL), m_nSize(0), m_nMaxSize(0) {}
    ~CArray() { if (m_pData) delete[] m_pData; }
    void Add(ARG_TYPE newElement) {
        if (m_nSize >= m_nMaxSize) {
            m_nMaxSize = m_nMaxSize == 0 ? 10 : m_nMaxSize * 2;
            TYPE* pNewData = new TYPE[m_nMaxSize];
            if (m_pData) {
                for (int i = 0; i < m_nSize; i++) pNewData[i] = m_pData[i];
                delete[] m_pData;
            }
            m_pData = pNewData;
        }
        m_pData[m_nSize++] = newElement;
    }
    int GetSize() const { return m_nSize; }
    TYPE& operator[](int nIndex) { return m_pData[nIndex]; }
    void RemoveAt(int nIndex) {
        for (int i = nIndex; i < m_nSize - 1; i++) m_pData[i] = m_pData[i+1];
        m_nSize--;
    }
    void RemoveAll() { m_nSize = 0; }
};

class CCriticalSection {
    CRITICAL_SECTION m_sect;
public:
    CCriticalSection() { InitializeCriticalSection(&m_sect); }
    ~CCriticalSection() { DeleteCriticalSection(&m_sect); }
    void Lock() { EnterCriticalSection(&m_sect); }
    void Unlock() { LeaveCriticalSection(&m_sect); }
};

class CSingleLock {
    CCriticalSection* m_pObject;
public:
    CSingleLock(CCriticalSection* pObject, BOOL bInitialLock = FALSE) {
        m_pObject = pObject;
        if (bInitialLock) m_pObject->Lock();
    }
    ~CSingleLock() { m_pObject->Unlock(); }
};

class CWinThread {
public:
    HANDLE m_hThread;
};

CWinThread* AfxBeginThread(UINT (*pfnThreadProc)(LPVOID), LPVOID pParam, int nPriority = THREAD_PRIORITY_NORMAL, UINT nStackSize = 0, DWORD dwCreateFlags = 0, LPSECURITY_ATTRIBUTES lpSecurityAttrs = NULL) {
    CWinThread* pThread = new CWinThread();
    pThread->m_hThread = CreateThread(lpSecurityAttrs, nStackSize, (LPTHREAD_START_ROUTINE)pfnThreadProc, pParam, dwCreateFlags, NULL);
    return pThread;
}

class CWinApp {};
CWinApp* AfxGetApp() { return NULL; }

