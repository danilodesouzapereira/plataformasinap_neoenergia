//---------------------------------------------------------------------------
#ifndef VTFasesH
#define VTFasesH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class TFase : public TObject
   {
   public:
      __fastcall TFase(int tag, AnsiString codigo) {this->tag = tag; this->codigo = codigo;}
      __fastcall ~TFase() {}

   public:
      int        tag;
      AnsiString codigo;
   };

//---------------------------------------------------------------------------
class VTFases : public TObject
   {
   public:  //property
      __property AnsiString AsStrABCN  [int tag]   = {read=PM_GetCodAsStrABCN};
      __property AnsiString AsStrDEFN  [int tag]   = {read=PM_GetCodAsStrDEFN};
      __property AnsiString AsStrLower [int tag]   = {read=PM_GetCodAsStrLower};
      __property AnsiString AsStrUpper [int tag]   = {read=PM_GetCodAsStrUpper};
      __property TFase*     IndexToFase[int index] = {read=PM_GetIndexToFase};
      __property int        IndexToTag [int index] = {read=PM_GetIndexToTag};
      __property TFase*     TagToFase  [int tag]   = {read=PM_GetTagToFase};
      __property int        TagToIndex [int tag]   = {read=PM_GetTagToIndex};

   public: //métodos
                          __fastcall VTFases(void) {};
      virtual            __fastcall ~VTFases(void) {};
      virtual AnsiString __fastcall ConverteABCN_DEFN(AnsiString codigo) = 0;
      virtual AnsiString __fastcall ConverteDEFN_ABCN(AnsiString codigo) = 0;
      virtual void       __fastcall DefCodigo(AnsiString cod_fase1, AnsiString cod_fase2,
                                              AnsiString cod_fase3) = 0;
      virtual void       __fastcall DefCodigoABC(void) = 0;
      virtual void       __fastcall DefCodigoDEF(void) = 0;
      virtual int        __fastcall ExisteFase(AnsiString codigo) = 0;
      virtual bool       __fastcall Fase1ContemFase2(int fase1, int fase2) = 0;
      virtual int        __fastcall Fase1IntersecaoFase2(int fase1, int fase2) = 0;
      virtual int        __fastcall Fase1UniaoFase2(int fase1, int fase2) = 0;
      virtual TList*     __fastcall LisFasesCarga(int carMBT) = 0;
      virtual TList*     __fastcall LisFasesTrafoMonoPrimario(void) = 0;
      virtual TList*     __fastcall LisFasesTrafoMonoSecundario(bool mono_5terminais) = 0;
      virtual TList*     __fastcall LisFasesTrecho(void) = 0;
      virtual TList*     __fastcall LisFasesTrechoTrifasico(void) = 0;
      virtual int        __fastcall NumeroDeFases(int fase) = 0;

   protected: //métodos acessados via property
      virtual AnsiString __fastcall PM_GetCodAsStrABCN(int tag) = 0;
      virtual AnsiString __fastcall PM_GetCodAsStrDEFN(int tag) = 0;
      virtual AnsiString __fastcall PM_GetCodAsStrLower(int tag) = 0;
      virtual AnsiString __fastcall PM_GetCodAsStrUpper(int tag) = 0;
      virtual TFase*     __fastcall PM_GetIndexToFase(int index) = 0;
      virtual int        __fastcall PM_GetIndexToTag(int index) = 0;
      virtual TFase*     __fastcall PM_GetTagToFase(int tag) = 0;
      virtual int        __fastcall PM_GetTagToIndex(int tag) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFases
//---------------------------------------------------------------------------
VTFases* __fastcall NewObjFases(void);

//---------------------------------------------------------------------------
#endif
//eof

