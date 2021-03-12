//---------------------------------------------------------------------------
#ifndef VTCargaH
#define VTCargaH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "Estrutura.h"
#include "TEqbar.h"
#include "..\Constante\Const.h"
#include "..\Constante\Fases.h"

//---------------------------------------------------------------------------
class VTClasse;
class VTCurva;
class VTCrescimento;

//---------------------------------------------------------------------------
class VTCarga : public TEqbar
   {
   public:  //property
      __property bool   Reducao       = {read=PD_CARGA.reducao,    write=PD_CARGA.reducao};
	  __property bool   Equilibrada   = {read=PM_GetEquilibrada};
	  __property int    MBTfasica     = {read=PM_GetMBTfasica};
	  __property int    Fases         = {read=PD_CARGA.fases,       write=PD_CARGA.fases};
	  __property double Energia_kwhmes= {read=PM_GetEnergia_kwhmes, write=PM_SetEnergia_kwhmes};
	  __property double Energia_mwhmes= {read=PM_GetEnergia_mwhmes, write=PM_SetEnergia_mwhmes};
	  __property int    ModeloCarga   = {read=PM_GetModeloCarga};
      __property bool   VeryImportant = {read=PM_GetVeryImportant,  write=PM_SetVeryImportant};
      __property bool   GrupoA        = {read=PM_GetGrupoA,         write=PM_SetGrupoA};
	  __property bool   IP            = {read=PM_GetClasseIP};
	  __property int    IP_NumLamp    = {read=PD_CARGA.ip.numlamp,  write=PD_CARGA.ip.numlamp};
      __property float  IP_Pot_kw     = {read=PD_CARGA.ip.pot_kw,   write=PD_CARGA.ip.pot_kw};
	  __property float  IP_Fatpot     = {read=PD_CARGA.ip.fatpot,   write=PD_CARGA.ip.fatpot};
	  __property float  Icte_pu       = {read=PD_CARGA.Icte,        write=PD_CARGA.Icte};
	  __property float  Scte_pu       = {read=PD_CARGA.Scte,        write=PD_CARGA.Scte};
	  __property float  Zcte_pu       = {read=PD_CARGA.Zcte,        write=PD_CARGA.Zcte};
	  __property bool   Ajustavel     = {read=AJUSTE_DEM.ajustavel, write=AJUSTE_DEM.ajustavel};
	  __property int	Categoria  = {read=AJUSTE_DEM.categoria, write=AJUSTE_DEM.categoria};
	  __property double FatorAjusteP[int nf] = {read=PM_GetFatorAjusteP, write=PM_SetFatorAjusteP};
	  __property double FatorAjusteQ[int nf] = {read=PM_GetFatorAjusteQ, write=PM_SetFatorAjusteQ};
	  //__property int    NumConsumidor = {read=PM_GetNumConsumidor, write=PM_SetNumConsumidor};
      __property int    NumConsResidencial = {read=PD_CARGA.num_cons.res, write=PD_CARGA.num_cons.res};
      __property int    NumConsComercial   = {read=PD_CARGA.num_cons.com, write=PD_CARGA.num_cons.com};
      __property int    NumConsIndustrial  = {read=PD_CARGA.num_cons.ind, write=PD_CARGA.num_cons.ind};
      __property int    NumConsRural       = {read=PD_CARGA.num_cons.rur, write=PD_CARGA.num_cons.rur};
      __property int    NumConsOutros      = {read=PD_CARGA.num_cons.out, write=PD_CARGA.num_cons.out};
      __property int    NumConsA4          = {read=PD_CARGA.num_cons.a4,  write=PD_CARGA.num_cons.a4};
      __property int    NumConsTotal       = {read=PM_GetNumConsTotal};
      __property bool   DefinidaPorFase    = {read=PM_GetDefinidaPorFase};

   public:
                       __fastcall  VTCarga(void) {};
      virtual          __fastcall ~VTCarga(void) {};
      virtual VTCarga* __fastcall  Clone(void) = 0;
      virtual bool     __fastcall  ConsumidorTipico(void)=0;
      virtual void     __fastcall  DefineClasse(VTClasse *classe)=0;
      virtual void     __fastcall  DefineCrescimento(VTCrescimento *crescimento)=0;
      virtual void     __fastcall  DefineCurva(VTCurva *curva)=0;

   protected: //métodos acessados via property
      virtual bool   __fastcall PM_GetClasseIP(void) = 0;
      virtual bool   __fastcall PM_GetDefinidaPorFase(void) = 0;
      virtual double __fastcall PM_GetEnergia_kwhmes(void) = 0;
      virtual double __fastcall PM_GetEnergia_mwhmes(void) = 0;
      virtual bool   __fastcall PM_GetEquilibrada(void) = 0;
      virtual bool   __fastcall PM_GetGrupoA(void) = 0;
      virtual int    __fastcall PM_GetMBTfasica(void) = 0;
      virtual int    __fastcall PM_GetModeloCarga(void) = 0;
	  virtual int    __fastcall PM_GetNumConsTotal(void) = 0;
	  virtual double __fastcall PM_GetFatorAjusteP(int nf) = 0;
	  virtual double __fastcall PM_GetFatorAjusteQ(int nf) = 0;
	  virtual bool   __fastcall PM_GetVeryImportant(void) = 0;
      //
      virtual void   __fastcall PM_SetEnergia_kwhmes(double energia_kwhmes) = 0;
      virtual void   __fastcall PM_SetEnergia_mwhmes(double energia_mwhmes) = 0;
	  virtual void   __fastcall PM_SetGrupoA(bool grupoA) = 0;
	  virtual void	 __fastcall PM_SetFatorAjusteP(int nf, double valor) = 0;
	  virtual void	 __fastcall PM_SetFatorAjusteQ(int nf, double valor) = 0;
      virtual void   __fastcall PM_SetVeryImportant(bool vip) = 0;

   public: //dados locais
      VTClasse      *classe;
      VTCrescimento *crescimento;
      VTCurva       *curva;

   protected: //dados acessados via property
      struct{
            bool  reducao; //carga de redução
            bool  vip;
            bool  grupoA;
            int   fases;
            float energia_kwhmes;
				struct{
						int   numlamp;
                  float pot_kw;
                  float fatpot;
                  } ip;
            struct{
                  int res;
                  int com;
                  int ind;
                  int rur;
                  int out;
                  int a4;
                  }num_cons;
            float Icte, Scte, Zcte;   //modelo de carga (pu)
			} PD_CARGA;

	  struct
	  {
			bool ajustavel;
			int categoria;
			double fator_ajusteP[MAX_FASE];
			double fator_ajusteQ[MAX_FASE];
	  }AJUSTE_DEM;

   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTCarga
//---------------------------------------------------------------------------
VTCarga* __fastcall NewObjCarga(int num_pat);

//---------------------------------------------------------------------------
#endif
//eof

 