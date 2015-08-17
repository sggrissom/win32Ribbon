#if !defined(RIBBON_H)
/* ========================================================================
   $File: $
   $Date: $
   $Revision: $
   $Creator: Steven Grissom $
   ======================================================================== */

#include <atlbase.h>
#include <atlcom.h>
#include <uiribbon.h>

#include "resource.h"

typedef int32 (*executeFunctionType)( UINT, UI_EXECUTIONVERB,const PROPERTYKEY*,const PROPVARIANT*,IUISimplePropertySet*);
executeFunctionType executeFunction = 0;

class CCommandHandler
    : public IUICommandHandler // Command handlers must implement IUICommandHandler.
{
public:

    // Static method to create an instance of the object.
    __checkReturn static HRESULT CreateInstance(__deref_out IUICommandHandler **ppCommandHandler);

    // IUnknown methods.
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    STDMETHODIMP QueryInterface(REFIID iid, void** ppv);

    // IUICommandHandler methods
    STDMETHOD(UpdateProperty)(UINT nCmdID,
        __in REFPROPERTYKEY key,
        __in_opt const PROPVARIANT* ppropvarCurrentValue,
        __out PROPVARIANT* ppropvarNewValue);

    STDMETHOD(Execute)(UINT nCmdID,
        UI_EXECUTIONVERB verb, 
        __in_opt const PROPERTYKEY* key,
        __in_opt const PROPVARIANT* ppropvarValue,
        __in_opt IUISimplePropertySet* pCommandExecutionProperties);

private:
    CCommandHandler()
        : m_cRef(1) 
    {
    }

    LONG m_cRef;                        // Reference count.
};


