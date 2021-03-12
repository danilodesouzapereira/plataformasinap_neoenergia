//---------------------------------------------------------------------------
#ifndef TEsquematicoH
#define TEsquematicoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTEsquematico.h"

//---------------------------------------------------------------------------
//class VTConsulta;
class VTEsqRadial;
class VTEsqMalha;
class VTEsqViz;
//class VTReta;
//class VTSecao;
class VTBarra;
class VTCorte;
class VTLigacao;
class VTReducao;
class VTRotaciona;

//---------------------------------------------------------------------------
class TEsquematico : public VTEsquematico
   {
   public:
           __fastcall TEsquematico(VTApl *apl);
			  __fastcall ~TEsquematico(void);
		bool __fastcall AjustaCoordBarraSemCoordUtm(VTRedes *redes);
      void __fastcall Align(VTRede *rede, int align);
      bool __fastcall ExisteBarraSemCoordEsq(VTRede *rede);
      bool __fastcall ExisteBarraSemCoordUtm(VTRede *rede);
      bool __fastcall ExisteBarraSemCoordEsq(VTRedes *redes);
      bool __fastcall ExisteBarraSemCoordUtm(VTRedes *redes);
      void __fastcall IniciaEsquematico(VTRedes *redes);
      bool __fastcall IniciaEsquematicoIgualGeo(VTRedes *redes);
      void __fastcall InverteCoordX(VTRedes *redes);
      void __fastcall InverteCoordY(VTRedes *redes);
      void __fastcall MontaEsquematicoGeral(VTRede *rede);
      void __fastcall MontaEsquematicoGeral(VTRedes *redes);
      void __fastcall MontaEsquematicoParcial(VTCluster *cluster);
      void __fastcall MontaEsquematicoParcial(VTRede *rede);
      bool __fastcall RotacionaBarras(VTBarra *barra_ref, double ang_grau);
      void __fastcall SalvaEsquematicoGeral(VTCluster *cluster);
      void __fastcall SalvaEsquematicoGeral(VTRede *rede);
      void __fastcall SalvaEsquematicoGeral(VTRedes *redes);
      void __fastcall SalvaEsquematicoParcial(VTCluster *cluster);
      void __fastcall SalvaEsquematicoParcial(VTRede *rede);

   private: //métodos
      VTBarra*   __fastcall BarraComum(VTRede *rede, VTCorte *corte);
      void       __fastcall CriaEsqRedesVizinhas(VTRede *rede);
      void       __fastcall Executa(VTReducao *reducao, TList *lisSECAO);
      VTCorte*   __fastcall ExisteCorte(TList *lisCORTE, VTBarra *barra);
      bool       __fastcall ExisteEsquematicoGeral(VTRedes *redes);
      bool       __fastcall ExisteEsquematicoParcial(VTRede *rede);
      VTLigacao* __fastcall ExisteLigacao(TList *lisLIG, VTBarra *barra);
      bool       __fastcall ExisteRedeVizinha(VTRede *rede, VTBarra *pbarra, TList *lisVIZ);
      bool       __fastcall ExisteReducao(VTRedes *redes);
      bool       __fastcall ExisteReducaoVizinha(VTRede *rede, VTBarra *pbarra, TList *lisVIZ);
      int        __fastcall IndexOf(TList *lisREDE, VTRede *rede);
      void       __fastcall IniciaEsquematicoGeral(VTRedes *redes);
      bool       __fastcall IniciaEsquematicoParcial(VTRede *rede);
      void       __fastcall OrdenaRedes(VTRedes *redes, TList *lisORD);
      void       __fastcall MontaEsquematicoReducao(VTLigacao *ligacao, VTBarra *barra, TList *lisREDU);
      void       __fastcall MontaEsquematicoRedeVizinha(VTLigacao *ligacao, VTBarra *barra, TList *lisREDE);
      void       __fastcall SalvaComprimentoLigacao(VTRede *rede);

   private: //dados externos
      VTApl   *apl;

   private: //dados locais
//      VTConsulta  *consulta;
      VTEsqMalha  *esq_malha;
      VTEsqRadial *esq_radial;
      VTEsqViz    *esq_viz;
      VTRotaciona *rotaciona;
//      VTReta      *reta;
      TList       *lisORD;
      TList       *lisTMP;
   };

//---------------------------------------------------------------------------
#endif
//eof

