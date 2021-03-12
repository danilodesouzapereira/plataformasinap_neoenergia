//---------------------------------------------------------------------------
#ifndef TFasoresH
#define TFasoresH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTFasores.h"

//---------------------------------------------------------------------------
class VTFasor;
class VTLigacao;
class TAngulos;

//---------------------------------------------------------------------------
class TFasores : public VTFasores
   {
   public:
               __fastcall  TFasores(VTApl *apl);
               __fastcall ~TFasores(void);
      void     __fastcall  DefineAnguloAjuste_rad(TAngulos *ang);
      void     __fastcall  DefinePatamar(int ind_pat);
      VTFasor* __fastcall  ExisteFasorCurtoIfas(int tipo_curto, int fases);
      VTFasor* __fastcall  ExisteFasorCurtoIseq(int tipo_curto, int ind_seq);
      VTFasor* __fastcall  ExisteFasorCurtoVfas(int tipo_curto, int fases);
      VTFasor* __fastcall  ExisteFasorCurtoVseq(int tipo_curto, int ind_seq);
      VTFasor* __fastcall  ExisteFasorFluxoIfas(int fases);
      VTFasor* __fastcall  ExisteFasorFluxoIseq(int ind_seq);
      VTFasor* __fastcall  ExisteFasorFluxoVfas(int fases);
      VTFasor* __fastcall  ExisteFasorFluxoVseq(int ind_seq);
      bool     __fastcall  InsereFasores(VTBarra *bar_ref, VTLigacao *ligacao);
      TList*   __fastcall  LisGrupo(void);
      TList*   __fastcall  LisFasorGruposSelecionados(void);
      void     __fastcall  RedefineBarraReferencia(VTBarra *bar_ref);
      void     __fastcall  Reinicia(void);

   private: //métodos
      void       __fastcall IniciaVetorCores(TColor *vet_color, int dim_vet);
      bool       __fastcall InsereFasoresCurto(VTBarra *bar_ref, VTLigacao *ligacao);
      bool       __fastcall InsereFasoresFluxo(VTBarra *bar_ref, VTLigacao *ligacao);
      bool       __fastcall MontaGrupoFasorCurtoAjusteA(int ind_def);
      bool       __fastcall MontaGrupoFasorCurtoAjusteB(int ind_def);
      bool       __fastcall MontaGrupoFasorCurtoAngulo(int ind_def);
      bool       __fastcall MontaGrupoFasorCurtoIfas(int ind_def, VTBarra *bar_ref, VTLigacao *ligacao);
      bool       __fastcall MontaGrupoFasorCurtoIseq(int ind_def, VTBarra *bar_ref, VTLigacao *ligacao);
      bool       __fastcall MontaGrupoFasorCurtoVff(int ind_def,  VTBarra *barra);
      bool       __fastcall MontaGrupoFasorCurtoVfn(int ind_def,  VTBarra *barra);
      bool       __fastcall MontaGrupoFasorCurtoVseq(int ind_def, VTBarra *barra);
      bool       __fastcall MontaGrupoFasorFluxoAjusteA(void);
      bool       __fastcall MontaGrupoFasorFluxoAjusteB(void);
      bool       __fastcall MontaGrupoFasorFluxoAngulo(void);
      bool       __fastcall MontaGrupoFasorFluxoIfas(VTBarra *barra_ref, VTLigacao *ligacao);
      bool       __fastcall MontaGrupoFasorFluxoIseq(VTBarra *barra_ref, VTLigacao *ligacao);
      bool       __fastcall MontaGrupoFasorFluxoVff(VTBarra *barra);
      bool       __fastcall MontaGrupoFasorFluxoVfn(VTBarra *barra);
      bool       __fastcall MontaGrupoFasorFluxoVseq(VTBarra *barra);
      AnsiString __fastcall NomeDefeito(int ind_def);

   private: //dados locais
      VTApl *apl;

   private: //dados locais
      TList *lisFASOR;
      TList *lisGRUPO;
   };

//---------------------------------------------------------------------------
#endif
//eof
