//---------------------------------------------------------------------------
#ifndef TZipH
#define TZipH

//---------------------------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTChave;
class VTRede;
class VTRedes;
class VTTrecho;

//---------------------------------------------------------------------------
class TZip : public TObject
   {
   public:  //métodos
           __fastcall  TZip(VTApl *apl);
           __fastcall ~TZip(void);
      bool __fastcall  FundeTrechos(void);
      bool __fastcall  RetiraChavesExcetoDJinicial(void);
      bool __fastcall  RetiraChavesSemMedicao(void);

   private:  //métodos
      bool __fastcall  AssociaBarrasComSuasLigacoes(VTRedes *redes);
      void __fastcall  DeleteEqpto(TList *lisEXT);
      bool __fastcall  FundeTrechosIntermediarios(VTRedes *redes);
      bool __fastcall  FundeTrechosTerminais(VTRedes *redes);
      bool __fastcall  FundeTrecho2NoTrecho1(VTTrecho *trecho1, VTTrecho *trecho2);
      int  __fastcall  LisBarraInicialRede(VTRedes *redes, TList *lisEXT);
      int  __fastcall  LisChaveComMedicao(VTRede *rede, TList *lisEXT);
      int  __fastcall  LisChaveSemMedicao(VTRedes *redes, TList *lisEXT);
      bool __fastcall  RetiraChavesExcetoDJinicial(VTRede *rede);
      bool __fastcall  RetiraChavesSemMedicao(VTRedes *redes);

   private:  //ojetos externos
      VTApl  *apl;

   private: //dados locais
      TList  *lisCHV;
      TList  *lisDEL;
      TList  *lisEQP;
      TList  *lisLIG;
      TList  *lisMED;
   };

#endif
//---------------------------------------------------------------------------
//eof
