//---------------------------------------------------------------------------
#ifndef TMalhasH
#define TMalhasH

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>
#include "VTMalhas.h"

//---------------------------------------------------------------------------
class VTBarra;
class VTLigacao;
class VTMalha;

//---------------------------------------------------------------------------
class TMalhas : public VTMalhas
   {
   public:
             __fastcall TMalhas(void);
             __fastcall ~TMalhas(void);
      bool   __fastcall Executa(VTRede *rede);
      TList* __fastcall LisBarraMalha(void);
      TList* __fastcall LisLigacaoMalha(void);
      TList* __fastcall LisMalha(void);

   private: //métodos
      void     __fastcall AnalisaPaiFilho(void);
      bool     __fastcall ExisteLigacao(TList *lis, VTLigacao *ligacao);
      bool     __fastcall ExisteBarra(TList *lisBAR, VTBarra *pbarra);
      bool     __fastcall ExisteMalha(void);
      VTMalha* __fastcall ExisteMalha(VTLigacao *ligacao);
      void     __fastcall IdentificaBarrasMalha(void);
      void     __fastcall IdentificaLigacoesMalha(void);
      void     __fastcall InsereBarraMalha(VTBarra *pbarra);
      void     __fastcall InsereLigacaoMalha(VTLigacao *ligacao);
      void     __fastcall MontaListBarraPai(void);
      void     __fastcall MontaListLigacoes(void);
      void     __fastcall SeparaMalhas(void);

   private: //dados
      VTRede *rede;
      TList  *lisMALHA; //lista de malhas
      TList  *lisBM;    //lista de barras na malha
      TList  *lisLM;    //lista de ligações na malha
      TList  *lisPAI;   //lista de barras
      TList  *lisLIG;   //lista c/ todas Ligações, sem chaves abertas
      TList  *lisREDU;  //lista de Reducoes vizinhas da Rede
   };

#endif
//---------------------------------------------------------------------------
//eof
