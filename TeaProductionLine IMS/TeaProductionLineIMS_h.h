

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0603 */
/* at Fri Nov 13 10:41:09 2015
 */
/* Compiler settings for TeaProductionLineIMS.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0603 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __TeaProductionLineIMS_h_h__
#define __TeaProductionLineIMS_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITeaProductionLineIMS_FWD_DEFINED__
#define __ITeaProductionLineIMS_FWD_DEFINED__
typedef interface ITeaProductionLineIMS ITeaProductionLineIMS;

#endif 	/* __ITeaProductionLineIMS_FWD_DEFINED__ */


#ifndef __TeaProductionLineIMS_FWD_DEFINED__
#define __TeaProductionLineIMS_FWD_DEFINED__

#ifdef __cplusplus
typedef class TeaProductionLineIMS TeaProductionLineIMS;
#else
typedef struct TeaProductionLineIMS TeaProductionLineIMS;
#endif /* __cplusplus */

#endif 	/* __TeaProductionLineIMS_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __TeaProductionLineIMS_LIBRARY_DEFINED__
#define __TeaProductionLineIMS_LIBRARY_DEFINED__

/* library TeaProductionLineIMS */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TeaProductionLineIMS;

#ifndef __ITeaProductionLineIMS_DISPINTERFACE_DEFINED__
#define __ITeaProductionLineIMS_DISPINTERFACE_DEFINED__

/* dispinterface ITeaProductionLineIMS */
/* [uuid] */ 


EXTERN_C const IID DIID_ITeaProductionLineIMS;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("E6E392E5-4C19-4ED0-B641-A0A468860781")
    ITeaProductionLineIMS : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ITeaProductionLineIMSVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITeaProductionLineIMS * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITeaProductionLineIMS * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITeaProductionLineIMS * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITeaProductionLineIMS * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITeaProductionLineIMS * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITeaProductionLineIMS * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITeaProductionLineIMS * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } ITeaProductionLineIMSVtbl;

    interface ITeaProductionLineIMS
    {
        CONST_VTBL struct ITeaProductionLineIMSVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITeaProductionLineIMS_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITeaProductionLineIMS_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITeaProductionLineIMS_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITeaProductionLineIMS_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITeaProductionLineIMS_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITeaProductionLineIMS_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITeaProductionLineIMS_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ITeaProductionLineIMS_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_TeaProductionLineIMS;

#ifdef __cplusplus

class DECLSPEC_UUID("1C8FCBEA-5A91-49C9-BCE6-A437D8025D2E")
TeaProductionLineIMS;
#endif
#endif /* __TeaProductionLineIMS_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


