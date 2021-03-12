#ifndef VTResCC_H
#define VTResCC_H

//arquivos incluídos-----------------------------------------------------------
#include <Classes.hpp>

//---------------------------------------------------------------------------
class VTApl;
class VTBarCC;
class VTLigCC;
class VTGrupo;

//---------------------------------------------------------------------------
class VTResCC : public TObject
   {
   public:
                       __fastcall VTResCC(void) {};
      virtual          __fastcall ~VTResCC(void) {};
      virtual VTGrupo* __fastcall IniciaGrupoBarra(VTBarCC *barCC) = 0;
      virtual VTGrupo* __fastcall IniciaGrupoBarraDefeito(VTBarCC *barCC) = 0;
      virtual VTGrupo* __fastcall IniciaGrupoBarraDefeitoCorrenteAssimetrica(VTBarCC *barCC) = 0;
      virtual VTGrupo* __fastcall IniciaGrupoContribuicao(VTBarCC *barCC) = 0;
      virtual VTGrupo* __fastcall IniciaGrupoLigacao(VTLigCC *ligCC) = 0;
      virtual VTGrupo* __fastcall IniciaGrupoPotenciaCurtoCircuito(void) = 0;
      virtual VTGrupo* __fastcall IniciaGrupoZentrada(void) = 0;
      virtual VTGrupo* __fastcall IniciaGrupoZentradaSoTrechos(void) = 0;
      virtual void     __fastcall LisDado(VTBarCC *barCC, int ind_def, TList *lisEXT) = 0;
      virtual void     __fastcall LisDado(VTLigCC *ligCC, int ind_def, int ind_bar, TList *lisEXT) = 0;
   };

//-----------------------------------------------------------------------------
//função global p/ criar objeto da classe VTResCC
//-----------------------------------------------------------------------------
VTResCC* __fastcall NewObjResCC(VTApl *apl);

#endif
//-----------------------------------------------------------------------------
// eof
