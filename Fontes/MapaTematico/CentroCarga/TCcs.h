//---------------------------------------------------------------------------
#ifndef TCcsH
#define TCcsH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTCcs.h"
#include "..\..\Rede\Estrutura.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTPoligono;

//---------------------------------------------------------------------------
class TCcs : public VTCcs
   {
   public:  //métodos
              __fastcall  TCcs(VTApl *apl, VTSubestacao *subestacao);
              __fastcall ~TCcs(void);
      bool    __fastcall  CoordenadasCentroCarga(int &x_cm, int &y_cm);
      TList * __fastcall  LisCelula(void);
      TList * __fastcall  LisPtoCarga(void);
      bool    __fastcall  MontaCentroCarga(int ind_pat);
      void    __fastcall  PintaCelulas(TColor color, bool merge);

   private:  //método da classe usada na ordenação da lista de Ptocarga
      static int __fastcall ComparaDistanciaPtoCarga(void *Item1, void *Item2);

   private:  //métodos
      bool     __fastcall CoordenadaSubestacao(void);
      void     __fastcall CalculaCoordenadasCentroCarga(void);
      //TColor   __fastcall Degrade(TColor color, double valor_pu);
      TColor   __fastcall Degrade(TColor beginColor, TColor endColor, double valor_pu);
      void     __fastcall IniciaLisCarga(TList *lisCAR);
      bool     __fastcall IniciaLisPtoCarga(int ind_pat);
      void     __fastcall MontaPoligono(void);
      void     __fastcall SelecionaCelulas(void);

   private:  //objetos externos
      VTApl *apl;

   private:  //dados locais
      int        ind_pat;
      strUTM     centro_carga;
      strUTM     centro_se;
      TList      *lisCEL;
      TList      *lisEQP;
      TList      *lisPTC;
      VTPoligono *poligono;
   };

//---------------------------------------------------------------------------
#endif
//eof
