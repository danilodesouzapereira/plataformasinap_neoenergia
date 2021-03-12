//---------------------------------------------------------------------------
#ifndef TCoefNodalH
#define TCoefNodalH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "..\Fluxo\VTFluxo.h"

//---------------------------------------------------------------------------
class TBar;
class TLig;
class VTData;
class VTMontaData;
class VTSisLin;
class VTCustoNodal;

//---------------------------------------------------------------------------
class TCoefNodal : public VTFluxo
   {
   public:
                 __fastcall TCoefNodal(VTApl *apl_owner);
                 __fastcall ~TCoefNodal(void);
      void       __fastcall CfgAvisoErro(bool enabled);
      bool       __fastcall Executa(VTNet *net, double Sbase,
                                    AnsiString arqRelFlow, AnsiString arqRelRede);
      AnsiString __fastcall Modelo(void);
      bool       __fastcall UtilizaBarraPV(void);
		bool       __fastcall ValidaAderencia(VTNet *net, VTLog *plog);

   protected:  //métodos acessados via property
      bool       __fastcall  PM_GetEquilibrado(void);

   private: //métodos
      bool  __fastcall CalculaCoeficientes(VTCustoNodal *custo_nodal, double *vetorX, int dim_x);
      bool  __fastcall ExecutaCalculo(void);
      TBar* __fastcall ExisteBarDoTipo(TList *lisBAR, int tipo);
      TBar* __fastcall ExisteBar(TList *lisBAR, void *eqpto);
      TLig* __fastcall ExisteLig(TList *lisLIG, void *eqpto);
      bool  __fastcall MontaTriangularizaBnodal(void);
      void  __fastcall PreparaBarSwing(void);
      void  __fastcall PreparaIndiceBar(void);

   private: //objetos externos
      VTApl *apl;

   private: //dados locais
      struct   {
               bool enabled;
               } aviso, relatorio;
      VTData      *data;
      VTMontaData *monta;
      double      *col_matZ;
      VTSisLin    *bnodal;
   };

#endif
//---------------------------------------------------------------------------
//eof
