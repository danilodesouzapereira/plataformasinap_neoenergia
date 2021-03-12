//---------------------------------------------------------------------------
#ifndef TRemoveBTH
#define TRemoveBTH

//arquivos incluídos---------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTCarga;
class VTCurva;
class VTRede;

//---------------------------------------------------------------------------
class TRemoveBT : public TObject
   {
   public:
           __fastcall  TRemoveBT(VTApl *apl_owner);
           __fastcall ~TRemoveBT(void);
      bool __fastcall  Equivalenta(void);
      bool __fastcall  Executa(void);

   private:
      void     __fastcall ApagaRede(TList *lisREDE);
      void     __fastcall AtualizaCargaEquivalente(VTRede *rede);
      bool     __fastcall CalculaFluxo(void);
      void     __fastcall CriaCargaEquivalente(VTRede *rede);
      VTCurva* __fastcall CriaCurvaNulaCarga(void);
      void     __fastcall DefineCrescimento(void);
      void     __fastcall DefineCurvaDemanda(VTRede *rede);
      void     __fastcall DefineCurvaFluxo(VTRede *rede);
      void     __fastcall MoveCargasEqv(bool para_primario);
      void     __fastcall MoveTrafoParaMT(VTRede *rede);
      void     __fastcall RemoveEqptos(TList *lisEQP, VTRede *rede);
      bool     __fastcall SalvaRedeBase(bool base_nova);
      bool     __fastcall SalvaRedeBaseExistente(void);
      bool     __fastcall SalvaRedeBaseNova(void);
      bool     __fastcall SalvaRedeComo(void);
      bool     __fastcall ValidaEqptoIsolado(void);

   private: //obj. interno
      VTCarga *carga_eqv;
      TList   *lisTRAFO;

   private: //obj. externo
      VTApl *apl;

   };

//---------------------------------------------------------------------------
#endif
