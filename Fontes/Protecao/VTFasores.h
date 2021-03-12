//---------------------------------------------------------------------------
#ifndef VTFasoresH
#define VTFasoresH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTChave;
class VTBarCC;
class VTLigCC;
class VTLigacao;
class VTFasor;
class TAngulos;

//---------------------------------------------------------------------------
class VTFasores : public TObject
   {
   public:
                       __fastcall  VTFasores(void) {};
      virtual          __fastcall ~VTFasores(void) {};
      virtual void     __fastcall  DefineAnguloAjuste_rad(TAngulos *ang) = 0;
      virtual void     __fastcall  DefinePatamar(int ind_pat=0) = 0;
      virtual VTFasor* __fastcall  ExisteFasorCurtoIfas(int tipo_curto, int fases) = 0;
      virtual VTFasor* __fastcall  ExisteFasorCurtoIseq(int tipo_curto, int ind_seq) = 0;
      virtual VTFasor* __fastcall  ExisteFasorCurtoVfas(int tipo_curto, int fases) = 0;
      virtual VTFasor* __fastcall  ExisteFasorCurtoVseq(int tipo_curto, int ind_seq) = 0;
      virtual VTFasor* __fastcall  ExisteFasorFluxoIfas(int fases) = 0;
      virtual VTFasor* __fastcall  ExisteFasorFluxoVfas(int fases) = 0;
      virtual bool     __fastcall  InsereFasores(VTBarra *bar_ref, VTLigacao *ligacao)=0;
      virtual TList*   __fastcall  LisGrupo(void) = 0;
      virtual TList*   __fastcall  LisFasorGruposSelecionados(void) = 0;
      virtual void     __fastcall  RedefineBarraReferencia(VTBarra *bar_ref) = 0;
      virtual void     __fastcall  Reinicia(void) = 0;
   };

//---------------------------------------------------------------------------
//função global p/ criar objeto da classe VTFasores
//---------------------------------------------------------------------------
VTFasores* __fastcall NewObjFasores(VTApl *apl);

//---------------------------------------------------------------------------
#endif
//eof