// Static method to create an instance of the object.
__checkReturn HRESULT CCommandHandler::CreateInstance(__deref_out IUICommandHandler **ppCommandHandler)
{
    if (!ppCommandHandler)
    {
        return E_POINTER;
    }

    *ppCommandHandler = NULL;

    HRESULT hr = S_OK;
   
    CCommandHandler* pCommandHandler = new CCommandHandler();

    if (pCommandHandler != NULL)
    {
        *ppCommandHandler = static_cast<IUICommandHandler *>(pCommandHandler);
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CCommandHandler::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CCommandHandler::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CCommandHandler::QueryInterface(REFIID iid, void** ppv)
{
    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUICommandHandler))
    {
        *ppv = static_cast<IUICommandHandler*>(this);
    }
    else 
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

//
//  FUNCTION: UpdateProperty()
//
//  PURPOSE: Called by the Ribbon framework when a command property (PKEY) needs to be updated.
//
//  COMMENTS:
//
//    This function is used to provide new command property values, such as labels, icons, or
//    tooltip information, when requested by the Ribbon framework.  
//    
//    In this SimpleRibbon sample, the method is not implemented.  
//
STDMETHODIMP CCommandHandler::UpdateProperty(
    UINT nCmdID,
    __in REFPROPERTYKEY key,
    __in_opt const PROPVARIANT* ppropvarCurrentValue,
    __out PROPVARIANT* ppropvarNewValue)
{
    UNREFERENCED_PARAMETER(nCmdID);
    UNREFERENCED_PARAMETER(key);
    UNREFERENCED_PARAMETER(ppropvarCurrentValue);
    UNREFERENCED_PARAMETER(ppropvarNewValue);

    return E_NOTIMPL;
}

//
//  FUNCTION: Execute()
//
//  PURPOSE: Called by the Ribbon framework when a command is executed by the user.  For example, when
//           a button is pressed.
//
STDMETHODIMP CCommandHandler::Execute(
    UINT nCmdID,
    UI_EXECUTIONVERB verb,
    __in_opt const PROPERTYKEY* key,
    __in_opt const PROPVARIANT* ppropvarValue,
    __in_opt IUISimplePropertySet* pCommandExecutionProperties)
{
    return (*executeFunction)(nCmdID, verb, key, ppropvarValue, pCommandExecutionProperties);
}

class CApplication
    : public IUIApplication // Applications must implement IUIApplication.
{
public:

    // Static method to create an instance of the object.
    static HRESULT CreateInstance(__deref_out IUIApplication **ppApplication);

    // IUnknown methods.
    STDMETHOD_(ULONG, AddRef());
    STDMETHOD_(ULONG, Release());
    STDMETHOD(QueryInterface(REFIID iid, void** ppv));

    // IUIApplication methods
    STDMETHOD(OnCreateUICommand)(UINT nCmdID,
        __in UI_COMMANDTYPE typeID,
        __deref_out IUICommandHandler** ppCommandHandler);

    STDMETHOD(OnViewChanged)(UINT viewId,
        __in UI_VIEWTYPE typeId,
        __in IUnknown* pView,
        UI_VIEWVERB verb,
        INT uReasonCode);

    STDMETHOD(OnDestroyUICommand)(UINT32 commandId, 
        __in UI_COMMANDTYPE typeID,
        __in_opt IUICommandHandler* commandHandler);

private:
    CApplication() 
        : m_cRef(1)
        , m_pCommandHandler(NULL)
    {
    }

    ~CApplication() 
    {
        if (m_pCommandHandler)
        {
            m_pCommandHandler->Release();
            m_pCommandHandler = NULL;
        }
    }

    LONG m_cRef;                            // Reference count.
    IUICommandHandler * m_pCommandHandler;  // Generic Command Handler
};

// Static method to create an instance of the object.
__checkReturn HRESULT CApplication::CreateInstance(__deref_out IUIApplication **ppApplication)
{
    if (!ppApplication)
    {
        return E_POINTER;
    }

    *ppApplication = NULL;

    HRESULT hr = S_OK;
   
    CApplication* pApplication = new CApplication();

    if (pApplication != NULL)
    {
        *ppApplication = static_cast<IUIApplication *>(pApplication);
    }
    else
    {
        hr = E_OUTOFMEMORY;
    }

    return hr;
}

// IUnknown method implementations.
STDMETHODIMP_(ULONG) CApplication::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

STDMETHODIMP_(ULONG) CApplication::Release()
{
    LONG cRef = InterlockedDecrement(&m_cRef);
    if (cRef == 0)
    {
        delete this;
    }

    return cRef;
}

STDMETHODIMP CApplication::QueryInterface(REFIID iid, void** ppv)
{
    if (iid == __uuidof(IUnknown))
    {
        *ppv = static_cast<IUnknown*>(this);
    }
    else if (iid == __uuidof(IUIApplication))
    {
        *ppv = static_cast<IUIApplication*>(this);
    }
    else 
    {
        *ppv = NULL;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

//
//  FUNCTION: OnCreateUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler)
//
//  PURPOSE: Called by the Ribbon framework for each command specified in markup, to allow
//           the host application to bind a command handler to that command.
//
//  COMMENTS:
//
//    In this SimpleRibbon sample, the same command handler is returned for all commands
//    specified in the SimpleRibbon.xml file.
//    
//    To view the OnCreateUICommand callbacks, uncomment the _cwprintf call.
//
//
STDMETHODIMP CApplication::OnCreateUICommand(
    UINT nCmdID,
    __in UI_COMMANDTYPE typeID,
    __deref_out IUICommandHandler** ppCommandHandler)
{
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(nCmdID);

    if (NULL == m_pCommandHandler)
    {
        HRESULT hr = CCommandHandler::CreateInstance(&m_pCommandHandler);
        if (FAILED(hr))
        {
            return hr;
        }
    }

    return m_pCommandHandler->QueryInterface(IID_PPV_ARGS(ppCommandHandler));
}

//
//  FUNCTION: OnViewChanged(UINT, UI_VIEWTYPE, IUnknown*, UI_VIEWVERB, INT)
//
//  PURPOSE: Called when the state of a View (Ribbon is a view) changes, for example, created, destroyed, or resized.
//
//
STDMETHODIMP CApplication::OnViewChanged(
    UINT viewId,
    __in UI_VIEWTYPE typeId,
    __in IUnknown* pView,
    UI_VIEWVERB verb,
    INT uReasonCode)
{
    UNREFERENCED_PARAMETER(uReasonCode);
    UNREFERENCED_PARAMETER(viewId);

    HRESULT hr = E_NOTIMPL;

    // Checks to see if the view that was changed was a Ribbon view.
    if (UI_VIEWTYPE_RIBBON == typeId)
    {
        switch (verb)
        {           
            // The view was newly created.
        case UI_VIEWVERB_CREATE:
            hr = S_OK;
            break;

            // The view has been resized.  For the Ribbon view, the application should
            // call GetHeight to determine the height of the ribbon.
        case UI_VIEWVERB_SIZE:
            {
                IUIRibbon* pRibbon = NULL;
                UINT uRibbonHeight;

                hr = pView->QueryInterface(IID_PPV_ARGS(&pRibbon));
                if (SUCCEEDED(hr))
                {
                    // Call to the framework to determine the desired height of the Ribbon.
                    hr = pRibbon->GetHeight(&uRibbonHeight);
                    pRibbon->Release();
                    // Use the ribbon height to position controls in the client area of the window.
                }
            }
            break;
            // The view was destroyed.
        case UI_VIEWVERB_DESTROY:
            hr = S_OK;
            break;
        }
    }

    return hr;
}


//
//  FUNCTION: OnDestroyUICommand(UINT, UI_COMMANDTYPE, IUICommandHandler*)
//
//  PURPOSE: Called by the Ribbon framework for each command at the time of ribbon destruction.
//
STDMETHODIMP CApplication::OnDestroyUICommand(
    UINT32 nCmdID,
    __in UI_COMMANDTYPE typeID,
    __in_opt IUICommandHandler* commandHandler)
{
    UNREFERENCED_PARAMETER(commandHandler);
    UNREFERENCED_PARAMETER(typeID);
    UNREFERENCED_PARAMETER(nCmdID);

    return E_NOTIMPL;
}



internal bool32
initializeRibbon(HWND window)
{
    bool32 result = false;
    
    IUIFramework *framework = 0;
    
    HRESULT hr = CoCreateInstance(
        CLSID_UIRibbonFramework,
        0,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&framework));

    if(SUCCEEDED(hr))
    {
        IUIApplication *application = 0;
        hr = CApplication::CreateInstance(&application);
        if(SUCCEEDED(hr))
        {
            hr = framework->Initialize(window, application);
            if(SUCCEEDED(hr))
            {
                hr = framework->LoadUI(GetModuleHandle(0), L"TEST_RIBBON");
                if(SUCCEEDED(hr))
                {
                    result = true;
                }
            }
        }
    }

    return result;
}


#define RIBBON_H
#endif
