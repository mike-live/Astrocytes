

/* this ALWAYS GENERATED file contains the IIDs and CLSIDs */

/* link this file in with the server and any clients */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Mon Dec 29 19:25:51 2014
 */
/* Compiler settings for win64\mwcomutil.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=IA64 7.00.0555 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


#ifdef __cplusplus
extern "C"{
#endif 


#include <rpc.h>
#include <rpcndr.h>

#ifdef _MIDL_USE_GUIDDEF_

#ifndef INITGUID
#define INITGUID
#include <guiddef.h>
#undef INITGUID
#else
#include <guiddef.h>
#endif

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        DEFINE_GUID(name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8)

#else // !_MIDL_USE_GUIDDEF_

#ifndef __IID_DEFINED__
#define __IID_DEFINED__

typedef struct _IID
{
    unsigned long x;
    unsigned short s1;
    unsigned short s2;
    unsigned char  c[8];
} IID;

#endif // __IID_DEFINED__

#ifndef CLSID_DEFINED
#define CLSID_DEFINED
typedef IID CLSID;
#endif // CLSID_DEFINED

#define MIDL_DEFINE_GUID(type,name,l,w1,w2,b1,b2,b3,b4,b5,b6,b7,b8) \
        const type name = {l,w1,w2,{b1,b2,b3,b4,b5,b6,b7,b8}}

#endif !_MIDL_USE_GUIDDEF_

MIDL_DEFINE_GUID(IID, IID_IMWUtil,0xC47EA90E,0x56D1,0x11d5,0xB1,0x59,0x00,0xD0,0xB7,0xBA,0x75,0x44);


MIDL_DEFINE_GUID(IID, LIBID_MWComUtil,0xC58F1023,0x4757,0x45C3,0x85,0xFD,0xD4,0x85,0x41,0xBE,0x57,0xAF);


MIDL_DEFINE_GUID(CLSID, CLSID_MWField,0x9848257E,0xC3D7,0x4CBE,0xA5,0xB3,0x5A,0x6B,0x60,0x98,0x08,0xB0);


MIDL_DEFINE_GUID(CLSID, CLSID_MWStruct,0x17676D81,0x92F2,0x4E7A,0xBD,0xEF,0x2D,0xF2,0xD6,0x88,0xA5,0x3A);


MIDL_DEFINE_GUID(CLSID, CLSID_MWComplex,0x0F82FA59,0x348D,0x4725,0xA7,0x1F,0x88,0xB8,0xE6,0x35,0xC8,0x26);


MIDL_DEFINE_GUID(CLSID, CLSID_MWSparse,0x8F0F889C,0xF0DA,0x44E1,0x89,0xBD,0xE7,0x0B,0xE3,0x7A,0x81,0x16);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArg,0x876406C3,0x9E33,0x43E3,0xA4,0x2C,0xEC,0xAC,0x19,0x15,0x79,0x5C);


MIDL_DEFINE_GUID(CLSID, CLSID_MWArrayFormatFlags,0x59D55010,0x4CDA,0x458A,0xBB,0x19,0x96,0x9C,0x02,0xC8,0x0A,0x88);


MIDL_DEFINE_GUID(CLSID, CLSID_MWDataConversionFlags,0x23C2EA79,0xEBDE,0x46E9,0xB9,0x7F,0x9E,0xBC,0x77,0x6B,0x90,0x5A);


MIDL_DEFINE_GUID(CLSID, CLSID_MWUtil,0x64AD1C94,0x72D3,0x40BB,0x80,0x38,0xC5,0xA5,0x9D,0xF7,0x7E,0x42);


MIDL_DEFINE_GUID(CLSID, CLSID_MWFlags,0xFBD63204,0xBA03,0x4896,0x95,0x64,0xA8,0x7C,0x49,0xD9,0x1F,0x71);

#undef MIDL_DEFINE_GUID

#ifdef __cplusplus
}
#endif



