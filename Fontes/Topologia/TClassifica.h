//---------------------------------------------------------------------------
#ifndef TClassificaH
#define TClassificaH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "VTClassifica.h"

//---------------------------------------------------------------------------
class VTMalha;
class VTRecorta;

//---------------------------------------------------------------------------
class TClassifica : public VTClassifica
   {
   public:
           __fastcall TClassifica(VTApl *apl);
           __fastcall ~TClassifica(void);
      bool __fastcall Executa(VTRedes *redes);

   private: //métodos
      void  __fastcall CancelaMRedeDeRede(void);
      void  __fastcall CriaMRedesCarregadas(void);
      void  __fastcall IdentificaMRedesComGeracaoPropria(void);
      void  __fastcall FundeMRedesEmMalha(void);
      void  __fastcall LimpaListas(void);

   private: //dados externos
      VTApl       *apl;
      VTRedes     *redes;
      VTRecorta   *recorta;

   private: //dados locais
      VTMalha    *malha;
      TList      *lisMR;      //lista de objetos VTMRede
      TList      *lisMRC;     //lista de objetos VTMRede carregada
      TList      *lisMRR;     //lista de objetos VTMRede reduzida
      TList      *lisMR_CAR;  //lista de objetos VTMRede de carga
      TList      *lisMR_GER;  //lista de objetos VTMRede de geração
      TList      *lisTMP;     //lista de uso temporário
   };

//---------------------------------------------------------------------------
#endif
//eof

