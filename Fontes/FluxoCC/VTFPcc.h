//---------------------------------------------------------------------------
#ifndef VTFPccH
#define VTFPccH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTData;

//---------------------------------------------------------------------------
class VTFPcc : public TObject
   {
   public:
                   __fastcall  VTFPcc(void) {};
      virtual      __fastcall ~VTFPcc(void) {};
      virtual void __fastcall  EnableAviso(bool enabled) = 0;
      virtual void __fastcall  EnableRelatorio(bool enabled) = 0;
      virtual bool __fastcall  Executa(VTData     *data,
                                       AnsiString NomeRede,
                                       int        ind_pat) = 0;
      virtual void __fastcall  Relatorios(TStringList *StrList) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFPcc
//---------------------------------------------------------------------------
VTFPcc* __fastcall NewObjFPcc(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof



 