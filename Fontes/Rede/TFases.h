//---------------------------------------------------------------------------
#ifndef TFasesH
#define TFasesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFases.h"

//---------------------------------------------------------------------------
class TFases : public VTFases
   {
   public: //métodos
                 __fastcall  TFases(void);
                 __fastcall ~TFases(void);
      AnsiString __fastcall ConverteABCN_DEFN(AnsiString codigo);
      AnsiString __fastcall ConverteDEFN_ABCN(AnsiString codigo);
      void       __fastcall DefCodigo(AnsiString A, AnsiString B, AnsiString C);
      void       __fastcall DefCodigoABC(void);
      void       __fastcall DefCodigoDEF(void);
      bool       __fastcall Fase1ContemFase2(int fase1, int fase2);
      int        __fastcall ExisteFase(AnsiString codigo);
      int        __fastcall Fase1IntersecaoFase2(int fase1, int fase2);
      int        __fastcall Fase1UniaoFase2(int fase1, int fase2);
      TList*     __fastcall LisFasesCarga(int carMBT);
      TList*     __fastcall LisFasesTrafoMonoPrimario(void);
      TList*     __fastcall LisFasesTrafoMonoSecundario(bool mono_5terminais);
      TList*     __fastcall LisFasesTrecho(void);
      TList*     __fastcall LisFasesTrechoTrifasico(void);
      int        __fastcall NumeroDeFases(int fase);

   protected: //métodos acessados via property
      AnsiString __fastcall PM_GetCodAsStrABCN(int tag);
      AnsiString __fastcall PM_GetCodAsStrDEFN(int tag);
      AnsiString __fastcall PM_GetCodAsStrLower(int tag);
      AnsiString __fastcall PM_GetCodAsStrUpper(int tag);
      TFase*     __fastcall PM_GetIndexToFase(int index);
      int        __fastcall PM_GetIndexToTag(int index);
      TFase*     __fastcall PM_GetTagToFase(int tag);
      int        __fastcall PM_GetTagToIndex(int tag);

   private: //dados
      TList *lisFASE;
      TList *lisTMP;
   };

//---------------------------------------------------------------------------
#endif
//eof

