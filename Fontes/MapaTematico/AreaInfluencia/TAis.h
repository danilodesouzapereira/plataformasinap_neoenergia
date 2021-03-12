//---------------------------------------------------------------------------
#ifndef TAisH
#define TAisH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTAis.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTPoligono;

//---------------------------------------------------------------------------
class TAis : public VTAis
   {
   public:  //métodos
              __fastcall  TAis(VTApl *apl, VTSubestacao *subestacao);
              __fastcall ~TAis(void);
      double  __fastcall  DemandaAtendida_mva(void);
      double  __fastcall  DemandaLimite_mva(void);
      TList * __fastcall  LisPtoCarga(void);
      TList * __fastcall  LisCelula(void);
      bool    __fastcall  MontaAreaInfluencia(strOPCAO_AIS &opcao);
      void    __fastcall  PintaCelulas(TColor color, bool merge);

   private:  //método da classe usada na ordenação da lista de Ptocarga
      static int __fastcall ComparaDistanciaPtoCarga(void *Item1, void *Item2);

   private:  //métodos
      VTBarra* __fastcall BarraRefSubestacao(void);
      void     __fastcall IniciaLisCarga(TList *lisCAR, bool carga_propria);
      bool     __fastcall IniciaLisPtoCarga(TList *lisCAR, int ind_pat);
      void     __fastcall InsereCarga(TList *lisCAR, VTSubestacao *subestacao) ;
      bool     __fastcall MontaPoligono(void);
      void     __fastcall SelecionaCelulas(void);

   private:  //objetos externos
      VTApl *apl;

   private:  //dados locais
      TList      *lisEQP;
      TList      *lisCEL;
      TList      *lisPTC;
      VTPoligono *poligono;
   };

//---------------------------------------------------------------------------
#endif
//eof
