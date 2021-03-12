//---------------------------------------------------------------------------
#ifndef TFormEvolucaoH
#define TFormEvolucaoH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

//---------------------------------------------------------------------------
class VTApl;

//---------------------------------------------------------------------------
class TFormEvolucao : public TForm
{
__published:   // IDE-managed Components
   TGroupBox *GBoxRedeEqpto;
   TListView *LViewEqpto;
   TGroupBox *GBoxRedeBalanco;
   TListView *LViewBalanco;
   TGroupBox *GBoxRedePerda;
   TListView *LViewPerda;

public:      // User declarations
        __fastcall  TFormEvolucao(TComponent* Owner, VTApl *apl, TWinControl *parent);
        __fastcall ~TFormEvolucao(void);
   void __fastcall  IniciaEvolucaoEmpresa(void);
   void __fastcall  IniciaEvolucaoRede(AnsiString codigo_rede);

private: //métodos
   void __fastcall LViewBalancoInicia(void);
   void __fastcall LViewEqptoInicia(void);
   void __fastcall LViewPerdaInicia(void);

private: //objetos externos
   VTApl *apl;

private: //dados locais
   TList *lisRESUMO;
};

//---------------------------------------------------------------------------
#endif
//eof

