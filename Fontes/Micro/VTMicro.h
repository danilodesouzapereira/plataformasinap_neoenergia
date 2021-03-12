//---------------------------------------------------------------------------
#ifndef VTMicroH
#define VTMicroH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTBarra;

//---------------------------------------------------------------------------
class VTMicro : public TObject
   {
   public:  //property
      __property AnsiString CPU_ID      = {read=PM_GetCPU_Id};
      __property AnsiString CPU_VENDOR  = {read=PM_GetCPU_Vendor};

   public:
              __fastcall VTMicro(void) {};
      virtual __fastcall ~VTMicro(void) {};

   protected: //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCPU_Id(void) = 0;
      virtual AnsiString __fastcall PM_GetCPU_Vendor(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe
VTMicro* __fastcall NewObjMicro(void);


#endif
//---------------------------------------------------------------------------
//eof

