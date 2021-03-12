//---------------------------------------------------------------------------
#ifndef TInsereFusivelH
#define TInsereFusivelH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarra;
class VTChave;
class VTRede;
class VTTrafo;
class TFusivel;

//---------------------------------------------------------------------------
class TInsereFusivel : public TObject
   {
   public:
           __fastcall  TInsereFusivel(VTApl *apl_owner);
           __fastcall ~TInsereFusivel(void);
      bool __fastcall  Insere(AnsiString arquivo);
      bool __fastcall  Remove(void);

   private: //métodos
      TFusivel* __fastcall CriaFusivel(TStringList *campos);
      TFusivel* __fastcall ExisteFusivel(VTTrafo *trafo);
      VTChave*  __fastcall ExisteFusivelConectandoBarra(VTRede *rede, VTBarra *barra);
      bool      __fastcall ImportaFusiveis(AnsiString arquivo);
      VTBarra*  __fastcall InsereBarra(VTBarra *barra_pri);
      VTChave*  __fastcall InsereChaveFusivelEntradaET(VTRede *rede, VTBarra *barra_1, VTBarra *barra_2);
      bool      __fastcall InsereFusivelEmTrafos(TList *lisTRAFO);
      bool      __fastcall RemoveFusivelEmTrafos(TList *lisTRAFO);
      bool      __fastcall VerificaComentario(AnsiString linha);

   private: //obj. externo
      VTApl *apl;

   private: //obj. interno
      TList *lisEQP, *lisFUSIVEL;
      TList *lisEQP_INSERE, *lisEQP_RETIRA, *lisEQP_ALTERA; //usadas pelo Edita

   };
//---------------------------------------------------------------------------
#endif
