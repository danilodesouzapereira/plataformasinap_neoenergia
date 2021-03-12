//---------------------------------------------------------------------------
#ifndef VTTrafo3EH
#define VTTrafo3EH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "TLigacao.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTTrafo3E : public TLigacao
   {
   public:  //property
      __property int    Ligacao = {read=PM_GetLigacao};
      __property double Sbase   = {read=PM_GetSbase};
      __property int    Tap     = {read=PM_GetTap,  write=PM_SetTap};

   public:
                         __fastcall VTTrafo3E(void) {};
      virtual            __fastcall ~VTTrafo3E(void) {};
      virtual VTTrafo3E* __fastcall Clone(void) = 0;
      virtual bool       __fastcall AngulosNoSecundario(double vet_ang_grau[MAX_FASE])=0;
      virtual bool       __fastcall AngulosNoTerciario(double vet_ang_grau[MAX_FASE])=0;
      virtual double     __fastcall DefasagemPriSec(void)=0;
      virtual double     __fastcall DefasagemPriTer(void)=0;
      virtual bool       __fastcall ExisteLigacaoDelta(void) = 0;
      virtual void       __fastcall OrdenaBar123PriSecTer(void) = 0;
      virtual double     __fastcall PerdaNoFerroMW(void) = 0;
      virtual void       __fastcall RedefineBarraPriSecTer(VTBarra *pb[]) = 0;
      virtual bool       __fastcall RedefineDeltaComoPrimario(void) = 0;

   protected:  //métodos acessados via property
      virtual int    __fastcall PM_GetLigacao(void) = 0;
      virtual int    __fastcall PM_GetTap(void) = 0;
      virtual double __fastcall PM_GetSbase(void) = 0;
      virtual void   __fastcall PM_SetTap(int tap) = 0;

   public: //dados locais
      float  perda_ferro;         //% da Snom máxima
      struct strPST {
                    int    ligacao;
                    float  snom;        //MVA
                    float  vnom, tap;   //kV
                    float  defasagem;   //graus
                    strIMP zterra;      //Zterra em ohm
                    } pri, sec, ter;
      struct {//LTC somente com tap fixo (sem ajuste automático de tap)
             int    tipo;
             float  dv_perc;       //variação % da tensão para baixo ou para cima
             int    num_passo;     //passos para baixo ou para cima
             int    passo;         //passo fixo selecionado
             } ltc;
      strIMP zps0, zps1, zpt0, zpt1, zst0, zst1; //pu Sb=max(Snom)
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe Trafo3E
//---------------------------------------------------------------------------
VTTrafo3E* __fastcall NewObjTrafo3E(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 