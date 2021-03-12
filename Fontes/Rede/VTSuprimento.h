//---------------------------------------------------------------------------
#ifndef VTSuprimentoH
#define VTSuprimentoH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "TEqbar.h"
#include "..\Constante\Const.h"

//---------------------------------------------------------------------------
class VTCurva;
class VTResFlowSup;

//---------------------------------------------------------------------------
class VTSuprimento : public TEqbar
   {
   public:  //property
      __property bool Reducao         = {read=PD_SUP.reducao,  write=PD_SUP.reducao};
      __property int  Fases           = {read=PD_SUP.fases,    write=PD_SUP.fases};
      __property bool DefinidoPorFase = {read=PM_GetDefinidoPorFase};


   public:
                            __fastcall  VTSuprimento(void) {};
      virtual               __fastcall ~VTSuprimento(void) {};
      virtual VTSuprimento* __fastcall  Clone(void) = 0;
      virtual void          __fastcall  DefineCurva(VTCurva *curva)=0;

   protected: //métodos acessados via property
      virtual bool       __fastcall PM_GetDefinidoPorFase(void) = 0;

   public:  //dados locais
      int          tiposup;         //tipo do suprimento{supPV, supPQ, supVF}
      float        vnom;            //kV
      float        smax;            //capacidade máxima de fornecimento (MVA)
      strPOT       pcc_3f, pcc_ft;  //pot.curto em MVA
      strIMP       zeq0, zeq1;      //impedância equivalentes em pu, Sb=100MVA
      VTCurva      *curva;
      VTResFlowSup *resflow;

   protected: //dados acessados via property
      struct   {
               bool   reducao; //suprimento de redução
               int    fases;
               } PD_SUP;

   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Suprimento
//---------------------------------------------------------------------------
VTSuprimento* __fastcall NewObjSuprimento(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 