//---------------------------------------------------------------------------
#ifndef VTPtoFixacaoH
#define VTPtoFixacaoH

//arquivos inclu�dos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTPtoFixacao : public TObject
   {
   public: //m�todos
                           __fastcall VTPtoFixacao(void) {};
     virtual               __fastcall ~VTPtoFixacao(void) {};
     virtual VTPtoFixacao* __fastcall Clone(void) = 0;
     virtual void          __fastcall CopiaAtributosDe(VTPtoFixacao &ref) = 0;

   public: //dados
      int        id;
      AnsiString codigo;
      struct   {
               float x, y;
               } coord;
   };

//---------------------------------------------------------------------------
//fun��o global p/ criar objeto da classe VTPtoFixacao
//---------------------------------------------------------------------------
VTPtoFixacao* __fastcall NewObjPtoFixacao(void);

//---------------------------------------------------------------------------
#endif
//eof

