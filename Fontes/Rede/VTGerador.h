//---------------------------------------------------------------------------
#ifndef VTGeradorH
#define VTGeradorH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TEqbar.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
enum enumTIPO_GERACAO {gerDIESEL=1, gerPCH, gerVAPOR, gerEOLICA,
                       gerSOLAR, gerTGAS, gerHIDRAULICA};

//---------------------------------------------------------------------------
class VTCurva;
class VTResFlowSup;

//---------------------------------------------------------------------------
class VTGerador : public TEqbar
   {
   public: //property
      __property double     vnom            = {read=PM_GetVnom_kv,     write=PM_SetVnom_kv};
      __property double     snom            = {read=PM_GetSnom_mva,    write=PM_SetSnom_mva};
      __property int        FonteEnergia    = {read=PM_GetFonte,       write=PM_SetFonte};
      __property AnsiString FonteStr        = {read=PM_GetFonteStr};
      __property bool       DefinidoPorFase = {read=PM_GetDefinidoPorFase};
      __property int        Fases           = {read=PM_GetFases,       write=PM_SetFases};

   public:
                         __fastcall  VTGerador(void) {};
      virtual            __fastcall ~VTGerador(void) {};
      virtual VTGerador* __fastcall  Clone(void) = 0;
      virtual void       __fastcall  DefineCurva(VTCurva *curva)=0;

   protected: //métodos acessados via property
      virtual bool       __fastcall PM_GetDefinidoPorFase(void) = 0;
      virtual int        __fastcall PM_GetFases(void) = 0;
      virtual int        __fastcall PM_GetFonte(void) = 0;
      virtual AnsiString __fastcall PM_GetFonteStr(void) = 0;
      virtual double     __fastcall PM_GetSnom_mva(void) = 0;
      virtual double     __fastcall PM_GetVnom_kv(void) = 0;
      //
      virtual void   __fastcall PM_SetFases(int fases) = 0;
      virtual void   __fastcall PM_SetFonte(int tipo) = 0;
      virtual void   __fastcall PM_SetSnom_mva(double snom_mva) = 0;
      virtual void   __fastcall PM_SetVnom_kv(double vnom_kv) = 0;

   public:  //objetos externos
      VTCurva      *curva;

   public:  //dados locais
      int      tiposup;       //tipo do suprimento{supPV, supPQ, supVF}
      int      ligacao;       //{0=delta, 1= Y isolado, 2=Y aterrado}
      //float    vnom;          //kV
      //float    snom;          //MVA
      float    fplim;         //fator de pot. limite
      float    smax;          //MVA
      strIMP   z0;            //impedância de seqüência 0
      strIMP   z1;            //impedância de seqüência 1
      VTResFlowSup *resflow;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Gerador
//---------------------------------------------------------------------------
VTGerador* __fastcall NewObjGerador(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 