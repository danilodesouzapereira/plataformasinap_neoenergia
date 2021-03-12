#ifndef TResCCH
#define TResCCH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTResCC.h"
#include "..\Legenda\TAue.h"

//---------------------------------------------------------------------------
class VTFases;
class VTRedeCC;

//---------------------------------------------------------------------------
class TResCC : public VTResCC
   {
   public:
               __fastcall TResCC(VTApl *apl);
               __fastcall ~TResCC(void);
      VTGrupo* __fastcall IniciaGrupoBarra(VTBarCC *barCC);
      VTGrupo* __fastcall IniciaGrupoBarraDefeito(VTBarCC *barCC);
      VTGrupo* __fastcall IniciaGrupoBarraDefeitoCorrenteAssimetrica(VTBarCC *barCC);
      VTGrupo* __fastcall IniciaGrupoContribuicao(VTBarCC *barCC);
      VTGrupo* __fastcall IniciaGrupoLigacao(VTLigCC *ligCC);
      VTGrupo* __fastcall IniciaGrupoPotenciaCurtoCircuito(void);
      VTGrupo* __fastcall IniciaGrupoZentrada(void);
      VTGrupo* __fastcall IniciaGrupoZentradaSoTrechos(void);
      void     __fastcall LisDado(VTBarCC *barCC, int ind_def, TList *lisEXT);
      void     __fastcall LisDado(VTLigCC *ligCC, int ind_def, int ind_bar, TList *lisEXT);

   private: //métodos
      void       __fastcall MontaGrupoIfas_amp(VTBarCC *barCC, VTGrupo *pai, int ind_def);
      void       __fastcall MontaGrupoIseq_amp(VTBarCC *barCC, VTGrupo *pai, int ind_def);
      void       __fastcall MontaGrupoVfas_kv(VTBarCC *barCC, VTGrupo *pai, int ind_def);
      void       __fastcall MontaGrupoVfas_pu(VTBarCC *barCC, VTGrupo *pai, int ind_def);
      void       __fastcall MontaGrupoVseq_kv(VTBarCC *barCC, VTGrupo *pai, int ind_def);
      void       __fastcall MontaGrupoVseq_pu(VTBarCC *barCC, VTGrupo *pai, int ind_def);
      void       __fastcall MontaLisDadoIass_amp(VTBarCC *barCC, int ind_def, TList *lisDADO);
      void       __fastcall MontaLisDadoIfas_amp(VTBarCC *barCC, int ind_def, TList *lisDADO);
      void       __fastcall MontaLisDadoIfas_amp(VTLigCC *ligCC, int ind_def, int ind_bar, TList *lisDADO);
      void       __fastcall MontaLisDadoIseq_amp(VTBarCC *barCC, int ind_def, TList *lisDADO);
      void       __fastcall MontaLisDadoIseq_amp(VTLigCC *ligCC, int ind_def, int ind_bar, TList *lisDADO);
      void       __fastcall MontaLisDadoVfas_kv(VTBarCC *barCC, int ind_def, TList *lisDADO);
      void       __fastcall MontaLisDadoVfas_pu(VTBarCC *barCC, int ind_def, TList *lisDADO);
      void       __fastcall MontaLisDadoVseq_kv(VTBarCC *barCC, int ind_def, TList *lisDADO);
      void       __fastcall MontaLisDadoVseq_pu(VTBarCC *barCC, int ind_def, TList *lisDADO);
      AnsiString __fastcall NomeDefeito(int ind_def);
      AnsiString __fastcall NomeFase(int ind_fase);
      AnsiString __fastcall NomeSeq(int ind_seq);

   private: //objetos externos
      VTApl    *apl;
      VTFases  *fases;
      VTRedeCC *redeCC;

   private: //dados locais
      TAue *aue;
   };

#endif
//-----------------------------------------------------------------------------
// eof

